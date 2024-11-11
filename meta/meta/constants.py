ARGUMENT_REGEX = "(?:const[\s]+?)?([a-zA-Z:0-9_*\s<>]+)(?:[&\s]+)([a-zA-Z0-9_]+)"
PROPERTY_REGEX = "([a-zA-Z0-9<> :*]+) ([a-zA-Z0-9_]+).*"
FUNCTION_REGEX = "(?:virtual\s)?(static|)([a-zA-Z0-9<>*_ :]+)\s([a-zA-Z0-9_]+)\("