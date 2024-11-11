import os
from uuid import uuid4
from io import TextIOWrapper
from meta.config import Config
from meta.types import ParsedStruct, ParsedField, ParsedProperty, ParsedFunction, ParsedFile, EParsedType, EParsedFieldType

def write_line(file: TextIOWrapper,line: str) -> None:
    file.write(f"{line}\n")

class IncludeGuard():
    def __init__(self,generator: 'FileGenerator',name: str):
        self.generator = generator
        self.name = name
         
    def __enter__(self):
        self.generator.write_line(f"#ifndef _META_GENERATED_{self.name}")
        self.generator.write_line(f"#define _META_GENERATED_{self.name}")
        return self
     
    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.generator.write_line("#endif")

def format_tags(tags:list[str]):
    return '{' + ','.join(map(lambda a : f"\"{a}\"",tags)) + '}'

class FileGenerator:
    def __init__(self,parsed_file: ParsedFile,dest_file: str,config: Config) -> None:
        self.parsed_file = parsed_file
        self.dest_file = dest_file
        self.config = config
        self.header_file_path = self.dest_file + f'{config.header_extension}'
        self.source_file_path = self.dest_file + f"{config.source_extension}"
        self.id = str(uuid4()).replace('-','')
        self.source_lines: list[str] = []
        self.metadata_str = f"{self.config.smart_pointer}<{self.config.namespace}::Metadata>"
        
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

        for inc in self.config.includes:
            self.include(inc)

        self.write_line("")

        self.write_id()

        self.write_line("")
        self.write_line("")

        for parsed in self.parsed_file.types:

            if parsed.type == EParsedType.Struct:
                self.write_struct(parsed)

            self.write_line("")
            self.write_line("")

    def write_id(self):
        self.write_lines(["#ifdef META_FILE_ID",
                          "#undef META_FILE_ID",
                          "#endif",
                          f"#define META_FILE_ID {self.id}"])
        

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

    def write_property(self,type_name: str,field: ParsedProperty) -> None:
        self.write_source_line(f'.AddProperty("{field.name}",&{type_name}::{field.name},' + format_tags(field.tags) + ')')
        
    def write_function(self,type_name: str,field: ParsedFunction) -> None:

        self.write_source_line(f'.AddFunction("{field.name}",' +  f'[](const {self.config.namespace}::Value& result,const {self.config.namespace}::Value& instance, const {self.config.vector}<{self.config.namespace}::Value>& args)' + '{')
        
        arg_names: list[str] = []

        i = 0
        for arg in field.arguments:
            arg_name = f"arg_{i}"

            self.write_source_line(f"auto {arg_name} = args[{i}].GetPtr<{arg.type}>(); \\")

            arg_names.append(arg_name)

            i += 1

        if field.is_static:
            func_call = f"{type_name}::{field.name}("
        else:
            func_call = f"instance.GetPtr<{type_name}>()->{field.name}("

        for arg_name in arg_names:
            func_call += f"*{arg_name}"

            if arg_name != arg_names[-1]:
                func_call += ","
        
        func_call += ");"

        if field.return_type != "void":
            checked_call = "if(result){ \n"
            checked_call += f"*result.GetPtr<{field.return_type}>() = {func_call} \n"
            checked_call += "}"
            func_call = checked_call

        self.write_source_line(" ")
        self.write_source_line(f"{func_call} ")
        self.write_source_line("}," + format_tags(field.tags) + ")\n")


    def write_fields(self,type_name: str,fields: list[ParsedField]) -> None:

        for field in fields:
            if field.field_type == EParsedFieldType.Function:
                self.write_function(type_name,field)
            elif field.field_type == EParsedFieldType.Property:
                self.write_property(type_name,field)
            

    def write_struct(self,item: ParsedStruct) -> None:
        with IncludeGuard(self,item.name):

            self.write_line(f"#define _meta_{self.id}_{item.body_line}() \\")
            self.write_line(f'static {self.metadata_str} Meta; \\')
            self.write_line(f'{self.metadata_str} GetMeta() const override;')

            has_namespace = item.namespace != ""

            if has_namespace:
                self.write_source_line(f"namespace {item.namespace} " +  "{")
                self.write_source_line("")

            self.write_source_line(f"{self.metadata_str} {item.name}::Meta = []()" + "{")
            self.write_source_line(f"return {self.config.namespace}::TypeBuilder()")

            self.write_fields(item.name,item.fields)

            self.write_source_line(f".Create<{item.name}>(\"{item.name}\"," + format_tags(item.tags) +  ");")

            self.write_source_line('}();')
            self.write_source_lines(['',''])



            self.write_source_lines([
                    f"{self.config.smart_pointer}<{self.config.namespace}::Metadata> {item.name}::GetMeta() const",
                    "{",
                    f"return {self.config.namespace}::find<{item.name}>();",
                    "}"
                ])
            
            if has_namespace:
                self.write_source_line("")
                self.write_source_line("}")
    

def generate(parsed_file: ParsedFile,file_path: str,config: Config):
    FileGenerator(parsed_file,file_path,config)

 


