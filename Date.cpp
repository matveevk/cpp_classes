#include <iostream>
#include <stdexcept>

class Date {
    static const int MIN_YEAR = 1970;

    int day, month, year;

    bool isLeap(int year) const {
        return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    }

    int daysInMonth(int month, int year) const {
        if (month == 2 && isLeap(year))
            return 29;
        if (month == 2)
            return 28;
        if ((month <= 7 && month % 2 == 0) || (month >= 8 && month % 2 != 0))
            return 30;
        return 31;
    }

    int daysInYear(int year) const {
        return 365 + isLeap(year);
    }

    bool isLastDayInMonth() const {
        return day == daysInMonth(month, year);
    }

    bool isLastDayInYear() const {
        return isLastDayInMonth() && month == 12;
    }

    bool isValid(int day, int month, int year) const {
        if (year < MIN_YEAR)
            return false;
        if (month > 12 || month < 1)
            return false;
        if (day < 1)
            return false;
        return day <= daysInMonth(month, year);
    }

    int toDays() const {
        int totalDays = day;
        for (int curMonth = 1; curMonth < month; ++curMonth) {
            totalDays += daysInMonth(curMonth, year);
        }
        for (int curYear = MIN_YEAR; curYear < year; ++curYear) {
            totalDays += daysInYear(curYear);
        }
        return totalDays;
    }

    explicit Date(int totalDays) {
        if (totalDays <= 0)
            throw std::invalid_argument("invalid date");

        int resYear = MIN_YEAR;
        while (totalDays > daysInYear(resYear)) {
            totalDays -= daysInYear(resYear);
            ++resYear;
        }
        int resMonth = 1;
        while (totalDays > daysInMonth(resMonth, resYear)) {
            totalDays -= daysInMonth(resMonth, resYear);
            ++resMonth;
        }
        day = totalDays;
        month = resMonth;
        year = resYear;
    }

public:
    Date(int day, int month, int year) {
        if (!isValid(day, month, year))
            throw std::invalid_argument("invalid date");
        this->day = day;
        this->month = month;
        this->year = year;
    }

    int GetDay() const {
        return day;
    }

    int GetMonth() const {
        return month;
    }

    int GetYear() const {
        return year;
    }

    Date& operator ++() {
        if (isLastDayInYear())
            *this = Date(1, 1, year + 1);
        else if (isLastDayInMonth())
            *this = Date(1, month + 1, year);
        else
            *this = Date(day + 1, month, year);
        return *this;
    }

    Date& operator --() {
        if (day == 1 && month == 1)
            *this = Date(31, 12, year - 1);
        else if (day == 1)
            *this = Date(daysInMonth(month - 1, year), month - 1, year);
        else
            *this = Date(day - 1, month, year);
        return *this;
    }

    Date operator + (int days) const {
        return Date(toDays() + days);
    }

    Date operator - (int days) const {
        return Date(toDays() - days);
    }

    int operator - (const Date& other) const {
        return toDays() - other.toDays();
    }
};
