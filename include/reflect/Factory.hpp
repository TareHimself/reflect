#pragma once
#include <memory>
#include <unordered_map>
#include "wrap/Reflected.hpp"

namespace reflect::factory
{
    class _INTERNAL_REFLECT_TYPE_FACTORY
    {
    public:
        std::unordered_map<reflect::Type,std::shared_ptr<reflect::wrap::Reflected>,Type::HashFunction> classes;
        std::unordered_map<std::string,reflect::Type> aliases;
    };

    inline std::shared_ptr<_INTERNAL_REFLECT_TYPE_FACTORY> _internalGetFactory()
    {
        static std::shared_ptr<_INTERNAL_REFLECT_TYPE_FACTORY> factory = std::make_shared<_INTERNAL_REFLECT_TYPE_FACTORY>();
        return factory;
    }
    
    
    
    inline std::vector<std::string> names()
    {
        const auto factory = _internalGetFactory();
        std::vector<std::string> keys;
        for(const auto & [fst, snd] : factory->aliases)
        {
            keys.push_back(fst);
        }
        
        return keys;
    }

    inline std::vector<Type> types()
    {
        const auto factory = _internalGetFactory();
        std::vector<Type> types;
        for(const auto & [fst, snd] : factory->aliases)
        {
            types.push_back(snd);
        }
        
        return types;
    }

    inline std::vector<std::shared_ptr<wrap::Reflected>> values()
    {
        const auto factory = _internalGetFactory();
        std::vector<std::shared_ptr<wrap::Reflected>> values;
        for(const auto & [fst, snd] : factory->classes)
        {
            values.push_back(snd);
        }
        
        return values;
    }

    inline std::shared_ptr<reflect::wrap::Reflected> find(const Type& type)
    {
        const auto factory = _internalGetFactory();
        
        if(const auto result = factory->classes.find(type); result != factory->classes.end())
        {
            return result->second;
        }

        return {};
    }

    template<typename T>
    inline std::shared_ptr<reflect::wrap::Reflected> find()
    {
        return find(Type::Infer<T>());
    }

    inline std::shared_ptr<reflect::wrap::Reflected> find(const std::string& name)
    {
        const auto factory = _internalGetFactory();
        if(const auto result = factory->aliases.find(name); result != factory->aliases.end())
        {
            return find(result->second);
        }

        return {};
    }

    
    inline void add(const std::shared_ptr<wrap::Reflected> & cls)
    {
        const auto factory = _internalGetFactory();
        factory->classes.emplace(cls->GetType(),cls);
        factory->aliases.emplace(cls->GetName(),cls->GetType());
    }

    struct ReflectTypeBuilder
    {
        std::vector<std::shared_ptr<wrap::Field>> fields;
        ReflectTypeBuilder& AddField(std::shared_ptr<wrap::Field>&& field);

        template <typename T>
        std::shared_ptr<wrap::Reflected> Create(const std::string& name);
    };

    template <typename T>
    std::shared_ptr<wrap::Reflected> ReflectTypeBuilder::Create(const std::string& name)
    {
        auto result = std::make_shared<wrap::Reflected>(Type::Infer<T>(),name,fields);
        factory::add(result);
        return result;
    }


    // template<typename T>
    // inline std::shared_ptr<reflect::wrap::Class> findClass()
    // {
    //     const auto factory = _internalGetFactory();
    //     auto result = factory->classes.find(Type::Infer<T>());
    //     if(result != factory->classes.end())
    //     {
    //         return result;
    //     }
    //
    //     return nullptr;
    // }
}
