#pragma once
#include <memory>

#include "Parser.hpp"

namespace reflect::generate
{

    std::string include(const std::string &file);
    void generateClass(std::ofstream& stream,const std::shared_ptr<parser::ParsedClass>& cls);
    void generateStruct(std::ofstream& stream,const std::shared_ptr<parser::ParsedStruct>& cls);
    void guardBegin(std::ofstream& stream,const std::string& name);
    void guardEnd(std::ofstream& stream);
    std::vector<std::string> writeMacros(std::ofstream& stream,const std::string& typeName,const std::vector<std::shared_ptr<parser::ParsedField>>& fields);
    std::string writeProperty(std::ofstream& stream,const std::string& typeName,const std::shared_ptr<parser::ParsedProperty>& prop);
    std::string writeFunction(std::ofstream& stream,const std::string& typeName,const std::shared_ptr<parser::ParsedFunction>& func);

    void writeBuilder(std::ofstream& stream,const std::string& typeName,const std::vector<std::string>& macros);
    void Generate(const std::shared_ptr<parser::ParsedFile> &parsedFile,const std::filesystem::path &filePath);
}
