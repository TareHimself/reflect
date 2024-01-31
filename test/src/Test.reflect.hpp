#pragma once
#include "reflect/Macro.hpp"
#include "reflect/Reflect.hpp"
#include "reflect/Factory.hpp"
#include "reflect/wrap/Wrap.hpp"


#ifndef __REFLECT_GENERATED_TestClass
#define __REFLECT_GENERATED_TestClass
#define __REFLECTED_GENERATED_TestClass_PROPERTY_num REFLECT_WRAP_PROPERTY(TestClass,num,int)

#define __REFLECTED_GENERATED_TestClass_PROPERTY_health REFLECT_WRAP_PROPERTY(TestClass,health,float)

#define __REFLECTED_GENERATED_TestClass_FUNCTION_HelloWorld REFLECT_WRAP_FUNCTION_BEGIN(HelloWorld) \
{ \
 \
instance.As<TestClass>()->HelloWorld(); \
})
#define __REFLECTED_GENERATED_TestClass_FUNCTION_WithArgs REFLECT_WRAP_FUNCTION_BEGIN(WithArgs) \
{ \
auto arg_0 = args[0].As<std::string>(); \
auto arg_1 = args[1].As<const uint64_t>(); \
auto arg_2 = args[2].As<int*>(); \
 \
instance.As<TestClass>()->WithArgs(*arg_0,*arg_1,*arg_2); \
})
#define __REFLECT_GENERATE_TestClass \
reflect::factory::ReflectTypeBuilder builder; \
builder.AddField(__REFLECTED_GENERATED_TestClass_PROPERTY_num); \
builder.AddField(__REFLECTED_GENERATED_TestClass_PROPERTY_health); \
builder.AddField(__REFLECTED_GENERATED_TestClass_FUNCTION_HelloWorld); \
builder.AddField(__REFLECTED_GENERATED_TestClass_FUNCTION_WithArgs); \
builder.Create<TestClass>("TestClass");
#endif



#ifndef __REFLECT_GENERATED_TestStruct
#define __REFLECT_GENERATED_TestStruct
#define __REFLECTED_GENERATED_TestStruct_PROPERTY_num REFLECT_WRAP_PROPERTY(TestStruct,num,int)

#define __REFLECTED_GENERATED_TestStruct_PROPERTY_num2 REFLECT_WRAP_PROPERTY(TestStruct,num2,int)

#define __REFLECTED_GENERATED_TestStruct_PROPERTY_num5 REFLECT_WRAP_PROPERTY(TestStruct,num5,int*)

#define __REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc REFLECT_WRAP_FUNCTION_BEGIN(TestFunc) \
{ \
 \
instance.As<TestStruct>()->TestFunc(); \
})
#define __REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc2 REFLECT_WRAP_FUNCTION_BEGIN(TestFunc2) \
{ \
auto arg_0 = args[0].As<std::string>(); \
 \
instance.As<TestStruct>()->TestFunc2(*arg_0); \
})
#define __REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc3 REFLECT_WRAP_FUNCTION_BEGIN(TestFunc3) \
{ \
auto arg_0 = args[0].As<int>(); \
auto arg_1 = args[1].As<int>(); \
auto arg_2 = args[2].As<int>(); \
 \
if(result){ \
*result.As<int>() = instance.As<TestStruct>()->TestFunc3(*arg_0,*arg_1,*arg_2); \
} \
})
#define __REFLECT_GENERATE_TestStruct \
reflect::factory::ReflectTypeBuilder builder; \
builder.AddField(__REFLECTED_GENERATED_TestStruct_PROPERTY_num); \
builder.AddField(__REFLECTED_GENERATED_TestStruct_PROPERTY_num2); \
builder.AddField(__REFLECTED_GENERATED_TestStruct_PROPERTY_num5); \
builder.AddField(__REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc); \
builder.AddField(__REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc2); \
builder.AddField(__REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc3); \
builder.Create<TestStruct>("TestStruct");
#endif



