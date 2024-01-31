#pragma once
#include <fstream>
#include <filesystem>
#include <regex>
#include <vector>

namespace reflect::parser
{

    const std::string REFLECT_CLASS_MACRO = "RCLASS(";
    const std::string REFLECT_STRUCT_MACRO = "RSTRUCT(";
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

        virtual void Debug() const;
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

        void Debug() const override;
    };

    struct ParsedProperty : ParsedField
    {
        std::string name;
        std::string type;
        ParsedProperty() : ParsedField(Property)
        {
        
        }

        void Debug() const override;
    };

    enum EParsedType
    {
        Class,
        Struct,
        Enum
    };

    struct ParsedType
    {
        std::string name;
        EParsedType type;
        
        virtual void Debug() const;
    };

    struct ParsedStruct : ParsedType
    {
        std::vector<std::shared_ptr<ParsedField>> fields;
        ParsedStruct()
        {
            type = EParsedType::Struct;
        }

        void Debug() const override;
    };

    struct ParsedClass : ParsedStruct
    {
        std::vector<std::shared_ptr<ParsedField>> fields;
        ParsedClass()
        {
            type = EParsedType::Class;
        }

        void Debug() const override;
    };

    

    struct ParsedFile
    {
        std::filesystem::path filePath;
        std::vector<std::shared_ptr<ParsedType>> types;

        virtual void Debug() const;
    };

    struct FileParser
    {

    private:
        bool _isInComment = false;

    public:
        int lineNumber{};
        std::filesystem::path filePath;
        std::ifstream stream;
        
        FileParser(const std::filesystem::path &path);
        bool InternalReadLine(std::string& line);
        bool ReadLine(std::string& line);
        std::shared_ptr<ParsedProperty> ParseProperty();
        std::shared_ptr<ParsedFunction> ParseFunction();
        std::shared_ptr<ParsedClass> ParseClass();
        std::shared_ptr<ParsedStruct> ParseStruct();
        std::shared_ptr<ParsedFile> Parse();
    };
    class Parser
    {
    public:
        std::vector<std::filesystem::path> files;
        
        [[nodiscard]] std::shared_ptr<ParsedFile> ParseFile(const std::filesystem::path &path) const;
        [[nodiscard]] std::vector<std::shared_ptr<ParsedFile>> Parse() const;
    };
    
}
