#pragma once
#include <functional>
#include "Field.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace meta
{
    typedef std::function<meta::Value(const meta::Value&)> getterFn;
    typedef std::function<void(const meta::Value&, const meta::Value&)> setterFn;

    template<typename TClass,typename  TPropertyType>
    using TProperty = TPropertyType TClass::*;
    
    struct PropertyAccessorBase
    {
        virtual ~PropertyAccessorBase() = default;
        virtual Value Get(const Value& instance) = 0;
        virtual void Set(const Value& instance,const Value& data) = 0;
    };

    template<typename TClass,typename  TPropertyType>
    struct PropertyAccessor : PropertyAccessorBase
    {
        
        PropertyAccessor(TProperty<TClass,TPropertyType> prop)
        {
            _ptr = prop;
        }

        Value Get(const Value& instance) override
        {
            return Value(&(instance.GetPtr<TClass>()->*_ptr));
        }

        void Set(const Value& instance,const Value& data) override
        {
            auto d = *data.GetPtr<TPropertyType>();
            instance.GetPtr<TClass>()->*_ptr = d;
        }
    private:
         TProperty<TClass,TPropertyType> _ptr = nullptr;
    };

    
    
    struct Property : Field
    {
    private:
        Type _type;
        std::shared_ptr<PropertyAccessorBase> _accessor;
    public:
        
        Property(const Type& type, const std::string& name, const std::shared_ptr<PropertyAccessorBase>& accessor);

        // template <typename T>
        // Value Get(T* instance) const;
        //
        // template <typename T, typename V>
        // void Set(T* instance, V&& v) const;
        //
        // template <typename T>
        // Value Get(T&& instance) const;
        //
        // template <typename T, typename V>
        // void Set(T&& instance, V&& v) const;


        template<typename T>
        Value Get(T * instance);

        template<typename T,typename V>
        void Set(T * instance, V&& data);

        template<typename T>
        Value Get(T &instance);

        template<typename T,typename V>
        void Set(T &instance, V&& data);
    };

    template <typename T>
    Value Property::Get(T* instance)
    {
        return _accessor->Get(instance);
    }

    template <typename T, typename V>
    void Property::Set(T* instance, V&& data)
    {
        auto d = data;
        _accessor->Set(instance,&data);
    }

    template <typename T>
    Value Property::Get(T& instance)
    {
        return Get(&instance);
    }

    template <typename T, typename V>
    void Property::Set(T& instance, V&& data)
    {
        Set(&instance,data);
    }

    template<typename TClass,typename  TPropertyType>
    inline std::shared_ptr<PropertyAccessorBase> makeAccessor(TProperty<TClass,TPropertyType> prop)
    {
        return std::make_shared<PropertyAccessor<TClass,TPropertyType>>(prop);
    }

    // template <typename T>
    // Value Property::Get(T* instance) const
    // {
    //     return _getter(instance);
    // }
    //
    // template <typename T, typename V>
    // void Property::Set(T* instance, V&& v) const
    // {
    //     _setter(instance, &v);
    // }
    //
    // template <typename T>
    // Value Property::Get(T&& instance) const
    // {
    //     return Get(&instance);
    // }
    //
    // template <typename T, typename V>
    // void Property::Set(T&& instance, V&& v) const
    // {
    //     return Set(&instance, v);
    // }
}
