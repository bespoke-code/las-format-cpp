//
// Created by vdx2 on 14.3.18.
//

#include "VariableLengthRecord.h"
#include <cstring>

namespace LAS {

    VariableLengthRecord::VariableLengthRecord(std::fstream* fileStream, int position) {
        char *tmp;
        tmp = new char[54];
        fileStream->seekp(position);
        fileStream->read(tmp, 54);
        reserved = *((unsigned short*)(tmp));
        std::strncpy(user_id, (tmp+2), 16);
        recordID = *((unsigned short*)(tmp+18));
        record_len_after_header = *((unsigned short*)(tmp+20));
        std::strncpy(description, (tmp+22), 32);
        delete tmp;
        contents = new char[record_len_after_header];
        fileStream->read(contents, record_len_after_header);
    }

    VariableLengthRecord::~VariableLengthRecord() {
        delete contents;
    }

    char* VariableLengthRecord::getContents() {
        return this->contents;
    }

    void VariableLengthRecord::saveTo(std::ofstream* outputStream) {
        outputStream->write((const char*)&reserved, sizeof(unsigned short));
        outputStream->write(user_id, 16*sizeof(char));
        outputStream->write((const char*)&recordID, sizeof(unsigned short));
        outputStream->write((const char*)&record_len_after_header, sizeof(unsigned short));
        outputStream->write(description, 32*sizeof(char));
        if(record_len_after_header > 0)
            outputStream->write(contents, record_len_after_header*sizeof(char));
    }

} //namespace