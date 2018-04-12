//
// Created by vdx2 on 14.3.18.
//

#ifndef LAS_READER_POINTDATARECORD_H
#define LAS_READER_POINTDATARECORD_H

#include <fstream>
#include <iostream>
#include "LAS_Structs.h"

namespace LAS {



    struct scan_data {
        unsigned short return_no:3;
        unsigned short number_of_returns:3;
        unsigned short scan_direction:1;
        unsigned short edge_of_flight_line:1;
        void serialize(std::ofstream& outputStream) {
            std::cout << (return_no <<5 | number_of_returns << 2 | scan_direction << 1 | edge_of_flight_line) << " ";
            unsigned char data = 0;
            data = data | (return_no & 0x00FF) << 5;
            data = data | (number_of_returns & 0x00FF) << 2;
            data = data | (scan_direction & 0x00FF) << 1;
            data = data | (edge_of_flight_line & 0x00FF) << 1;
            std::cout << "concatenated value: " << (unsigned int) data << std::endl;
            outputStream.write((const char*) &data, 1);
        }
    };


    struct Colour {
        unsigned short red, green, blue;
        void serialize(std::ofstream& outputStream) {
            outputStream.write((const char*) &red, sizeof(red));
            outputStream.write((const char*) &green, sizeof(green));
            outputStream.write((const char*) &blue, sizeof(blue));
        }
    };


    class PointDataRecord {
    private:
        int x;
        int y;
        int z;

        unsigned short intensity;
        LAS::scan_data lidar_scan_properties;
        //unsigned char lidar_scan_properties;
        unsigned char classification;
        char scan_angle_rank;

        unsigned char user_data;
        unsigned short point_source_id;

        // for Point data record format 1, 3
        double gps_time;

        // for Point data record format 2, 3
        LAS::Colour colour;

    public:
        PointDataRecord();
        int getX();
        int getY();
        int getZ();

        void setCoordinates(int x, int y, int z);

        void setGPSTime(std::fstream &fileStream);
        void setGPSTime(double time);

        void setRGB(std::fstream &fileStream);
        void setRGB(LAS::Colour& colour);

        void serialize(std::ofstream &outputStream, LAS::POINT_DATA_FORMAT format);
    };

}
#endif //LAS_READER_POINTDATARECORD_H
