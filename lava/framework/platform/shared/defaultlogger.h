//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_DEFAULTLOGGER_H
#define LAVAVK_DEFAULTLOGGER_H

#include "lava/framework/platform/logger.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk
{
    class DefaultLogger : public Logger
    {
    public:
        std::type_index getType() const override;
        void error(std::string_view error) override;
        void warning(std::string_view warning) override;
        void information(std::string_view information) override;
        void debug(std::string_view information) override;
    };
}


#endif //LAVAVK_DEFAULTLOGGER_H
