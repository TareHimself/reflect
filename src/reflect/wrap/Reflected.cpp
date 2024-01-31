#include "reflect/wrap/Reflected.hpp"

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
        for (auto& field : fields)
        {
            _fields[field->GetName()] = field;
        }
    }

    Type Reflected::GetType() const
    {
        return _type;
    }

    std::string Reflected::GetName() const
    {
        return _name;
    }

    std::unordered_map<std::string, std::shared_ptr<Field>> Reflected::GetFields() const
    {
        return _fields;
    }

    std::shared_ptr<Property> Reflected::GetProperty(const std::string& property)
    {
        if (!_fields.contains(property) || _fields[property]->GetType() != EFieldType::FieldType_Property)
        {
            return {};
        }

        return std::static_pointer_cast<Property>(_fields[property]);
    }

    std::shared_ptr<Function> Reflected::GetFunction(const std::string& function)
    {
        if (!_fields.contains(function) || _fields[function]->GetType() != EFieldType::FieldType_Function)
        {
            return {};
        }

        return std::static_pointer_cast<Function>(_fields[function]);
    }
}
