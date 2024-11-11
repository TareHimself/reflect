#pragma once
#include "meta/Macro.hpp"
#include "meta/IMetadata.hpp"
#include "Test.meta.hpp"

namespace a::b::c {
    namespace f {
    
    }

    namespace g::h::i {
    
    }
}

RSTRUCT()
class TestClass : meta::IMetadata
{
public:


    REFLECTED_BODY()


    RPROPERTY(IGNORE)
    int num{};

#ifdef SWOOO
    RPROPERTY()
    float health = 200.0f;

    RFUNCTION()
    void HelloWorld()
    {
        std::cout << "THIS WAS REFLECTED" << std::endl;
    }

#endif

    RFUNCTION()

    void WithArgs(const std::string& text
                  , const uint64_t count
                  , int* ptr)
    {
        std::cout << text << " Count [" << count << "]" << std::endl;
    }

    RFUNCTION()

    static std::shared_ptr<TestClass> Construct()
    {
        return std::make_shared<TestClass>();
    }
};

RSTRUCT(VisibleInCaptures)
struct TestStruct : public meta::IMetadata
{
public:

    REFLECTED_BODY()
    
    RPROPERTY(TestTag)
    int num = 0;

    RPROPERTY(Two,Tags)
    int num2 = 20;

    RPROPERTY(ATagFoMe,AndForYou)
    int* num5 = nullptr;

    RFUNCTION()
    void Foo()
    {
        std::cout << "CALLED FOO" << std::endl;
    }

    RFUNCTION()

    void TestFunc2(const std::string& data)
    {
        std::cout << "FROM TEST FUNC: " << data << std::endl;
    }

    RFUNCTION()

    int TestFunc3(int &a, int b, int c)
    {
        a = 200;
        return a + b + c;
    }
};

struct TestClassNotReflected
{
    int num = 0;
    int num2 = 20;
    int* num5 = nullptr;

    void TestFunc()
    {
        std::cout << "YO FROM TEST FUNC" << std::endl;
    }

    void TestFunc2(const std::string& data)
    {
        std::cout << "FROM TEST FUNC: " << data << std::endl;
    }

    int TestFunc3(const int a, const int b, const int c)
    {
        return a + b + c;
    }
};

