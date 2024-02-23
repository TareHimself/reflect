#include <functional>
#include <iostream>
#include <ostream>
#include "reflect/wrap/Function.hpp"
#include "reflect/wrap/Property.hpp"
#include "reflect/wrap/Value.hpp"
#include "Test.hpp"
#include "reflect/Factory.hpp"
using namespace reflect::wrap;

void doAdd(Value&& result,Value&& a,Value&& b)
{
    auto aval = a.As<int>();
    auto bval = b.As<int>();
    *result.As<int>() = *aval + *bval;
}
int main(int argc, char** argv)
{
    // TestStruct instance;
    // instance.TestFunc();
    //
    // auto testP = REFLECT_WRAP_PROPERTY(TestStruct,num5,int *);
    // auto testF = REFLECT_WRAP_FUNCTION_BEGIN("Test")
    // {
    //     if(result)
    //     {
    //         *result.As<int>() = instance.As<TestStruct>()->TestFunc3(*args[0].As<int>(),*args[1].As<int>(),*args[3].As<int>());
    //     }
    // });
    //
    // auto a = 1;
    // auto b = 50;
    // auto c = new int{256};
    // testP->Set(instance,c);
    // auto r = *testP->Get(instance).As<int *>();
    // auto result = 0;
    // testF->Call(&result,instance,1,2,3,4);

    for(const auto& reflected : reflect::factory::values())
    {
        std::cout << "Reflected Type " << reflected->GetName() << std::endl;
        for(const auto &field : reflected->GetFields())
        {
            std::cout << "\tField: " << field << " \n\t\tType: " << (reflected->GetField(field)->GetType() == EFieldType::FieldType_Function ? "Function" : "Property") << std::endl;
        }

        std::cout << "\n\n";
    }

    
    if(const auto reflected = reflect::factory::find<TestClass>())
    {
        TestStruct instance;

        if(const auto targetProp = reflected->GetProperty("num2"))
        {
            std::cout << "Reflected Value " << *targetProp->Get(instance).As<int>() << std::endl;
            auto v = 1738;
            targetProp->Set(instance,1738);
            std::cout << "Reflected Value " << *targetProp->Get(instance).As<int>() << std::endl;
        }
        if(const auto targetFunc = reflected->GetFunction("TestFunc2"))
        {
            targetFunc->Call({},instance,std::string("HELLO REFLECTED WORLD"));
        }
    }

    if(const auto reflected = reflect::factory::find("TestClass"))
    {
        if(const auto targetFunc = reflected->GetFunction("Construct"))
        {
            std::shared_ptr<TestClass> classRef;
            targetFunc->CallStatic(&classRef);
        }
    }
    
    
    return 0;
}
