import re
from io import TextIOWrapper
from typing import Union
from reflect.constants import REFLECT_CLASS_MACRO, REFLECT_STRUCT_MACRO, REFLECT_PROPERTY_MACRO,REFLECT_FUNCTION_MACRO, REFLECT_PROPERTY_REGEX, REFLECT_ARGUMENT_REGEX
from reflect.types import ParsedFile,ParsedClass, ParsedStruct, ParsedFunction, ParsedProperty, ParsedFunctionArgument
class FileParser:
    def __init__(self,file_path: str) -> None:
        self.is_in_comment = False
        self.line_number = 0
        self.file_path = file_path
        self.file: Union[None,TextIOWrapper] = None

    def _read(self) -> Union[None,str]:
        if self.file is None:
            return None
        
        line = self.file.readline()

        if line == "":
            return None

        self.line_number += 1

        return line

    def read(self) -> Union[None,str]:
        
        line = ""

        while True:
            line_temp = self._read()

            if line_temp is None:
                return None
            
            line += line_temp.strip()

            if self.is_in_comment:
                if "*/" in line:
                    line = line[line.index("*/") + 1:]
                    self.is_in_comment = False
                else:
                    line = self._read()

            elif "/*" in line:
                line = line[line.index("/*") + 1:]
                self.is_in_comment = True
            elif "//" in line:
                line = line[:line.index("//")]

            if len(line) == 0 or self.is_in_comment:
                continue

            return line
            

    def parse_property(self) -> Union[None,ParsedProperty]:
        result  = ParsedProperty()

        line = self.read()

        match_result = re.findall(REFLECT_PROPERTY_REGEX,line)

        if len(match_result) < 1 or len(match_result[0]) < 2:
            return None
        
        match_result = match_result[0]
        
        result.type = match_result[0]
        result.name = match_result[1]

        return result



    def parse_function(self) -> Union[None,ParsedFunction]:
        line = self.read()
        static_str = "static "

        result = ParsedFunction()

        result.is_static = static_str in line

        if result.is_static:
            line = line[line.index(static_str) + len(static_str):]

        split_line = line.split(" ")

        result.name = split_line[1].split("()")[0]
        result.return_type = split_line[0]

        args_line = line.split("(")[1]

        while ")" not in args_line:
            args_line += self.read()

        args_line = args_line[:args_line.index(")")]

        args_arr = args_line.split(",")

        for arg in args_arr:
            match_result = re.findall(REFLECT_ARGUMENT_REGEX,arg)

            if len(match_result) < 1 or len(match_result[0]) < 2:
                continue

            match_result = match_result[0]

            p_arg = ParsedFunctionArgument()
            p_arg.name = match_result[1]
            p_arg.type = match_result[0]
            result.arguments.append(p_arg)

        return result

    def parse_class(self) -> Union[None,ParsedClass]:
        search_str = "class "
        line = self.read()

        while line is not None and search_str not in line:
            line = self.read()

        if line is None:
            return None
        
        split_result = line[line.index(search_str) + len(search_str):].split(" ")

        if len(split_result) == 0:
            return None
        
        result = ParsedClass()
        result.name = split_result[0]

        line = self.read()
        
        while line is not None:
            if "};" in line:
                return result
            
            if REFLECT_PROPERTY_MACRO in line:
                r = self.parse_property()

                if r is not None:
                    result.fields.append(r)
            elif REFLECT_FUNCTION_MACRO in line:
                r = self.parse_function()

                if r is not None:
                    result.fields.append(r)

            line = self.read()





    def parse_struct(self) -> Union[None,ParsedStruct]:
        search_str = "struct "
        line = self.read()

        while line is not None and search_str not in line:
            line = self.read()

        if line is None:
            return None
        
        split_result = line[line.index(search_str) + len(search_str):].split(" ")

        if len(split_result) == 0:
            return None
        
        result = ParsedStruct()
        result.name = split_result[0]

        line = self.read()
        
        while line is not None:
            if "};" in line:
                return result
            
            if REFLECT_PROPERTY_MACRO in line:
                r = self.parse_property()

                if r is not None:
                    result.fields.append(r)
            elif REFLECT_FUNCTION_MACRO in line:
                r = self.parse_function()

                if r is not None:
                    result.fields.append(r)

            line = self.read()

    def parse(self) -> Union[None,ParsedFile]:
        with open(self.file_path,'r') as f:
            self.file = f

            result = ParsedFile()

            result.file_path = self.file_path

            line = self.read()

            while line is not None:
                if REFLECT_CLASS_MACRO in line:
                    r = self.parse_class()
                    if r is not None:
                        result.types.append(r)
                elif REFLECT_STRUCT_MACRO in line:
                    r = self.parse_struct()
                    if r is not None:
                        result.types.append(r)
                
                line = self.read()

            self.line_number = 0

            return result
        

class Parser:
    def __init__(self,files: list[str]) -> None:
        self.files = files

    def parse_file(self,file_path: str) -> Union[None,ParsedFile]:
        parser = FileParser(file_path=file_path)

        return parser.parse()

    def parse(self) -> list[ParsedFile]:
        results: list[ParsedFile] = []

        for file in self.files:
            print(f"Parsing {file}")

            result = self.parse_file(file)

            if result is not None and len(result.types) > 0:
                results.append(result)

            print(f"Parsed {file}")

        return results