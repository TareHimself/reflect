#pragma once
#include "reflect/Macro.hpp"
#include "reflect/Reflect.hpp"
#include "reflect/Factory.hpp"
#include "reflect/wrap/Wrap.hpp"

#ifdef REFLECT_FILE_ID
#undef REFLECT_FILE_ID
#endif
#define REFLECT_FILE_ID e359c886fdcb4fbcb6a03617fc0cb607


#ifndef _REFLECT_GENERATED_TestClass
#define _REFLECT_GENERATED_TestClass
#define _REFLECTED_GENERATED_TestClass_PROPERTY_num REFLECT_WRAP_PROPERTY(TestClass,num,int)

#define _REFLECTED_GENERATED_TestClass_PROPERTY_health REFLECT_WRAP_PROPERTY(TestClass,health,float)

#define _REFLECTED_GENERATED_TestClass_FUNCTION_HelloWorld REFLECT_WRAP_FUNCTION_BEGIN(HelloWorld) \
{ \
 \
instance.As<TestClass>()->HelloWorld(); \
})

#define _REFLECTED_GENERATED_TestClass_FUNCTION_WithArgs REFLECT_WRAP_FUNCTION_BEGIN(WithArgs) \
{ \
auto arg_0 = args[0].As<std::string>(); \
auto arg_1 = args[1].As< const uint64_t>(); \
auto arg_2 = args[2].As< int*>(); \
 \
instance.As<TestClass>()->WithArgs(*arg_0,*arg_1,*arg_2); \
})

#define _REFLECTED_GENERATED_TestClass_FUNCTION_Construct REFLECT_WRAP_FUNCTION_BEGIN(Construct) \
{ \
 \
if(result){ \
*result.As< std::shared_ptr<TestClass>>() = TestClass::Construct(); \
} \
})

#define _reflected_e359c886fdcb4fbcb6a03617fc0cb607_13() \
static std::shared_ptr<reflect::wrap::Reflected> Reflected; \
std::shared_ptr<reflect::wrap::Reflected> GetReflected() const override;
#endif


#ifndef _REFLECT_GENERATED_TestStruct
#define _REFLECT_GENERATED_TestStruct
#define _REFLECTED_GENERATED_TestStruct_PROPERTY_num REFLECT_WRAP_PROPERTY(TestStruct,num,int)

#define _REFLECTED_GENERATED_TestStruct_PROPERTY_num2 REFLECT_WRAP_PROPERTY(TestStruct,num2,int)

#define _REFLECTED_GENERATED_TestStruct_PROPERTY_num5 REFLECT_WRAP_PROPERTY(TestStruct,num5,int*)

#define _REFLECTED_GENERATED_TestStruct_FUNCTION_Foo REFLECT_WRAP_FUNCTION_BEGIN(Foo) \
{ \
 \
instance.As<TestStruct>()->Foo(); \
})

#define _REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc2 REFLECT_WRAP_FUNCTION_BEGIN(TestFunc2) \
{ \
auto arg_0 = args[0].As<std::string>(); \
 \
instance.As<TestStruct>()->TestFunc2(*arg_0); \
})

#define _REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc3 REFLECT_WRAP_FUNCTION_BEGIN(TestFunc3) \
{ \
auto arg_0 = args[0].As<int>(); \
auto arg_1 = args[1].As< int>(); \
auto arg_2 = args[2].As< int>(); \
 \
if(result){ \
*result.As<int>() = instance.As<TestStruct>()->TestFunc3(*arg_0,*arg_1,*arg_2); \
} \
})

#define _reflected_e359c886fdcb4fbcb6a03617fc0cb607_51() \
static std::shared_ptr<reflect::wrap::Reflected> Reflected; \
std::shared_ptr<reflect::wrap::Reflected> GetReflected() const override;
#endif


