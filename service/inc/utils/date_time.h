#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <string>
#include <ctime>

namespace zinpass::utils {

class DateTime {
public:
    static std::tm safe_localtime(const std::time_t& time);
    static std::string format_time(const char* fmt);
    static std::string format_time(const std::tm& tm, const char* fmt); // 重载

    // 获取 yyyy-MM-dd HH:mm::ss 格式时间
    static std::string getCurrentDateTime();
    static std::string getCurrentDate();
    static std::string getCurrentTime();

    // 日期时间增减方法
    static std::string addDays(const std::string& dateStr, int days, const char* fmt_in = "%Y-%m-%d", const char* fmt_out = nullptr);
    static std::string addMonths(const std::string& dateStr, int months, const char* fmt_in = "%Y-%m-%d", const char* fmt_out = nullptr);
    static std::string addYears(const std::string& dateStr, int years, const char* fmt_in = "%Y-%m-%d", const char* fmt_out = nullptr);
    static std::string addHours(const std::string& dateTimeStr, int hours, const char* fmt_in = "%Y-%m-%d %H:%M:%S", const char* fmt_out = nullptr);
    static std::string addMinutes(const std::string& dateTimeStr, int minutes, const char* fmt_in = "%Y-%m-%d %H:%M:%S", const char* fmt_out = nullptr);
    static std::string addSeconds(const std::string& dateTimeStr, int seconds, const char* fmt_in = "%Y-%m-%d %H:%M:%S", const char* fmt_out = nullptr);

    // 日期时间比较方法
    /**
     * @brief
     * @param dateStr1
     * @param dateStr2
     * @param fmt
     * @return 返回值大于 0 表示 dateStr1>dateStr2, 返回值小于 0 表示 dateStr1<dateStr2, 返回值等于0 表示 dateStr1==dateStr2
     */
    static int compareDates(const std::string& dateStr1, const std::string &dateStr2, const char *fmt = "%Y-%m-%d");

    /**
     *
     * @param timeStr1
     * @param timeStr2
     * @param fmt
     * @return 返回值大于 0 表示 timeStr1>timeStr2, 返回值小于 0 表示 timeStr1<timeStr2, 返回值等于0 表示 timeStr1==timeStr2
     */
    static int compareTimes(const std::string& timeStr1,
                            const std::string &timeStr2,
                            const char *fmt = "%H:%M:%S");

    /**
     *
     * @param dateTimeStr1
     * @param dateTimeStr2
     * @param fmt
     * @return 返回值大于 0 表示 dateTimeStr1>dateTimeStr2, 返回值小于 0 表示 dateTimeStr1<dateTimeStr2, 返回值等于0 表示 dateTimeStr1==dateTimeStr2
     */
    static int compareDateTimes(const std::string& dateTimeStr1, const std::string& dateTimeStr2, const char* fmt = "%Y-%m-%d %H:%M:%S");


};

}

#endif