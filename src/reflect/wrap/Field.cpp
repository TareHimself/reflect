#include "reflect/wrap/Field.hpp"

namespace reflect::wrap
{
    std::string Field::GetName() const
    {
        return _name;
    }

    EFieldType Field::GetType() const
    {
        return _fieldType;
    }
}
