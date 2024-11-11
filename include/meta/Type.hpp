#pragma once
#include <ostream>
#include <string>

#include "Macro.hpp"
#include "Utils.hpp"

namespace meta
{
    struct Type
    {
    private:
        std::string _name{};
    public:

        Type();
        Type(const std::string& name);
        template <typename T>
        static Type Infer();

        bool operator==(const Type& other) const;

        friend std::ostream& operator<<(std::ostream &stream,const Type& other)
        {
            stream << other._name;
            
            return stream;
        }

        struct HashFunction
        {
            size_t operator()(const Type& t) const
            {
                constexpr std::hash<std::string> hasher;
                return hasher(t._name);
            }
        };
    };

    template <typename T>
    Type Type::Infer()
    {
        // std::string name = ;
        // const auto parts = utils::split(utils::trim(name),">");
        // name = parts[0];
        //
        return {std::string(REFLECT_TYPE_NAME)};
    }

    

    
}
