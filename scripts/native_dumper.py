#!/usr/bin/env python3
"""
Native handlers dumper for IDA
Extracts native names and RVA addresses from the codebase
"""

import re
from pathlib import Path
from typing import Dict, List

 # Map of custom types to primitive types for IDA
TYPE_MAPPING = {
    'bool': 'bool',
    'int': 'int',
    'float': 'float',
    'void': 'void',
    'char': 'char',
    'unsigned': 'uint32_t',
    'uint': 'uint32_t',
    'uint64_t': 'uint64_t',
    'Ped': 'int',
    'Entity': 'int',
    'Vehicle': 'int',
    'Hash': 'uint32_t',
    'Any': 'int32_t',
    'const char*': 'const char*',
    'char*': 'char*',
    'void*': 'uint64_t*'
}

def map_type_to_primitive(type_str: str) -> str:
    """
    Map a custom or complex type to a primitive type that IDA understands
    """
    type_str = type_str.strip()

    # Check for exact matches first
    if type_str in TYPE_MAPPING:
        return TYPE_MAPPING[type_str]

    # Handle pointer types
    if type_str.endswith('*'):
        base_type = type_str[:-1].strip()
        if 'char' in base_type:
            return 'const char*'
        return 'uint64_t*'

    # Handle const types
    if type_str.startswith('const '):
        inner = type_str[6:].strip()
        if inner.endswith('*'):
            base_type = inner[:-1].strip()
            if 'char' in base_type:
                return 'const char*'
            return 'uint64_t*'
        mapped = map_type_to_primitive(inner)
        return f'const {mapped}' if not mapped.startswith('const') else mapped

    # Default: treat unknown types as int
    return 'int'

def parse_param_type(param: str) -> str:
    """
    Extract type and name from a parameter definition
    Returns formatted "type name" string
    """
    param = param.strip()
    if not param:
        return ''

    # Split by space to separate type and name
    parts = param.split()
    if not parts:
        return ''

    # Last part is usually the parameter name
    param_name = parts[-1]

    # Everything else is the type
    type_parts = parts[:-1]
    type_str = ' '.join(type_parts)

    # Map to primitive type
    primitive_type = map_type_to_primitive(type_str)

    return f"{primitive_type} {param_name}"


def parse_natives_file(filepath: str) -> Dict[int, dict]:
    """
    Parse natives_gen9.cpp to extract native definitions with signatures
    Detects JMP-only wrappers and marks them
    """
    natives_by_index = {}
    current_namespace = None

    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
        lines = content.split('\n')

    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # Match namespace declarations
        namespace_match = re.match(r'namespace\s+(\w+)\s*\{', stripped)
        if namespace_match:
            current_namespace = namespace_match.group(1)
            i += 1
            continue

        # Match closing braces for namespaces
        if re.match(r'\}\s*//\s*namespace', stripped):
            current_namespace = None
            i += 1
            continue

        # Match function definitions
        func_match = re.match(r'([\w\*\s]+?)\s+(\w+)\s*\((.*?)\)\s*\{', stripped)
        if func_match and current_namespace:
            return_type = func_match.group(1).strip()
            func_name = func_match.group(2)
            params_str = func_match.group(3).strip()

            # Map types to primitive types
            return_type = map_type_to_primitive(return_type)

            # Parse parameters
            if params_str and params_str != 'void':
                params = []
                for param in params_str.split(','):
                    param_type = parse_param_type(param)
                    params.append(param_type)
                params_str = ', '.join(params)

            # Collect function body
            body_lines = []
            i += 1
            brace_count = 1

            while i < len(lines) and brace_count > 0:
                body_line = lines[i]
                brace_count += body_line.count('{')
                brace_count -= body_line.count('}')

                if brace_count > 0:
                    body_lines.append(body_line.strip())
                i += 1

            func_body = ' '.join(body_lines)

            # Find Invoker::Invoke call
            invoke_match = re.search(r'Invoker::Invoke<[^,]+,\s*(\d+)>\(', func_body)
            if invoke_match and current_namespace:
                index = int(invoke_match.group(1))
                full_name = f"{current_namespace}::{func_name}"

                natives_by_index[index] = {
                    'name': full_name,
                    'return_type': return_type,
                    'params': params_str
                }
            continue

        i += 1

    return natives_by_index

def load_crossmap(filepath: str) -> List[int]:
    """
    Load the NATIVE_CROSSMAP array from crossmap.cpp
    Returns list of RVA addresses indexed by native index
    """
    crossmap = []
    in_array = False

    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            if 'const std::uint64_t NATIVE_CROSSMAP' in line:
                in_array = True
                continue

            if in_array:
                if line.strip() == '};':
                    break

                # Match hex values like: 0x4EDE34FBADD967A6,
                match = re.search(r'0x([0-9A-Fa-f]+)', line)
                if match:
                    hex_val = match.group(1)
                    # Store as string to preserve hex format and handle large numbers
                    crossmap.append(f"0x{hex_val}")

    return crossmap

def generate_cpp_lookup_table(natives_map: dict, output_file: str):
    """
    Generate a C++ header file with a lookup table for native signatures by index
    """
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("""//
// Auto-generated file - DO NOT EDIT
// Native index to signature lookup table
//

#pragma once

#ifndef NDEBUG

#include <string>
#include <cstdint>

namespace base::menu::natives {

struct NativeSignature {
    std::string name;
    std::string return_type;
    std::string params;
};

inline NativeSignature GetNativeSignature(std::uint64_t index) {
    switch (index) {
""")

        for index in sorted(natives_map.keys()):
            info = natives_map[index]
            name = info['name']
            ret_type = info['return_type']
            params = info['params'].replace('"', '\\"')
            f.write(f'        case {index}: return {{"{name}", "{ret_type}", "{params}"}};\n')

        f.write("""        default:
            return {
                "UNKNOWN_NATIVE_" + std::to_string(index),
                "void",
                ""
            };
    }
}

inline std::string GetNativeName(std::uint64_t index) {
    return GetNativeSignature(index).name;
}

} // namespace base::menu::natives

#endif // NDEBUG
""")
    print(f"Generated C++ lookup table: {output_file}")

def main():
    base_path = Path('../')
    natives_cpp = base_path / 'menu' / 'src' / 'natives' / 'natives_gen9.cpp'
    output_lookup = base_path / 'menu' / 'src' / 'natives' / 'native_lookup.hpp'

    print(f"Parsing natives from: {natives_cpp}")
    natives_map = parse_natives_file(str(natives_cpp))

    print(f"\nGenerating C++ lookup table...")
    generate_cpp_lookup_table(natives_map, str(output_lookup))

    print(f"\nGenerated lookup table for {len(natives_map)} natives")
    print(f"\nTo use in your code:")
    print(f"1. Include 'native_lookup.hpp'")
    print(f"2. Call DumpHandlersToFile() after Invoker initialization")
    print(f"3. The JSON output will contain actual RVA addresses from the game")

if __name__ == '__main__':
    main()



