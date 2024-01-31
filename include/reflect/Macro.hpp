#pragma once

namespace reflect
{
#ifndef REFLECT_MACROS
#define REFLECT_MACROS

#define REFLECT_CONCAT_IMPL(x, y) x##y

#define REFLECT_CONCAT(x, y) REFLECT_CONCAT_IMPL(x, y)

// Marks the class below it for reflection
#define RCLASS(...)

// Marks the struct below it for reflection
#define RSTRUCT(...)

// Marks the enum below it for reflection
#define RENUM(...)

// Marks the variable below it for reflection
#define RPROPERTY(...)
    
// Marks the function below it for reflection
#define RFUNCTION(...)

// Marks the constructor below it for reflection
#define RCONSTRUCTOR(...)

#define REFLECTED_BODY() REFLECT_CONCAT(REFLECTED_,REFLECT_CONCAT(__FILE__,__LINE__))

#define REFLECT_WRAP_PROPERTY(Owner,Name,PropertyType) std::make_shared<reflect::wrap::Property>( \
    reflect::Type::Infer<PropertyType>(),   \
    #Name,  \
    [](const reflect::wrap::Value& instance)    \
    {   \
        return reflect::wrap::Value(&instance.As<Owner>()->Name); \
    },  \
    [](const reflect::wrap::Value& instance,const reflect::wrap::Value& value)   \
    {   \
        instance.As<Owner>()->Name = *value.As<PropertyType>();   \
    })

#define REFLECT_WRAP_FUNCTION_BEGIN(Name) std::make_shared<reflect::wrap::Function>(#Name,[](const reflect::wrap::Value& result,const reflect::wrap::Value& instance, const std::vector<reflect::wrap::Value>& args)

#define REFLECT_JOIN_IMPL(A,B) A##B
    
#define REFLECT_JOIN(A,B) REFLECT_JOIN_IMPL(A,B)


#define REFLECT_IMPLEMENT(Type)  \
static void REFLECT_JOIN(_reflect_define_function_,REFLECT_JOIN(Type,__LINE__))(); \
namespace { \
struct REFLECT_JOIN(_reflect_define_struct_,REFLECT_JOIN(Type,__LINE__)) {  \
REFLECT_JOIN(_reflect_define_struct_,REFLECT_JOIN(Type,__LINE__))() { \
REFLECT_JOIN(_reflect_define_function_,REFLECT_JOIN(Type,__LINE__))(); \
} \
};  \
} \
static const REFLECT_JOIN(_reflect_define_struct_,REFLECT_JOIN(Type,__LINE__)) REFLECT_JOIN(_reflect_define_,__LINE__);  \
static void REFLECT_JOIN(_reflect_define_function_,REFLECT_JOIN(Type,__LINE__))()  \
{   \
    __REFLECT_GENERATE_##Type \
}   
    

#endif
}