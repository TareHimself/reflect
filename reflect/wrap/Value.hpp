#pragma once
#include "../Type.hpp"

namespace reflect::wrap
{
    class Value
    {
        bool _isValid = false;
        Type _type;
        void * _data = nullptr;
    public:

        Value(const Type& type,void * data);

        template <typename T>
        Value(T * data);
        
        
        template <typename T>
        T * As() const;
        
        template <typename T>
        T * As();

        Value();

        operator bool() const;
    };

    template <typename T>
    Value::Value(T * data) : Value(Type::Infer<T>(),data)
    {
    }

    template <typename T>
    T * Value::As() const
    {
        return static_cast<T *>(_data);
    }

    template <typename T>
    T * Value::As()
    {
        return static_cast<T *>(_data);
    }
}
