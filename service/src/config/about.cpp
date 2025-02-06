#include "config/about.h"

#include <ostream>

#define PROJ_NAME        "zinpassd"
#define PROJ_VER         ZINPASS_VERSION
#define PROJ_LICENSE     "GPLv3"
#define PROJ_YEARS       "2025-2026"
#define PROJ_AUTHOR      "Mr. ZENG Lai (Zin)"
#define PROJ_EMAIL       "vip201@126.com"

namespace zinpass::config {
    std::string About::get_version() {
        return PROJ_VER;
    }

    std::string About::get_version_build_info() {
        std::string info = PROJ_NAME " v" PROJ_VER "(compiled " __TIME__ ", " __DATE__ ")\n";
        return info;
    }

    std::string About::get_project_name() {
        return PROJ_NAME;
    }

    std::string About::get_copyleft() {
        std::string copyleft = PROJ_NAME " version " PROJ_VER " (compiled " __DATE__ " " __TIME__ ")\n"
            "Copyleft (c) " PROJ_YEARS " " PROJ_AUTHOR ". Licensed under the GNU General Public License.\n"
            "Email: " PROJ_EMAIL;
        return copyleft;
    }

    inline std::string About::get_copyright() {
        return get_copyleft();
    }

}
