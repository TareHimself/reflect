﻿#include "reflect/Type.hpp"


namespace reflect
{
    Type::Type() = default;

    Type::Type(const std::string& name)
    {
        _name = name;
    }

    bool Type::operator==(const Type& other) const
    {
        return _name == other._name;
    }
}
