#ifndef CHANNEL_MANAGER_H
#define CHANNEL_MANAGER_H

#include <memory>
#include <mutex>
#include <grpcpp/grpcpp.h>

namespace zinpass::state {

    class ChannelManager {
    public:
        static ChannelManager& get_instance(void);

        // 删除拷贝构造函数和赋值运算符
        ChannelManager(const ChannelManager&) = delete;
        ChannelManager& operator=(const ChannelManager&) = delete;

        std::shared_ptr<grpc::Channel> get_channel(void);

        void initialize(const std::string& target);
        void update_channel(const std::string& target);

    private:
        ChannelManager(void) = default;
        ~ChannelManager(void) = default;

        std::shared_ptr<grpc::Channel> channel_;
        std::mutex mutex_;
    };

} // zinpass::state

#endif //CHANNEL_MANAGER_H