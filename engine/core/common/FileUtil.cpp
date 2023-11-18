#include "FileUtil.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace ige
{
    std::string FileUtil::ReadFileToString(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Error: Failed to open file " << filepath << std::endl;
            return "";
        }

        std::stringstream ss;
        std::string line;
        while (getline(file, line))
        {
            ss << line << '\n';
        }

        return ss.str();
    }
}