import glob
import json
import os

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
    os.system("i686-elf-as boot.s -o boot.o")
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

    for lib in priority_build_order:
        build_lib(lib)

    
    #get includes for all the libraries
    includes = ""
    for lib in priority_build_order:
        includes += get_include_string(lib)
    lib_object_files = []
    for lib in priority_build_order:
        c_files = glob.glob(lib + "/*/*.c")
        for c_file in c_files:
            obj_file = c_file.replace(".c", ".o")
            lib_object_files.append(obj_file)
        
    os.system("i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra" + includes)

    # Link the kernel with the library executables
    object_files_str = " ".join(lib_object_files)
    print("i686-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib kernel.o " + includes)

    os.system("i686-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o " + object_files_str)

def get_include_string(lib):
    dependencies = get_dependencies(lib)
    includes = ""
    for dep in dependencies:
        includes += " -I" + dep + "/include"
    includes += " -I" + lib + "/include"
    return includes

def build_lib(lib):
    dependencies = get_dependencies(lib)
    print("Building " + lib)

    #discover all the .c files in the library
    c_files = glob.glob(lib + "/*/*.c")

    includes =get_include_string(lib)

    for c_file in c_files:
        print("Building " + c_file)
        os.system("i686-elf-gcc -c " + c_file + " -o " +  c_file.replace(".c", ".o") + " -std=gnu99 -ffreestanding -O2 -Wall -Wextra" + includes)
if __name__ == "__main__":
    main()


