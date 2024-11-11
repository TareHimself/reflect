#include "meta/Property.hpp"

namespace meta
{
    Property::Property(const Type& type, const std::string& name,
                              const std::shared_ptr<PropertyAccessorBase>& accessor)
    {
        _fieldType = EFieldType::FieldProperty;
        _type = type;
        _name = name;
        _accessor = accessor;
    }
}
