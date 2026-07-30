#pragma once
#include <string>

namespace StringUtils
{
    /** Format a float to a string with a maximum number of decimals. */
    [[nodiscard]] std::string FormatFloat(float value, int maxDecimals, bool discardZeroDecimals = true);
    
    /** Format bytes into the octet format, with the unit. */
    [[nodiscard]] std::string FormatOctet(uint64_t bytes);
    
    /** Convert a bool into "True" or "False". */
    [[nodiscard]] std::string BoolToString(bool value);
    
    /** Ensure a string has the required number of chars, adding spaces at the end to fill if necessary. */
    [[nodiscard]] std::string SanitizeString(std::string input, size_t desiredLength, bool putSpacesBefore = false);
};
