#include "Generate.hpp"

namespace reflect::generate
{
    std::string Include(const std::string& file)
    {
        return "#include \"" + file + "\"";
    }

    void GenerateClass(std::ofstream& stream, const std::shared_ptr<parser::ParsedClass>& cls)
    {
        stream << "#ifndef REFLECT_CLASS_" << cls->name << std::endl;
        stream << "#define REFLECT_CLASS_" << cls->name << std::endl;
        
        std::vector<std::string> reflectedFieldMacros;
        for(const auto &field : cls->fields)
        {
            switch (field->fieldType)
            {
            case parser::EParsedFieldType::Function:
                {
                    const auto prop = std::dynamic_pointer_cast<parser::ParsedFunction>(field);
                    auto macro = "REFLECTED_CLASS_" + cls->name + "_FUNCTION_" + prop->name;
                    reflectedFieldMacros.push_back(macro);
                    stream << "#define " << macro << " REFLECT_WRAP_FUNCTION_BEGIN(" << prop->name << ")" << " \\" << std::endl;
                    stream << "{" << " \\" << std::endl;
                    std::vector<std::string> argNames;
                    auto i = 0;
                    for(const auto &arg : prop->arguments)
                    {
                        auto argName = "arg_" + std::to_string(i);
                        
                        stream << "auto " << argName << " = " << "args[" << i << "]" << ".As<" << arg->type << ">();" << " \\" << std::endl;
                        argNames.push_back(argName);
                        i++;
                    }

                    std::string funcCall = "instance.As<" + cls->name + ">()->" + prop->name + "(";
                    for(const auto &argName : argNames)
                    {
                        funcCall += "*" + argName;
                        if(argName != argNames[argNames.size() - 1])
                        {
                            funcCall += ",";
                        }
                        
                    }
                    funcCall += ");";

                    if(prop->result != "void")
                    {
                        std::stringstream checkedCall;
                        checkedCall << "if(result){" << " \\" << std::endl
                        << "*result.As<" << prop->result << ">() = " << funcCall << " \\" << std::endl
                        << "}";
                        funcCall = checkedCall.str();
                    }

                    stream << " \\" << std::endl;
                    stream << funcCall << " \\" << std::endl;
                    stream << "})" << std::endl;
                }
                break;
            case parser::Property:
                {
                    
                    
                    const auto prop = std::dynamic_pointer_cast<parser::ParsedProperty>(field);

                    auto macro = "REFLECTED_CLASS_" + cls->name + "_PROPERTY_" + prop->name;
                    
                    reflectedFieldMacros.push_back(macro);
                    
                    stream << "#define " << macro << " REFLECT_WRAP_PROPERTY(" << cls->name << "," << prop->name << "," << prop->type << ")" << std::endl;
                    stream << std::endl;
                }
                break;

                default:
                break;
            }
        }

        stream << "#define REFLECT_GENERATE_" + cls->name << " \\" << std::endl;
        stream << "reflect::factory::ReflectTypeBuilder builder; \\" << std::endl;
        
        for(auto &macro : reflectedFieldMacros)
        {
            stream << "builder.AddField(" << macro << "); \\" << std::endl;
        }

        stream << "builder.Create<" << cls->name << ">(\"" << cls->name << "\");" << std::endl;

        stream << "#endif" << std::endl;
    }

    void Generate(const std::shared_ptr<parser::ParsedFile>& parsedFile, const std::filesystem::path& filePath)
    {
        std::ofstream file(filePath);

        if(!file.is_open()) return;

        file << "#pragma once" << std::endl;
        file << Include("reflect/Macro.hpp") << std::endl;
        file << Include("reflect/Reflect.hpp") << std::endl;
        file << Include("reflect/Factory.hpp") << std::endl;
        file << Include("reflect/wrap/Wrap.hpp") << std::endl;
        file << std::endl;
        file << std::endl;

        for (auto &parsed : parsedFile->types)
        {
            switch (parsed->type)
            {
            case parser::Class:
                GenerateClass(file,std::dynamic_pointer_cast<parser::ParsedClass>(parsed));
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
