#include "reflect/Factory.hpp"

namespace reflect::factory
{
    TypeBuilder& reflect::factory::TypeBuilder::AddField(
        std::shared_ptr<wrap::Field>&& field)
    {
        fields.push_back(field);
        return *this;
    }
}
