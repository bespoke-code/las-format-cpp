//
// Created by vdx2 on 14.3.18.
//

#include "VariableLengthRecord.h"
#include <cstring>

namespace LAS {

    VariableLengthRecord::VariableLengthRecord(std::fstream* fileStream, long int position) {
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
        fileStream->read((char*) contents, record_len_after_header);
    }

    VariableLengthRecord::VariableLengthRecord(LAS::RECORD_TYPE type): reserved(0){
        std::strcpy(user_id, "LASF_Projection\0");
        switch (type) {
            case LAS::RECORD_TYPE::GEO_KEY_DIRECTORY_TAG : {
                contents = new GeoKeys;
                GeoKeys *geokey = (GeoKeys *) contents;
                geokey->keyDirectoryVersion = 1;
                geokey->keyRevision = 1;
                geokey->minorRevision = 0;
                geokey->numberOfKeys = 0;
                geokey->entries = NULL;
                recordID = (unsigned short) RECORD_TYPE::GEO_KEY_DIRECTORY_TAG;
                record_len_after_header = 8;
                this->recordID = LAS::RECORD_TYPE::GEO_KEY_DIRECTORY_TAG;
                break;
            }
            case LAS::RECORD_TYPE::GEO_ASCII_PARAMS_TAG :
                this->recordID = (unsigned short) LAS::RECORD_TYPE::GEO_ASCII_PARAMS_TAG;
                break;
            case LAS::RECORD_TYPE::GEO_DOUBLE_PARAMS_TAG_RECORD :
                this->recordID = (unsigned short) LAS::RECORD_TYPE::GEO_DOUBLE_PARAMS_TAG_RECORD;
                break;
            case LAS::RECORD_TYPE::CLASSIFICATION_LOOKUP :
                record_len_after_header = 255*16;
                this->recordID = (unsigned short) LAS::RECORD_TYPE::CLASSIFICATION_LOOKUP;
                std::strcpy(user_id, "LASF_Spec\0");
                break;
            case LAS::RECORD_TYPE::HISTOGRAM :
                this->recordID = (unsigned short) LAS::RECORD_TYPE::HISTOGRAM;
                std::strcpy(user_id, "LASF_Spec\0");
                break;
            case LAS::RECORD_TYPE::TEXT_AREA_DESCRIPTION :
                this->recordID = (unsigned short) LAS::RECORD_TYPE::TEXT_AREA_DESCRIPTION;
                std::strcpy(user_id, "LASF_Spec\0");
                break;
            default:
                break;
        }
    }

    VariableLengthRecord::~VariableLengthRecord() {
        delete (char*) contents;
    }

    char* VariableLengthRecord::getContents() {
        return (char*)(this->contents);
    }

    void VariableLengthRecord::saveTo(std::ofstream* outputStream) {
        outputStream->write((const char*)&reserved, sizeof(unsigned short));
        outputStream->write(user_id, 16*sizeof(char));
        outputStream->write((const char*)&recordID, sizeof(unsigned short));
        outputStream->write((const char*)&record_len_after_header, sizeof(unsigned short));
        outputStream->write(description, 32*sizeof(char));
        if(record_len_after_header > 0)
            outputStream->write((char*)contents, record_len_after_header*sizeof(char));
    }

    int VariableLengthRecord::size() {
        return 54 + this->record_len_after_header;
    }

    LAS::RECORD_TYPE VariableLengthRecord::type() {
        return (RECORD_TYPE) recordID;
    }

} //namespace