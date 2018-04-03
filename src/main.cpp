#include <iostream>
#include <fstream>
#include "DataStructures/LAS_File.h"

int main(int argc, char **argv) {

    std::fstream fileStream("/home/vdx2/Desktop/LAS/libLAS_1.2.las", std::ios::binary | std::ios::in);
    LAS::LAS_File las_file(&fileStream);
    fileStream.close();

    #ifdef DEBUG
    las_file.debug_test();
    #endif

    LAS::LAS_File randomPoints;
    LAS::PointDataRecord point;

    int x, y, z;
    // double gpsTime;
    LAS::Colour color;

    randomPoints.setPointFormat(LAS::POINT_DATA_FORMAT::FORMAT_2);

    for(int i=0; i<1000; ++i) {
        x = std::rand()%1000000;
        y = std::rand()%1000000;
        z = std::rand()%1000000;

        color.red = (unsigned short)(std::rand()%255);
        color.green = (unsigned short)(std::rand()%255);
        color.blue = (unsigned short)(std::rand()%255);

        point.setCoordinates(x, y, z);
        point.setRGB(color);
        randomPoints.addPoint(&point);
    }

    std::ofstream output("/home/vdx2/Desktop/testFile1_LAS.las", std::ios::binary);
    las_file.saveTo(&output);
    std::ofstream output1("/home/vdx2/Desktop/testFile2_LAS.las", std::ios::binary);
    randomPoints.saveTo(&output1);
    output.close();
    output1.close();
    return 0;
}