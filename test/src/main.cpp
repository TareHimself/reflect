#include <functional>
#include <iostream>
#include <ostream>
#include "reflect/Function.hpp"
#include "reflect/Property.hpp"
#include "reflect/Value.hpp"
#include "Test.hpp"
#include "reflect/Factory.hpp"
using namespace reflect;

void doAdd(Value&& result,Value&& a,Value&& b)
{
    auto aval = a.GetPtr<int>();
    auto bval = b.GetPtr<int>();
    *result.GetPtr<int>() = *aval + *bval;
}

struct Test
{

    int * test = nullptr;
    float num = 50;
    
    void Foo();

    int Bar(int val)
    {
        return val;
    }
};


int main(int argc, char** argv)
{
    TestStruct instance;
    
    auto testP = REFLECT_WRAP_PROPERTY(TestStruct,num5,int *);
    auto testF = REFLECT_WRAP_FUNCTION_BEGIN("Test")
    {
        if(result)
        {
            *result.GetPtr<int>() = instance.GetPtr<TestStruct>()->TestFunc3(*args[0].GetPtr<int>(),*args[1].GetPtr<int>(),*args[3].GetPtr<int>());
        }
    });
    
    auto a = 1;
    auto b = 50;
    auto c = new int{256};
    
    testP->Set(instance,c);
    int * r = testP->Get(instance);
    auto result = 0;
    testF->Call(&result,instance,a,2,3,4);

    for(const auto& reflected : reflect::factory::values())
    {
        std::cout << "Reflected Type " << reflected->GetName() << std::endl;
        for(const auto &field : reflected->GetFields())
        {
            std::cout << "\tField: " << field << " \n\t\tType: " << (reflected->GetField(field)->GetType() == EFieldType::FieldFunction ? "Function" : "Property") << std::endl;
        }
    
        std::cout << "\n\n";
    }
    
    
    if(const auto reflected = reflect::factory::find<TestClass>())
    {
        TestClass instance;
    
        auto t1 = reflect::factory::find<TestClass>() == TestClass::Reflected;
        auto t2 = reflect::factory::find<TestClass>() == instance.Reflected;
        if(t1 && t2)
        {
            std::cout << "Static member matches" << std::endl;
        }
    
        if(const auto targetProp = reflected->GetProperty("num2"))
        {
            std::cout << "Reflected Value " << *targetProp->Get(instance).GetPtr<int>() << std::endl;
            auto v = 1738;
            targetProp->Set(&instance,v);
            std::cout << "Reflected Value " << *targetProp->Get(instance).GetPtr<int>() << std::endl;
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
            std::cout << classRef.get() << std::endl;
        }
    }
    
    
    return 0;
}
