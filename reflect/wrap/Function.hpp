#pragma once
#include <functional>
#include "Field.hpp"
#include "Value.hpp"

namespace reflect::wrap
{
    typedef std::function<void(const Value& result,const Value& instance,const std::vector<Value>& args)> functionTypedef;
    struct Function : Field
    {
        
    private:
        functionTypedef _call;
        
        template<typename T>
        void AddArgument(std::vector<Value>& vec,T& arg)
        {
            vec.push_back(Value(Type::Infer<T>(),&arg));
        }
    public:
        Function(const std::string &name,functionTypedef call);

        template <typename T,typename... Args>
        void Call(const Value& result,T * instance,Args&&... args);
        
        template <typename T,typename... Args>
        void Call(const Value& result,T& instance,Args&&... args);
    };

    template <typename T, typename ... Args>
    void Function::Call(const Value& result, T * instance, Args&&... args)
    {
        std::vector<Value> values;
        ((AddArgument(values, args)), ...);
        
        _call(result,instance,values);
    }

    template <typename T,typename ... Args>
    void Function::Call(const Value& result,T& instance,Args&&... args)
    {
        Call(result,&instance,std::forward<Args>(args)...);
    }
}
