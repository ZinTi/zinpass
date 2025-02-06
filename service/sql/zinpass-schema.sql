-- 密码管理系统
PRAGMA foreign_keys= ON;       -- 每次连接数据库后执行
BEGIN TRANSACTION;

CREATE TABLE system_user
(
    id              VARCHAR(36) NOT NULL PRIMARY KEY,   -- 主键 UUID
    username        VARCHAR(16) NOT NULL UNIQUE,        -- 登录用户名
    nickname        TEXT NOT NULL,                      -- 昵称
    hashed_pwd      CHAR(64) NOT NULL, -- SHA256
    salt            VARCHAR(50) NOT NULL,
    created_time    DATETIME NOT NULL,
    updated_time    DATETIME NOT NULL
);

-- 新增session表
CREATE TABLE session
(
    id              VARCHAR(36) NOT NULL PRIMARY KEY,
    sys_user_id     VARCHAR(36) NOT NULL,
    data            TEXT,
    created_time    DATETIME NOT NULL,
    expires_time    DATETIME NOT NULL,
    accessed_time   DATETIME NOT NULL,
    CONSTRAINT session_fk_1 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE
);

-- 电信运营商
CREATE TABLE telecom_operator
(
    id   TINYINT     NOT NULL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);
-- 电信运营商默认数据
INSERT INTO telecom_operator
VALUES (1, '中国联通'),
       (2, '中国电信'),
       (3, '中国移动'),
       (4, '中国广电'),
       (5, '临时接码台'),
       (6, '其他运营商');

CREATE TABLE mobile_phone
(
    id               INT         NOT NULL PRIMARY KEY,
    phone_number     VARCHAR(15) NOT NULL,     -- 手机号码
    telecom_operator VARCHAR(8)  DEFAULT NULL, -- 电信运营商
    service_pwd      VARCHAR(10) DEFAULT NULL, -- 服务密码
    pin              VARCHAR(10) DEFAULT NULL, -- PIN
    puk              CHAR(8)     DEFAULT NULL, -- PUK
    join_time        DATETIME    DEFAULT NULL, -- 入网时间
    phone_area       VARCHAR(50) NOT NULL,     -- 归属地
    postscript       TEXT        DEFAULT NULL, -- 备注
    sys_user_id      VARCHAR(36) NOT NULL,     -- 所属系统用户
    created_time     DATETIME    NOT NULL,
    updated_time     DATETIME    NOT NULL,
    CONSTRAINT mobile_phone_fk_1 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE account_category
(
    id   TINYINT     NOT NULL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);
-- 类别（选择类别）
INSERT INTO account_category
VALUES (1, '电子邮件'),
       (2, '设备密码'),
       (3, '社交媒体'),
       (4, '教育培训'),
       (5, '财务'),
       (6, '购物'),
       (7, '娱乐'),
       (8, '出行旅行'),
       (9, '游戏'),
       (10, '体育'),
       (11, '企业'),
       (12, '科技'),
       (13, '资讯'),
       (14, '政务服务'),
       (15, '公共事业'),
       (16, '医疗卫生'),
       (17, '其他');

-- 添加一个 category_id 字段
-- email_id 为 自引用外键：邮件也有可能绑定邮件，比如@outlook邮箱
CREATE TABLE account
(
    id            VARCHAR(36)  NOT NULL PRIMARY KEY, -- 主键(改为UUID)
    username      VARCHAR(100) NOT NULL,             -- 登录用户名
    nickname      VARCHAR(100) DEFAULT NULL,         -- 昵称
    encrypted_pwd BLOB         NOT NULL,             -- 加密后的密码
    iv            BLOB         NOT NULL,             -- 初始化向量（IV），用于AES-128-CTR模式解密
    sub_account   VARCHAR(100) DEFAULT NULL,         -- 子账号
    phone_id      INT          DEFAULT NULL,         -- 绑定手机号编号
    email_id      VARCHAR(36)  DEFAULT NULL,         -- 绑定邮箱编号
    postscript    TEXT         DEFAULT NULL,         -- 附言
    platform_name VARCHAR(255) NOT NULL,             -- 平台名称
    provider_name VARCHAR(255) DEFAULT NULL,         -- 服务商名称
    URL           VARCHAR(255) DEFAULT NULL,         -- 网址
    hotline       VARCHAR(50)  DEFAULT NULL,         -- 客服热线
    category_id   TINYINT      NOT NULL,             -- 类别
    sys_user_id   VARCHAR(36)  NOT NULL,             -- 所属系统用户
    created_time  DATETIME     NOT NULL,
    updated_time  DATETIME     NOT NULL,
    CONSTRAINT account_fk_1 FOREIGN KEY (phone_id) REFERENCES mobile_phone (id) ON UPDATE CASCADE ON DELETE SET NULL,
    CONSTRAINT account_fk_2 FOREIGN KEY (email_id) REFERENCES account (id) ON UPDATE CASCADE ON DELETE RESTRICT, -- 自引用外键
    CONSTRAINT account_fk_3 FOREIGN KEY (category_id) REFERENCES account_category (id) ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT account_fk_4 FOREIGN KEY (sys_user_id) REFERENCES system_user (id) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE VIEW view_account AS
SELECT a1.id,
       a1.provider_name,
       a1.platform_name,
       a1.username,
       a1.nickname,
       a1.sub_account,
       mobile_phone.phone_number AS phone,
       a2.username               AS email,
       a1.postscript,
       a1.URL,
       a1.hotline,
       c1.name                   AS category,
       a1.created_time,
       a1.updated_time,
       a1.sys_user_id
FROM account a1
         LEFT JOIN mobile_phone ON a1.phone_id = mobile_phone.id
         LEFT JOIN account a2 ON a1.email_id = a2.id -- 自引用外键 email_id
         LEFT JOIN account_category c1 ON a1.category_id = c1.id
;

CREATE VIEW view_email AS
SELECT *
FROM view_account
WHERE category = '电子邮件';

CREATE VIEW view_pwd AS
SELECT account.id,
       account.encrypted_pwd,
       account.iv,
       account.sys_user_id
FROM account;

COMMIT;
