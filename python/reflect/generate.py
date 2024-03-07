import os
from uuid import uuid4
from io import TextIOWrapper
from reflect.types import ParsedClass, ParsedStruct, ParsedField, ParsedProperty, ParsedFunction, ParsedFile, EParsedType, EParsedFieldType, ParsedType

def write_line(file: TextIOWrapper,line: str) -> None:
    file.write(f"{line}\n")

class IncludeGuard():
    def __init__(self,generator: 'FileGenerator',name: str):
        self.generator = generator
        self.name = name
         
    def __enter__(self):
        self.generator.write_line(f"#ifndef _REFLECT_GENERATED_{self.name}")
        self.generator.write_line(f"#define _REFLECT_GENERATED_{self.name}")
        return self
     
    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.generator.write_line("#endif")

class FileGenerator:
    def __init__(self,parsed_file: ParsedFile,dest_file: str,dest_header_ext: str) -> None:
        self.parsed_file = parsed_file
        self.dest_file = dest_file
        self.header_file_path = self.dest_file + f'{dest_header_ext}'
        self.source_file_path = self.dest_file + f"cpp"
        self.id = str(uuid4()).replace('-','')
        self.source_lines: list[str] = []
        
        with open(self.header_file_path,'w') as f:
            self.file = f
            self.generate_header()
            self.file = None


        with open(self.source_file_path,'w') as f:
            self.file = f
            self.generate_source()
            self.file = None

    
    def generate_source(self):
        result_dir = os.path.dirname(self.source_file_path)
        rel_path = os.path.relpath(self.parsed_file.file_path,result_dir)
        rel_include = rel_path.replace('\\','/')
        self.write_line(f'#include "{rel_include}"')
        self.write_line("")
        for line in self.source_lines:
            self.write_line(line)

    def generate_header(self):
        self.write_line("#pragma once")
        self.include("reflect/Macro.hpp")
        self.include("reflect/Reflect.hpp")
        self.include("reflect/Factory.hpp")
        self.include("reflect/wrap/Wrap.hpp")
        self.write_line("")

        self.write_id()

        self.write_line("")
        self.write_line("")

        for parsed in self.parsed_file.types:

            if parsed.type == EParsedType.Class:
                self.write_class(parsed)
            elif parsed.type == EParsedType.Struct:
                self.write_struct(parsed)

            self.write_line("")
            self.write_line("")

    def write_id(self):
        self.write_lines(["#ifdef REFLECT_FILE_ID",
                          "#undef REFLECT_FILE_ID",
                          "#endif",
                          f"#define REFLECT_FILE_ID {self.id}"])
        

    def write_line(self,line: str):
        self.file.write(f"{line}\n")

    def write_lines(self,lines: list[str]):
        for line in lines:
            self.write_line(line)

    def write_source_line(self,line: str):
        self.source_lines.append(line)

    def write_source_lines(self,lines: list[str]):
        for line in lines:
            self.write_source_line(line)


    def include(self,file: str):
        self.write_line(f"#include \"{file}\"")

    def write_property(self,type_name: str,field: ParsedProperty) -> tuple[str,list[str]]:
        macro = f"_REFLECTED_GENERATED_{type_name}_PROPERTY_{field.name}"

        self.write_line(f"#define {macro} REFLECT_WRAP_PROPERTY({type_name},{field.name},{field.type})")

        self.write_line("")

        return [macro,field.tags]
        
    def write_function(self,type_name: str,field: ParsedFunction) -> tuple[str,list[str]]:
        macro = f"_REFLECTED_GENERATED_{type_name}_FUNCTION_{field.name}"

        self.write_line(f"#define {macro} REFLECT_WRAP_FUNCTION_BEGIN({field.name}) \\")
        
        self.write_line("{ \\")

        arg_names: list[str] = []

        i = 0
        for arg in field.arguments:
            arg_name = f"arg_{i}"

            self.write_line(f"auto {arg_name} = args[{i}].As<{arg.type}>(); \\")

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

        self.write_line(" \\")
        self.write_line(f"{func_call} \\")
        self.write_line("})\n")

        return [macro,field.tags]

    def write_builder(self,type: ParsedType,macros:  list[tuple[str,list[str]]]) -> None:
        self.write_line(f"#define _reflected_{self.id}_{type.body_line}() \\")
        self.write_line('static std::shared_ptr<reflect::wrap::Reflected> Reflected; \\')
        self.write_line('std::shared_ptr<reflect::wrap::Reflected> GetReflected() const override;')

        self.write_source_line(f"std::shared_ptr<reflect::wrap::Reflected> {type.name}::Reflected = []()" + "{")
        self.write_source_line("reflect::factory::TypeBuilder builder;")

        for macro,tags in macros:
            self.write_source_line("{")
            self.write_source_line(f"  auto field = {macro};")
            for tag in  tags:
                self.write_source_line(f"  field->AddTag(\"{tag}\");") 
            self.write_source_line(f"  builder.AddField(field);")
            self.write_source_line("}")

        self.write_source_line(f"auto created = builder.Create<{type.name}>(\"{type.name}\");")

        for tag in  type.tags:
            self.write_source_line(f"created->AddTag(\"{tag}\");") 

        self.write_source_line('return created;')
        self.write_source_line('}();')
        self.write_source_lines(['',''])

        self.write_source_lines([
                f"std::shared_ptr<reflect::wrap::Reflected> {type.name}::GetReflected() const",
                "{",
                f"return reflect::factory::find<{type.name}>();",
                "}"
            ])
        

    def write_fields(self,type_name: str,fields: list[ParsedField]) -> list[str]:
        macros: list[str] = []

        for field in fields:
            if field.field_type == EParsedFieldType.Function:
                macros.append(self.write_function(type_name,field))
            elif field.field_type == EParsedFieldType.Property:
                macros.append(self.write_property(type_name,field))

        return macros

    def write_class(self,item: ParsedClass) -> None:
        with IncludeGuard(self,item.name):

            macros = self.write_fields(item.name,item.fields)

            self.write_builder(item,macros)

            

    def write_struct(self,item: ParsedStruct) -> None:
        with IncludeGuard(self,item.name):
            
            macros = self.write_fields(item.name,item.fields)

            self.write_builder(item,macros)
    

def generate(parsed_file: ParsedFile,file_path: str,ext: str):
    FileGenerator(parsed_file,file_path,ext)

 


