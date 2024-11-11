#include "meta/Field.hpp"

namespace meta
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
