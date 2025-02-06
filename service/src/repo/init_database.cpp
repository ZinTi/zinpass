#include "repo/init_database.h"
#include "utils/log_manager.h"

namespace zinpass::repo {
    InitDatabase::InitDatabase(const std::string& db_path) {
        this->db_path_ = db_path;
    }

    // 函数用于执行 SQL 语句
    int InitDatabase::executeSQL(sqlite3* conn, const std::string& sql) {
        char* errMsg = nullptr;
        if (
            const int rc = sqlite3_exec(conn, sql.c_str(), nullptr, nullptr, &errMsg);
            SQLITE_OK != rc)
        {
            utils::LogManager::AddLog(std::string("SQL error: ") + std::string(errMsg) + std::string("\n"));
            sqlite3_free(errMsg);
            return rc;
        }
        return 0;
    }

    int InitDatabase::init() const {
        sqlite3* conn;
        if (const int rc = sqlite3_open(this->db_path_.c_str(), &conn)){
            utils::LogManager::AddLog(std::string("[ERROR] Failed to open database: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_close(conn);
            return rc;
        }

        // 开启外键约束
        const std::string pragma = "PRAGMA foreign_keys=ON;";
        executeSQL(conn, pragma);

        // 开始事务
        const std::string beginTransaction = "BEGIN TRANSACTION;";
        executeSQL(conn, beginTransaction);

        // 创建 system_user 表
        const std::string createSystemUser =
            "CREATE TABLE system_user\n"
            "(\n"
            "    id              VARCHAR(36) NOT NULL PRIMARY KEY,   -- 主键 UUID\n"
            "    username        VARCHAR(16) NOT NULL UNIQUE,        -- 登录用户名\n"
            "    nickname        TEXT NOT NULL,                      -- 昵称\n"
            "    hashed_pwd      CHAR(64) NOT NULL, -- SHA256\n"
            "    salt            VARCHAR(50) NOT NULL,\n"
            "    created_time    DATETIME NOT NULL,\n"
            "    updated_time    DATETIME NOT NULL\n"
            ");";
        executeSQL(conn, createSystemUser);

        // 创建 session 表
        const std::string createSession =
            "CREATE TABLE session\n"
            "(\n"
            "    id              VARCHAR(36) NOT NULL PRIMARY KEY,\n"
            "    sys_user_id     VARCHAR(36) NOT NULL,\n"
            "    data            TEXT,\n"
            "    created_time    DATETIME NOT NULL,\n"
            "    expires_time    DATETIME NOT NULL,\n"
            "    accessed_time   DATETIME NOT NULL,\n"
            "    CONSTRAINT session_fk_1 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE\n"
            ");";
        executeSQL(conn, createSession);

        // 创建 telecom_operator 表
        const std::string createTelecomOperator =
            "CREATE TABLE telecom_operator\n"
            "(\n"
            "    id   TINYINT     NOT NULL PRIMARY KEY,\n"
            "    name VARCHAR(50) NOT NULL UNIQUE\n"
            ");";
        executeSQL(conn, createTelecomOperator);
        // 电信运营商默认数据
        const std::string insertTelecomOperator =
            "INSERT INTO telecom_operator\n"
            "VALUES (1, '中国联通'),\n"
            "       (2, '中国电信'),\n"
            "       (3, '中国移动'),\n"
            "       (4, '中国广电'),\n"
            "       (5, '临时接码台'),\n"
            "       (6, '其他运营商');\n";
        executeSQL(conn, insertTelecomOperator);

        // 创建 mobile_phone 表
        const std::string createMobilePhone =
            "CREATE TABLE mobile_phone\n"
            "(\n"
            "    id               INT         NOT NULL PRIMARY KEY,\n"
            "    phone_number     VARCHAR(15) NOT NULL,     -- 手机号码\n"
            "    telecom_operator VARCHAR(8)  DEFAULT NULL, -- 电信运营商\n"
            "    service_pwd      VARCHAR(10) DEFAULT NULL, -- 服务密码\n"
            "    pin              VARCHAR(10) DEFAULT NULL, -- PIN\n"
            "    puk              CHAR(8)     DEFAULT NULL, -- PUK\n"
            "    join_time        DATETIME    DEFAULT NULL, -- 入网时间\n"
            "    phone_area       VARCHAR(50) NOT NULL,     -- 归属地\n"
            "    postscript       TEXT        DEFAULT NULL, -- 备注\n"
            "    sys_user_id      VARCHAR(36) NOT NULL,     -- 所属系统用户\n"
            "    created_time     DATETIME    NOT NULL,\n"
            "    updated_time     DATETIME    NOT NULL,\n"
            "    CONSTRAINT mobile_phone_fk_1 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE\n"
            ");";
        executeSQL(conn, createMobilePhone);

        const std::string createAccountCategory =
            "CREATE TABLE account_category\n"
            "(\n"
            "    id   TINYINT     NOT NULL PRIMARY KEY,\n"
            "    name VARCHAR(50) NOT NULL UNIQUE\n"
            ");";
        executeSQL(conn, createAccountCategory);

        const std::string insertAccountCategory =
            "INSERT INTO account_category\n"
            "VALUES (1, '电子邮件'),\n"
            "       (2, '设备密码'),\n"
            "       (3, '社交媒体'),\n"
            "       (4, '教育培训'),\n"
            "       (5, '财务'),\n"
            "       (6, '购物'),\n"
            "       (7, '娱乐'),\n"
            "       (8, '出行旅行'),\n"
            "       (9, '游戏'),\n"
            "       (10, '体育'),\n"
            "       (11, '企业'),\n"
            "       (12, '科技'),\n"
            "       (13, '资讯'),\n"
            "       (14, '政务服务'),\n"
            "       (15, '公共事业'),\n"
            "       (16, '医疗卫生'),\n"
            "       (17, '其他');\n";
        executeSQL(conn, insertAccountCategory);

        // 创建 account 表
        const std::string createAccount =
            "CREATE TABLE account\n"
            "(\n"
            "    id            VARCHAR(36)  NOT NULL PRIMARY KEY, -- 主键\n"
            "    username      VARCHAR(100) NOT NULL,             -- 登录用户名\n"
            "    nickname      VARCHAR(100) DEFAULT NULL,         -- 昵称\n"
            "    encrypted_pwd BLOB         NOT NULL,             -- 加密后的密码\n"
            "    iv            BLOB         NOT NULL,             -- 初始化向量（IV），用于AES-128-CTR模式解密\n"
            "    sub_account   VARCHAR(100) DEFAULT NULL,         -- 子账号\n"
            "    phone_id      INT          DEFAULT NULL,         -- 绑定手机号编号\n"
            "    email_id      VARCHAR(36)  DEFAULT NULL,         -- 绑定邮箱编号\n"
            "    postscript    TEXT         DEFAULT NULL,         -- 附言\n"
            "    platform_name VARCHAR(255) NOT NULL,             -- 平台名称\n"
            "    provider_name VARCHAR(255) DEFAULT NULL,         -- 服务商名称\n"
            "    URL           VARCHAR(255) DEFAULT NULL,         -- 网址\n"
            "    hotline       VARCHAR(50)  DEFAULT NULL,         -- 客服热线\n"
            "    category_id   TINYINT      NOT NULL,             -- 类别\n"
            "    sys_user_id   VARCHAR(36)  NOT NULL,             -- 所属系统用户\n"
            "    created_time  DATETIME     NOT NULL,\n"
            "    updated_time  DATETIME     NOT NULL,\n"
            "    CONSTRAINT account_fk_1 FOREIGN KEY (phone_id) REFERENCES mobile_phone (id) ON UPDATE CASCADE ON DELETE SET NULL,\n"
            "    CONSTRAINT account_fk_2 FOREIGN KEY (email_id) REFERENCES account (id) ON UPDATE CASCADE ON DELETE RESTRICT, -- 自引用外键\n"
            "    CONSTRAINT account_fk_3 FOREIGN KEY (category_id) REFERENCES account_category (id) ON UPDATE CASCADE ON DELETE RESTRICT,\n"
            "    CONSTRAINT account_fk_4 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE\n"
            ");";
        executeSQL(conn, createAccount);

        // 创建 view_account 视图
        const std::string create_ViewAccount =
            "CREATE VIEW view_account AS\n"
            "SELECT a1.id,\n"
            "       a1.provider_name,\n"
            "       a1.platform_name,\n"
            "       a1.username,\n"
            "       a1.nickname,\n"
            "       a1.sub_account,\n"
            "       mobile_phone.phone_number AS phone,\n"
            "       a2.username               AS email,\n"
            "       a1.postscript,\n"
            "       a1.URL,\n"
            "       a1.hotline,\n"
            "       c1.name                   AS category,\n"
            "       a1.created_time,\n"
            "       a1.updated_time,\n"
            "       a1.sys_user_id\n"
            "FROM account a1\n"
            "         LEFT JOIN mobile_phone ON a1.phone_id = mobile_phone.id\n"
            "         LEFT JOIN account a2 ON a1.email_id = a2.id -- 自引用外键 email_id\n"
            "         LEFT JOIN account_category c1 ON a1.category_id = c1.id\n"
            ";\n";
        executeSQL(conn, create_ViewAccount);

        const std::string create_viewEmail =
            "CREATE VIEW view_email AS\n"
            "SELECT *\n"
            "FROM view_account\n"
            "WHERE category = '电子邮件';\n";
        executeSQL(conn, create_viewEmail);

        const std::string create_ViewPwd =
            "CREATE VIEW view_pwd AS\n"
            "SELECT account.id,\n"
            "       account.encrypted_pwd,\n"
            "       account.iv,\n"
            "       account.sys_user_id\n"
            "FROM account;\n";
        executeSQL(conn, create_ViewPwd);

    // 提交事务
    const std::string commitTransaction = "COMMIT;";
    executeSQL(conn, commitTransaction);

    sqlite3_close(conn);
    return 0;
}
}
