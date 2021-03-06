//
// Created by vdx2 on 14.3.18.
//

#ifndef LAS_READER_VARIABLELENGTHRECORD_H
#define LAS_READER_VARIABLELENGTHRECORD_H

#include <fstream>
#include "VLR_Structs.h"


namespace LAS {

    enum RECORD_TYPE : unsigned short {
        GEO_KEY_DIRECTORY_TAG = 34735,
        GEO_DOUBLE_PARAMS_TAG_RECORD = 34736,
        GEO_ASCII_PARAMS_TAG = 34737,
        CLASSIFICATION_LOOKUP = 0,
        HISTOGRAM = 2,
        TEXT_AREA_DESCRIPTION = 3
    };

    class VariableLengthRecord {
    private:
        unsigned short reserved;
        char user_id[16];
        unsigned short recordID;
        unsigned short record_len_after_header;
        char description[32];
        void *contents;

    public:
        VariableLengthRecord(std::fstream &fileStream);
        VariableLengthRecord(LAS::RECORD_TYPE type);
        ~VariableLengthRecord();
        char *getContents();
        int size();
        LAS::RECORD_TYPE type();
        void serialize(std::ofstream &outputStream);
    };
}


#endif //LAS_READER_VARIABLELENGTHRECORD_H
