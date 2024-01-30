#include "Reflected.hpp"

namespace reflect::wrap
{
    Reflected::Reflected()
    {
        _type = {};
        _name = "";
    }

    Reflected::Reflected(const Type& type, const std::string& name, const std::vector<std::shared_ptr<Field>>& fields)
    {
        _type = type;
        _name = name;
        _fields = fields;
    }

    Type Reflected::GetType() const
    {
        return _type;
    }

    std::string Reflected::GetName() const
    {
        return _name;
    }

    std::vector<std::shared_ptr<Field>> Reflected::GetFields() const
    {
        return _fields;
    }
}

