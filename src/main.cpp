#include <iostream>
#include <fstream>
#include "DataStructures/LAS_File.h"
#include "DataStructures/PointDataRecord.h"

int main(int argc, char **argv) {


    std::fstream fileStream("/home/vdx2/Desktop/chasha-lavche-3D.las", std::ios::binary | std::ios::in);
    LAS::LAS_File las_file(&fileStream);
    fileStream.close();

    #ifdef DEBUG
    las_file.debug_test();
    #endif

    LAS::LAS_File randomPoints;

    std::ofstream output("/home/vdx2/Desktop/testFile_LAS.las", std::ios::binary);
    las_file.saveTo(&output);
    output.close();
    return 0;
}