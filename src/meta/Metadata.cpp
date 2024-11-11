#include "meta/Metadata.hpp"

namespace meta
{
    Metadata::Metadata()
    {
        _type = {};
        _name = "";
    }

    Metadata::Metadata(const Type& type, const std::string& name, const std::vector<std::shared_ptr<Field>>& fields,
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

    Type Metadata::GetType() const
    {
        return _type;
    }

    std::string Metadata::GetName() const
    {
        return _name;
    }

    std::vector<std::string> Metadata::GetFields() const
    {
        return _fieldNames;
    }

    std::shared_ptr<Field> Metadata::GetField(const std::string& field)
    {
        return _fields[field];
    }

    std::shared_ptr<Property> Metadata::GetProperty(const std::string& property)
    {
        if (!_fields.contains(property) || _fields[property]->GetType() != EFieldType::FieldProperty)
        {
            return {};
        }

        return std::static_pointer_cast<Property>(_fields[property]);
    }

    std::shared_ptr<Function> Metadata::GetFunction(const std::string& function)
    {
        if (!_fields.contains(function) || _fields[function]->GetType() != EFieldType::FieldFunction)
        {
            return {};
        }

        return std::static_pointer_cast<Function>(_fields[function]);
    }

    bool Metadata::HasField(const std::string& field) const
    {
        return std::ranges::find(_fieldNames,field) != _fieldNames.end();
    }
}
