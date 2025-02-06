#include "mgr/telecom_operator_mgr.h"
#include "repo/telecom_operator_dao.h"

namespace zinpass::business{

TelecomOperatorManager::Return<std::vector<std::string>> TelecomOperatorManager::get_telecom_operator_options() {
    const repo::TelecomOperatorDAO telecom_operator_dao;
    std::vector<std::string> telecom_operator_options;
    std::string message;
    if (
        const repo::DaoStatus ret_status = telecom_operator_dao.findList(telecom_operator_options);
        repo::DaoStatus::Success != ret_status) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {telecom_operator_options, message};
}

}