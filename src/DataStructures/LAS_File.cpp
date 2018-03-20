//
// Created by vdx2 on 13.3.18.
//

#include "LAS_File.h"
#include <vector>
#include <iostream>
#include <fstream>


namespace LAS {

    LAS_File::LAS_File(std::fstream *fileStream) {
        this->header = new LAS_Header();
        fileStream->read((char*)header, sizeof(LAS_Header));
        fileStream->seekp(header->getOffsetToPointData());

        PointDataRecord point;
        std::cout<<"Reading points..."<<std::endl;
        for(unsigned int i=0; i<4; i++)
        for(unsigned int j=0; j<header->getNumberOfPointsByReturn(i); j++){
            fileStream->read((char*)&point, (std::streamsize) LAS::POINT_DATA_SIZE::POINT_DATA_FORMAT_0_SIZE);

            switch (header->getPointDataFormat()) {
                case LAS::POINT_DATA_FORMAT::FORMAT_0:
                    break;
                case LAS::POINT_DATA_FORMAT::FORMAT_1:
                    point.setGPSTime(fileStream);
                    break;
                case LAS::POINT_DATA_FORMAT::FORMAT_2:
                    point.setRGB(fileStream);
                    break;
                case LAS::POINT_DATA_FORMAT::FORMAT_3:
                    point.setGPSTime(fileStream);
                    point.setRGB(fileStream);
                    break;
                default:
                    break;
            }

                std::cout<<"File pointer at: "<<fileStream->tellp()<<std::endl;
            points.push_back(point);
        }
    }

    LAS_File::~LAS_File() {
        delete header;
    }

    void LAS_File::addPoint(PointDataRecord point) {
        points.push_back(point);
    }

    void LAS_File::debug_test() {
        int count=0;
        for(std::vector<LAS::PointDataRecord>::iterator i=points.begin(); i!=points.end() || count<10; ++i){
            std::cout<<"Point "<<count<<" is:"<<std::endl;
            std::cout<<"x = "<<i->getX()*header->getOffset(LAS::AXIS::X_AXIS)<<std::endl;
            std::cout<<"y = "<<i->getY()*header->getOffset(LAS::AXIS::Y_AXIS)<<std::endl;
            std::cout<<"z = "<<i->getZ()*header->getOffset(LAS::AXIS::Z_AXIS)<<std::endl;
            std::cout<<std::endl;
            count++;
        }
    }
}