#include "config/about.h"

#include <ostream>

#if defined(_WIN32)
    #define PROJECT_NAME        "ZinPassSvc"
#else
    #define PROJECT_NAME        "zinpassd"
#endif

#define PROJECT_VERSION     "0.0.1"
#define PROJECT_LICENSE     "GPLv3"
#define PROJECT_YEARS       "2025"
#define PROJECT_AUTHOR      "Mr. ZENG Lai (Zin)"
#define PROJECT_EMAIL       "vip201@126.com"

namespace zinpass::config {
    std::string About::get_version() {
        return PROJECT_VERSION;
    }

    std::string About::get_version_build_info() {
        std::string info = PROJECT_NAME " v" PROJECT_VERSION "(compiled " __TIME__ ", " __DATE__ ")\n";
        return info;
    }

    std::string About::get_project_name() {
        return PROJECT_NAME;
    }

    std::string About::get_copyleft() {
        std::string copyleft = PROJECT_NAME " version " PROJECT_VERSION " (compiled " __DATE__ " " __TIME__ ")\n"
            "Copyleft (c) " PROJECT_YEARS " " PROJECT_AUTHOR ". Licensed under the GNU General Public License.\n"
            "Email: " PROJECT_EMAIL;
        return copyleft;
    }

    inline std::string About::get_copyright() {
        return get_copyleft();
    }

}