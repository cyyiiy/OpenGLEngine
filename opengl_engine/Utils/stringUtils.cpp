#include "stringUtils.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <sstream>


std::string StringUtils::FormatFloat(float value, int maxDecimals, bool discardZeroDecimals)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(maxDecimals) << value;

    std::string str = oss.str();
    
    if (!discardZeroDecimals) return str;

    str.erase(str.find_last_not_of('0') + 1);

    if (!str.empty() && str.back() == '.')
        str.pop_back();

    return str;
}

std::string StringUtils::FormatOctet(uint64_t bytes)
{
    static constexpr std::array units{
        "o", "Ko", "Mo", "Go", "To", "Po", "Eo"
    };
    
    double value = static_cast<double>(bytes);
    std::size_t unit = 0;
    
    while (value >= 1024.0 && unit < units.size() - 1)
    {
        value /= 1024.0;
        ++unit;
    }
    
    char buffer[32];
    
    if (unit == 0)
    {
        std::snprintf(buffer, sizeof(buffer), "%llu %s", static_cast<unsigned long long>(bytes), units[unit]);
    }
    else if (std::abs(value - std::round(value)) < 0.05)
    {
        std::snprintf(buffer, sizeof(buffer), "%.0f %s", value, units[unit]);
    }
    else
    {
        std::snprintf(buffer, sizeof(buffer), "%.2f %s", value, units[unit]);
    }
    
    return buffer;
}

std::string StringUtils::BoolToString(bool value)
{
    return value ? "True" : "False";
}

std::string StringUtils::SanitizeString(std::string input, size_t desiredLength, bool putSpacesBefore)
{
    if (input.length() >= desiredLength) return input;
    
    const std::string spaces(desiredLength - input.length(), ' ');
    return putSpacesBefore ? (spaces + input) : (input + spaces);
}
