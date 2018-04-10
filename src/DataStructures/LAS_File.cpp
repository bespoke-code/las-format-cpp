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

    LAS_File::LAS_File(std::fstream *fileStream) {
        this->header = new LAS_Header(fileStream);

        if(header->getOffsetToPointData() > header->getSize()) {
            while (fileStream->tellp() <= header->getOffsetToPointData() - 1) {
                LAS::VariableLengthRecord vlr(fileStream);
                // add new VLR to list
                records.push_back(vlr);
            }
        }

        PointDataRecord point;

        for(unsigned int i=0; i<4; i++) {
            for(unsigned int j=0; j<header->getNumberOfPointsByReturn(i); j++) {
                fileStream->read((char *) &point, (std::streamsize) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE);

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

    void LAS_File::saveTo(std::ofstream* outputStream) {
        this->header->saveTo(outputStream);
        for (auto &record : records) {
            record.saveTo(outputStream);
        }
        for (auto &point : points) { //std::vector<LAS::PointDataRecord>::iterator
            point.saveTo(outputStream, (LAS::POINT_DATA_FORMAT) header->getPointDataFormat());
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