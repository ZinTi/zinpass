#ifndef SQL_DEBUG_H
#define SQL_DEBUG_H
extern "C" {
    #include <sqlite3.h>
}

namespace zinpass::repo {

class SQLDebug {
public:
    static void log_sql(sqlite3_stmt* stmt, bool release = false);
};

}

#endif //SQL_DEBUG_H
