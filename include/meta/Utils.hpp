#pragma once
#include <string>
#include <vector>

namespace meta::utils {
    std::vector<std::string> split(const std::string& str,const std::string& sep);

    size_t count(const std::string& str,const std::string& search);

    std::string trim(const std::string& str);
}
