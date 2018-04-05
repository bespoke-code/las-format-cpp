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
        //fileStream->read((char*)header, sizeof(LAS_Header));
        long int pos = fileStream->tellp();
        std::cout << pos << " " << header->getOffsetToPointData() << std::endl;


        int totalVRLs = 1;
        fileStream->seekp(header->getOffsetToPointData());

        LAS::VariableLengthRecord vrl_geokey(fileStream);
        if(vrl_geokey.type() == LAS::RECORD_TYPE::GEO_KEY_DIRECTORY_TAG){
            auto geokeys = (LAS::GeoKeys*) vrl_geokey.getContents();
            totalVRLs = geokeys->numberOfKeys;
            addVariableRecord(&vrl_geokey);
            while(totalVRLs!=0)
            {
                LAS::VariableLengthRecord vrl(fileStream);
                addVariableRecord(&vrl);
                totalVRLs--;
            }
        }


        PointDataRecord point;

        for(unsigned int i=0; i<4; i++) {
            for(unsigned int j=0; j<header->getNumberOfPointsByReturn(i); j++) {
                fileStream->read((char *) &point, (std::streamsize) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE);

                switch (header->getPointDataFormat()) {
                    case LAS::POINT_DATA_FORMAT::FORMAT_0:
                        break;
                    case LAS::POINT_DATA_FORMAT::FORMAT_1:
                        // setting GPS time only
                        point.setGPSTime(fileStream);
                        break;
                    case LAS::POINT_DATA_FORMAT::FORMAT_2:
                        // setting RGB only
                        point.setRGB(fileStream);
                        break;
                    case LAS::POINT_DATA_FORMAT::FORMAT_3:
                        // order is important!
                        point.setGPSTime(fileStream);
                        point.setRGB(fileStream);
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

        if(point->getX() > header->getMaximum(LAS::AXIS::X_AXIS))
            header->setMaximum(LAS::AXIS::X_AXIS, point->getX());
        if(point->getY() > header->getMaximum(LAS::AXIS::Y_AXIS))
            header->setMaximum(LAS::AXIS::Y_AXIS, point->getY());
        if(point->getZ() > header->getMaximum(LAS::AXIS::Z_AXIS))
            header->setMaximum(LAS::AXIS::Z_AXIS, point->getZ());

        if(point->getX() < header->getMinimum(LAS::AXIS::X_AXIS))
            header->setMinimum(LAS::AXIS::X_AXIS, point->getX());
        if(point->getY() < header->getMinimum(LAS::AXIS::Y_AXIS))
            header->setMinimum(LAS::AXIS::Y_AXIS, point->getY());
        if(point->getZ() < header->getMinimum(LAS::AXIS::Z_AXIS))
            header->setMinimum(LAS::AXIS::Z_AXIS, point->getZ());

        this->header->incrementPointCount();
    }

    void LAS_File::saveTo(std::ofstream* outputStream) {
        this->header->saveTo(outputStream);
        for (auto &record : records) {
            record.saveTo(outputStream);
        }
        for (auto &point : points) { //std::vector<LAS::PointDataRecord>::iterator
            point.saveTo(outputStream, header->getPointDataFormat());
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