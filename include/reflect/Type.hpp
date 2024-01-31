#pragma once
#include <ostream>
#include <string>
#include "Utils.hpp"

namespace reflect
{
    struct Type
    {
    private:
        std::string _name{};
        size_t _size{0};
    public:

        Type();
        Type(const std::string& name,const size_t& size);
        template <typename T>
        static Type Infer();

        bool operator==(const Type& other) const;

        friend std::ostream& operator<<(std::ostream &stream,const Type& other)
        {
            stream << other._name << " of size " << other._size;
            
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
        std::string name = std::string(typeid(T).name());
        const auto parts = utils::split(utils::trim(name)," ");
        name = parts[parts.size() - 1];
        
        return {name,sizeof(T)};
    }

    
}
