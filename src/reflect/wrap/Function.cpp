#include "reflect/wrap/Function.hpp"

namespace reflect::wrap
{
    Function::Function(const std::string& name, functionTypedef call):
        _call(std::move(call))
    {
        _fieldType = EFieldType::FieldFunction;
        _name = name;
    }
}
