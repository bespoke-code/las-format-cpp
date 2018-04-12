//
// Created by vdx2 on 13.3.18.
//

#include "LAS_File.h"
#include "LAS_Header.h"
#include <vector>
#include <iostream>
#include <fstream>


namespace LAS {

    LAS_File::LAS_File() {
        this->header = new LAS_Header();
    }

    LAS_File::LAS_File(std::fstream& fileStream) {
        this->header = new LAS_Header(fileStream);
        std::cout << "Get pointer at: " << fileStream.tellg() << " after reading the header." << std::endl;
        std::cout << "Put pointer at: " << fileStream.tellp() << " after reading the header." << std::endl;

        if(header->getOffsetToPointData() > header->getSize()) {
            while (fileStream.tellp() <= header->getOffsetToPointData() - 1) {
                LAS::VariableLengthRecord vlr(fileStream);
                // add new VLR to list
                records.push_back(vlr);
            }
        }

        PointDataRecord point;
        std::cout << "Get pointer at: " << fileStream.tellg() << " after reading ALL variable length records." << std::endl;
        std::cout << "Put pointer at: " << fileStream.tellp() << " after reading ALL variable length records." << std::endl;

        for(unsigned int i=0; i<4; i++) {
            for(unsigned int j=0; j<header->getNumberOfPointsByReturn(i); j++) {
                fileStream.read((char *) &point, (std::streamsize) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE);

                switch (header->getPointDataFormat()) {
                    case (unsigned char) LAS::POINT_DATA_FORMAT::FORMAT_0:
                        break;
                    case (unsigned char) LAS::POINT_DATA_FORMAT::FORMAT_1:
                        // setting GPS time only
                        point.setGPSTime(fileStream);
                        break;
                    case (unsigned char) LAS::POINT_DATA_FORMAT::FORMAT_2:
                        // setting RGB only
                        point.setRGB(fileStream);
                        break;
                    case (unsigned char) LAS::POINT_DATA_FORMAT::FORMAT_3:
                        // order is important!
                        point.setGPSTime(fileStream);
                        point.setRGB(fileStream);
                        break;
                    default:
                        break;
                }
                points.push_back(point);
                std::cout << "Get pointer at: " << fileStream.tellg() << " after reading a point." << std::endl;
                std::cout << "Put pointer at: " << fileStream.tellp() << " after reading a point." << std::endl;

            }
        }
    }

    LAS_File::~LAS_File() {
        delete header;
    }

    void LAS_File::addPoint(PointDataRecord *point) {
        points.push_back(*point);

        double x_scale = header->scaleFactor(LAS::AXIS::X_AXIS);
        double y_scale = header->scaleFactor(LAS::AXIS::Y_AXIS);
        double z_scale = header->scaleFactor(LAS::AXIS::Z_AXIS);

        double x_offset = header->offset(LAS::AXIS::X_AXIS);
        double y_offset = header->offset(LAS::AXIS::Y_AXIS);
        double z_offset = header->offset(LAS::AXIS::Z_AXIS);

        double x_point = (double)point->getX() * x_scale + x_offset;
        double y_point = (double)point->getY() * y_scale + y_offset;
        double z_point = (double)point->getZ() * z_scale + z_offset;

        if(x_point > header->getMaximum(LAS::AXIS::X_AXIS))
            header->setMaximum(LAS::AXIS::X_AXIS, point->getX());
        if(y_point > header->getMaximum(LAS::AXIS::Y_AXIS))
            header->setMaximum(LAS::AXIS::Y_AXIS, point->getY());
        if(z_point > header->getMaximum(LAS::AXIS::Z_AXIS))
            header->setMaximum(LAS::AXIS::Z_AXIS, point->getZ());

        if(x_point < header->getMinimum(LAS::AXIS::X_AXIS))
            header->setMinimum(LAS::AXIS::X_AXIS, point->getX());
        if(y_point < header->getMinimum(LAS::AXIS::Y_AXIS))
            header->setMinimum(LAS::AXIS::Y_AXIS, point->getY());
        if(z_point < header->getMinimum(LAS::AXIS::Z_AXIS))
            header->setMinimum(LAS::AXIS::Z_AXIS, point->getZ());

        this->header->incrementPointCount();
    }

    void LAS_File::serialize(std::ofstream& outputStream) {

        std::cout << "Put pointer at: " << outputStream.tellp() << " before saving the header." << std::endl;
        this->header->serialize(outputStream);
        std::cout << "Put pointer at: " << outputStream.tellp() << " after saving the header." << std::endl;
        for (auto &record : records) {
            record.serialize(outputStream);
        }
        std::cout << "Put pointer at: " << outputStream.tellp() << " after saving the VLRs." << std::endl;
        for (auto &point : points) { //std::vector<LAS::PointDataRecord>::iterator
            point.serialize(outputStream, (LAS::POINT_DATA_FORMAT) header->getPointDataFormat());
            std::cout << "Put pointer at: " << outputStream.tellp() << " after saving a point." << std::endl;
        }
    }

    void LAS_File::setPointFormat(POINT_DATA_FORMAT format) {
        this->header->setPointFormat(format);
    }

    void LAS_File::addVariableRecord(VariableLengthRecord *vlrecord) {
        this->header->increasePointDataOffset((unsigned int) vlrecord->size());
        this->records.push_back(*vlrecord);
    }
}