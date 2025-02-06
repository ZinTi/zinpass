#ifndef ACCOUNT_DAO_H
#define ACCOUNT_DAO_H
#include <map>
#include <string>
#include <vector>
#include <variant>
#include "models/account.h"
#include "models/view_account.h"
#include "common/dao_status.h"
#include "connection_pool.h"
extern "C" {
	#include <sqlite3.h>
}

namespace zinpass::repo{

struct EncryptedData {
	std::string account_id;
	std::vector<unsigned char> encrypted_pwd;
	std::vector<unsigned char> iv;
};

class AccountDAO {
public:

	enum class FilterType {
		ALL,		// 不筛选（包括NULL、""在内的任意值）
		LIKE,		// 模糊匹配
		EXACT,		// 精确查询
		IS_NULL		// 筛选只为NULL的情况
	};

	enum class ColumnType {
		id,
		username,
		nickname,
		subAccount,
		postscript,
		platformName,
		providerName,
		URL,
		hotline,
		owner,
		createdTime,
		updatedTime,

		encryptedPwd,
		iv,
		phoneId,
		emailId,
		categoryId
	};

	// 定义存储不同类型值的结构体
	struct KeyValuePair {
		ColumnType key;
		std::variant<int, char, std::string, void*, bool> value;
	};

	explicit AccountDAO();
	~AccountDAO();

	/**
	 * @brief 根据主键查询账号记录
	 * @param id 目标主键
	 * @param view_account 查询到的账号信息
	 * @return 状态码
     */
    DaoStatus findById(const std::string& id, models::ViewAccount& view_account) const;

    /**
     * @brief 查询账号记录
     * @param sys_user_id 限制范围为某用户
	 * @param view_accounts 查询到的账号信息
	 * @return 状态码
	 */
	DaoStatus find(const std::string& sys_user_id, std::vector<models::ViewAccount>& view_accounts) const;

	/**
	 * @brief 查询所有符合条件的记录，使用模糊查询
	 * @param provider_name 服务商名
	 * @param platform_name 平台名
	 * @param username 账号
	 * @param nickname 昵称
	 * @param phone 绑定手机号
	 * @param email 绑定邮箱
	 * @param category 所属类别
	 * @param postscript 备注
	 * @param sys_user_id 指定系统用户
	 * @param view_accounts 保存查询到的viewAccount信息的vector容器
	 * @return 成功与否
	 */
	DaoStatus find(
		const std::string& provider_name,
		const std::string& platform_name,
		const std::string& username,
		const std::string& nickname,
		const std::string& phone,
		const std::string& email,
		const std::string& category,
		const std::string& postscript,
		const std::string& sys_user_id,
		std::vector<models::ViewAccount>& view_accounts) const;

    /**
     * @brief 根据 id 和 sys_user_id 查询账号的 密码密文 和 初始化向量值 iv
	 * @param id 查询目标
	 * @param sys_user_id 目标所属用户
	 * @param encrypted_pwd 查询到的密码
	 * @param iv 查询到的初始化向量值
	 * @return 成功与否
	 */
	DaoStatus findEncryptedPwdAndIv(const std::string& id, const std::string& sys_user_id, std::vector<unsigned char>& encrypted_pwd, std::vector<unsigned char>& iv) const;

	/**
	 * @brief 根据 sys_user_id 查询账号的所有 密码密文 和 初始化向量值 iv
	 * @param sys_user_id 目标所属用户
	 * @return 成功与否
	 */
	// std::vector<EncryptedData> findEncryptedPwdAndIv(const std::string& sys_user_id) const;

	/**
	 * @brief 根据 sys_user_id 列出所有匹配的 id
	 * @param sys_user_id 目标所属用户
	 * @return 查询到的所有符合的 account_id
	 */
	std::vector<std::string> list_id(const std::string& sys_user_id) const;

