#pragma once
#include <memory>

#include "Parser.hpp"

namespace reflect::generate
{

    std::string Include(const std::string &file);
    void GenerateClass(std::ofstream& stream,const std::shared_ptr<parser::ParsedClass>& cls);
    void Generate(const std::shared_ptr<parser::ParsedFile> &parsedFile,const std::filesystem::path &filePath);
}
