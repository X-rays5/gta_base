#!/usr/bin/env python3
r"""
IDA Python Script for importing native handlers

Usage in IDA:
1. File > Script File... or Alt+F7
2. Select this script
3. It will prompt for the native_handlers.json file

Or run directly in IDA Python console:
    exec(open(r'C:\path\to\this\script.py').read())
"""

import json
import os

import ida_name
import idaapi
import idc


def import_native_handlers(json_file=None):
    """
    Import native handlers from JSON dump into IDA

    Args:
        json_file: Path to native_handlers.json. If None, prompts user.
    """

    if json_file is None:
        # Ask user for file path
        json_file = idaapi.ask_file(
            False,
            "*.json",
            "Select native_handlers.json"
        )

        if not json_file:
            print("No file selected")
            return

    if not os.path.exists(json_file):
        print(f"File not found: {json_file}")
        return

    try:
        with open(json_file, 'r', encoding='utf-8') as f:
            handlers = json.load(f)
    except Exception as e:
        print(f"Error reading JSON file: {e}")
        return

    if not isinstance(handlers, list):
        print("Error: JSON must be an array of handlers")
        return

    # Get IDA's image base
    image_base = idaapi.get_imagebase()

    print(f"[+] Importing {len(handlers)} native handlers...")
    print(f"[+] Image base: 0x{image_base:X}")

    success_count = 0
    skip_count = 0
    error_count = 0

    for i, handler in enumerate(handlers):
        try:
            # RVA can be either an integer or a hex string
            rva = handler['rva']
            if isinstance(rva, str):
                rva = int(rva, 16)
            else:
                rva = int(rva)

            name = handler['name'].replace('::', '_').replace('<', '_').replace('>', '_')
            return_type = handler.get('return_type', 'void')
            params = handler.get('params', '')

            # Skip very short or invalid names
            if len(name) < 3:
                skip_count += 1
                continue

            # Add image base to RVA to get absolute address in IDA
            addr = image_base + rva

            # Check if address exists in IDA
            if not idaapi.is_mapped(addr):
                skip_count += 1
                continue

            # Check if this is a tail byte (part of another function)
            if idaapi.is_tail(idaapi.get_flags(addr)):
                skip_count += 1
                continue

            # Check if first instruction is a JMP
            insn = idaapi.insn_t()
            decoded_len = idaapi.decode_insn(insn, addr)
            if decoded_len <= 0:
                print(f"  [!] Failed to decode instruction at {name} (0x{addr:X})")
                skip_count += 1
                continue

            is_jmp = insn.get_canon_mnem() == "jmp"

            if is_jmp:
                # This is a JMP wrapper
                # Name current function with _JMP suffix
                jmp_name = name + "_JMP"
                flags = idaapi.SN_CHECK | idaapi.SN_AUTO
                if idaapi.set_name(addr, jmp_name, flags):
                    # Set prototype on JMP wrapper
                    if params or return_type != 'void':
                        try:
                            proto = f"void {jmp_name}(scrNativeCallContext* context)"
                            idc.SetType(addr, proto)
                        except Exception as e:
                            print(f"    [!] Failed to set type on {jmp_name}: {e}")

                    # Follow the JMP to get target
                    target_addr = insn.Op1.addr
                    if idaapi.is_mapped(target_addr) and target_addr != idaapi.BADADDR:
                        # Ensure target is defined as code
                        idaapi.create_insn(target_addr)

                        # Name target function without suffix
                        if idaapi.set_name(target_addr, name, flags):
                            # Set prototype on target function
                            if params or return_type != 'void':
                                try:
                                    proto = f"void {name}(scrNativeCallContext* context)"
                                    idc.SetType(target_addr, proto)
                                except Exception as e:
                                    print(f"    [!] Failed to set type on {name}: {e}")
                            success_count += 1
                        else:
                            print(f"  [!] Failed to name target {name} at 0x{target_addr:X}")
                            skip_count += 1
                    else:
                        print(f"  [!] Invalid target address for JMP at {jmp_name} (0x{addr:X})")
                        skip_count += 1
                else:
                    print(f"  [!] Failed to set name {jmp_name} at 0x{addr:X}")
                    skip_count += 1
            else:
                # Regular function (not a JMP)
                flags = idaapi.SN_CHECK | idaapi.SN_AUTO
                if idaapi.set_name(addr, name, flags):
                    # Try to set function prototype
                    if params or return_type != 'void':
                        try:
                            proto = f"void {name}(scrNativeCallContext* context)"
                            idc.SetType(addr, proto)
                        except Exception as e:
                            print(f"    [!] Failed to set type on {name}: {e}")

                    success_count += 1
                    if success_count % 500 == 0:
                        print(f"  [*] Named {success_count} functions...")
                else:
                    # Skip if we can't name it (tail byte, etc.)
                    print(f"  [!] Failed to set name {name} at 0x{addr:X}")
                    skip_count += 1

        except Exception as e:
            print(f"  [!] Error processing handler {i} ({name}): {type(e).__name__}: {e}")
            error_count += 1


    print(f"\n[+] Import Complete!")
    print(f"    Successfully named: {success_count}")
    print(f"    Skipped: {skip_count}")
    print(f"    Errors: {error_count}")


if __name__ == '__main__':
    # When run as a script
    import_native_handlers()
else:
    # When executed in IDA console
    import_native_handlers()
