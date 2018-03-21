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

    void PointDataRecord::saveTo(std::ofstream *outputStream, LAS::POINT_DATA_FORMAT format) {
        outputStream->write((const char*)this, (std::streamsize) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE);

        switch (format) {
            case LAS::POINT_DATA_FORMAT::FORMAT_0:
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_1:
                outputStream->write((char*) &gps_time, sizeof(double));
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_2:
                outputStream->write((char*) &colour, sizeof(LAS::Colour));
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_3:
                outputStream->write((char*) &gps_time, sizeof(double));
                outputStream->write((char*) &colour, sizeof(LAS::Colour));
                break;
            default:
                break;
        }
    }

}