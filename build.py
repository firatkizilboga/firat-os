import glob
import json
import os

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

def main():
    #delete all .o and .bin files in the current directory and all subdirectories
    os.system("find . -name '*.o' -type f -delete")
    os.system("find . -name '*.bin' -type f -delete")
    os.system("i686-elf-as boot.s -o build/boot.o")
    build_order = []

    with open('build.json') as build_file:
        build_info = json.load(build_file)

    print(build_info)
    libraries = discover_libs()

    for lib in libraries:
        build_order.append(lib)
        build_order.extend(get_dependencies(lib))

    #deduce order of build
    priority_build_order = []
    for lib in build_order:
        if lib not in priority_build_order:
            priority_build_order.append(lib)
    
    #prinr pwd
    print(os.getcwd())
    object_files = []

    for lib in priority_build_order:
        objs = build_lib(lib)
        object_files.extend(objs)

    
    #get includes for all the libraries
    includes = ""
    for lib in priority_build_order:
        includes += get_include_string(lib)
    
        
    os.system("i686-elf-gcc -c kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra" + includes)
    object_files_str = " ".join(object_files)
    os.system("i686-elf-gcc -T linker.ld -o build/kernel.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o " + object_files_str)

def get_include_string(lib):
    dependencies = get_dependencies(lib)
    includes = ""
    for dep in dependencies:
        includes += " -I" + dep + "/include"
    includes += " -I" + lib + "/include"
    return includes

def get_file_name(path):
    return path.split("/")[-1]

def build_lib(lib):
    print("Building " + lib)
    c_files = glob.glob(lib + "/*/*.c")
    asm_files = glob.glob(lib + "/*/*.s")

    includes = get_include_string(lib)

    obj_names = []


    for c_file in c_files:
        print("Building " + c_file)
        os.system("i686-elf-gcc -c " + c_file + " -o " +  BUILD_DIR +get_file_name(c_file).replace(".c", ".o") + " -std=gnu99 -ffreestanding -O2 -Wall -Wextra" + includes)
        obj_names.append(BUILD_DIR + get_file_name(c_file).replace(".c", ".o"))

    for asm_file in asm_files:
        print("Building " + asm_file)
        os.system("nasm -f elf32 " + asm_file + " -o " + BUILD_DIR +get_file_name(asm_file).replace(".s", ".o"))
        obj_names.append(BUILD_DIR + get_file_name(asm_file).replace(".s", ".o"))
    
    return obj_names
if __name__ == "__main__":
    main()


