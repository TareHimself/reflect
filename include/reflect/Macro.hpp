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

// Marks the constructor below it for reflection not yet parsed
#define RCONSTRUCTOR(...)

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

#ifdef _MSC_VER
#define REFLECT_TYPE_NAME __FUNCSIG__
#else
#define REFLECT_TYPE_NAME __PRETTY_FUNCTION__
#endif
    

#define REFLECT_DECLARE(ID)  \
static void REFLECT_JOIN(_reflect_define_function_,ID)(); \
namespace { \
struct REFLECT_JOIN(_reflect_define_struct_,ID) {  \
REFLECT_JOIN(_reflect_define_struct_,ID)() { \
REFLECT_JOIN(_reflect_define_function_,ID)(); \
} \
};  \
} \
static const REFLECT_JOIN(_reflect_define_struct_,ID) REFLECT_JOIN(_reflect_define_,ID);  \
static void REFLECT_JOIN(_reflect_define_function_,ID)()

#define REFLECT_IMPLEMENT(Type)  \
REFLECT_DECLARE(Type) \
{   \
    _REFLECT_GENERATE_##Type \
}   

#endif

#define REFLECTED_BODY() REFLECT_JOIN(_reflected_,REFLECT_JOIN(REFLECT_FILE_ID,REFLECT_JOIN(_,__LINE__)))()
}