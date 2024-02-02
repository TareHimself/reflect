#include <iostream>
#include <string>
#include "reflect/Parser.hpp"
#include "reflect/Utils.hpp"

namespace reflect::parser
{
    void ParsedField::Debug() const
    {
    }

    void ParsedFunction::Debug() const
    {
        std::cout << "Function: " << result << " " << name << "(";
        for (auto& arg : arguments)
        {
            std::cout << arg->type << " " << arg->name;
            if (arg != arguments.at(arguments.size() - 1))
            {
                std::cout << ",";
            }
        }

        std::cout << ")" << std::endl;
    }

    void ParsedProperty::Debug() const
    {
        ParsedField::Debug();
    }

    void ParsedType::Debug() const
    {
        switch (type)
        {
        case Class:
            std::cout << "Class : " << name << std::endl;
            break;
        case Enum:
            std::cout << "Enum : " << name << std::endl;
            break;
        case Struct:
            std::cout << "Struct : " << name << std::endl;
            break;
        default:
            std::cout << "Unknown : " << name << std::endl;
            break;
        }
    }

    void ParsedStruct::Debug() const
    {
        ParsedType::Debug();
        for (auto& f : fields)
        {
            f->Debug();
        }
    }

    void ParsedClass::Debug() const
    {
        ParsedStruct::Debug();
    }

    void ParsedFile::Debug() const
    {
        std::cout << "File: " << filePath << "\n";
        for (auto& t : types)
        {
            t->Debug();
        }
    }

    FileParser::FileParser(const std::filesystem::path& path)
    {
        filePath = path;
        stream.open(path);
    }

    bool FileParser::InternalReadLine(std::string& line)
    {
        const auto result = static_cast<bool>(std::getline(stream, line));
        if (result)
        {
            ++lineNumber;
        }
        return result;
    }

    bool FileParser::ReadLine(std::string& line)
    {
        std::string lineTemp;
        auto result = false;
        do
        {
            if (utils::trim(lineTemp).empty())
            {
                result = InternalReadLine(lineTemp);
            }

            if (_isInComment)
            {
                if (const auto pos = lineTemp.find("*/"); pos != std::string::npos)
                {
                    lineTemp = lineTemp.substr(pos + 2);
                    _isInComment = false;
                }
                else
                {
                    result = InternalReadLine(lineTemp);
                }
            }
            else if (const auto pos = lineTemp.find("/*"); pos != std::string::npos)
            {
                lineTemp = lineTemp.substr(pos + 2);
                _isInComment = true;
            }
            else if (const auto pos = lineTemp.find("//"); pos != std::string::npos)
            {
                lineTemp = lineTemp.substr(pos);
            }
        }
        while (result && (utils::trim(lineTemp).empty() || _isInComment));

        line = lineTemp;

        return result;
    }

    std::shared_ptr<ParsedProperty> FileParser::ParseProperty()
    {
        auto result = std::make_shared<ParsedProperty>();

        std::string line;
        ReadLine(line);
        line = utils::trim(line);

        std::smatch match;
        if (std::regex_match(line, match, REFLECT_PROPERTY_REGEX))
        {
            result->type = utils::trim(std::string(match[1].str()));
            result->name = utils::trim(std::string(match[2].str()));
        }

        return result;
    }

    std::shared_ptr<ParsedFunction> FileParser::ParseFunction()
    {
        auto result = std::make_shared<ParsedFunction>();

        std::string line;
        ReadLine(line);
        line = utils::trim(line);
        const auto staticStr = std::string("static ");
        
        result->bIsStatic = line.find(staticStr) != std::string::npos;
        if(result->bIsStatic)
        {
            auto strSize = staticStr.size();
            line = line.substr(line.find(staticStr) + strSize);
        }
        const auto splitLine = utils::split(utils::trim(line), " ");
        result->name = utils::split(splitLine.at(1), "(").at(0);
        result->result = splitLine.at(0);
        auto argsLine = utils::trim(line);
        argsLine = utils::split(argsLine, "(").at(1);
        size_t pos = 0;
        pos = argsLine.find(")");
        while (pos == std::string::npos)
        {
            ReadLine(line);
            argsLine += utils::trim(line);
            pos = argsLine.find(")");
        }
        pos = argsLine.find(")");
        argsLine = argsLine.substr(0, pos);
        const auto argsArr = utils::split(utils::trim(argsLine), ",");

        for (auto& arg : argsArr)
        {
            std::smatch match;
            if (std::regex_match(arg, match, REFLECT_ARGUMENT_REGEX))
            {
                auto type = match[1];
                auto name = match[2];
                result->arguments.push_back(
                    std::make_shared<ParsedFunctionArgument>(utils::trim(type.str()), utils::trim(name.str())));
            }
        }
        return result;
    }

