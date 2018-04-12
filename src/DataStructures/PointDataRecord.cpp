//
// Created by vdx2 on 14.3.18.
//

#include "PointDataRecord.h"
#include <iostream>


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

    void PointDataRecord::setGPSTime(std::fstream &fileStream) {
        fileStream.read((char*)&gps_time, sizeof(double));
    }

    void PointDataRecord::setRGB(std::fstream &fileStream) {
        fileStream.read((char*)&colour, sizeof(LAS::Colour));
    }

    void PointDataRecord::serialize(std::ofstream& outputStream, LAS::POINT_DATA_FORMAT format) {
        outputStream.write((const char*) &x, sizeof(x));
        outputStream.write((const char*) &y, sizeof(y));
        outputStream.write((const char*) &z, sizeof(z));
        outputStream.write((const char*) &intensity, sizeof(intensity));
        lidar_scan_properties.serialize(outputStream);
        //outputStream.write((const char*) &lidar_scan_properties, sizeof(lidar_scan_properties));
        outputStream.write((const char*) &classification, sizeof(classification));
        outputStream.write((const char*) &scan_angle_rank, sizeof(scan_angle_rank));
        outputStream.write((const char*) &user_data, sizeof(user_data));
        outputStream.write((const char*) &point_source_id, sizeof(point_source_id));

        std::cout << outputStream.tellp() << std::endl;
        switch (format) {
            case LAS::POINT_DATA_FORMAT::FORMAT_0:
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_1:
                outputStream.write((const char*) &gps_time, sizeof(gps_time));
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_2:
                colour.serialize(outputStream);
                break;
            case LAS::POINT_DATA_FORMAT::FORMAT_3:
                outputStream.write((const char*) &gps_time, sizeof(double));
                colour.serialize(outputStream);
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