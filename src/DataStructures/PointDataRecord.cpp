//
// Created by vdx2 on 14.3.18.
//

#include "PointDataRecord.h"
namespace LAS {
    PointDataRecord::PointDataRecord() {

    }

    int PointDataRecord::getX() {
        return this->x;
    }

    int PointDataRecord::getY() {
        return this->y;
    }

    int PointDataRecord::getZ() {
        return this->z;
    }

    void PointDataRecord::setGPSTime(std::fstream *fileStream) {
        fileStream->read((char*)&gps_time, sizeof(double));
    }

    void PointDataRecord::setRGB(std::fstream *fileStream) {
        fileStream->read((char*)&colour, sizeof(LAS::Colour));
    }

}