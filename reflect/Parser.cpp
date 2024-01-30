#include "Parser.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "Uitils.hpp"

namespace reflect::parser
{
    std::shared_ptr<ParsedProperty> Parser::ParseProperty(std::ifstream& file) const
    {
        auto result = std::make_shared<ParsedProperty>();

        std::string line;
        while(line.empty())
        {
            std::getline(file,line);
            line = utils::trim(line);
        }

        const auto splitLine = utils::split(utils::trim(line)," ");
        result->name = utils::trim(splitLine.at(1));
        result->type = utils::trim(splitLine.at(0));
        
        return result;
    }

    std::shared_ptr<ParsedFunction> Parser::ParseFunction(std::ifstream& file) const
    {

        auto result = std::make_shared<ParsedFunction>();
        
        std::string line;
        while(line.empty())
        {
            std::getline(file,line);
            line = utils::trim(line);
        }

        const auto splitLine = utils::split(utils::trim(line)," ");
        result->name = utils::split(splitLine.at(1),"(").at(0);
        result->result = splitLine.at(0);
        auto argsLine = utils::trim(line);
        argsLine = utils::split(argsLine,"(").at(1);
        size_t pos  = 0;
        pos = argsLine.find(")");
        while(pos == std::string::npos)
        {
            std::getline(file,line);
            argsLine += utils::trim(line);
            pos = argsLine.find(")");
        }
        pos = argsLine.find(")");
        argsLine = argsLine.substr(0,pos);
        const auto argsArr = utils::split(utils::trim(argsLine),",");

        for(auto &arg : argsArr)
        {
            std::smatch match;
            if(std::regex_match(arg,match,REFLECT_ARGUMENT_REGEX))
            {
                auto type = match[1];
                auto name = match[2];
                result->arguments.push_back(std::make_shared<ParsedFunctionArgument>(utils::trim(type.str()),utils::trim(name.str())));
            }
        }
        return result;
    }

    std::shared_ptr<ParsedClass> Parser::ParseClass(std::ifstream& file) const
    {
        auto result = std::make_shared<ParsedClass>();
        std::string line;
        const std::string searchStr = "class ";
        while(line.empty() || line.find(searchStr) == std::string::npos)
        {
            std::getline(file,line);
            line = utils::trim(line);
        }
        // Get Class Name
        const auto classSpecPos = line.find(searchStr);
            
        if(classSpecPos == std::string::npos) return {};

        const auto splitResult = utils::split(utils::trim(line.substr(classSpecPos + searchStr.size()))," ");
        
        if(splitResult.empty()) return {};
        
        result->name = splitResult.at(0);
        
        // Parse Fields
        while(std::getline(file,line))
        {
            if(line.find("};") != std::string::npos) return result;
                
            if(line.find(REFLECT_PROPERTY_MACRO) != std::string::npos)
            {
                if(auto r = ParseProperty(file))
                {
                    result->fields.push_back(r);
                }
            }
            else if(line.find(REFLECT_FUNCTION_MACRO) != std::string::npos)
            {
                if(auto r = ParseFunction(file))
                {
                    result->fields.push_back(r);
                }
            }
        }

        return result;
    }

    std::shared_ptr<ParsedFile> Parser::ParseFile(const std::filesystem::path &path) const
    {
        if(!std::filesystem::exists(path)) return {};
        
        std::ifstream file(path);

        if(!file.is_open()) return {};

        auto result = std::make_shared<ParsedFile>();
        result->filePath = path;
        std::string line;
        std::string content;
        while(std::getline(file,line))
        {
            if(line.find(REFLECT_CLASS_MACRO) != std::string::npos)
            {
                if(auto cls = ParseClass(file))
                {
                    result->types.push_back(cls);
                }
            }
        }

        return result;
    }

    std::vector<std::shared_ptr<ParsedFile>> Parser::Parse() const
    {
        std::vector<std::shared_ptr<ParsedFile>> result;

        for(auto &file : files)
        {
            if(auto r = ParseFile(file))
            {
                result.push_back(r);
            }
        }
        return result;
    }
}
