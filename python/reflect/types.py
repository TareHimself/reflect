from enum import Enum

class EParsedFieldType(Enum):
    Property = 0
    Function = 1

class ParsedField:
    def __init__(self,type: EParsedFieldType) -> None:
        self.field_type = type

class ParsedFunctionArgument:
    def __init__(self) -> None:
        self.type = ""
        self.name = ""

class ParsedFunction(ParsedField):
    def __init__(self) -> None:
        super().__init__(EParsedFieldType.Function)
        self.name = ""
        self.return_type = ""
        self.is_static = False
        self.arguments: list[ParsedFunctionArgument] = []

class ParsedProperty(ParsedField):
    def __init__(self) -> None:
        super().__init__(EParsedFieldType.Property)
        self.name = ""
        self.type = ""

class EParsedType(Enum):
    Class = 0
    Struct = 1
    Enum = 2

class ParsedType:
    def __init__(self,type: EParsedType) -> None:
        self.name = ""
        self.type = type

class ParsedStruct(ParsedType):
    def __init__(self) -> None:
        super().__init__(EParsedType.Struct)
        self.fields: list[ParsedField] = []

class ParsedClass(ParsedStruct):
    def __init__(self) -> None:
        super().__init__()
        self.type = EParsedType.Class

class ParsedFile:
    def __init__(self) -> None:
        self.file_path = ""
        self.types: list[ParsedType] = []
        
