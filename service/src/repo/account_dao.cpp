#include "repo/account_dao.h"
#include "utils/date_time.h"
#include "utils/log_manager.h"
#include "repo/common/sql_debug.h"
#include <map>
#include <sstream>
#include <stdexcept>
#include "utils/uuid_generator.h"
#include "models/view_account.h"

namespace zinpass::repo{

AccountDAO::AccountDAO() : pool_(ConnectionPool::get_instance()) {
}

AccountDAO::~AccountDAO() = default;

// 注意保持和 SQLite3 Schema 一致
models::ViewAccount AccountDAO::ViewAccountFromStatement(sqlite3_stmt* stmt){
    models::ViewAccount view_account;

    const auto* account_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    view_account.setId(account_id ? account_id : "");
    const auto* provider_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    view_account.setProviderName(provider_name ? provider_name : "");
    const auto* platform_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    view_account.setPlatformName(platform_name ? platform_name : "");
    const auto* username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    view_account.setUsername(username ? username : "");
    const auto* nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    view_account.setNickname(nickname ? nickname : "");
    const auto* sub_account = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    view_account.setSubAccount(sub_account ? sub_account : "");
    const auto* phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    view_account.setPhone(phone ? phone : "");
    const auto* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
    view_account.setEmail(email ? email : "");
    const auto* postscript = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    view_account.setPostscript(postscript ? postscript : "");
    const auto* Url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
    view_account.setURL(Url ? Url : "");
    const auto* hotline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
    view_account.setHotline(hotline ? hotline : "");
    const auto* category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
    view_account.setCategory(category ? category : "");
    const auto* created_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
    view_account.setCreatedTime(created_time ? created_time : "");
    const auto* updated_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
    view_account.setUpdatedTime(updated_time ? updated_time : "");
    const auto* sys_user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
    view_account.setSysUserId(sys_user_id ? sys_user_id : "");
    return view_account; // 编译器会自动优化调用拷贝构造函数返回对象的副本
}

std::string AccountDAO::buildSqlPart(const std::map<ColumnType, std::string>& data, const std::string& separator) {
    std::string part;
    for (const auto& [col, value] : data) {
        std::stringstream ss;
        switch (col) {
        case ColumnType::id:
            ss << "id =?";
            break;
        case ColumnType::username:
            ss << "username =?";
            break;
        case ColumnType::nickname:
            ss << "nickname =?";
            break;
        case ColumnType::encryptedPwd:
            ss << "encrypted_pwd =?";
            break;
        case ColumnType::iv:
            ss << "iv =?";
            break;
        case ColumnType::subAccount:
            ss << "sub_account =?";
            break;
        case ColumnType::phoneId:
            ss << "phone_id =?";
            break;
        case ColumnType::emailId:
            ss << "email_id =?";
            break;
        case ColumnType::postscript:
            ss << "postscript =?";
            break;
        case ColumnType::platformName:
            ss << "platform_name =?";
            break;
        case ColumnType::providerName:
            ss << "provider_name =?";
            break;
        case ColumnType::URL:
            ss << "URL =?";
            break;
        case ColumnType::hotline:
            ss << "hotline =?";
            break;
        case ColumnType::categoryId:
            ss << "category_id =?";
            break;
        case ColumnType::owner:
            ss << "sys_user_id =?";
            break;
        case ColumnType::createdTime:
            ss << "created_time =?";
            break;
        case ColumnType::updatedTime:
            ss << "updated_time =?";
            break;
        default:
            continue;
        }
        if (!part.empty()) {
            part += separator;
        }
        part += ss.str();
    }
    return part;
}

DaoStatus AccountDAO::findById(const std::string& id, models::ViewAccount& view_account) const {
    DaoStatus status;
    sqlite3* conn = pool_.get_connection();

    const std::string sql("SELECT * FROM view_account WHERE id = ? LIMIT 1");
    sqlite3_stmt* stmt = nullptr;

    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW == sqlite3_step(stmt)) {
        view_account = ViewAccountFromStatement(stmt);
        utils::LogManager::AddLog("[OK] Account found.");
        status = DaoStatus::Success;
    } else {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Account not found with the given ID in the database."));
        status = DaoStatus::NotFound;
    }

    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

DaoStatus AccountDAO::find(const std::string& sys_user_id, std::vector<models::ViewAccount>& view_accounts) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT * FROM view_account WHERE sys_user_id = ?";
    sqlite3_stmt* stmt = nullptr;

    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        view_accounts.push_back(ViewAccountFromStatement(stmt));
    }

    utils::LogManager::AddLog(std::string("[OK] All accounts fetched."));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

