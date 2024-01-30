#include "Type.hpp"


namespace reflect
{
    Type::Type() = default;

    Type::Type(const std::string& name, const size_t& size)
    {
        _name = name;
        _size = size;
    }

    bool Type::operator==(const Type& other) const
    {
        return _name == other._name && _size == other._size;
    }
}
