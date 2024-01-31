#include "reflect/Factory.hpp"

namespace reflect::factory
{
    ReflectTypeBuilder& reflect::factory::ReflectTypeBuilder::AddField(
        std::shared_ptr<wrap::Field>&& field)
    {
        fields.push_back(field);
        return *this;
    }
}
