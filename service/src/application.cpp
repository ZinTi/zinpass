#include "application.h"
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>

#include "repo/init_database.h"
#include "utils/log_manager.h"
#include "mod_session/session_mgr.h"
#include "repo/connection_pool.h"
#include "rpc/grpc_client_service.h"
#include "rpc/grpc_control_service.h"
#include "config/app_config.h"
#include "utils/tmp_msg.h"

using namespace zinpass;

void Application::init() {
    // 1. 读取配置文件
    std::string config_msg = "";
    config::AppConfig::instance().read_config_from_file(config_msg);    // 从 yaml 配置文件中读取应用相关配置（在此之前因未读取日志配置，即不确定日志文件的路径，故不使用 LogManager 类）
    const std::string tmp_msg = tmp_msg_take_all();
    utils::LogManager::add_log(tmp_msg);
    utils::LogManager::add_log(std::string("[MESSAGE] ") + config_msg);

    // 2. 数据库不存在则创建并初始化
    const std::string db_path = config::AppConfig::instance().get_db_path();
    if (!std::filesystem::exists(db_path)) {
        utils::LogManager::add_log("[MESSAGE] Database file not found. Creating and initializing...");
        if (const repo::InitDatabase initDb(db_path); 0 == initDb.init()) {
            utils::LogManager::add_log("[MESSAGE] Initial database success.");
        }
    }

    // 3. 初始化连接池单例
    repo::ConnectionPool::initialize(db_path, 5);

    // 4. 配置服务器地址和端口
    const std::string rpc_client_svc_ap = config::AppConfig::instance().get_client_ipv4() + ":" + std::to_string(config::AppConfig::instance().get_client_port());
    utils::LogManager::add_log("[CONFIG] Server address: " + rpc_client_svc_ap);
}

void Application::run() {
    utils::LogManager::add_log("Main thread - Service/Daemon started");
    std::atomic<bool> is_running(true);     // 局部原子控制标志

    // 创建两个 gRPC 服务实例
    const std::string rpc_ctrl_svc_ap = config::AppConfig::instance().get_cli_ipv4() + ":" + std::to_string(config::AppConfig::instance().get_cli_port());
    const std::string rpc_client_svc_ap = config::AppConfig::instance().get_client_ipv4() + ":" + std::to_string(config::AppConfig::instance().get_client_port());
    service::gRPCControlService control_interface(is_running, rpc_ctrl_svc_ap); // gRPC 实例1 - 管理控制
    service::gRPCClientService business_server(is_running, rpc_client_svc_ap);  // gRPC 实例2 - 业务逻辑

    // 管理控制交互线程：处理管理员命令
    std::thread thread_control_interface([&control_interface]() {
        control_interface.run();
    });

    // 业务服务线程：处理客户端请求
    std::thread thread_business_server([&business_server]() {
        business_server.run();
    });

    // 周期性清理线程：监控运行状态、定期处理过期会话、检查运行环境等
    std::thread thread_periodic_task_worker([this]() { this->periodic_task_worker(); });

    // 等待所有线程结束
    thread_periodic_task_worker.detach();
    thread_control_interface.join();
    thread_business_server.join();

    utils::LogManager::add_log("Main thread - All workers completed");            // 无需手动调用Stop，析构函数会自动处理
}

[[noreturn]] void Application::periodic_task_worker() {
    while (true) {
        const unsigned int seconds = static_cast<unsigned int>(config::AppConfig::instance().get_log_cleanup_interval());
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        // 自动清理内存中的无效 session 记录等操作
        auto& session_mgr = mod_session::SessionMgr::instance();
        const int sum = session_mgr.purge_expired_sessions();
        utils::LogManager::add_log("[MESSAGE] purged expired sessions, sum: " + std::to_string(sum));
    }
}
