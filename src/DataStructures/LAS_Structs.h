//
// Created by vdx2 on 21.3.18.
//

#ifndef LAS_READER_LAS_STRUCTS_H
#define LAS_READER_LAS_STRUCTS_H

namespace LAS {

    enum struct POINT_DATA_FORMAT : unsigned char {
        FORMAT_0 = 0,
        FORMAT_1,
        FORMAT_2,
        FORMAT_3
    };

    enum struct POINT_DATA_SIZE : int {
        POINT_DATA_FORMAT_0_SIZE = 20,
        POINT_DATA_FORMAT_1_SIZE = 28,
        POINT_DATA_FORMAT_2_SIZE = 26,
        POINT_DATA_FORMAT_3_SIZE = 34
    };

}
#endif //LAS_READER_LAS_STRUCTS_H
