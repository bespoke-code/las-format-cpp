//
// Created by vdx2 on 14.3.18.
//

#ifndef LAS_READER_VARIABLELENGTHRECORD_H
#define LAS_READER_VARIABLELENGTHRECORD_H

#include <fstream>

namespace LAS {

    class VariableLengthRecord {
    private:
        unsigned short reserved;
        char user_id[16];
        unsigned short recordID;
        unsigned short record_len_after_header;
        char description[32];
        char *contents;

    public:
        VariableLengthRecord(std::fstream *fileStream, int position=227);
        ~VariableLengthRecord();
        char *getContents();
        void saveTo(std::ofstream* outputStream);
    };
}


#endif //LAS_READER_VARIABLELENGTHRECORD_H
