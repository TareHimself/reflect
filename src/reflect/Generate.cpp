#include "reflect/Generate.hpp"

namespace reflect::generate
{
    std::string include(const std::string& file)
    {
        return "#include \"" + file + "\"";
    }

    void generateClass(std::ofstream& stream, const std::shared_ptr<parser::ParsedClass>& cls)
    {
        guardBegin(stream,cls->name);
        
        const std::vector<std::string> macros = writeMacros(stream,cls->name,cls->fields);

        writeBuilder(stream,cls->name,macros);

        guardEnd(stream);
    }

    void generateStruct(std::ofstream& stream, const std::shared_ptr<parser::ParsedStruct>& cls)
    {
        guardBegin(stream,cls->name);

        const std::vector<std::string> macros = writeMacros(stream,cls->name,cls->fields);

        writeBuilder(stream,cls->name,macros);

        guardEnd(stream);
    }

    void guardBegin(std::ofstream& stream,const std::string& name)
    {
        stream << "#ifndef __REFLECT_GENERATED_" << name << std::endl;
        stream << "#define __REFLECT_GENERATED_" << name << std::endl;
    }

    void guardEnd(std::ofstream& stream)
    {
        stream << "#endif" << std::endl;
        stream << std::endl;
    }

    std::vector<std::string> writeMacros(std::ofstream& stream,const std::string& typeName,const std::vector<std::shared_ptr<parser::ParsedField>>& fields)
    {
        std::vector<std::string> macros;
        
        for(const auto &field : fields)
        {
            switch (field->fieldType)
            {
            case parser::EParsedFieldType::Function:
                macros.push_back(writeFunction(stream,typeName,std::dynamic_pointer_cast<parser::ParsedFunction>(field)));
                break;
            case parser::EParsedFieldType::Property:
                macros.push_back(writeProperty(stream,typeName,std::dynamic_pointer_cast<parser::ParsedProperty>(field)));
                break;

            default:
                break;
            }
        }

        return macros;
    }

    std::string writeProperty(std::ofstream& stream, const std::string& typeName, const std::shared_ptr<parser::ParsedProperty>& prop)
    {
        auto macro = "__REFLECTED_GENERATED_" + typeName + "_PROPERTY_" + prop->name;
                    
        stream << "#define " << macro << " REFLECT_WRAP_PROPERTY(" << typeName << "," << prop->name << "," << prop->type << ")" << std::endl;
        stream << std::endl;

        return macro;
    }

    std::string writeFunction(std::ofstream& stream, const std::string& typeName,
        const std::shared_ptr<parser::ParsedFunction>& func)
    {
        auto macro = "__REFLECTED_GENERATED_" + typeName + "_FUNCTION_" + func->name;

        stream << "#define " << macro << " REFLECT_WRAP_FUNCTION_BEGIN(" << func->name << ")" << " \\" << std::endl;
        stream << "{" << " \\" << std::endl;
        std::vector<std::string> argNames;
        auto i = 0;
        for(const auto &arg : func->arguments)
        {
            auto argName = "arg_" + std::to_string(i);
                        
            stream << "auto " << argName << " = " << "args[" << i << "]" << ".As<" << arg->type << ">();" << " \\" << std::endl;
            argNames.push_back(argName);
            i++;
        }

        std::string funcCall = "instance.As<" + typeName + ">()->" + func->name + "(";
        for(const auto &argName : argNames)
        {
            funcCall += "*" + argName;
            if(argName != argNames[argNames.size() - 1])
            {
                funcCall += ",";
            }
                        
        }
        funcCall += ");";

        if(func->result != "void")
        {
            std::stringstream checkedCall;
            checkedCall << "if(result){" << " \\" << std::endl
            << "*result.As<" << func->result << ">() = " << funcCall << " \\" << std::endl
            << "}";
            funcCall = checkedCall.str();
        }

        stream << " \\" << std::endl;
        stream << funcCall << " \\" << std::endl;
        stream << "})" << std::endl;

        return macro;
    }

    void writeBuilder(std::ofstream& stream, const std::string& typeName, const std::vector<std::string>& macros)
    {
        stream << "#define __REFLECT_GENERATE_" + typeName << " \\" << std::endl;
        
        stream << "reflect::factory::ReflectTypeBuilder builder; \\" << std::endl;
        
        for(auto &macro : macros)
        {
            stream << "builder.AddField(" << macro << "); \\" << std::endl;
        }

        stream << "builder.Create<" << typeName << ">(\"" << typeName << "\");" << std::endl;
    }

    void Generate(const std::shared_ptr<parser::ParsedFile>& parsedFile, const std::filesystem::path& filePath)
    {
        std::ofstream file(filePath);

        if(!file.is_open()) return;

        file << "#pragma once" << std::endl;
        file << include("reflect/Macro.hpp") << std::endl;
        file << include("reflect/Reflect.hpp") << std::endl;
        file << include("reflect/Factory.hpp") << std::endl;
        file << include("reflect/wrap/Wrap.hpp") << std::endl;
        file << std::endl;
        file << std::endl;

        for (auto &parsed : parsedFile->types)
        {
            switch (parsed->type)
            {
            case parser::Class:
                generateClass(file,std::dynamic_pointer_cast<parser::ParsedClass>(parsed));
                break;
            case parser::Struct:
                generateStruct(file,std::dynamic_pointer_cast<parser::ParsedStruct>(parsed));
                break;
                default:
                    break;
            }

            file << std::endl;
            file << std::endl;
        }

        file.close();
    }
}
