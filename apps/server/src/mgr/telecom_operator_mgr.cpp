#include "mgr/telecom_operator_mgr.h"
#include "repo/telecom_operator_dao.h"

namespace zinpass {

TelecomOperatorManager::Return<std::vector<std::string>> TelecomOperatorManager::get_telecom_operator_options() {
    const zinpass::TelecomOperatorDAO telecom_operator_dao;
    std::vector<std::string> telecom_operator_options;
    std::string message;
    if (
        const zinpass::DaoStatus ret_status = telecom_operator_dao.findList(telecom_operator_options);
        zinpass::DaoStatus::Success != ret_status) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {telecom_operator_options, message};
}

}