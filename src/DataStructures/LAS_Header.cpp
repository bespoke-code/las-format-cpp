//
// Created by vdx2 on 13.3.18.
//

#include "LAS_Header.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "LAS_Structs.h"

namespace LAS {

    LAS_Header::LAS_Header() {
        std::strncpy(signature, "LASF", 4);
        source_id = 0;
        global_encoding = 0;
        version.major = 1;
        version.minor = 2;
        std::strcpy(system_identifier, "");
        std::strcpy(generating_software, "las-format-cpp-library\0");
        std::time_t timer = std::time(NULL);
        std::tm *timePtr = std::localtime(&timer);
        creation_day = (unsigned short) timePtr->tm_yday;
        creation_year = (unsigned short) (timePtr->tm_year + 1900);
        delete timePtr;
        size = 227;
        offset_to_point_data = 227;
        variable_length_records_count = 0;
        point_data_format = LAS::POINT_DATA_FORMAT::FORMAT_0;
        point_data_record_length = (unsigned short) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE;
        number_of_point_records = 0;
        for(int i=0; i<5; i++)
            number_of_points_by_return[i] = 0;
        x_scale_factor = 0.001;
        y_scale_factor = 0.001;
        z_scale_factor = 0.001;
        x_offset = 0.0;
        y_offset = 0.0;
        z_offset = 0.0;
        x_max = x_min = y_max = y_min = z_max = z_min = 0.0;
    }

    LAS_Header::LAS_Header(std::fstream* fileStream) {
        char buffer[227];
        fileStream->read(buffer, 227);

        // pointer to elements from byte-formatted file
        void* item = buffer;

        std::strncpy((char*)item, this->signature, 4);
        source_id = *((unsigned short*)(item + 4));
        global_encoding = *((unsigned short*)(item + 6));
        guid_data.data1 = *((unsigned int*)(item + 8));
        guid_data.data2 = *((unsigned short*)(item + 12));
        guid_data.data3 = *((unsigned short*)(item + 14));
        std::memcpy(guid_data.data4, item+16, 8);
        version.major = *((unsigned char*)(item + 24));
        version.minor = *((unsigned char*)(item + 25));
        std::strncpy(system_identifier, (char*)(item + 26), 32);
        std::strncpy(generating_software, (char*)(item + 58), 32);
        creation_day = *((unsigned short*)(item + 90));
        creation_year = *((unsigned short*)(item + 92));
        size = *((unsigned short*)(item + 94));
        offset_to_point_data = *((unsigned int*)(item + 96));
        variable_length_records_count = *((unsigned int*)(item + 100));
        point_data_format = (POINT_DATA_FORMAT) *((unsigned char*)(item + 104));
        point_data_record_length = *((unsigned char*)(item + 105));
        number_of_point_records = *((unsigned int*)(item + 107));
        for(int i=111, index=0; i<130 || index<5; i+=8, index++) {
            number_of_points_by_return[index] = *((unsigned int*)(item + i));
        }
        x_scale_factor = *((double*)(item + 131));
        y_scale_factor = *((double*)(item + 139));
        z_scale_factor = *((double*)(item + 147));
        x_offset = *((double*)(item + 155));
        y_offset = *((double*)(item + 163));
        z_offset = *((double*)(item + 171));
        x_max = *((double*)(item + 179));
        x_min = *((double*)(item + 187));
        y_max = *((double*)(item + 195));
        y_min = *((double*)(item + 203));
        z_max = *((double*)(item + 211));
        z_min = *((double*)(item + 219));
    }

    unsigned int LAS_Header::getOffsetToPointData() {
        return this->offset_to_point_data;
    }

    LAS::POINT_DATA_FORMAT LAS_Header::getPointDataFormat() {
        return this->point_data_format;
    }

    unsigned int LAS_Header::getNumberOfPointRecords() {
        return this->number_of_point_records;
    }

    unsigned int LAS_Header::getPointDataRecordLength() {
        return this->point_data_record_length;
    }

    unsigned int LAS_Header::getNumberOfPointsByReturn(int ret) {
        if (ret < 4)
            return this->number_of_points_by_return[ret];
        else
            return 0;
    }

    double LAS_Header::getOffset(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_offset;
        if (axis == LAS::AXIS::Y_AXIS) return this->y_offset;
        if (axis == LAS::AXIS::Z_AXIS) return this->z_offset;
    }

    double LAS_Header::getMinimum(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_min;
        if (axis == LAS::AXIS::Y_AXIS) return this->y_min;
        if (axis == LAS::AXIS::Z_AXIS) return this->z_min;
    }

    double LAS_Header::getMaximum(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_max;
        if (axis == LAS::AXIS::Y_AXIS) return this->y_max;
        if (axis == LAS::AXIS::Z_AXIS) return this->z_max;
    }

    void LAS_Header::saveTo(std::ofstream *outputFile) {
        if(!outputFile->is_open()){
            std::cout << "Stream is not open! NOT writing to disk."<<std::endl;
        }
        outputFile->write((const char*)this, sizeof(LAS::LAS_Header));
    }

    void LAS_Header::setPointCount(unsigned int amount) {
        this->number_of_point_records = amount;
    }

    void LAS_Header::incrementPointCount() {
        ++this->number_of_point_records;
    }

    void LAS_Header::decrementPointCount() {
        --this->number_of_point_records;
    }
}