	/**
	 * @brief 存储账号记录（需手动设置好 phone_id email_id category_id 等）
     * @param account Account 类型记录
     * account.id 主键不需要设置，会自动生成
     * account.emailId 为空字符串时，设置为NULL,
     * account.phoneId < 0 时, 设置为NULL
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus add(const models::Account& account) const;

    /**
     * @brief 存储账号记录（减少数据库查询，自动处理 phone_id email_id category_id）
	 * @param view_account ViewAccount 类型记录
     * account.id 主键不需要设置，会自动生成
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus add(const models::ViewAccount& view_account) const;

    /**
     * @brief （update_1_1/3）修改账号记录
     * @param view_account 修改对象（以 id 和 sys_user_id 为目标，忽略 created_time ）
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus update(const models::ViewAccount& view_account) const;

    /**
     * @brief （update_1_2/3）修改账号记录但不包括 encrypted_pwd 和 iv
     * @param view_account 修改对象（ 以 id 和 sys_user_id 为目标，忽略 encrypted_pwd 、 iv 、 created_time）
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus update_main_properties(const models::ViewAccount& view_account) const;

	/**
	 * @brief （update_1_3/3）修改账号记录的密码（密文密码、初始化向量、更新时间）
     * @param id 主键，修改目标
	 * @param sys_user_id 所属用户，若与 id 不匹配则不会修改
	 * @param encrypted_pwd 加密后的密码
	 * @param iv 初始化向量值
	 * @param update_time 更新时间
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus update_password(
		const std::string& id,
		const std::string& sys_user_id,
		const std::vector<unsigned char>& encrypted_pwd,
		const std::vector<unsigned char>& iv,
		const std::string& update_time
		) const;

	/**
	 * @brief （update_2_1/4）修改Account表中的平台信息（1/4）
	 * @param id 筛选条件
	 * @param provider_name 服务商名
	 * @param platform_name 平台名
	 * @param url 平台网址
	 * @param hotline 服务热线
	 * @param update_time 更新时间
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus update_platform(
		const std::string& id,
		const std::string& provider_name,
		const std::string& platform_name,
		const std::string& url,
		const std::string& hotline,
		const std::string& update_time
		) const;

    /**
     * @brief （update_2_2/4）修改Account表中的账号信息（2/4）
     * @param id 筛选条件
     * @param username 账号
     * @param nickname 昵称
     * @param encrypted_pwd 密码
     * @param iv 初始化向量新值
     * @param category_id 类别，小于 0 时 设置为 NULL
     * @param update_time 更新时间
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus update_user(
		const std::string& id,
		const std::string& username,
		const std::string& nickname,
		const std::vector<unsigned char>& encrypted_pwd,
		const std::vector<unsigned char>& iv,
		short category_id,
		const std::string& update_time
		) const;

	/**
	 * @brief （update_2_3/4）修改Account表中的第三方绑定账号信息（3/4）
	 * @param id 筛选条件
	 * @param phone_id 绑定手机号，小于 0 时 设置为 NULL
	 * @param email_id 绑定邮箱，小于 0 时 设置为 NULL
	 * @param update_time 更新时间
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus update_third(
		const std::string& id,
		int phone_id,
		const std::string& email_id,
		const std::string& update_time
		) const;

	/**
	 * @brief （update_2_4/4）修改Account表中的其他信息（4/4）
	 * @param id 筛选条件
	 * @param sub_account 子账号
	 * @param postscript 备注（附言）
	 * @param update_time 更新信息
	 * @return 成功与否
     */
	[[nodiscard]] DaoStatus update_other(
		const std::string& id,
		const std::string& sub_account,
		const std::string& postscript,
		const std::string& update_time
		) const;

	// 此函数有问题
	[[nodiscard]] DaoStatus update(const std::vector<KeyValuePair>& data, const std::string& id) const;

	/**
	 * @brief 删除Account记录
	 * @param id 删除目标的id
	 * @return 成功与否
	 */
	[[nodiscard]] DaoStatus remove(const std::string& id) const;

	// Email部分（特殊的Account）
	/**
	 * @brief 查询所有emailAddress
	 * @param sys_user_id 系统用户账号
	 * @param email_addresses 存放所有查询到的邮箱地址
	 * @return 成功与否
	 */
	DaoStatus findEmailList(const std::string& sys_user_id, std::vector<std::string>& email_addresses) const;

	/**
	 * @brief getIdByEmailAddress 根据 emailAddress 查询对应的主键 id
	 * @param email_address 邮箱地址
	 * @return 若查询到返回主键 id ，若未查询到则返回空字符串 ""
	 */
	std::string getIdByEmailAddress(const std::string& email_address) const;

    /**
     * @brief 根据id查询目标email记录
	 * @param id id
     * @param email 保存查询到的email信息
     * @return 成功与否
     */
    DaoStatus findEmailById(const std::string& id, models::ViewAccount& email) const;

	/**
	 * @brief 查询所有email记录
	 * @param sys_user_id 筛选条件：当前登录用户的ID
	 * @param emails 保存查询到的email信息
	 * @return 成功与否
	 */
	DaoStatus findEmail(const std::string& sys_user_id, std::vector<models::ViewAccount>& emails) const;

private:
	ConnectionPool& pool_;

	/**
	 * @brief 将 Statement 数据保存到 view_account 中
	 * @param stmt stmt
	 * @return 账号记录视图
	 */
	static models::ViewAccount ViewAccountFromStatement(sqlite3_stmt* stmt);

	static std::string buildSqlPart(const std::map<ColumnType, std::string>& data, const std::string& separator);
};

}

#endif
