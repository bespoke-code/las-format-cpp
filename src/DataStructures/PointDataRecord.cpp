//
// Created by vdx2 on 14.3.18.
//

#include "PointDataRecord.h"
namespace LAS {
    PointDataRecord::PointDataRecord() {
        this->gps_time = 0;
        this->classification = 0;
        this->intensity = 0;
        this->colour.blue = 0;
        this->colour.red = 0;
        this->colour.green = 0;
        x = y = z = 0;
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

    void PointDataRecord::setGPSTime(double time) {
        this->gps_time = time;
    }

    void PointDataRecord::setRGB(LAS::Colour& rgb) {
        this->colour.red = rgb.red;
        this->colour.green = rgb.green;
        this->colour.blue = rgb.blue;
    }

    void PointDataRecord::setCoordinates(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

}