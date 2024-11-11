#pragma once
#include <memory>
#include <unordered_map>
#include "Metadata.hpp"

namespace meta
{
    class _INTERNAL_TYPE_FACTORY
    {
    public:
        std::unordered_map<Type,std::shared_ptr<Metadata>,Type::HashFunction> classes;
        std::unordered_map<std::string,Type> aliases;
    };

    inline std::shared_ptr<_INTERNAL_TYPE_FACTORY> _internalGetFactory()
    {
        static std::shared_ptr<_INTERNAL_TYPE_FACTORY> factory = std::make_shared<_INTERNAL_TYPE_FACTORY>();
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

    inline std::vector<std::shared_ptr<Metadata>> values()
    {
        const auto factory = _internalGetFactory();
        std::vector<std::shared_ptr<Metadata>> values;
        for(const auto & [fst, snd] : factory->classes)
        {
            values.push_back(snd);
        }
        
        return values;
    }

    inline std::shared_ptr<Metadata> find(const Type& type)
    {
        const auto factory = _internalGetFactory();
        
        if(const auto result = factory->classes.find(type); result != factory->classes.end())
        {
            return result->second;
        }

        return {};
    }

    template<typename T>
    inline std::shared_ptr<Metadata> find()
    {
        return find(Type::Infer<T>());
    }

    template<typename T>
    inline std::shared_ptr<Metadata> find(T instance)
    {
        return find(Type::Infer<T>());
    }

    inline std::shared_ptr<meta::Metadata> find(const std::string& name)
    {
        const auto factory = _internalGetFactory();
        if(const auto result = factory->aliases.find(name); result != factory->aliases.end())
        {
            return find(result->second);
        }

        return {};
    }

    
    inline void add(const std::shared_ptr<Metadata> & cls)
    {
        const auto factory = _internalGetFactory();
        factory->classes.emplace(cls->GetType(),cls);
        factory->aliases.emplace(cls->GetName(),cls->GetType());
    }

    struct TypeBuilder
    {
        std::vector<std::shared_ptr<Field>> fields = {};
        
        TypeBuilder& AddField(const std::shared_ptr<Field>& field,const std::vector<std::string>& tags = {});

        template<typename TClass,typename  TPropertyType>
        TypeBuilder& AddProperty(const std::string& name,TProperty<TClass,TPropertyType> prop,const std::vector<std::string>& tags = {});
        
        TypeBuilder& AddFunction(const std::string& name, const functionTypedef& caller,const std::vector<std::string>& tags = {});
        

        template <typename T>
        std::shared_ptr<Metadata> Create(const std::string& name,const std::vector<std::string>& tags = {});
    };

    template <typename TClass, typename TPropertyType>
    TypeBuilder& TypeBuilder::AddProperty(const std::string& name, TProperty<TClass, TPropertyType> prop,const std::vector<std::string>& tags)
    {
        return AddField(std::make_shared<Property>(Type::Infer<TClass>(),name,makeAccessor(prop)));
    }

    template <typename T>
    std::shared_ptr<Metadata> TypeBuilder::Create(const std::string& name,const std::vector<std::string>& tags )
    {
        auto result = std::make_shared<Metadata>(Type::Infer<T>(),name,fields);
        for(auto &tag : tags)
        {
            result->AddTag(tag);
        }
        
        factory::add(result);
        return result;
    }


    // template<typename T>
    // inline std::shared_ptr<reflect::Class> findClass()
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
