from reflect.parser import Parser
from reflect.generate import generate
import os
import sys
import argparse

class SmartFormatter(argparse.HelpFormatter):
    def _split_lines(self, text, width):
        if text.startswith("R|"):
            return text[2:].splitlines()
        # this is the RawTextHelpFormatter._split_lines
        return argparse.HelpFormatter._split_lines(self, text, width)
    
def search_for_headers(dir: str,header_ext: str) -> list[str]:
    results: list[str] = []
    
    for entry in os.listdir(dir):
        entry = os.path.join(dir,entry)
        if os.path.isdir(entry):
            results = results + search_for_headers(entry,header_ext)
        elif entry.endswith(f".{header_ext}") and not entry.endswith(f"reflect.{header_ext}"):
            results.append(entry)

    return results
            
def main():
    parser = argparse.ArgumentParser(
        prog="Reflect Header Generator",
        description="Generates headers that can be used with Reflect",
        formatter_class=SmartFormatter,
        exit_on_error=True
    )

    parser.add_argument(
        "-s",
        "--source",
        type=str,
        help="The directory to search for source files.",
        required=True
    )

    parser.add_argument(
        "-f",
        "--filter",
        type=str,
        default="hpp",
        help="Filter extension for source files.",
        required=False
    )

    parser.add_argument(
        "-o",
        "--output",
        type=str,
        help="The directory to output to. Defaults to the source directory.",
        required=False
    )

    args = parser.parse_args()

    source_dir = os.path.abspath(args.source)
    output_dir = os.path.abspath(args.output if args.output is not None else source_dir)
    filter_ext = args.filter

    files_to_parse = search_for_headers(source_dir,filter_ext)

    p = Parser(files_to_parse)

    parsed_files = p.parse()

    for parsed_file in parsed_files:
        new_relative_path = parsed_file.file_path[len(source_dir) + 1:-(len(filter_ext))] + f"reflect.{filter_ext}"
        out_file_path = os.path.join(output_dir,new_relative_path)
        generate(parsed_file,out_file_path)

if __name__ == "__main__":
    main()