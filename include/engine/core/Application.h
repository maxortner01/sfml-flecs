#pragma once

#include <engine/Utility.h>

namespace s2de
{
    class Application :
        public utility::Singleton<Application>
    {
        Application();
        ~Application();

    public:
        friend class utility::Singleton<Application>
    };
}