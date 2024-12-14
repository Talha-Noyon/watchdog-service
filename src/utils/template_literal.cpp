#include <iostream>
#include <string>
#include <sstream>

template <typename... Args>
std::string templateLiteral(const std::string& templateStr, Args... args) {
    std::ostringstream ss;
    std::string result = templateStr;
    std::string replacements[] = {args...};
    size_t pos = 0;

    // Replace "{}" placeholders with arguments
    for (const auto& replacement : replacements) {
        pos = result.find("{}");
        if (pos != std::string::npos) {
            result.replace(pos, 2, replacement);
        }
    }

    return result;
}