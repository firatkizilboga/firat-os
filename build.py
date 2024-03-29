import glob
import json
import os
import uuid

BUILD_DIR = "build/"

def discover_libs():
    libraries = []
    # Discover all files in the current directory that contain an info.json file
    for file in glob.glob("*/info.json"):
        libraries.append(file.replace("/info.json", ""))
    return libraries

def get_dependencies(lib):
    with open(lib + "/info.json") as info_file:
        info = json.load(info_file)
    return info["dependencies"]

includes = ""

def main():
    #delete all .o and .bin files in the current directory and all subdirectories
    os.system("find . -name '*.o' -type f -delete")
    os.system("find . -name '*.bin' -type f -delete")
    os.system("i686-elf-as boot.s -o build/boot.o")
    #find all the locations of .c and .s files in the current directory and all subdirectories
    build_order = []

    libraries = discover_libs()
    object_files = []

    for lib in libraries:
        get_include_string(lib)

    for lib in libraries:
        objs = build_lib(lib)
        object_files.extend(objs)

    os.system("i686-elf-gcc -c kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra " + includes)
    object_files_str = " ".join(object_files)
    os.system("i686-elf-gcc -T linker.ld -o build/kernel.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o " + object_files_str)

def get_include_string(lib):
    global includes
    includes += "  -I" + lib + "/include "

def get_file_name(path):
    return path.split("/")[-1]

def find_files(directory, extensions):
    """Find files with given extensions in all subdirectories."""
    files_found = {ext: [] for ext in extensions}
    for ext in extensions:
        files_found[ext].extend(glob.glob(f"{directory}/**/*.{ext}", recursive=True))
    return files_found

def build_lib(lib):
    files = find_files(lib, ["c", "s"])


    c_files = files["c"]
    asm_files = files["s"]
    obj_names = []


    for c_file in c_files:
        print("Building " + c_file)
        identifier = uuid.uuid4().hex
        name = identifier + get_file_name(c_file).replace(".c", ".o")
        build_location = BUILD_DIR + name

        os.system("i686-elf-gcc -c " + c_file + " -o " +  build_location + " -std=gnu99 -ffreestanding -O2 -w -Wall -Wextra " + includes)
        obj_names.append(build_location)

    for asm_file in asm_files:
        print("Building " + asm_file)
        identifier = uuid.uuid4().hex

        name = identifier+get_file_name(asm_file).replace(".s", ".o")
        build_location = BUILD_DIR + name
        print("buil loc: ", build_location, name)

        os.system("nasm -f elf32 " + asm_file + " -o " + build_location)
        obj_names.append(build_location)

    return obj_names
if __name__ == "__main__":
    main()
