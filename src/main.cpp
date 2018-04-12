#include <iostream>
#include <fstream>
#include <random>
#include "DataStructures/LAS_File.h"

int main(int argc, char **argv) {

    std::fstream fileStream("/home/vdx2/Desktop/LAS/libLAS_1.2.las", std::ios::binary | std::ios::in);
    LAS::LAS_File las_file(fileStream);
    fileStream.close();

    //std::fstream fileStream("/home/vdx2/Desktop/testFile2_LAS.las", std::ios::binary | std::ios::in);
    //std::cout << "Get pointer at: " << fileStream.tellg() << std::endl;
    //LAS::LAS_File las_file(&fileStream);
    //fileStream.close();

    LAS::LAS_File randomPoints;
    LAS::PointDataRecord point;

    int x, y, z;
    LAS::Colour color{0, 0, 0};

    randomPoints.setPointFormat(LAS::POINT_DATA_FORMAT::FORMAT_2);

    std::random_device random_device; // create object for seeding
    std::mt19937 engine{random_device()}; // create engine and seed it
    std::uniform_int_distribution<> dist(INT32_MIN,INT32_MAX); // create distribution for integers with [min; max] range
    for(int i=0; i<1000; ++i) {
        x = dist(engine)%1000000;
        y = dist(engine)%1000000;
        z = dist(engine)%1000000;

        color.red = (unsigned short)(dist(engine)%255);
        color.green = (unsigned short)(dist(engine)%255);
        color.blue = (unsigned short)(dist(engine)%255);

        point.setCoordinates(x, y, z);
        point.setRGB(color);
        randomPoints.addPoint(&point);
    }

    std::ofstream output("/home/vdx2/Desktop/testFile1_LAS1.las", std::ios::binary);
    las_file.serialize(output);
    std::ofstream output1("/home/vdx2/Desktop/testFile2_LAS1.las", std::ios::binary);
    randomPoints.serialize(output1);
    output.close();
    output1.close();

    return 0;
}