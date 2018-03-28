//
// Created by vdx2 on 13.3.18.
//

#ifndef LAS_READER_LAS_FILE_H
#define LAS_READER_LAS_FILE_H

#include "LAS_Header.h"
#include "VariableLengthRecord.h"
#include "PointDataRecord.h"
#include <vector>
#include <iostream>

namespace LAS {

    class LAS_File {
    private:
        LAS::LAS_Header* header;
        std::vector<LAS::VariableLengthRecord> records;
        std::vector<LAS::PointDataRecord> points;

    public:
        LAS_File();
        LAS_File(std::fstream* fileStream);
        ~LAS_File();
        void addPoint(PointDataRecord* point);
        void addVariableRecord(VariableLengthRecord* vlrecord);
        void saveTo(std::ofstream* outputStream);

        void setPointFormat(LAS::POINT_DATA_FORMAT format);

        #ifdef DEBUG
        void debug_test();
        #endif

    }; // LAS_File

} // namespace

#endif //LAS_READER_LAS_FILE_H
