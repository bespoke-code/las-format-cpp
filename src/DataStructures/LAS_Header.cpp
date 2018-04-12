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
        std::memset(system_identifier, 0, sizeof(system_identifier));
        std::strcpy(system_identifier, "");
        std::memset(generating_software, 0, sizeof(generating_software));
        std::strcpy(generating_software, "las-format-cpp-library\0");
        std::time_t timer = std::time(nullptr);
        std::tm *timePtr = std::localtime(&timer);
        creation_day = (unsigned short) timePtr->tm_yday;
        creation_year = (unsigned short) (timePtr->tm_year + 1900);
        size = 227;
        offset_to_point_data = 227;
        variable_length_records_count = 0;
        point_data_format = (unsigned char) LAS::POINT_DATA_FORMAT::FORMAT_0;
        point_data_record_length = (unsigned short) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE;
        number_of_point_records = 0;
        for (unsigned int &i : number_of_points_by_return)
            i = 0;
        x_scale_factor = 0.001;
        y_scale_factor = 0.001;
        z_scale_factor = 0.001;
        x_offset = 0.0;
        y_offset = 0.0;
        z_offset = 0.0;
        x_max = x_min = y_max = y_min = z_max = z_min = 0.0;
    }

    LAS_Header::LAS_Header(std::fstream &fileStream) {
        char buffer[227];
        std::memset(buffer, 0, sizeof(buffer));
        fileStream.read(buffer, 227);

        // pointer to elements from byte-formatted file
        void* item = buffer;

        std::strncpy((char*)item, this->signature, 4);
        if(std::strcmp("LASF", signature) != 0)
            std::strncpy(signature, "LASF", 4);
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
        point_data_format = *((unsigned char*)(item + 104));
        std::cout<<"Point data format size: "<<(sizeof(POINT_DATA_FORMAT))<<std::endl;
        point_data_record_length = *((unsigned short*)(item + 105));
        number_of_point_records = *((unsigned int*)(item + 107));
        for(int i=111, index=0; i<130 && index<5; i+=sizeof(int), index++) {
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

    unsigned char LAS_Header::getPointDataFormat() {
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

    double LAS_Header::offset(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_offset;
        else if (axis == LAS::AXIS::Y_AXIS) return this->y_offset;
        else if (axis == LAS::AXIS::Z_AXIS) return this->z_offset;
        else return -1.0;
    }

    double LAS_Header::getMinimum(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_min;
        else if (axis == LAS::AXIS::Y_AXIS) return this->y_min;
        else if (axis == LAS::AXIS::Z_AXIS) return this->z_min;
        else return -1.0;
    }

    double LAS_Header::getMaximum(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_max;
        else if (axis == LAS::AXIS::Y_AXIS) return this->y_max;
        else if (axis == LAS::AXIS::Z_AXIS) return this->z_max;
        else return -1.0;
    }

    void LAS_Header::serialize(std::ofstream& outputFile) {
        if(!outputFile.is_open()){
            std::cout << "Stream is not open! NOT writing to disk."<<std::endl;
        }
        //TODO: change this to something more sane?
        //outputFile->write((const char*)this, sizeof(LAS::LAS_Header));
        outputFile.write(signature, sizeof(signature));
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the signature." << std::endl;
        outputFile << source_id;
        std::cout << "Size of source id: " << sizeof(source_id) << " and size of u_short: " << sizeof(unsigned short) << std::endl;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the source ID." << std::endl;
        //outputFile->write((const char*) source_id, sizeof(source_id));
        outputFile << global_encoding;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the global encoding." << std::endl;
        outputFile.write((const char*) &version, sizeof(Version));
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the Version." << std::endl;
        outputFile.write(system_identifier, sizeof(system_identifier));
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the system_identifier." << std::endl;
        outputFile.write(generating_software, sizeof(generating_software));
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the generating_software." << std::endl;
        outputFile << creation_day << creation_year << size << offset_to_point_data << variable_length_records_count;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the creation date, size, offset to pcl data and vlr count." << std::endl;
        outputFile << point_data_format;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the point_data_format." << std::endl;
        outputFile << point_data_record_length << number_of_point_records;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the number_of_point_records." << std::endl;
       for (unsigned int i : number_of_points_by_return)
            outputFile << i;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the number_of_points_by_return." << std::endl;
        outputFile << x_scale_factor  << y_scale_factor << z_scale_factor;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the axis_scale_factor." << std::endl;
        outputFile << x_offset  << y_offset << z_offset;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the axis offsets." << std::endl;
        outputFile << x_max  << x_min;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the x_max_min." << std::endl;
        outputFile << y_max  << y_max;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the y_max_min." << std::endl;
        outputFile << z_max  << z_min;
        std::cout << "Put pointer at: " << outputFile.tellp() << " after saving the z_max_min." << std::endl;
    }

    void LAS_Header::setPointCount(unsigned int amount) {
        this->number_of_point_records = amount;
        this->number_of_points_by_return[0] = amount;
    }

    void LAS_Header::incrementPointCount() {
        ++(this->number_of_point_records);
        ++(this->number_of_points_by_return[0]);
    }

    void LAS_Header::decrementPointCount() {
        --(this->number_of_point_records);
        --(this->number_of_points_by_return[0]);
    }

    void LAS_Header::setMinimum(AXIS axis, double min) {
        switch (axis) {
            case LAS::AXIS::X_AXIS:
                this->x_min = min; break;
            case LAS::AXIS::Y_AXIS:
                this->y_min = min; break;
            case LAS::AXIS::Z_AXIS:
                this->z_min = min; break;
        }
    }

    void LAS_Header::setMaximum(AXIS axis, double max) {
        switch (axis) {
            case LAS::AXIS::X_AXIS:
                this->x_max = max; break;
            case LAS::AXIS::Y_AXIS:
                this->y_max = max; break;
            case LAS::AXIS::Z_AXIS:
                this->z_max = max; break;
        }
    }

    void LAS_Header::setPointFormat(POINT_DATA_FORMAT format) {
        this->point_data_format = (unsigned char) format;
    }

    void LAS_Header::increasePointDataOffset(unsigned int amount) {
        this->offset_to_point_data += amount;
    }

    double LAS_Header::scaleFactor(AXIS axis) {
        if (axis == LAS::AXIS::X_AXIS) return this->x_scale_factor;
        else if (axis == LAS::AXIS::Y_AXIS) return this->y_scale_factor;
        else if (axis == LAS::AXIS::Z_AXIS) return this->z_scale_factor;
        else return 1.0;
    }

    unsigned short LAS_Header::getSize() {
        return size;
    }
}