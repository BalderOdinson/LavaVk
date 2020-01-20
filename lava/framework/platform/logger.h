//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_LOGGER_H
#define LAVAVK_LOGGER_H

#include <string_view>
#include "lava/framework/object.h"

namespace LavaVk
{
    /// Interface for application logging.
    class Logger : public virtual Object
    {
    public:
        /// Log error.
        /// \param error.
        virtual void error(std::string_view error) = 0;

        /// Log warning.
        /// \param warning.
        virtual void warning(std::string_view warning) = 0;

        /// Log information.
        /// \param information.
        virtual void information(std::string_view information) = 0;

        /// Log debug information.
        /// \param debug information.
        virtual void debug(std::string_view information) = 0;
    };

    using UniqueLogger = std::unique_ptr<Logger>;
    using SharedLogger = std::shared_ptr<Logger>;
    using WeakLogger = std::weak_ptr<Logger>;
}


#endif //LAVAVK_LOGGER_H
