"""
Generate C++ header and implementation files for GTA5 natives from nativedb data.
"""
import sys
from pathlib import Path
from typing import Dict, List, Tuple

import requests

# Configuration
NATIVES_URL = "https://raw.githubusercontent.com/alloc8or/gta5-nativedb-data/master/natives_gen9.json"
BASE_NAMESPACE = "base::menu::natives"
INVOKER_FUNCTION = "Invoker::Invoke"
OUTPUT_HEADER = "natives_gen9.hpp"
OUTPUT_IMPL = "natives_gen9.cpp"
OUTPUT_CROSSMAP_HEADER = "crossmap.hpp"
OUTPUT_CROSSMAP_IMPL = "crossmap.cpp"
CROSSMAP_FILE = "crossmap.txt"
LOG_NATIVE_ERROR_FUNC = "LOG_ERROR"

# Optional header includes to add at the top of the generated header file
HEADER_INCLUDES = [
    "#include \"../rage/native_types.hpp\"",
    "#include \"invoker.hpp\""
]

# Native groups to exclude from generation (case-insensitive)
EXCLUDED_GROUPS = [
    "BUILTIN"
]


class NativeGenerator:
    """Generates C++ header and implementation files for GTA5 natives."""

    def __init__(self, base_namespace: str, invoker_function: str, header_includes: List[str] = None, excluded_groups: List[str] = None):
        self.base_namespace = base_namespace
        self.invoker_function = invoker_function
        self.header_includes = header_includes or []
        self.excluded_groups = {g.lower() for g in (excluded_groups or [])}  # Normalize to lowercase
        self.natives_data = {}
        self.crossmap = []  # List of (hash1, hash2) tuples
        self.hash_to_index = {}  # Maps native hash to array index

    def fetch_natives(self, url: str) -> bool:
        """Fetch natives data from the specified URL."""
        try:
            print("Fetching natives data...")
            response = requests.get(url, timeout=10)
            response.raise_for_status()
            self.natives_data = response.json()
            print(f"Successfully fetched natives data ({len(self.natives_data)} groups)")
            return True
        except requests.exceptions.RequestException as e:
            print(f"Error: Failed to fetch natives data: {e}")
            return False

    def load_crossmap(self, crossmap_path: str) -> bool:
        """Load crossmap file and build hash to index mapping."""
        crossmap_file = Path(crossmap_path)
        if not crossmap_file.exists():
            print(f"Warning: Crossmap file not found: {crossmap_path}")
            print("Generating without crossmap (using hashes directly)")
            return False

        try:
            print(f"Loading crossmap from {crossmap_path}...")
            with open(crossmap_file, 'r', encoding='utf-8') as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue

                    parts = line.split(',')
                    if len(parts) >= 2:
                        hash1 = parts[0].strip()
                        hash2 = parts[1].strip()

                        # Add to crossmap array
                        index = len(self.crossmap)
                        self.crossmap.append((hash1, hash2))

                        # Map both hashes to this index
                        self.hash_to_index[hash1] = index
                        self.hash_to_index[hash2] = index

            print(f"Loaded {len(self.crossmap)} crossmap entries")
            return True
        except IOError as e:
            print(f"Error: Failed to read crossmap file: {e}")
            return False

    def _build_params(self, params: List[Dict]) -> Tuple[str, str]:
        """
        Build parameter strings for function signature and call.

        Returns:
            Tuple of (signature_params, call_params)
        """
        if not params:
            return "", ""

        signature_parts = [f"{p['type']} {p['name']}" for p in params]
        call_parts = [p['name'] for p in params]

        return ", ".join(signature_parts), ", ".join(call_parts)

    def _is_group_excluded(self, group_name: str) -> bool:
        """Check if a native group should be excluded from generation."""
        return group_name.lower() in self.excluded_groups

    def _generate_function_declaration(self, native_name: str, native_data: Dict, native_comment: str, native_hash: str) -> str:
        """Generate function declaration for header file."""
        ret_type = native_data.get("return_type", "void")
        params_str, _ = self._build_params(native_data.get("params", []))
        comment = ""
        if native_comment:
            # Format multi-line comments properly
            # Split by newlines and clean up each line
            comment_lines = native_comment.split('\n')
            formatted_lines = []
            for line in comment_lines:
                # Strip whitespace from each line
                cleaned = line.strip()
                # Escape */ sequences to prevent premature comment closure
                cleaned = cleaned.replace('*/', '* /')
                if cleaned:
                    formatted_lines.append(cleaned)
                else:
                    # Preserve empty lines as separators
                    formatted_lines.append("")

            if formatted_lines:
                formatted_comment = '\n\t\t * '.join(formatted_lines)
                comment = f"\t\t/**\n\t\t * {formatted_comment}\n\t\t */\n"

        if self.crossmap and native_hash in self.hash_to_index:
            return f"{comment}\t\t{ret_type} {native_name}({params_str});\n"
        else:
            return f"{comment}\t\tvoid {native_name}({params_str}); // Missing in crossmap\n"


    def _generate_function_implementation(self, native_name: str, native_data: Dict,
                                         native_hash: str) -> str:
        """Generate function implementation for cpp file."""
        ret_type = native_data.get("return_type", "void")
        params_str, call_params = self._build_params(native_data.get("params", []))

        # Determine whether to use index or hash
        if self.crossmap and native_hash in self.hash_to_index:
            # Use index-based invocation
            index = self.hash_to_index[native_hash]
            if call_params:
                invoke_call = f"{self.invoker_function}<{ret_type}, {index}>({call_params})"
            else:
                invoke_call = f"{self.invoker_function}<{ret_type}, {index}>()"
        else:
            return f"\tvoid {native_name}({params_str}) {{\n\t\t\t{LOG_NATIVE_ERROR_FUNC}(\"[NATIVES] Missing in crossmap: {native_name}\");\n\t\t}}\n\n"

        # Handle void return type
        if ret_type == "void":
            return_stmt = f"{invoke_call};"
        else:
            return_stmt = f"return {invoke_call};"

        return f"\t{ret_type} {native_name}({params_str}) {{\n\t\t\t{return_stmt}\n\t\t}}\n\n"

    def generate_header(self) -> str:
        """Generate the header file content."""
        lines = [
            "#pragma once\n",
            "\n",
            "#pragma warning(push)\n",
            "#pragma warning(disable: 4100)\n",
            "\n",
            "// Auto-generated file - DO NOT EDIT\n",
            "// Generated from GTA5 NativeDB data\n",
            "\n",
        ]

        # Add custom includes if configured
        if self.header_includes:
            for include in self.header_includes:
                lines.append(f"{include}\n")
            lines.append("\n")

        lines.append(f"namespace {self.base_namespace} {{\n\n")


        for group_name, group in self.natives_data.items():
            # Skip excluded groups
            if self._is_group_excluded(group_name):
                print(f"Skipping excluded group: {group_name}")
                continue

            lines.append(f"\tnamespace {group_name} {{\n\n")

            for native_hash, native_data in group.items():
                native_name = native_data.get("name", "UNKNOWN")
                native_comment = native_data.get("comment", "")
                print(f"Processing: {group_name}::{native_name} ({native_hash})")

                decl = self._generate_function_declaration(native_name, native_data, native_comment, native_hash)
                lines.append(decl)

            lines.append(f"\n\t}} // namespace {group_name}\n\n")

        lines.append(f"}} // namespace {self.base_namespace}\n")
        lines.append("\n#pragma warning(pop)\n")

        return "".join(lines)

    def generate_implementation(self) -> str:
        """Generate the implementation file content."""
        lines = [
            "#pragma warning(push)\n",
            "#pragma warning(disable: 4100)\n",
            "\n",
            "// Auto-generated file - DO NOT EDIT\n",
            "// Generated from GTA5 NativeDB data\n",
            "\n",
            f'#include "{OUTPUT_HEADER}"\n',
            "\n",
            f"namespace {self.base_namespace} {{\n",
            "\n"
        ]

        for group_name, group in self.natives_data.items():
            # Skip excluded groups
            if self._is_group_excluded(group_name):
                continue

            lines.append(f"\tnamespace {group_name} {{\n\n")

            for native_hash, native_data in group.items():
                native_name = native_data.get("name", "UNKNOWN")

                impl = self._generate_function_implementation(
                    native_name, native_data, native_hash
                )
                lines.append(f"\t{impl}")

            lines.append(f"\t}} // namespace {group_name}\n\n")

        lines.append(f"}} // namespace {self.base_namespace}\n")
        lines.append("\n#pragma warning(pop)\n")

        return "".join(lines)

    def generate_crossmap_header(self) -> str:
        """Generate the crossmap header file content."""
        lines = [
            "#pragma once\n",
            "\n",
            "#pragma warning(push)\n",
            "#pragma warning(disable: 4100)\n",
            "\n",
            "// Auto-generated file - DO NOT EDIT\n",
            "// Native crossmap array for index-based lookup\n",
            "\n",
            "#include <cstdint>\n",
            "\n",
            f"namespace {self.base_namespace} {{\n",
            "\n"
        ]

        if self.crossmap:
            lines.append(f"\t// Crossmap array size\n")
            lines.append(f"\tconstexpr std::size_t NATIVE_CROSSMAP_SIZE = {len(self.crossmap)};\n\n")
            lines.append(f"\t// Native crossmap array declaration\n")
            lines.append(f"\textern const std::uint64_t NATIVE_CROSSMAP[NATIVE_CROSSMAP_SIZE];\n")
        else:
            lines.append("\t// No crossmap data available\n")
            lines.append(f"\tconstexpr std::size_t NATIVE_CROSSMAP_SIZE = 0;\n")

        lines.append(f"\n}} // namespace {self.base_namespace}\n")
        lines.append("\n#pragma warning(pop)\n")

        return "".join(lines)

    def generate_crossmap_implementation(self) -> str:
        """Generate the crossmap implementation file content."""
        lines = [
            "#pragma warning(push)\n",
            "#pragma warning(disable: 4100)\n",
            "\n",
            "// Auto-generated file - DO NOT EDIT\n",
            "// Native crossmap array for index-based lookup\n",
            "\n",
            f'#include "{OUTPUT_CROSSMAP_HEADER}"\n',
            "\n",
            f"namespace {self.base_namespace} {{\n",
            "\n"
        ]

        if self.crossmap:
            lines.append(f"\t// Native crossmap array definition\n")
            lines.append(f"\tconst std::uint64_t NATIVE_CROSSMAP[NATIVE_CROSSMAP_SIZE] = {{\n")
            for i, (hash1, hash2) in enumerate(self.crossmap):
                # Use hash2 (the second hash) as the actual native hash
                comma = "," if i < len(self.crossmap) - 1 else ""
                lines.append(f"\t\t{hash2}{comma}  // Index {i}: {hash1} -> {hash2}\n")
            lines.append("\t};\n")
        else:
            lines.append("\t// No crossmap data available\n")

        lines.append(f"\n}} // namespace {self.base_namespace}\n")
        lines.append("\n#pragma warning(pop)\n")

        return "".join(lines)

    def write_files(self, header_path: str, impl_path: str, crossmap_header_path: str = None, crossmap_impl_path: str = None) -> bool:
        """Write generated header and implementation files."""
        try:
            header_content = self.generate_header()
            impl_content = self.generate_implementation()

            Path(header_path).write_text(header_content, encoding='utf-8')
            print(f"✓ Generated {header_path}")

            Path(impl_path).write_text(impl_content, encoding='utf-8')
            print(f"✓ Generated {impl_path}")

            # Generate crossmap files if crossmap is loaded
            if self.crossmap and crossmap_header_path and crossmap_impl_path:
                crossmap_header_content = self.generate_crossmap_header()
                crossmap_impl_content = self.generate_crossmap_implementation()

                Path(crossmap_header_path).write_text(crossmap_header_content, encoding='utf-8')
                print(f"✓ Generated {crossmap_header_path}")

                Path(crossmap_impl_path).write_text(crossmap_impl_content, encoding='utf-8')
                print(f"✓ Generated {crossmap_impl_path}")

            return True
        except IOError as e:
            print(f"Error: Failed to write files: {e}")
            return False


def main():
    """Main entry point."""
    generator = NativeGenerator(BASE_NAMESPACE, INVOKER_FUNCTION, HEADER_INCLUDES, EXCLUDED_GROUPS)

    if not generator.fetch_natives(NATIVES_URL):
        sys.exit(1)

    # Load crossmap if available
    generator.load_crossmap(CROSSMAP_FILE)

    if not generator.write_files(OUTPUT_HEADER, OUTPUT_IMPL, OUTPUT_CROSSMAP_HEADER, OUTPUT_CROSSMAP_IMPL):
        sys.exit(1)

    print("\n✓ Native generation completed successfully!")


if __name__ == "__main__":
    main()
