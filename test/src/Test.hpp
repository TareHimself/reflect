#pragma once
#include <iostream>
#include "reflect/Macro.hpp"
#include "Test.reflect.hpp"

RCLASS()
class TestClass
{
public:
    RPROPERTY()
    int num = 0;

    RPROPERTY()
    float health = 200.0f;

    RFUNCTION()

    void HelloWorld()
    {
        std::cout << "THIS WAS REFLECTED" << std::endl;
    }

    RFUNCTION()

    void WithArgs(const std::string& text
                  , const uint64_t count
                  , int* ptr)
    {
        std::cout << text << " Count [" << count << "]" << std::endl;
    }
};

REFLECT_IMPLEMENT(TestClass)

RSTRUCT()
struct TestStruct
{
public:
    
    RPROPERTY()
    int num = 0;

    RPROPERTY()
    int num2 = 20;

    RPROPERTY()
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

    int TestFunc3(int a, int b, int c)
    {
        return a + b + c;
    }
};

REFLECT_IMPLEMENT(TestStruct)

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

