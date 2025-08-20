#include "business/date_time.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace zinpass::utils {

    std::tm DateTime::safe_localtime(const std::time_t& time) {
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &time);
#else
        localtime_r(&time, &tm);
#endif
        return tm;
    }

    // 原有format_time方法
    std::string DateTime::format_time(const char* fmt) {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);
        const std::tm tm = safe_localtime(time);
        return format_time(tm, fmt); // 调用重载方法
    }

    // 新增重载：格式化自定义tm结构体
    std::string DateTime::format_time(const std::tm& tm, const char* fmt) {
        std::stringstream ss;
        ss << std::put_time(&tm, fmt);
        return ss.str();
    }

    // 日期增减实现
    std::string DateTime::addDays(const std::string& dateStr, int days, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        std::time_t time = std::mktime(&tm);
        time += days * 86400;
        std::tm new_tm = safe_localtime(time);
        return format_time(new_tm, fmt_out ? fmt_out : fmt_in);
    }

    std::string DateTime::addMonths(const std::string& dateStr, int months, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        tm.tm_mon += months;
        std::mktime(&tm); // 标准化日期
        return format_time(tm, fmt_out ? fmt_out : fmt_in);
    }

    std::string DateTime::addYears(const std::string& dateStr, int years, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        tm.tm_year += years;
        std::mktime(&tm); // 标准化日期
        return format_time(tm, fmt_out ? fmt_out : fmt_in);
    }

    // 时间增减实现
    std::string DateTime::addHours(const std::string& dateTimeStr, int hours, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateTimeStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        std::time_t time = std::mktime(&tm);
        time += hours * 3600;
        std::tm new_tm = safe_localtime(time);
        return format_time(new_tm, fmt_out ? fmt_out : fmt_in);
    }

    std::string DateTime::addMinutes(const std::string& dateTimeStr, int minutes, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateTimeStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        std::time_t time = std::mktime(&tm);
        time += minutes * 60;
        std::tm new_tm = safe_localtime(time);
        return format_time(new_tm, fmt_out ? fmt_out : fmt_in);
    }

    std::string DateTime::addSeconds(const std::string& dateTimeStr, int seconds, const char* fmt_in, const char* fmt_out) {
        std::tm tm = {};
        std::istringstream ss(dateTimeStr);
        ss >> std::get_time(&tm, fmt_in);
        if (ss.fail()) return "";
        std::time_t time = std::mktime(&tm);
        time += seconds;
        std::tm new_tm = safe_localtime(time);
        return format_time(new_tm, fmt_out ? fmt_out : fmt_in);
    }

    int DateTime::compareDates(const std::string& dateStr1, const std::string& dateStr2, const char* fmt) {
        std::tm tm1 = {};
        std::istringstream ss1(dateStr1);
        ss1 >> std::get_time(&tm1, fmt);
        if (ss1.fail()) return -2;

        std::tm tm2 = {};
        std::istringstream ss2(dateStr2);
        ss2 >> std::get_time(&tm2, fmt);
        if (ss2.fail()) return -2;

        // 标准化时间部分为00:00:00
        tm1.tm_hour = tm1.tm_min = tm1.tm_sec = 0;
        tm2.tm_hour = tm2.tm_min = tm2.tm_sec = 0;

        std::time_t time1 = std::mktime(&tm1);
        std::time_t time2 = std::mktime(&tm2);

        if (time1 < time2)       return -1;
        else if (time1 > time2)  return 1;
        else                      return 0;
    }

    int DateTime::compareTimes(const std::string& timeStr1, const std::string& timeStr2, const char* fmt) {
        std::tm tm1 = {};
        std::istringstream ss1(timeStr1);
        ss1 >> std::get_time(&tm1, fmt);
        if (ss1.fail()) return -2;

        std::tm tm2 = {};
        std::istringstream ss2(timeStr2);
        ss2 >> std::get_time(&tm2, fmt);
        if (ss2.fail()) return -2;

        // 计算总秒数进行比较
        int sec1 = tm1.tm_hour * 3600 + tm1.tm_min * 60 + tm1.tm_sec;
        int sec2 = tm2.tm_hour * 3600 + tm2.tm_min * 60 + tm2.tm_sec;

        if (sec1 < sec2)       return -1;
        else if (sec1 > sec2)  return 1;
        else                    return 0;
    }

    int DateTime::compareDateTimes(const std::string& dateTimeStr1, const std::string& dateTimeStr2, const char* fmt) {
        std::tm tm1 = {};
        std::istringstream ss1(dateTimeStr1);
        ss1 >> std::get_time(&tm1, fmt);
        if (ss1.fail()) return -2;

        std::tm tm2 = {};
        std::istringstream ss2(dateTimeStr2);
        ss2 >> std::get_time(&tm2, fmt);
        if (ss2.fail()) return -2;

        std::time_t time1 = std::mktime(&tm1);
        std::time_t time2 = std::mktime(&tm2);

        if (time1 < time2)       return -1;
        else if (time1 > time2)  return 1;
        else                      return 0;
    }

    std::string DateTime::getCurrentDateTime() { return format_time("%Y-%m-%d %H:%M:%S"); }
    std::string DateTime::getCurrentDate()     { return format_time("%Y-%m-%d"); }
    std::string DateTime::getCurrentTime()     { return format_time("%H:%M:%S"); }

}