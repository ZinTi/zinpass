#ifndef ABOUT_H
#define ABOUT_H
#include <string>

namespace zinpass::config {
class About {
public:

    /**
     * @brief 获取版本号
     * @return 版本号
     */
    static std::string get_version();

    /**
     * @brief 获取版本和构建信息
     * @return 名称 版本号 构建信息
     */
    static std::string get_version_build_info();

    /**
     * @brief 获取项目名称
     * @return 项目名称
     */
    static std::string get_project_name();

    /**
     * @brief 获取 copyleft 信息
     * @return 著作权声明等信息
     */
    static std::string get_copyleft();

    /**
     * @brief 同 get_copyleft()
     * @return 著作权声明等信息
     */
    inline static std::string get_copyright();
};
}

#endif //ABOUT_H
