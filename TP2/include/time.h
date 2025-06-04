#ifndef TIME_H
#define TIME_H

#include "estruturas_auxiliares.h"

const int REFERENCE_YEAR = 2025;
const int REFERENCE_MONTH = 1;
const int REFERENCE_DAY = 1;
const int REFERENCE_HOUR = 0;
const int REFERENCE_MINUTE = 0;
const int REFERENCE_SECOND = 0;

int charToInt(char c) {
    return c - '0';
}
int extractAndConvertToInt(const std::string& s, size_t start, size_t length) {
    int result = 0;
    for (size_t i = 0; i < length; ++i) {
        result = result * 10 + charToInt(s[start + i]);
    }
    return result;
}
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int getDaysInMonth(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeapYear(year) ? 29 : 28;
        default: return 0;
    }
}

class Time {
    public:

        Time() : total_hours_(0.0) {}
        Time(double hours) : total_hours_(hours) {}

        static Time fromString(const std::string& datetime_str) {
            // Expected format: YYYY-MM-DD HH:MM:SS
            if (datetime_str.length() != 19) {
                std::cerr << "Erro: Formato de data/hora inválido. Esperado YYYY-MM-DD HH:MM:SS. Recebido: " << datetime_str << std::endl;
                return Time(); // Retorna um tempo zero em caso de erro
            }

            int year = extractAndConvertToInt(datetime_str, 0, 4);
            int month = extractAndConvertToInt(datetime_str, 5, 2);
            int day = extractAndConvertToInt(datetime_str, 8, 2);
            int hour = extractAndConvertToInt(datetime_str, 11, 2);
            int minute = extractAndConvertToInt(datetime_str, 14, 2);
            int second = extractAndConvertToInt(datetime_str, 17, 2);

            if (month < 1 || month > 12 || day < 1 || day > getDaysInMonth(year, month) ||
                hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
                std::cerr << "Erro: Valores de data/hora fora do intervalo. Recebido: " << datetime_str << std::endl;
                return Time();
            }

            double total_hours_since_reference = 0.0;

            for (int y = REFERENCE_YEAR; y < year; ++y) {
                total_hours_since_reference += (isLeapYear(y) ? 366 : 365) * 24.0;
            }
            for (int m = REFERENCE_MONTH; m < month; ++m) {
                total_hours_since_reference += getDaysInMonth(year, m) * 24.0;
            }
            total_hours_since_reference += (day - REFERENCE_DAY) * 24.0;

            total_hours_since_reference += hour;
            total_hours_since_reference += minute / 60.0;
            total_hours_since_reference += second / 3600.0;

            return Time(total_hours_since_reference);
        }


        double toHours() const {
            return total_hours_;
        }
        Time operator+(const Time& other) const {
            return Time(total_hours_ + other.total_hours_);
        }

        Time operator-(const Time& other) const {
            return Time(total_hours_ - other.total_hours_);
        }

        bool operator<(const Time& other) const {
            return total_hours_ < other.total_hours_;
        }
        bool operator<=(const Time& other) const {
            return total_hours_ <= other.total_hours_;
        }
        bool operator>(const Time& other) const {
            return total_hours_ > other.total_hours_;
        }
        bool operator>=(const Time& other) const {
            return total_hours_ >= other.total_hours_;
        }
        bool operator==(const Time& other) const {
            return total_hours_ == other.total_hours_;
        }
        bool operator!=(const Time& other) const {
            return total_hours_ != other.total_hours_;
        }

    private:
        double total_hours_;

};

#endif