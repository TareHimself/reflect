#include "meta/Factory.hpp"

namespace meta
{
    TypeBuilder& meta::TypeBuilder::AddField(
        const std::shared_ptr<Field>& field,const std::vector<std::string>& tags)
    {
        for(auto &tag : tags)
        {
            field->AddTag(tag);
        }
        fields.push_back(field);
        return *this;
    }

    TypeBuilder& TypeBuilder::AddFunction(const std::string& name, const functionTypedef& caller,
        const std::vector<std::string>& tags)
    {
        return AddField(std::make_shared<meta::Function>(name,caller),tags);
    }
}
