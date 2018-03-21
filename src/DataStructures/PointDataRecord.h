//
// Created by vdx2 on 14.3.18.
//

#ifndef LAS_READER_POINTDATARECORD_H
#define LAS_READER_POINTDATARECORD_H

#include <fstream>
#include "LAS_Structs.h"

namespace LAS {



    struct scan_data {
        unsigned short return_no:3;
        unsigned short number_of_returns:3;
        unsigned short scan_direction:1;
        unsigned short edge_of_flight_line:1;
    };


    struct Colour {
        unsigned short red, green, blue;
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

        void setGPSTime(std::fstream* fileStream);
        void setRGB(std::fstream* fileStream);
        void saveTo(std::ofstream* outputStream, LAS::POINT_DATA_FORMAT format);
    };

}
#endif //LAS_READER_POINTDATARECORD_H
