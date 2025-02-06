#ifndef PHONE_RPC_H
#define PHONE_RPC_H

#include <string>
#include <tuple>
// #include <map>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "phone.grpc.pb.h"
#include "mobile_phone.h"

namespace zinpass::rpc{

class PhoneRPC{
public:
    template<typename T>
    struct Return{
        T data;
        std::string message;
    };

    // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
    PhoneRPC(std::shared_ptr<grpc::Channel> channel);

    // 2、RPC service
    /**
     * @brief add_phone
     * @param session_id
     * @param telecom_operator
     * @param phone_area
     * @param phone_number
     * @param pin
     * @param puk
     * @param service_password
     * @param join_time
     * @param postscript
     * @return
     */
    Return<bool> add_phone(
        const std::string& session_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript);


    /**
     * @brief list_phone_numbers
     * @param session_id
     * @return phone_numbers + message
     */
    Return<std::vector<std::string>> list_phone_numbers(const std::string& session_id);

    /**
     * @brief list_phones
     * @param session_id
     * @param telecom
     * @param phone_number
     * @return
     */
    Return<std::vector<zinpass::models::MobilePhone>> list_phones(const std::string& session_id, const std::string& telecom, const std::string& phone_number);

    /**
     * @brief find_phone_by_id
     * @param session_id
     * @param phone_id
     * @return
     */
    Return<zinpass::models::MobilePhone> find_phone_by_id(const std::string& session_id, int phone_id);

    /**
     *
     * @param session_id
     * @param phone_id
     * @return
     */
    int get_reference_count(const std::string& session_id, int phone_id);

    /**
     * @brief update_phone_by_id
     * @param session_id
     * @param phone_id
     * @param telecom_operator
     * @param phone_area
     * @param phone_number
     * @param pin
     * @param puk
     * @param service_password
     * @param join_time
     * @param postscript
     * @return
     */
    Return<bool> update_phone_by_id(
        const std::string& session_id,
        int phone_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript
        );

    /**
     * @brief 删除目标手机号
     * @param session_id 会话 id
     * @param phone_id 手机号 id
     * @param mode 0-1-2-3 代表不同的删除模式，数值越大越激进，删除效果越不可控
     * 0 - 若存在子表引用行，则不删除 ;
     * 1 - 若存在子表引用行，批量修改被引用记录的手机号为指定的手机号 ;
     * 2 - 若存在子表引用行，将引用表的外键设置为 NULL ;
     * 3 - 若存在子表引用行，级联删除所有子表引用记录 ;
     * 若查询被引用记录时发生错误，则不会执行删除操作，忽略 mode 值
     * @param replace_phone_id 指定新的手机号 id（仅在 mode=1 时有效）
     * @return 结果 + 消息 + 子表引用行数
     */
    std::tuple<bool, std::string, int> delete_phone_by_id(const std::string& session_id, int phone_id, int mode, int replace_phone_id);

private:
    // PhoneService服务的存根
    std::unique_ptr<account::v1::PhoneService::PhoneService::Stub> stub_;
};

}

#endif
