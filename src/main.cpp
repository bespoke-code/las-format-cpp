#include <iostream>
#include <fstream>
#include "DataStructures/LAS_File.h"
#include "DataStructures/PointDataRecord.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "int: " << sizeof(int) << std::endl;
    std::cout << "long: " << sizeof(long) << std::endl;
    std::cout << "float: " << sizeof(float) << std::endl;
    std::cout << "double: " << sizeof(double) << std::endl;
    std::cout << "unsigned long: " << sizeof(unsigned long) << std::endl;
    std::cout << "short: " << sizeof(short) << std::endl;
    std::cout << "unsigned char: " << sizeof(unsigned char) << std::endl;

    std::fstream fileStream("/home/vdx2/Desktop/chasha-lavche-3D.las", std::ios::binary | std::ios::in);
    LAS::LAS_File las_file(&fileStream);
    las_file.debug_test();

    return 0;
}