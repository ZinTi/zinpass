class Application {
public:
    Application() = default;
    ~Application() = default;

    /**
     * @brief 初始化应用程序
     */
    void init();

    /**
     * @brief 运行应用程序
     */
    void run();

    /**
     * @brief 周期性任务工作线程
     */
    [[noreturn]] void periodic_task_worker();

private:



};