    std::shared_ptr<ParsedClass> FileParser::ParseClass()
    {
        auto result = std::make_shared<ParsedClass>();
        std::string line;
        const std::string searchStr = "class ";
        while (line.find(searchStr) == std::string::npos)
        {
            ReadLine(line);
            line = utils::trim(line);
        }
        // Get Class Name
        const auto classSpecPos = line.find(searchStr);

        if (classSpecPos == std::string::npos) return {};

        const auto splitResult = utils::split(utils::trim(line.substr(classSpecPos + searchStr.size())), " ");

        if (splitResult.empty()) return {};

        result->name = splitResult.at(0);

        // Parse Fields
        while (ReadLine(line))
        {
            if (line.find("};") != std::string::npos) return result;

            if (line.find(REFLECT_PROPERTY_MACRO) != std::string::npos)
            {
                if (auto r = ParseProperty())
                {
                    result->fields.push_back(r);
                }
            }
            else if (line.find(REFLECT_FUNCTION_MACRO) != std::string::npos)
            {
                if (auto r = ParseFunction())
                {
                    result->fields.push_back(r);
                }
            }
        }

        return result;
    }

    std::shared_ptr<ParsedStruct> FileParser::ParseStruct()
    {
        auto result = std::make_shared<ParsedStruct>();
        std::string line;
        const std::string searchStr = "struct ";
        while (line.find(searchStr) == std::string::npos)
        {
            ReadLine(line);
            line = utils::trim(line);
        }
        // Get Class Name
        const auto classSpecPos = line.find(searchStr);

        if (classSpecPos == std::string::npos) return {};

        const auto splitResult = utils::split(utils::trim(line.substr(classSpecPos + searchStr.size())), " ");

        if (splitResult.empty()) return {};

        result->name = splitResult.at(0);

        // Parse Fields
        while (ReadLine(line))
        {
            if (line.find("};") != std::string::npos) return result;

            if (line.find(REFLECT_PROPERTY_MACRO) != std::string::npos)
            {
                if (auto r = ParseProperty())
                {
                    result->fields.push_back(r);
                }
            }
            else if (line.find(REFLECT_FUNCTION_MACRO) != std::string::npos)
            {
                if (auto r = ParseFunction())
                {
                    result->fields.push_back(r);
                }
            }
        }

        return result;
    }

    std::shared_ptr<ParsedFile> FileParser::Parse()
    {
        if (!stream.is_open()) return {};

        auto result = std::make_shared<ParsedFile>();

        result->filePath = filePath;
        std::string line;
        std::string content;
        while (ReadLine(line))
        {
            if (line.find(REFLECT_CLASS_MACRO) != std::string::npos)
            {
                if (auto cls = ParseClass())
                {
                    result->types.push_back(cls);
                }
            }
            else if (line.find(REFLECT_STRUCT_MACRO) != std::string::npos)
            {
                if (auto rstruct = ParseStruct())
                {
                    result->types.push_back(rstruct);
                }
            }
        }

        lineNumber = 0;
        stream.close();

        return result;
    }


    std::shared_ptr<ParsedFile> Parser::ParseFile(const std::filesystem::path& path) const
    {
        if (!std::filesystem::exists(path)) return {};

        FileParser parser(path);

        return parser.Parse();
    }

    std::vector<std::shared_ptr<ParsedFile>> Parser::Parse() const
    {
        std::vector<std::shared_ptr<ParsedFile>> result;

        for (auto& file : files)
        {
            std::cout << "Parsing " << file << std::endl;
            if (auto r = ParseFile(file); !r->types.empty())
            {
                result.push_back(r);
            }
            std::cout << "Parsed " << file << std::endl;
        }
        return result;
    }
}
