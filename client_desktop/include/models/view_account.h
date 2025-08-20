#ifndef VIEW_ACCOUNT_H
#define VIEW_ACCOUNT_H

#include <string>
#include "models/account.h"

namespace zinpass::models{

  class ViewAccount final : public Account {
    std::string phone_;        // 手机号
    std::string email_;        // 邮箱号
    std::string category_;     // 类别

  public:
    ViewAccount();
    ViewAccount(
        const std::string& id,
        const std::string& username,
        const std::string& nickname,
        const std::string& sub_account,
        const std::string& postscript,
        const std::string& platform_name,
        const std::string& provider_name,
        const std::string& url,
        const std::string& hotline,
        const std::string& sys_user_id,
        const std::string& created_time,
        const std::string& updated_time,

        const std::string& phone,
        const std::string& email,
        const std::string& category
        );
    ViewAccount(ViewAccount const& other);
    ~ViewAccount();

    [[nodiscard]] std::string getPhone() const;
    [[nodiscard]] std::string getEmail() const;
    [[nodiscard]] std::string getCategory() const;
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setCategory(const std::string& category);

    void copy(const ViewAccount& other);
    void print() const;
  };

}

#endif
