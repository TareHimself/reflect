#include <functional>
#include <iostream>
#include <ostream>

#include "reflect/Generate.hpp"
#include "reflect/Macro.hpp"
#include "reflect/Parser.hpp"
#include "reflect/wrap/Function.hpp"
#include "reflect/wrap/Property.hpp"
#include "reflect/wrap/Value.hpp"
#include "Test.hpp"
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

    for(const auto reflected : reflect::factory::values())
    {
        std::cout << "Reflected Type " << reflected->GetName() << std::endl;
        for(const auto &field : reflected->GetFields())
        {
            std::cout << "\tField: " << field->GetName() << " \n\t\tType: " << (field->GetType() == EFieldType::FieldType_Function ? "Function" : "Property") << std::endl;
        }

        std::cout << "\n\n";
    }
    reflect::parser::Parser parser;
    parser.files.emplace_back(R"(D:\Github\reflect\Test.hpp)");
    const auto parsedFiles = parser.Parse();
    for (auto& f : parsedFiles)
    {
        reflect::generate::Generate(f,f->filePath.parent_path() / "Test.reflect.hpp");
    }
    return 0;
}
