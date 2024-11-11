#pragma once

namespace meta
{
#ifndef META_MACROS
#define META_MACROS

#define REFLECT_CONCAT_IMPL(x, y) x##y

#define REFLECT_CONCAT(x, y) REFLECT_CONCAT_IMPL(x, y)

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

#define REFLECT_CREATE_PROPERTY(Owner,Name,PropertyType) std::make_shared<reflect::Property>( \
reflect::Type::Infer<PropertyType>(),   \
#Name,  \
makeAccessor(&Owner::Name))

#define REFLECT_CREATE_FUNCTION(Name) std::make_shared<reflect::Function>(#Name,[](const reflect::Value& result,const reflect::Value& instance, const std::vector<reflect::Value>& args)

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

#define REFLECTED_BODY() REFLECT_JOIN(_meta_,REFLECT_JOIN(META_FILE_ID,REFLECT_JOIN(_,__LINE__)))()
}