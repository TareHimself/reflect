#pragma once
#include <memory>
#include "wrap/Reflected.hpp"

namespace reflect
{
    struct IReflected
    {
        virtual ~IReflected() = default;
        virtual std::shared_ptr<reflect::wrap::Reflected> GetReflected() const = 0;
    };
}
