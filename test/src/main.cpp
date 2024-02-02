#include <functional>
#include <iostream>
#include <ostream>

#include "reflect/Generate.hpp"
#include "reflect/Parser.hpp"
#include "reflect/wrap/Function.hpp"
#include "reflect/wrap/Property.hpp"
#include "reflect/wrap/Value.hpp"
#include "Test.hpp"
#include "reflect/Factory.hpp"
using namespace reflect::wrap;

void getAllHeadersInDir(std::vector<std::filesystem::path>& files,const std::filesystem::path& dir,const std::string& headerExt)
{
    for (auto & entry : std::filesystem::directory_iterator(dir))
    {
        auto ext = entry.path().extension();
        if(is_directory(entry.path()))
        {
            getAllHeadersInDir(files,entry.path(),headerExt);
        }
        else if(entry.path().extension() == "." + headerExt && !entry.path().string().ends_with("reflect" + entry.path().extension().string()))
        {
            files.push_back(entry.path());
        }
    }
}

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
    
    //std::filesystem::path sourceDir = R"(D:\Github\reflect\test\src)";

    std::filesystem::path sourceDir = R"(D:\Github\reflect\test\src)";
    sourceDir = sourceDir.lexically_normal();
    std::filesystem::path outputDir = sourceDir;
        
    reflect::parser::Parser parser;
    getAllHeadersInDir(parser.files,sourceDir,"hpp");

    for (const auto parsedFiles = parser.Parse(); auto& f : parsedFiles)
    {
        auto relativeToSourceDir = f->filePath.string().substr(sourceDir.string().size());
        relativeToSourceDir = relativeToSourceDir.substr(0, relativeToSourceDir.size() - f->filePath.filename().string().size());
        auto fileName = f->filePath.filename().string();
        auto extension = f->filePath.extension().string();
        std::filesystem::path newFilePath  = outputDir.string() + relativeToSourceDir + (fileName.substr(0,fileName.size() - extension.size()) + ".reflect" + extension);
        if(!exists(newFilePath.parent_path()))
        {
            std::filesystem::create_directories(newFilePath.parent_path());
        }
            
        reflect::generate::Generate(f,newFilePath);
    }
    return 0;
}
