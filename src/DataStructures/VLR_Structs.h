//
// Created by vdx2 on 10.4.18.
//

#ifndef LAS_READER_VLR_STRUCTS_H
#define LAS_READER_VLR_STRUCTS_H

struct KeyEntry {
    unsigned short keyID, TIFFTagLocation, count, value_offset;
};

struct Classification {
    unsigned char classNumber;
    char description[15];
};

struct GeoKeys {
    unsigned short keyDirectoryVersion;
    unsigned short keyRevision, minorRevision, numberOfKeys;
    KeyEntry* entries;
};

struct GeoParamsTagRecord {
    double *values;
};

struct GeoASCIIParamsTag {
    char *values;
};

struct ClassificationLookup {
    Classification records[255];
};

#endif //LAS_READER_VLR_STRUCTS_H
