#include <functional>
#include <iostream>
#include <ostream>

#include "reflect/Generate.hpp"
#include "reflect/Parser.hpp"
int main(int argc, char** argv)
{
    reflect::parser::Parser parser;
    parser.files.emplace_back(R"(D:\Github\reflect\test\src\Test.hpp)");
    const auto parsedFiles = parser.Parse();
    for (auto& f : parsedFiles)
    {
        reflect::generate::Generate(f,f->filePath.parent_path() / "Test.reflect.hpp");
    }
    return 0;
}
