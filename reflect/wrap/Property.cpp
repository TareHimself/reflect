#include "Property.hpp"

namespace reflect::wrap
{
    Property::Property(const Type& type, const std::string& name, const getterFn& getter, const setterFn& setter)
    {
        _fieldType = EFieldType::FieldType_Property;
        _type = type;
        _name = name;
        _getter = getter;
        _setter = setter;
    }
    
}
