#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct ByteUInt {
    union {
        uint32_t FullUint;
        struct {
            uint8_t Byte0;
            uint8_t Byte1;
            uint8_t Byte2;
            uint8_t Byte3;
            };
        }; 
    };

struct CipherKeyContainer {
    union {
        uint8_t AllBytes[10];// = new Byte[10];
        struct {
            uint8_t CipherBytes[5];// = new Byte[5];
            uint8_t KeyBytes[5];// = new Byte[5];
        };
    };
};

struct MeteoFrames {
    uint16_t meteoFrame[3];
};

struct KeyPackage {
    union {
        uint8_t moment[6];
        struct {
            uint8_t  minute;
            uint8_t  hour;
            uint8_t  day;
            uint8_t  month;
            uint8_t  dow;
            uint8_t  year;
            };
        };
};

struct MeteoPackage {
    union {
        uint16_t package[3];
        struct {
            uint16_t package1;
            uint16_t package2;
            uint16_t package3;
            };
        }; 
    union {
        uint8_t moment[6];
        struct {
            uint8_t  bcdMinute;
            uint8_t  bcdHour;
            uint8_t  bcdDay;
            uint8_t  bcdMonth;
            uint8_t  bcdDow;
            uint8_t  bcdYear;
            };
        };
    uint8_t uses_summertime;     
};


/// <summary>
/// Container, wich contains all former global vars
/// </summary>
struct DataContainer {
    /// <summary>
    /// Registers R12 to R15
    /// </summary>
    ByteUInt mByteUint1;// = new ByteUInt();

    /// <summary>
    /// Registers R08 to R0A
    /// </summary>
    ByteUInt mByteUint2;// = new ByteUInt();

    /// <summary>
    /// Registers R0B to R0E
    /// </summary>
    ByteUInt mByteUint3;// = new ByteUInt();

    /// <summary>
    /// Registers R1C to R1E
    /// </summary>
    ByteUInt mByteUint4;// = new ByteUInt();

    uint8_t mByteUpperTime2;//, mByteR1B;
    uint32_t mUintLowerTime;
};