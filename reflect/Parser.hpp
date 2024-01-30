#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <regex>
#include <vector>

namespace reflect::parser
{

    const std::string REFLECT_CLASS_MACRO = "RCLASS(";
    const std::string REFLECT_STRUCT_MACRO = "RSRUCT(";
    const std::string REFLECT_FUNCTION_MACRO = "RFUNCTION(";
    const std::string REFLECT_PROPERTY_MACRO = "RPROPERTY(";
    const std::regex REFLECT_ARGUMENT_REGEX{R"((?:const[\s]+?)?([a-zA-Z:0-9_*\s<>]+)(?:[&\s]+)([a-zA-Z0-9_]+))"};
    
    enum EParsedFieldType
    {
        Property,
        Function
    };

    struct ParsedField
    {
    
        EParsedFieldType fieldType;
        ParsedField(EParsedFieldType _type)
        {
            fieldType = _type;
        }

        virtual void Debug() const
        {
            
        }
    };

    struct ParsedFunctionArgument
    {
        std::string type;
        std::string name;
    };

    struct ParsedFunction : ParsedField
    {
        std::string name;
        std::string result;
        std::vector<std::shared_ptr<ParsedFunctionArgument>> arguments{};

        ParsedFunction() : ParsedField(Function)
        {
        
        }

        void Debug() const override
        {
            std::cout << "Function: " << result << " " << name << "(";
            for(auto &arg : arguments)
            {
                std::cout << arg->type << " " << arg->name;
                if(arg != arguments.at(arguments.size() - 1))
                {
                    std::cout << ",";
                }
            }

            std::cout << ")" << std::endl;
        }
    };

    struct ParsedProperty : ParsedField
    {
        std::string name;
        std::string type;
        ParsedProperty() : ParsedField(Property)
        {
        
        }

        void Debug() const override
        {
            std::cout << "Property: " << type << " " << name << std::endl;
        }
    };

    enum EParsedType
    {
        Class,
        Enum
    };

    struct ParsedType
    {
        std::string name;
        EParsedType type;
        
        virtual void Debug() const
        {
            switch (type)
            {
            case Class:
                std::cout << "Class : " << name << std::endl;
                break;
            case Enum:
                std::cout << "Enum : " << name << std::endl;
                break;
            default:
                std::cout << "Unknown : " << name << std::endl;
                break;
            }
        }
    };

    struct ParsedClass : ParsedType
    {
        std::vector<std::shared_ptr<ParsedField>> fields;
        ParsedClass()
        {
            type = EParsedType::Class;
        }

        void Debug() const override
        {
            ParsedType::Debug();
            for(auto &f : fields)
            {
                f->Debug();
            }
        }
    };

    struct ParsedFile
    {
        std::filesystem::path filePath;
        std::vector<std::shared_ptr<ParsedType>> types;

        void Debug() const
        {
            std::cout << "File: " << filePath << "\n";
            for(auto &t : types)
            {
                t->Debug();
            }
        }
    };
    class Parser
    {
    public:
        std::vector<std::filesystem::path> files;
        std::shared_ptr<ParsedProperty> ParseProperty(std::ifstream &file) const;
        std::shared_ptr<ParsedFunction> ParseFunction(std::ifstream &file) const;
        std::shared_ptr<ParsedClass> ParseClass(std::ifstream &file) const;
        std::shared_ptr<ParsedFile> ParseFile(const std::filesystem::path &path) const;
        std::vector<std::shared_ptr<ParsedFile>> Parse() const;
    };
    
}
