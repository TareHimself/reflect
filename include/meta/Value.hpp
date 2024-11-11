#pragma once
#include "Type.hpp"

namespace meta
{
    class Value
    {
        bool _isValid = false;
        Type _type;
        void* _data = nullptr;

    public:
        Value(const Type& type, void* data);

        template <typename T>
        Value(T* data);


        template <typename T>
        T* GetPtr() const;

        template <typename T>
        T* GetPtr();

        template<typename T>
        operator T();

        template<typename T>
        operator T() const;

        Value();

        operator bool() const;
    };

    template <typename T>
    Value::Value(T* data) : Value(Type::Infer<T>(), data)
    {
    }

    template <typename T>
    T* Value::GetPtr() const
    {
        return static_cast<T*>(_data);
    }

    template <typename T>
    T* Value::GetPtr()
    {
        return static_cast<T*>(_data);
    }

    template <typename T>
    Value::operator T()
    {
        return *GetPtr<T>();
    }

    template <typename T>
    Value::operator T() const
    {
        *GetPtr<T>();
    }
}
