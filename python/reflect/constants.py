REFLECT_CLASS_MACRO = "RCLASS("
REFLECT_STRUCT_MACRO = "RSTRUCT("
REFLECT_FUNCTION_MACRO = "RFUNCTION("
REFLECT_PROPERTY_MACRO = "RPROPERTY("
REFLECT_BODY_MACRO = "REFLECTED_BODY()"
REFLECT_ARGUMENT_REGEX = "(?:const[\s]+?)?([a-zA-Z:0-9_*\s<>]+)(?:[&\s]+)([a-zA-Z0-9_]+)"
REFLECT_PROPERTY_REGEX = "([a-zA-Z0-9<> :*]+) ([a-zA-Z0-9_]+).*"
REFLECT_FUNCTION_REGEX = "(?:virtual\s)?(static|)([a-zA-Z0-9<>*_ :]+)\s([a-zA-Z0-9_]+)\("