#ifndef TELECOM_OPERATOR_MANAGER_H
#define TELECOM_OPERATOR_MANAGER_H

#include <string>
#include <vector>

namespace zinpass::business {

class TelecomOperatorManager {
public:
    template<typename T>
    struct Return {
        T data;
        std::string message;
    };

    /**
     * @brief get_telecom_operator_options
     * @return 获取到的电信运营商名称 + 信息
     */
    static Return<std::vector<std::string>> get_telecom_operator_options();
};

}

#endif