// __attribute__((optimize("O0")))
DaoStatus AccountDAO::find(
    const std::string& provider_name,
    const std::string& platform_name,
    const std::string& username,
    const std::string& nickname,
    const std::string& phone,
    const std::string& email,
    const std::string& category,
    const std::string& postscript,
    const std::string& sys_user_id,
    std::vector<models::ViewAccount>& view_accounts) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT * FROM view_account WHERE "
        "(provider_name LIKE ? OR (? = '' AND provider_name IS NULL)) AND "
        "(platform_name LIKE ? OR (? = '' AND platform_name IS NULL)) AND "
        "(username LIKE ? OR (? = '' AND username IS NULL)) AND "
        "(nickname LIKE ? OR (? = '' AND nickname IS NULL)) AND "
        "(phone = ? OR (? = '' AND (phone IS NOT NULL OR phone IS NULL))) AND "
        "(email = ? OR (? = '' AND (email IS NOT NULL OR email IS NULL))) AND "
        "(category = ? OR (? = '' AND (category IS NOT NULL OR category IS NULL))) AND "
        "(postscript LIKE ? OR (? = '' AND postscript IS NULL)) AND "
        "(sys_user_id = ? OR (? = '' AND sys_user_id IS NULL));";

    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    int index = 1;
    std::ostringstream oss_provider_name;
    oss_provider_name << "%" << provider_name << "%";
    sqlite3_bind_text(stmt, index++, oss_provider_name.str().c_str(), static_cast<int>(oss_provider_name.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, provider_name.c_str(), static_cast<int>(provider_name.size()), SQLITE_TRANSIENT);

    std::ostringstream oss_platform_name;
    oss_platform_name << "%" << platform_name << "%";
    sqlite3_bind_text(stmt, index++, oss_platform_name.str().c_str(), static_cast<int>(oss_platform_name.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, platform_name.c_str(), static_cast<int>(platform_name.size()), SQLITE_TRANSIENT);

    std::ostringstream oss_username;
    oss_username << "%" << username << "%";
    sqlite3_bind_text(stmt, index++, oss_username.str().c_str(), static_cast<int>(oss_username.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, username.c_str(), static_cast<int>(username.size()), SQLITE_TRANSIENT);

    std::ostringstream oss_nickname;
    oss_nickname << "%" << nickname << "%";
    sqlite3_bind_text(stmt, index++, oss_nickname.str().c_str(), static_cast<int>(oss_nickname.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, nickname.c_str(), static_cast<int>(nickname.size()), SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, index++, phone.c_str(), static_cast<int>(phone.size()), SQLITE_TRANSIENT); // phone是精确而非模糊筛选（当为空时则不筛选）
    sqlite3_bind_text(stmt, index++, phone.c_str(), static_cast<int>(phone.size()), SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, index++, email.c_str(), static_cast<int>(email.size()), SQLITE_TRANSIENT); // email是精确而非模糊筛选（当为空时则不筛选）
    sqlite3_bind_text(stmt, index++, email.c_str(), static_cast<int>(email.size()), SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, index++, category.c_str(), static_cast<int>(category.size()), SQLITE_TRANSIENT); // category是精确而非模糊筛选（当为空时则不筛选）
    sqlite3_bind_text(stmt, index++, category.c_str(), static_cast<int>(category.size()), SQLITE_TRANSIENT);

    std::ostringstream oss_postscript;
    oss_postscript << "%" << postscript << "%";
    sqlite3_bind_text(stmt, index++, oss_postscript.str().c_str(), static_cast<int>(oss_postscript.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, postscript.c_str(), static_cast<int>(postscript.size()), SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, index++, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        view_accounts.push_back(ViewAccountFromStatement(stmt));
    }

    utils::LogManager::AddLog("[OK] Accounts fetched based on the search criteria.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus AccountDAO::findEncryptedPwdAndIv(const std::string& id, const std::string& sys_user_id,
    std::vector<unsigned char>& encrypted_pwd, std::vector<unsigned char>& iv) const{
    sqlite3* conn = pool_.get_connection();

    sqlite3_stmt* stmt = nullptr;
    const std::string sql("SELECT encrypted_pwd, iv FROM view_pwd WHERE id =? AND sys_user_id =?");
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ]") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (const int stepResult = sqlite3_step(stmt);
        SQLITE_ROW == stepResult)
    {
        const int blobSizeEncryptedPwd = sqlite3_column_bytes(stmt, 0);
        const auto* blobPtrEncryptedPwd = static_cast<const unsigned char*>(sqlite3_column_blob(stmt, 0));
        encrypted_pwd.assign(blobPtrEncryptedPwd, blobPtrEncryptedPwd + blobSizeEncryptedPwd);

        const int blobSizeIV = sqlite3_column_bytes(stmt, 1);
        const auto* blobPtrIV = static_cast<const unsigned char*>(sqlite3_column_blob(stmt, 1));
        iv.assign(blobPtrIV, blobPtrIV + blobSizeIV);
    } else if (SQLITE_DONE == stepResult) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] No rows found."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    } else {
        utils::LogManager::AddLog(std::string("[FAILED] Step failed: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

std::vector<std::string> AccountDAO::list_id(const std::string& sys_user_id) const {
    std::vector<std::string> data_list;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id FROM account WHERE sys_user_id = ?";
    sqlite3_stmt* stmt = nullptr;

    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return data_list;
    }
    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        const auto* account_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        data_list.push_back(account_id);
    }

    utils::LogManager::AddLog(std::string("[OK] All accounts id fetched."));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return data_list;
}

// __attribute__((optimize("O0")))     // 禁用优化
DaoStatus AccountDAO::add(const models::Account& account) const {
    DaoStatus status;
    sqlite3* conn = pool_.get_connection();

    const std::string sql("INSERT INTO account (id, username, nickname, encrypted_pwd, iv, sub_account, phone_id, email_id, postscript, platform_name, "
        "provider_name, URL, hotline, category_id, sys_user_id, created_time, updated_time) "
        "VALUES (:id, :username, :nickname, :encrypted_pwd, :iv, :sub_account, :phone_id, :email_id, :postscript, :platform_name, :provider_name, :URL, :hotline, :category_id, :sys_user_id, :created_time, :updated_time)");
    sqlite3_stmt* stmt = nullptr;

    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    const std::string new_account_id = utils::UUIDGenerator::generate(); // 生成id主键, NOT NULL
    sqlite3_bind_text(stmt, 1, new_account_id.c_str(), static_cast<int>(new_account_id.size()), SQLITE_TRANSIENT);
    if (account.getUsername().empty())
        sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, account.getUsername().c_str(), static_cast<int>(account.getUsername().size()), SQLITE_TRANSIENT);
    if (account.getNickname().empty())
        sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, account.getNickname().c_str(), static_cast<int>(account.getNickname().size()), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 4, account.getEncryptedPwd().data(), static_cast<int>(account.getEncryptedPwd().size()), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 5, account.getIv().data(), static_cast<int>(account.getIv().size()), SQLITE_TRANSIENT);
    if (account.getSubAccount().empty()) sqlite3_bind_null(stmt, 6);
    else sqlite3_bind_text(stmt, 6, account.getSubAccount().c_str(), static_cast<int>(account.getSubAccount().size()), SQLITE_TRANSIENT);
    if (account.getPhoneId() < 0) sqlite3_bind_null(stmt, 7);
    else sqlite3_bind_int(stmt, 7, account.getPhoneId());
    if (account.getEmailId().empty()) sqlite3_bind_null(stmt, 8);
    else sqlite3_bind_text(stmt, 8, account.getEmailId().c_str(), static_cast<int>(account.getEmailId().size()), SQLITE_TRANSIENT);
    if (account.getPostscript().empty())
        sqlite3_bind_null(stmt, 9);
    else sqlite3_bind_text(stmt, 9, account.getPostscript().c_str(), static_cast<int>(account.getPostscript().size()), SQLITE_TRANSIENT);
    if (account.getPlatformName().empty())
        sqlite3_bind_null(stmt, 10);
    else sqlite3_bind_text(stmt, 10, account.getPlatformName().c_str(), static_cast<int>(account.getPlatformName().size()), SQLITE_TRANSIENT);
    if (account.getProviderName().empty())
        sqlite3_bind_null(stmt, 11);
    else sqlite3_bind_text(stmt, 11, account.getProviderName().c_str(), static_cast<int>(account.getProviderName().size()), SQLITE_TRANSIENT);
    if (account.getURL().empty())
        sqlite3_bind_null(stmt, 12);
    else sqlite3_bind_text(stmt, 12, account.getURL().c_str(), static_cast<int>(account.getURL().size()), SQLITE_TRANSIENT);
    if (account.getHotline().empty())
        sqlite3_bind_null(stmt, 13);
    else sqlite3_bind_text(stmt, 13, account.getHotline().c_str(), static_cast<int>(account.getHotline().size()), SQLITE_TRANSIENT);
    if (account.getCategoryId() < 0) sqlite3_bind_null(stmt, 14);
    else sqlite3_bind_int(stmt, 14, account.getCategoryId());
    if (account.getSysUserId().empty())
        sqlite3_bind_null(stmt, 15);
    else sqlite3_bind_text(stmt, 15, account.getSysUserId().c_str(), static_cast<int>(account.getSysUserId().size()), SQLITE_TRANSIENT);
    if (account.getCreatedTime().empty())
        sqlite3_bind_null(stmt, 16);
    else sqlite3_bind_text(stmt, 16, account.getCreatedTime().c_str(), static_cast<int>(account.getCreatedTime().size()), SQLITE_TRANSIENT);
    if (account.getUpdatedTime().empty())
        sqlite3_bind_null(stmt, 17);
    else sqlite3_bind_text(stmt, 17, account.getUpdatedTime().c_str(), static_cast<int>(account.getUpdatedTime().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if ( SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to insert account record: ") + std::string(sqlite3_errmsg(conn)));
        status = DaoStatus::DuplicateKey;
    } else {
        utils::LogManager::AddLog(std::string("[OK] Account inserted successfully."));
        status = DaoStatus::Success;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

DaoStatus AccountDAO::add(const models::ViewAccount& view_account) const {
    DaoStatus status;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "INSERT INTO account(id,username,nickname,encrypted_pwd,iv,sub_account,postscript,platform_name,provider_name,URL,hotline,sys_user_id,created_time,updated_time,phone_id,email_id,category_id) "
                            "SELECT ?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14,"
                            "(SELECT id FROM mobile_phone WHERE phone_number = ?15),"
                            "(SELECT id FROM view_email WHERE username = ?16),"
                            "(SELECT id FROM account_category WHERE name = ?17);";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    const std::string new_account_id = utils::UUIDGenerator::generate(); // 生成id主键, NOT NULL
    sqlite3_bind_text(stmt, 1, new_account_id.c_str(), static_cast<int>(new_account_id.size()), SQLITE_TRANSIENT);
    if (view_account.getUsername().empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, view_account.getUsername().c_str(), static_cast<int>(view_account.getUsername().size()), SQLITE_TRANSIENT);
    if (view_account.getNickname().empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, view_account.getNickname().c_str(), static_cast<int>(view_account.getNickname().size()), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 4, view_account.getEncryptedPwd().data(), static_cast<int>(view_account.getEncryptedPwd().size()), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 5, view_account.getIv().data(), static_cast<int>(view_account.getIv().size()), SQLITE_TRANSIENT);
    if (view_account.getSubAccount().empty()) sqlite3_bind_null(stmt, 6);
    else sqlite3_bind_text(stmt, 6, view_account.getSubAccount().c_str(), static_cast<int>(view_account.getSubAccount().size()), SQLITE_TRANSIENT);
    if (view_account.getPostscript().empty()) sqlite3_bind_null(stmt, 7);
    else sqlite3_bind_text(stmt, 7, view_account.getPostscript().c_str(), static_cast<int>(view_account.getPostscript().size()), SQLITE_TRANSIENT);
    if (view_account.getPlatformName().empty()) sqlite3_bind_null(stmt, 8);
    else sqlite3_bind_text(stmt, 8, view_account.getPlatformName().c_str(), static_cast<int>(view_account.getPlatformName().size()), SQLITE_TRANSIENT);
    if (view_account.getProviderName().empty()) sqlite3_bind_null(stmt, 9);
    else sqlite3_bind_text(stmt, 9, view_account.getProviderName().c_str(), static_cast<int>(view_account.getProviderName().size()), SQLITE_TRANSIENT);
    if (view_account.getURL().empty()) sqlite3_bind_null(stmt, 10);
    else sqlite3_bind_text(stmt, 10, view_account.getURL().c_str(), static_cast<int>(view_account.getURL().size()), SQLITE_TRANSIENT);
    if (view_account.getHotline().empty()) sqlite3_bind_null(stmt, 11);
    else sqlite3_bind_text(stmt, 11, view_account.getHotline().c_str(), static_cast<int>(view_account.getHotline().size()), SQLITE_TRANSIENT);
    if (view_account.getSysUserId().empty()) sqlite3_bind_null(stmt, 12);
    else sqlite3_bind_text(stmt, 12, view_account.getSysUserId().c_str(), static_cast<int>(view_account.getSysUserId().size()), SQLITE_TRANSIENT);
    if (view_account.getCreatedTime().empty()) sqlite3_bind_null(stmt, 13);
    else sqlite3_bind_text(stmt, 13, view_account.getCreatedTime().c_str(), static_cast<int>(view_account.getCreatedTime().size()), SQLITE_TRANSIENT);
    if (view_account.getUpdatedTime().empty()) sqlite3_bind_null(stmt, 14);
    else sqlite3_bind_text(stmt, 14, view_account.getUpdatedTime().c_str(), static_cast<int>(view_account.getUpdatedTime().size()), SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, 15, view_account.getPhone().c_str(), static_cast<int>(view_account.getPhone().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 16, view_account.getEmail().c_str(), static_cast<int>(view_account.getEmail().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 17, view_account.getCategory().c_str(), static_cast<int>(view_account.getCategory().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if ( SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to insert account record: ") + std::string(sqlite3_errmsg(conn)));
        status = DaoStatus::DuplicateKey;
    } else {
        utils::LogManager::AddLog(std::string("[OK] Account inserted successfully."));
        status = DaoStatus::Success;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

/**
 * @brief 修改账号记录（ 以 view_account.getId() 为目标 ）
 */
DaoStatus AccountDAO::update(const models::ViewAccount& view_account) const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "UPDATE account SET"
                            "  username = ?1,"
                            "  nickname = ?2,"
                            "  encrypted_pwd = ?3,"
                            "  iv = ?4,"
                            "  sub_account = ?5,"
                            "  phone_id = (SELECT id FROM mobile_phone WHERE phone_number = ?6),"
                            "  email_id = (SELECT id FROM view_email WHERE username = ?7),"
                            "  postscript = ?8,"
                            "  platform_name = ?9,"
                            "  provider_name = ?10,"
                            "  URL = ?11,"
                            "  hotline = ?12,"
                            "  category_id = (SELECT id FROM account_category WHERE name = ?13),"
                            "  updated_time = ?14 WHERE id = ?15 AND sys_user_id = ?16;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (view_account.getUsername().empty())
        sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, view_account.getUsername().c_str(), static_cast<int>(view_account.getUsername().size()), SQLITE_TRANSIENT);
    if (view_account.getNickname().empty())
        sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, view_account.getNickname().c_str(), static_cast<int>(view_account.getNickname().size()), SQLITE_TRANSIENT);
    if (view_account.getEncryptedPwd().empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] Encrypted password is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 3, view_account.getEncryptedPwd().data(), static_cast<int>(view_account.getEncryptedPwd().size()), SQLITE_TRANSIENT);
    if (view_account.getIv().empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] IV is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 4, view_account.getIv().data(), static_cast<int>(view_account.getIv().size()), SQLITE_TRANSIENT);
    if (view_account.getSubAccount().empty())
        sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, view_account.getSubAccount().c_str(), static_cast<int>(view_account.getSubAccount().size()), SQLITE_TRANSIENT);
    if (view_account.getPhone().empty()) sqlite3_bind_null(stmt, 6);
    else sqlite3_bind_text(stmt, 6, view_account.getPhone().c_str(), static_cast<int>(view_account.getPhone().size()), SQLITE_TRANSIENT);
    if (view_account.getEmail().empty()) sqlite3_bind_null(stmt, 7);
    else sqlite3_bind_text(stmt, 7, view_account.getEmail().c_str(), static_cast<int>(view_account.getEmail().size()), SQLITE_TRANSIENT);
    if (view_account.getPostscript().empty())
        sqlite3_bind_null(stmt, 8);
    else sqlite3_bind_text(stmt, 8, view_account.getPostscript().c_str(), static_cast<int>(view_account.getPostscript().size()), SQLITE_TRANSIENT);
    if (view_account.getPlatformName().empty())
        sqlite3_bind_null(stmt, 9);
    else sqlite3_bind_text(stmt, 9, view_account.getPlatformName().c_str(), static_cast<int>(view_account.getPlatformName().size()), SQLITE_TRANSIENT);
    if (view_account.getProviderName().empty())
        sqlite3_bind_null(stmt, 10);
    else sqlite3_bind_text(stmt, 10, view_account.getProviderName().c_str(), static_cast<int>(view_account.getProviderName().size()), SQLITE_TRANSIENT);
    if (view_account.getURL().empty()) sqlite3_bind_null(stmt, 11);
    else sqlite3_bind_text(stmt, 11, view_account.getURL().c_str(), static_cast<int>(view_account.getURL().size()), SQLITE_TRANSIENT);
    if (view_account.getHotline().empty())
        sqlite3_bind_null(stmt, 12);
    else sqlite3_bind_text(stmt, 12, view_account.getHotline().c_str(), static_cast<int>(view_account.getHotline().size()), SQLITE_TRANSIENT);
    if (view_account.getCategory().empty())
        sqlite3_bind_null(stmt, 13);
    else sqlite3_bind_text(stmt, 13, view_account.getCategory().c_str(), static_cast<int>(view_account.getCategory().size()), SQLITE_TRANSIENT);
    if (view_account.getUpdatedTime().empty()) {
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 14, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    }else {
        sqlite3_bind_text(stmt, 14, view_account.getUpdatedTime().c_str(), static_cast<int>(view_account.getUpdatedTime().size()), SQLITE_TRANSIENT);
    }
    SQLDebug::log_sql(stmt, true);
    if (view_account.getId().empty()) {
        utils::LogManager::AddLog(std::string("[INVALID DATA] 不合法主键。主键长度为0."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 15, view_account.getId().c_str(), static_cast<int>(view_account.getId().size()), SQLITE_TRANSIENT);
    if (view_account.getSysUserId().empty()) {
        utils::LogManager::AddLog(std::string("[INVALID DATA] SysUserId is NULL."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 16, view_account.getSysUserId().c_str(), static_cast<int>(view_account.getSysUserId().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);

    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + view_account.getId());
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改账号记录但不包括 encrypted_pwd 和 iv
 */
DaoStatus AccountDAO::update_main_properties(const models::ViewAccount& view_account) const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "UPDATE account SET "
                            "  username = ?1,"
                            "  nickname = ?2,"
                            "  sub_account = ?3,"
                            "  phone_id = (SELECT id FROM mobile_phone WHERE phone_number = ?4),"
                            "  email_id = (SELECT id FROM view_email WHERE username = ?5),"
                            "  postscript = ?6,"
                            "  platform_name = ?7,"
                            "  provider_name = ?8,"
                            "  URL = ?9,"
                            "  hotline = ?10,"
                            "  category_id = (SELECT id FROM account_category WHERE name = ?11),"
                            "  updated_time = ?12 WHERE id = ?13 AND sys_user_id = ?14";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    if (view_account.getUsername().empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, view_account.getUsername().c_str(), static_cast<int>(view_account.getUsername().size()), SQLITE_TRANSIENT);
    if (view_account.getNickname().empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, view_account.getNickname().c_str(), static_cast<int>(view_account.getNickname().size()), SQLITE_TRANSIENT);
    if (view_account.getSubAccount().empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, view_account.getSubAccount().c_str(), static_cast<int>(view_account.getSubAccount().size()), SQLITE_TRANSIENT);
    if (view_account.getPhone().empty()) sqlite3_bind_null(stmt, 4);
    else sqlite3_bind_text(stmt, 4, view_account.getPhone().c_str(), static_cast<int>(view_account.getPhone().size()), SQLITE_TRANSIENT);
    if (view_account.getEmail().empty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, view_account.getEmail().c_str(), static_cast<int>(view_account.getEmail().size()), SQLITE_TRANSIENT);
    if (view_account.getPostscript().empty()) sqlite3_bind_null(stmt, 6);
    else sqlite3_bind_text(stmt, 6, view_account.getPostscript().c_str(), static_cast<int>(view_account.getPostscript().size()), SQLITE_TRANSIENT);
    if (view_account.getPlatformName().empty()) sqlite3_bind_null(stmt, 7);
    else sqlite3_bind_text(stmt, 7, view_account.getPlatformName().c_str(), static_cast<int>(view_account.getPlatformName().size()), SQLITE_TRANSIENT);
    if (view_account.getProviderName().empty()) sqlite3_bind_null(stmt, 8);
    else sqlite3_bind_text(stmt, 8, view_account.getProviderName().c_str(), static_cast<int>(view_account.getProviderName().size()), SQLITE_TRANSIENT);
    if (view_account.getURL().empty()) sqlite3_bind_null(stmt, 9);
    else sqlite3_bind_text(stmt, 9, view_account.getURL().c_str(), static_cast<int>(view_account.getURL().size()), SQLITE_TRANSIENT);
    if (view_account.getHotline().empty()) sqlite3_bind_null(stmt, 10);
    else sqlite3_bind_text(stmt, 10, view_account.getHotline().c_str(), static_cast<int>(view_account.getHotline().size()), SQLITE_TRANSIENT);
    if (view_account.getCategory().empty()) sqlite3_bind_null(stmt, 11);
    else sqlite3_bind_text(stmt, 11, view_account.getCategory().c_str(), static_cast<int>(view_account.getCategory().size()), SQLITE_TRANSIENT);
    if (view_account.getUpdatedTime().empty()) {
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 12, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    }
    else {
        sqlite3_bind_text(stmt, 12, view_account.getUpdatedTime().c_str(), static_cast<int>(view_account.getUpdatedTime().size()), SQLITE_TRANSIENT);
    }
    if (view_account.getId().empty()) {
        utils::LogManager::AddLog(std::string("[INVALID DATA] 不合法主键。主键长度为0."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 13, view_account.getId().c_str(), static_cast<int>(view_account.getId().size()), SQLITE_TRANSIENT);
    if (view_account.getSysUserId().empty()) {
        utils::LogManager::AddLog(std::string("[INVALID DATA] SysUserId is NULL."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 14, view_account.getSysUserId().c_str(), static_cast<int>(view_account.getSysUserId().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);

    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }
    // utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + std::to_string(view_account.getId()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改账号记录的密码（密文密码、初始化向量、更新时间）
 */
DaoStatus AccountDAO::update_password(const std::string& id,
    const std::string& sys_user_id,
    const std::vector<unsigned char>& encrypted_pwd,
    const std::vector<unsigned char>& iv,
    const std::string& update_time) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE account SET encrypted_pwd = ?, iv = ?, updated_time = ? WHERE id = ? AND sys_user_id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (encrypted_pwd.empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] Encrypted password is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 1, encrypted_pwd.data(), static_cast<int>(encrypted_pwd.size()), SQLITE_TRANSIENT);
    if (iv.empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] IV is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 2, iv.data(), static_cast<int>(iv.size()), SQLITE_TRANSIENT);
    if (update_time.empty()){
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 3, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 3, update_time.c_str(), static_cast<int>(update_time.size()), SQLITE_TRANSIENT);
    }
    sqlite3_bind_text(stmt, 4, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改Account表中的平台信息（1/4）
 */
__attribute__((optimize("O0"))) // 禁用优化
DaoStatus AccountDAO::update_platform(
    const std::string& id,
    const std::string& provider_name,
    const std::string& platform_name,
    const std::string& url,
    const std::string& hotline,
    const std::string& update_time) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE account SET provider_name = ?, platform_name = ?, URL = ?, hotline = ?, updated_time = ? WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (provider_name.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, provider_name.c_str(), static_cast<int>(provider_name.size()), SQLITE_TRANSIENT);
    if (platform_name.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, platform_name.c_str(), static_cast<int>(platform_name.size()), SQLITE_TRANSIENT);
    if (url.empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, url.c_str(), static_cast<int>(url.size()), SQLITE_TRANSIENT);
    if (hotline.empty()) sqlite3_bind_null(stmt, 4);
    else sqlite3_bind_text(stmt, 4, hotline.c_str(), static_cast<int>(hotline.size()), SQLITE_TRANSIENT);
    if (update_time.empty()){
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 5, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    } else {
      sqlite3_bind_text(stmt, 5, update_time.c_str(), static_cast<int>(update_time.size()), SQLITE_TRANSIENT);
    }
    sqlite3_bind_text(stmt, 6, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改Account表中的账号信息（2/4）
 */
__attribute__((optimize("O0"))) // 禁用优化
DaoStatus AccountDAO::update_user(
    const std::string& id,
    const std::string& username,
    const std::string& nickname,
    const std::vector<unsigned char>& encrypted_pwd,
    const std::vector<unsigned char>& iv,
    const short category_id,
    const std::string& update_time) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE account SET username = ?, nickname = ?, encrypted_pwd = ?, iv = ?, category_id = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (username.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, username.c_str(), static_cast<int>(username.size()), SQLITE_TRANSIENT);
    if (nickname.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, nickname.c_str(), static_cast<int>(nickname.size()), SQLITE_TRANSIENT);
    if (encrypted_pwd.empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] Encrypted password is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 3, encrypted_pwd.data(), static_cast<int>(encrypted_pwd.size()), SQLITE_TRANSIENT);

    if (iv.empty()) {
        utils::LogManager::AddLog(std::string("[ERROR] IV is empty."));
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_blob(stmt, 4, iv.data(), static_cast<int>(iv.size()), SQLITE_TRANSIENT);
    if (category_id < 0) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_int(stmt, 5, category_id);
    if (update_time.empty()){
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 6, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 6, update_time.c_str(), static_cast<int>(update_time.size()), SQLITE_TRANSIENT);
    }
    sqlite3_bind_text(stmt, 7, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改Account表中的第三方绑定账号信息（3/4）
 */
__attribute__((optimize("O0"))) // 禁用优化
DaoStatus AccountDAO::update_third(
    const std::string& id,
    const int phone_id,
    const std::string& email_id,
    const std::string& update_time) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE account SET phone_id = ?, email_id = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (phone_id < 0) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_int(stmt, 1, phone_id);
    if (email_id.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, email_id.c_str(), static_cast<int>(email_id.size()), SQLITE_TRANSIENT);
    if (update_time.empty()){
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 3, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    } else {sqlite3_bind_text(stmt, 3, update_time.c_str(), static_cast<int>(update_time.size()), SQLITE_TRANSIENT);}
    sqlite3_bind_text(stmt, 4, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

/**
 * @brief 修改Account表中的其他信息（4/4）
 */
__attribute__((optimize("O0"))) // 禁用优化
DaoStatus AccountDAO::update_other(
    const std::string& id,
    const std::string& sub_account,
    const std::string& postscript,
    const std::string& update_time) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE account SET sub_account = ?, postscript = ?, updated_time = ? WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (sub_account.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, sub_account.c_str(), static_cast<int>(sub_account.size()), SQLITE_TRANSIENT);
    if (postscript.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, postscript.c_str(), static_cast<int>(postscript.size()), SQLITE_TRANSIENT);
    if (update_time.empty()) {
        const std::string now = utils::DateTime::getCurrentDateTime();
        sqlite3_bind_text(stmt, 3, now.c_str(), static_cast<int>(now.size()), SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 3, update_time.c_str(), static_cast<int>(update_time.size()), SQLITE_TRANSIENT);
    }
    sqlite3_bind_text(stmt, 4, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

// 此版本方法有很多BUG，不要使用
DaoStatus AccountDAO::update(
    const std::vector<KeyValuePair>& data,
    const std::string& id) const{
    sqlite3* conn = pool_.get_connection();

    std::stringstream sqlStream;
    sqlStream << "UPDATE account SET ";
    bool first = true;
    std::vector<int> textIndices;
    std::vector<int> intIndices;
    for (const auto& kv : data) {
        if (!first) {
            sqlStream << ", ";
        }
        switch (kv.key) {
        case ColumnType::username: {
            sqlStream << "username =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::nickname: {
            sqlStream << "nickname =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::encryptedPwd: {
            sqlStream << "encrypted_pwd =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::subAccount: {
            sqlStream << "sub_account =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::phoneId: {
            sqlStream << "phone_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::emailId: {
            sqlStream << "email_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::postscript: {
            sqlStream << "postscript =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::platformName: {
            sqlStream << "platform_name =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::providerName: {
            sqlStream << "provider_name =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::URL: {
            sqlStream << "URL =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::hotline: {
            sqlStream << "hotline =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::categoryId: {
            sqlStream << "category_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::owner: {
            sqlStream << "sys_user_id =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::updatedTime: {
            sqlStream << "updated_time =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        default: {
            // 可以考虑更合适的处理方式，这里暂时不做任何操作
            break;
        }
        }
        first = false;
    }
    sqlStream << " WHERE id =?";

    const std::string sql = sqlStream.str();
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    for (const int index : textIndices) {
        for (const auto& [key, value] : data) {
            switch (key) {
            case ColumnType::username: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                // 可以添加错误处理，例如类型不匹配的情况
                break;
            }
            case ColumnType::nickname: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::encryptedPwd: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::subAccount: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::postscript: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::platformName: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::providerName: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::URL: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::hotline: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::owner: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            case ColumnType::updatedTime: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), static_cast<int>(std::get<std::string>(value).size()), SQLITE_TRANSIENT);
                    break;
                }
                break;
            }
            default: {
                // 可以考虑更合适的处理方式，这里暂时不做任何操作
                break;
            }
            }
        }
    }

    for (const int index : intIndices) {
        for (const auto& [key, value] : data) {
            switch (key) {
            case ColumnType::phoneId: {
                if (std::holds_alternative<int>(value)) {
                    sqlite3_bind_int(stmt, index, std::get<int>(value));
                    break;
                }
                // 可以添加错误处理，例如类型不匹配的情况
                break;
            }
            case ColumnType::emailId: {
                if (std::holds_alternative<int>(value)) {
                    sqlite3_bind_int64(stmt, index, std::get<int>(value));
                    break;
                }
                break;
            }
            default: {
                // 可以考虑更合适的处理方式，这里暂时不做任何操作
                break;
            }
            }
        }
    }

    sqlite3_bind_text(stmt, static_cast<int>(textIndices.size() + intIndices.size() + 1), id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account updated successfully: ") + id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus AccountDAO::remove(const std::string& id) const {
    if (id.empty()) {
        throw std::invalid_argument("AccountDAO::remove: Empty ID");
    }

    sqlite3* conn = pool_.get_connection();

    const std::string sql = "DELETE FROM account WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);

    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to remove record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }

    utils::LogManager::AddLog(std::string("[OK] Account removed successfully: ") +id);
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

// Email 部分（特殊的 Account）
DaoStatus AccountDAO::findEmailList(
    const std::string& sys_user_id,
    std::vector<std::string>& email_addresses) const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT username FROM view_email WHERE sys_user_id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        email_addresses.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    utils::LogManager::AddLog(std::string("[OK] Number of email records found: ") + std::to_string(email_addresses.size()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

std::string AccountDAO::getIdByEmailAddress(const std::string& email_address) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id FROM view_email WHERE username = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") +
          std::string(sqlite3_errmsg(conn)));
      sqlite3_finalize(stmt);
      pool_.release_connection(conn);
      return "";
    }
    sqlite3_bind_text(stmt, 1, email_address.c_str(),
                      static_cast<int>(email_address.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Email(Account) ID not found with the given emailAddress in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return "";
    }

    std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    utils::LogManager::AddLog("[OK] Account found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return id;
}

// 根据 id 查询目标 email 记录
DaoStatus AccountDAO::findEmailById(const std::string& id, models::ViewAccount& email) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT * FROM view_email WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT); // NOT NULL
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Email(Account) not found with the given ID in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    }
    SQLDebug::log_sql(stmt, true);
    DaoStatus status;
    if (SQLITE_ROW == sqlite3_step(stmt)) {
        email = ViewAccountFromStatement(stmt);
        utils::LogManager::AddLog("[OK] Account found.");
        status = DaoStatus::Success;
    }
    else {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Email(Account) not found with the given ID in the database."));
        status = DaoStatus::NotFound;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

// 查询所有 email 记录
DaoStatus AccountDAO::findEmail(const std::string& sys_user_id, std::vector<models::ViewAccount>& emails) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT * FROM view_email WHERE sys_user_id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        emails.push_back(ViewAccountFromStatement(stmt));
    }

    utils::LogManager::AddLog("[OK] Account found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

}