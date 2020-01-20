//
// Created by dorian on 07. 12. 2019..
//

#include "defaultlogger.h"
#include <iostream>

std::type_index LavaVk::DefaultLogger::getType() const
{
    return typeid(DefaultLogger);
}

void LavaVk::DefaultLogger::error(std::string_view error)
{
    std::cerr << "[ERROR] " << error << std::endl;
}

void LavaVk::DefaultLogger::warning(std::string_view warning)
{
    std::cerr << "[WARNING] " << warning << std::endl;
}

void LavaVk::DefaultLogger::information(std::string_view information)
{
    std::cout << "[INFO] " << information << std::endl;
}

void LavaVk::DefaultLogger::debug(std::string_view information)
{
    std::cout << "[DEBUG] " << information << std::endl;
}
