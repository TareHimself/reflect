#pragma once
#include <memory>
#include <vector>
#include "Field.hpp"
#include "Function.hpp"
#include "Property.hpp"
#include "../Type.hpp"

namespace reflect::wrap
{
    struct Reflected
    {
    protected:
        std::string _name;
        Type _type;
        std::unordered_map<std::string, std::shared_ptr<Field>> _fields;

    public:
        Reflected();

        Reflected(const Type& type, const std::string& name, const std::vector<std::shared_ptr<Field>>& fields);
        [[nodiscard]] Type GetType() const;
        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] std::unordered_map<std::string, std::shared_ptr<Field>> GetFields() const;
        std::shared_ptr<Property> GetProperty(const std::string& property);
        std::shared_ptr<Function> GetFunction(const std::string& function);
    };
}
