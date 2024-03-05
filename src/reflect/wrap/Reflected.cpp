#include "reflect/wrap/Reflected.hpp"

namespace reflect::wrap
{
    Reflected::Reflected()
    {
        _type = {};
        _name = "";
    }

    Reflected::Reflected(const Type& type, const std::string& name, const std::vector<std::shared_ptr<Field>>& fields,
        const std::set<std::string>& tags)
    {
        _type = type;
        _name = name;
        _tags = tags;
        for (auto& field : fields)
        {
            _fields[field->GetName()] = field;
            _fieldNames.push_back(field->GetName());
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

    std::vector<std::string> Reflected::GetFields() const
    {
        return _fieldNames;
    }

    std::shared_ptr<Field> Reflected::GetField(const std::string& field)
    {
        return _fields[field];
    }

    std::shared_ptr<Property> Reflected::GetProperty(const std::string& property)
    {
        if (!_fields.contains(property) || _fields[property]->GetType() != EFieldType::FieldProperty)
        {
            return {};
        }

        return std::static_pointer_cast<Property>(_fields[property]);
    }

    std::shared_ptr<Function> Reflected::GetFunction(const std::string& function)
    {
        if (!_fields.contains(function) || _fields[function]->GetType() != EFieldType::FieldFunction)
        {
            return {};
        }

        return std::static_pointer_cast<Function>(_fields[function]);
    }

    bool Reflected::HasField(const std::string& field) const
    {
        return std::ranges::find(_fieldNames,field) != _fieldNames.end();
    }
}
