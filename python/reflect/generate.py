from io import TextIOWrapper
from reflect.types import ParsedClass, ParsedStruct, ParsedField, ParsedProperty, ParsedFunction, ParsedFile, EParsedType, EParsedFieldType

def write_line(file: TextIOWrapper,line: str) -> None:
    file.write(f"{line}\n")

class IncludeGuard():
    def __init__(self,file: TextIOWrapper,name: str):
        self.file = file
        self.name = name
         
    def __enter__(self):
        write_line(self.file,f"#ifndef _REFLECT_GENERATED_{self.name}")
        write_line(self.file,f"#define _REFLECT_GENERATED_{self.name}")
        return self
     
    def __exit__(self, exc_type, exc_value, exc_traceback):
        write_line(self.file,"#endif")



def include(file: TextIOWrapper,header: str) -> None:
    write_line(file,f"#include \"{header}\"")

def write_property(file: TextIOWrapper,type_name: str,field: ParsedProperty) -> str:
    macro = f"_REFLECTED_GENERATED_{type_name}_PROPERTY_{field.name}"

    write_line(file,f"#define {macro} REFLECT_WRAP_PROPERTY({type_name},{field.name},{field.type})")

    write_line(file,"")

    return macro
    
def write_function(file: TextIOWrapper,type_name: str,field: ParsedFunction) -> str:
    macro = f"_REFLECTED_GENERATED_{type_name}_FUNCTION_{field.name}"

    write_line(file,f"#define {macro} REFLECT_WRAP_FUNCTION_BEGIN({field.name}) \\")
    
    write_line(file,"{ \\")

    arg_names: list[str] = []

    i = 0
    for arg in field.arguments:
        arg_name = f"arg_{i}"

        write_line(file,f"auto {arg_name} = arg[{i}].As<{arg.type}>(); \\")

        arg_names.append(arg_name)

        i += 1

    if field.is_static:
        func_call = f"{type_name}::{field.name}("
    else:
        func_call = f"instance.As<{type_name}>()->{field.name}("

    for arg_name in arg_names:
        func_call += f"*{arg_name}"

        if arg_name != arg_names[-1]:
            func_call += ","
    
    func_call += ");"

    if field.return_type != "void":
        checked_call = "if(result){ \\\n"
        checked_call += f"*result.As<{field.return_type}>() = {func_call} \\\n"
        checked_call += "}"
        func_call = checked_call

    write_line(file," \\")
    write_line(file,f"{func_call} \\")
    write_line(file,"})\n")

    return macro

def write_builder(file: TextIOWrapper,type_name: str,macros: list[str]) -> None:
    write_line(file,f"#define _REFLECT_GENERATE_{type_name} \\")
    write_line(file,"reflect::factory::ReflectTypeBuilder builder; \\")

    for macro in macros:
        write_line(file,f"builder.AddField({macro}); \\")

    write_line(file,f"builder.Create<{type_name}>(\"{type_name}\");")

def write_fields(file: TextIOWrapper,type_name: str,fields: list[ParsedField]) -> list[str]:
    macros: list[str] = []

    for field in fields:
        if field.field_type == EParsedFieldType.Function:
            macros.append(write_function(file,type_name,field))
        elif field.field_type == EParsedFieldType.Property:
            macros.append(write_property(file,type_name,field))

    return macros

def write_class(file: TextIOWrapper,item: ParsedClass) -> None:
    with IncludeGuard(file,item.name):

        macros = write_fields(file,item.name,item.fields)

        write_builder(file,item.name,macros)

def write_struct(file: TextIOWrapper,item: ParsedStruct) -> None:
    with IncludeGuard(file,item.name):
        
        macros = write_fields(file,item.name,item.fields)

        write_builder(file,item.name,macros)

def generate(parsed_file: ParsedFile,file_path: str):
    with open(file_path,'w') as f:
        write_line(f,"#pragma once")
        include(f,"reflect/Macro.hpp")
        include(f,"reflect/Reflect.hpp")
        include(f,"reflect/Factory.hpp")
        include(f,"reflect/wrap/Wrap.hpp")
        write_line(f,"")
        write_line(f,"")

        for parsed in parsed_file.types:

            if parsed.type == EParsedType.Class:
                write_class(f,parsed)
            elif parsed.type == EParsedType.Struct:
                write_struct(f,parsed)

            write_line(f,"")
            write_line(f,"")




