#include "state/channel_manager.h"

namespace zinpass::state {

    ChannelManager& ChannelManager::get_instance() {
        static ChannelManager instance;
        return instance;
    }

    std::shared_ptr<grpc::Channel> ChannelManager::get_channel() {
        std::lock_guard<std::mutex> lock(mutex_);
        return channel_;
    }

    void ChannelManager::initialize(const std::string& target) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!channel_) {
            channel_ = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
        }
    }

    void ChannelManager::update_channel(const std::string& target) {
        std::lock_guard<std::mutex> lock(mutex_);
        channel_ = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    }

}