#include "Value.hpp"

namespace reflect::wrap
{
    Value::Value(const Type& type, void* data)
    {
        _type = type;
        _data = data;
        _isValid = data != nullptr;
    }

    Value::Value()
    {
        _data = nullptr;
        _type = {};
        _isValid = false;
    }

    Value::operator bool() const
    {
        return _isValid;
    }
}
