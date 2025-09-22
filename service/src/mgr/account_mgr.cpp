#include "mgr/account_mgr.h"
#include "repo/account_dao.h"
#include "repo/category_dao.h"
#include "utils/date_time.h"
#include "repo/phone_dao.h"
#include "utils/pwd_utils.h"
#include <stdexcept>

#include "utils/log_manager.h"

namespace zinpass::business{

std::vector<unsigned char> AccountManager::main_key_to_real_key(const std::string& main_key){
    const std::string realKeyHex = utils::PwdUtils::calculateMD5(main_key);
    const std::vector<unsigned char> realKey = utils::PwdUtils::hexStringToKey(realKeyHex);
    return realKey;
}

AccountManager::Return<std::vector<std::string>> AccountManager::get_email_options(const std::string& sys_user_id) {
    std::vector<std::string> email_addresses;
    std::string message;
    try {
        repo::AccountDAO email_dao;
        if (const repo::DaoStatus dao_ret = email_dao.findEmailList(sys_user_id, email_addresses);
            dao_ret == repo::DaoStatus::Success)
        {
            message = "成功";
        } else if (dao_ret == repo::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {email_addresses, message};
}

AccountManager::Return<std::vector<std::string>> AccountManager::get_category_options() {
    std::vector<std::string> categories;
    std::string message;
    try {
        repo::CategoryDAO category_dao;
        if (const repo::DaoStatus dao_ret = category_dao.findCategories(categories);
            dao_ret == repo::DaoStatus::Success)
        {
            message = "成功";
        } else if (dao_ret == repo::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {categories, message};
}

AccountManager::Return<std::vector<models::ViewAccount>> AccountManager::get_accounts(
    const std::string& provider_name,
    const std::string& platform_name,
    const std::string& username,
    const std::string& nickname,
    const std::string& telephone,
    const std::string& email,
    const std::string& category,
    const std::string& postscript,
    const std::string& sys_user_id
) {
    std::vector<models::ViewAccount> view_accounts;
    std::string message;
    try {
        repo::AccountDAO account_dao;
        if (const repo::DaoStatus dao_ret = account_dao.find(provider_name,platform_name,
            username,nickname,telephone,email,category,postscript,sys_user_id,view_accounts);
            dao_ret == repo::DaoStatus::Success) {
            message = "成功";
        } else if (dao_ret == repo::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {view_accounts, message};
}

AccountManager::Return<models::ViewAccount> AccountManager::get_account_by_id(const std::string& account_id) {
    models::ViewAccount view_account;
    std::string message;
    const repo::AccountDAO account_dao;
    if (const repo::DaoStatus ret = account_dao.findById(account_id, view_account);
        repo::DaoStatus::Success != ret
        ) {
        message = "获取失败: " + std::to_string(static_cast<int>(ret));
    }else {
        message = "成功";
    }
    return {view_account, message};
}

AccountManager::Return<std::string> AccountManager::retrieve_decrypted_password(const std::string& account_id, const std::string& sys_user_id, const std::string& main_key) {
    // 1. 从数据库读取数据
    std::vector<unsigned char> ciphertext, iv;
    const repo::AccountDAO account_dao;
    account_dao.findEncryptedPwdAndIv(account_id, sys_user_id, ciphertext, iv);
    if (ciphertext.empty()) {
        return {"查询目标不存在", ""};
    }

    // 2. 解密
    Return<std::string> ret;
    if(std::string plaintext;
        utils::PwdUtils::decryptTextByAES128CTR(ciphertext, iv, main_key_to_real_key(main_key), plaintext))
    {
        ret.data = plaintext;
        ret.message = "成功";
    }else{
        ret.data = "";
        ret.message = "解密失败";
    }
    return ret;
}

[[deprecated("This function is obsolete. Use add_view_account() instead.")]]
AccountManager::Return<bool> AccountManager::add_account(
    const std::string& main_key,
    const std::string& username,
    const std::string& nickname,
    const std::string& password,
    const std::string& sub_account,
    const std::string& phone,
    const std::string& email,
    const std::string& postscript,
    const std::string& platform,
    const std::string& provider,
    const std::string& url,
    const std::string& hotline,
    const std::string& sys_user_id,
    const std::string& category){
    // 0. 定义变量

    // 1. 校验参数数据合法性

    // 2. 加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        password,                               // 原明文密码
        main_key_to_real_key(main_key),   // 由主密钥生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (!ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    // 3. 将 phone -> phone_id , 将 email -> email_id , 将 category -> category_id， 获取时间
    int phone_id = -1;
    std::string email_id;
    short category_id = -1;

    if (phone != "无" && phone != "空" && phone != "NULL") {
        const repo::MobilePhoneDAO phone_dao;
        if (const int temp_phone_id = phone_dao.find_id_by_number(phone);
            temp_phone_id >= 0) {
            phone_id = temp_phone_id;
        }
    }

    if (email != "无" && email != "空" && email != "NULL") {
        const repo::AccountDAO email_dao;
        email_id = email_dao.getIdByEmailAddress(email);
    }

    const repo::CategoryDAO category_dao;
    short temp_category_id;
    if (const repo::DaoStatus ret_category_dao = category_dao.categoryNameToId(category, &temp_category_id);
        repo::DaoStatus::Success == ret_category_dao) {
        category_id = temp_category_id;
    }

    const std::string current_date_time = utils::DateTime::getCurrentDateTime();

    // 4. 创建对象并写入到数据库中
    const models::Account account(
        "",                  // 底层函数会自动分配主键
        username,           // 参数
        nickname,           // 参数
        sub_account,        // 参数
        postscript,         // 参数
        platform,           // 参数
        provider,           // 参数
        url,                // 参数
        hotline,            // 参数
        sys_user_id,           // 参数
        current_date_time,  // 当前时间
        current_date_time,  // 当前时间
        ciphertext,         // 已生成
        initialization_vector,// 已生成
        phone_id,
        email_id,
        category_id
        );

    const repo::AccountDAO account_dao;
    if (repo::DaoStatus ret_account_dao = account_dao.add(account);
        repo::DaoStatus::Success != ret_account_dao
        ) {
        return {false, "失败" + std::to_string(static_cast<int>(ret_account_dao))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::add_view_account(
    const std::string& main_key,
    models::ViewAccount& view_account,
    const std::string& plaintext_pwd){
    // 0. 定义变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();

    // 1. 校验参数数据合法性
    if (plaintext_pwd.empty()) {
        return {false, "密码不得为空"};
    }

    // 2. 加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_key_to_real_key(main_key),   // 由主密钥生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (!ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    // 3. 更新对象部分属性并写入到数据库中
    view_account.setEncryptedPwd(ciphertext);
    view_account.setIv(initialization_vector);
    view_account.setCreatedTime(current_date_time);
    view_account.setUpdatedTime(current_date_time);

    const repo::AccountDAO account_dao;
    if (repo::DaoStatus ret_account_dao = account_dao.add(view_account);
        repo::DaoStatus::Success != ret_account_dao
        ) {
        return {false, "失败" + std::to_string(static_cast<int>(ret_account_dao))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account(
    const models::ViewAccount& view_account,
    const std::string& plaintext_pwd,
    const std::string& main_key){
    // 0. 定义变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    repo::AccountDAO account_dao;
    models::ViewAccount new_view_account(view_account);

    // 1. 检查输入数据的合法性
    if (view_account.getId().empty()) {
        return {false, "因不合法主键中断更新账号数据，主键长度为0"};
    }

    // 2. 新密码：加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_key_to_real_key(main_key),   // 由主密钥生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (!ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }
    new_view_account.setEncryptedPwd(ciphertext);
    new_view_account.setIv(initialization_vector);
    new_view_account.setUpdatedTime(current_date_time);

    if (const repo::DaoStatus ret_account_dao_2 = account_dao.update(new_view_account);
        repo::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account_base_info(const models::ViewAccount& view_account){
    // 0. 定义变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    const repo::AccountDAO account_dao;

    // 1. 检查输入数据的合法性
    if (view_account.getId().empty()) {
        return {false, "因不合法主键中断更新账号数据，主键长度为0"};
    }

    if (const repo::DaoStatus ret_account_dao_2 = account_dao.update_main_properties(view_account);   // 忽略 encrypted_pwd 和 iv
        repo::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account_password(
    const std::string& account_id,
    const std::string& sys_user_id,
    const std::string& main_key,
    const std::string& plaintext_pwd){

    // 0. 定义变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    const repo::AccountDAO account_dao;

    // 1. 检查输入数据的合法性
    if (main_key.empty()) {
        return {false, "主密钥不能为空"};
    }
    if (plaintext_pwd.empty()) {
        return {false, "账号新密码不能为空"};
    }

    // 2. 新密码：加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_key_to_real_key(main_key),         // 由主密钥生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (!ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    if (const repo::DaoStatus ret_account_dao_2 = account_dao.update_password(account_id, sys_user_id, ciphertext, initialization_vector, current_date_time);
        repo::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

bool AccountManager::re_encrypt_password(
    const std::string& account_id,
    const std::string& sys_user_id,
    const std::string& old_main_key,
    const std::string& new_main_key){
    // 0. 检查输入数据的合法性、定义变量
    if (account_id.empty() || sys_user_id.empty() || old_main_key.empty() || new_main_key.empty()) {
        return false;
    }
    const std::string now = utils::DateTime::getCurrentDateTime();
    const repo::AccountDAO account_dao;
    std::vector<unsigned char> old_ec_pwd, old_iv;

    // 1. 解密旧密码
    if (const repo::DaoStatus ret_find1 = account_dao.findEncryptedPwdAndIv(account_id, sys_user_id, old_ec_pwd, old_iv);
        repo::DaoStatus::Success != ret_find1) {
        return false;
    }
    std::string plaintext_pwd;                 // 解密后的明文密码
    const std::vector<unsigned char> old_real_key = utils::PwdUtils::hexStringToKey(utils::PwdUtils::calculateMD5(old_main_key));
    if (const bool ret_decrypt = utils::PwdUtils::decryptTextByAES128CTR(old_ec_pwd, old_iv, old_real_key, plaintext_pwd);
        !ret_decrypt) {
        return false;    // 解密失败
        }

    // 2. 加密新密码
    const std::vector<unsigned char> new_real_key = utils::PwdUtils::hexStringToKey(utils::PwdUtils::calculateMD5(new_main_key));
    std::vector<unsigned char> new_ec_pwd;
    std::vector<unsigned char> new_iv;
    if (const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(plaintext_pwd, new_real_key, new_ec_pwd, new_iv);
        !ret_encrypt) {
        return false;    // 加密失败
        }
    utils::PwdUtils::encryptTextByAES128CTR(plaintext_pwd, new_real_key, new_ec_pwd, new_iv);

    // 3. 更新数据库中密码和初始化向量
    if (const repo::DaoStatus ret_update = account_dao.update_password(account_id, sys_user_id, new_ec_pwd, new_iv, now);
        repo::DaoStatus::Success != ret_update) {
        return false;    // 更新失败
    }

    return true;
}

std::tuple<int, int, std::vector<std::string>> AccountManager::re_encrypt_password_all(const std::string& sys_user_id, const std::string& old_main_key, const std::string& new_main_key) {
    // 0. 检查输入数据的合法性、定义变量
    if(sys_user_id.empty() || old_main_key.empty() || new_main_key.empty()) {
        return {0, 0, std::vector<std::string>()};
    }
    const std::string now = utils::DateTime::getCurrentDateTime();
    const repo::AccountDAO account_dao;

    // 1. 查询所有 account_id
    const std::vector<std::string> id_list = account_dao.list_id(sys_user_id);
    std::vector<std::string> failed_list;
    int total_count = id_list.size();
    int success_count = 0;

    // 2. 遍历重加密
    for (const auto& id : id_list) {
        // 2.1 使用 old_main_key 解密
        const auto [plaintext_pwd, msg_retrieve] = retrieve_decrypted_password(id, sys_user_id, old_main_key);
        if (plaintext_pwd.empty()) {
            failed_list.push_back(id);
            continue;
        }

        // 2.2 使用 new_main_key 加密并更新
        const auto [ret_update, msg_update] = update_account_password(id, sys_user_id, new_main_key, plaintext_pwd);
        if (!ret_update) {
            failed_list.push_back(id);
            continue;
        }
        success_count++;
    }

    return {total_count, success_count, failed_list};
}


AccountManager::Return<bool> AccountManager::delete_account(const std::string& account_id) {
    try {
        const repo::AccountDAO account_dao;
        const repo::DaoStatus ret_account_dao = account_dao.remove(account_id);
        if (repo::DaoStatus::Success != ret_account_dao) {
            return {false, "删除失败：" + std::to_string(static_cast<int>(ret_account_dao))};
        }
        return {true, "删除成功"};
    } catch (const std::invalid_argument& e) {
        utils::LogManager::AddLog(std::string("[ERROR] Invalid argument when deleting account: ") + e.what());
    } catch (const std::exception& e) {
        utils::LogManager::AddLog(std::string("[ERROR] Exception when deleting account: ") + e.what());
    }
    return {false, "删除失败"};
}

} // zinpass::mgr