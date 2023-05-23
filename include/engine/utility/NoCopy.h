#pragma once

namespace s2de::utility
{
    struct NoCopy
    {
        NoCopy() = default;
        virtual ~NoCopy() = default;

        NoCopy(const NoCopy&) = delete;
    };
}