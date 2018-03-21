//
// Created by vdx2 on 13.3.18.
//

#ifndef LAS_READER_LAS_HEADER_H
#define LAS_READER_LAS_HEADER_H

#include <cstring>
#include <fstream>
#include "LAS_Structs.h"

namespace LAS {

    struct ProjectID_GUID {
        unsigned int data1;
        unsigned short data2;
        unsigned short data3;
        unsigned char data4[8];
    };

    struct Version {
        unsigned char major;
        unsigned char minor;
    };

    enum struct AXIS : std::int8_t {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS
    };


    class LAS_Header {
    private:
        char signature[4];
        unsigned short source_id;
        unsigned short global_encoding;
        ProjectID_GUID guid_data;
        Version version;
        char system_identifier[32];
        char generating_software[32];
        unsigned short creation_day;
        unsigned short creation_year;
        unsigned short size;

        unsigned int offset_to_point_data;
        unsigned int variable_length_records_count;

        POINT_DATA_FORMAT point_data_format;
        unsigned short point_data_record_length;

        unsigned int number_of_point_records;
        unsigned int number_of_points_by_return[5];

        double x_scale_factor;
        double y_scale_factor;
        double z_scale_factor;

        double x_offset;
        double y_offset;
        double z_offset;

        double x_max;
        double x_min;
        double y_max;
        double y_min;
        double z_max;
        double z_min;

    public:
        LAS_Header();
        LAS_Header(std::fstream *fileStream);

        double getOffset(AXIS axis);
        double getMinimum(AXIS axis);
        double getMaximum(AXIS axis);
        unsigned int getOffsetToPointData();
        LAS::POINT_DATA_FORMAT getPointDataFormat();
        unsigned int getNumberOfPointRecords();
        unsigned int getPointDataRecordLength();
        unsigned int getNumberOfPointsByReturn(int ret);

        void saveTo(std::ofstream *outputFile);

    };
}

#endif //LAS_READER_LAS_HEADER_H
