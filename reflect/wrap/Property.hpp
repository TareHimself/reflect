#pragma once
#include <functional>
#include "Field.hpp"
#include "../Type.hpp"
#include "Value.hpp"

namespace reflect::wrap
{
    typedef std::function<reflect::wrap::Value(const reflect::wrap::Value&)> getterFn;
    typedef std::function<void(const reflect::wrap::Value&, const reflect::wrap::Value&)> setterFn;

    struct Property : Field
    {
    private:
        Type _type;
        getterFn _getter;
        setterFn _setter;

    public:
        Property(const Type& type, const std::string& name, const getterFn& getter, const setterFn& setter);

        template <typename T>
        Value Get(T* instance) const;

        template <typename T, typename V>
        void Set(T* instance, V& v) const;

        template <typename T>
        Value Get(T&& instance) const;

        template <typename T, typename V>
        void Set(T&& instance, V& v) const;

        static std::shared_ptr<Property> MakeShared(const Type& type, const std::string& name, const getterFn& getter, const setterFn& setter);
    };

    template <typename T>
    Value Property::Get(T* instance) const
    {
        return _getter(instance);
    }

    template <typename T, typename V>
    void Property::Set(T* instance, V& v) const
    {
        _setter(instance, &v);
    }

    template <typename T>
    Value Property::Get(T&& instance) const
    {
        return Get(&instance);
    }

    template <typename T, typename V>
    void Property::Set(T&& instance, V& v) const
    {
        return Set(&instance, v);
    }
}
