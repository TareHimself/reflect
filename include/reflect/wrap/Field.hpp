#pragma once
#include <string>
#include <set>

#include "Taggable.hpp"

namespace reflect::wrap
{
    enum EFieldType
    {
        FieldType_Unspecified,
        FieldType_Property,
        FieldType_Function,
        FieldType_EnumField,
    };

    struct Field : Taggable
    {
    protected:
        std::string _name;
        EFieldType _fieldType = EFieldType::FieldType_Unspecified;
    public:
        std::string GetName() const;
        EFieldType GetType() const;
        
    };
}
