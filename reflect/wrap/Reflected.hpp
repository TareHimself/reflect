#pragma once
#include <memory>
#include <vector>
#include "Field.hpp"
#include "../Type.hpp"

namespace reflect::wrap
{
    struct Reflected
    {
    protected:
        std::string _name;
        Type _type;
        std::vector<std::shared_ptr<Field>> _fields;
    public:

        Reflected();

        Reflected(const Type& type,const std::string &name,const std::vector<std::shared_ptr<Field>>& fields);
        [[nodiscard]] Type GetType() const;
        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] std::vector<std::shared_ptr<Field>> GetFields() const;
    };
}
