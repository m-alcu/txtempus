// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Part of txtempus, a LF time signal transmitter.
// Copyright (C) 2018 Henner Zeller <h.zeller@acm.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "time-signal-source.h"
#include "dcf77-source.h"

const uint32_t mUintArrBitPattern12[12] = {
    0x80000, //0b10000000000000000000, /* 0D.3 */
    0x00010, //0b00000000000000010000, /* 0B.4 */
    0x00008, //0b00000000000000001000, /* 0B.3 */
    0x00100, //0b00000000000100000000, /* 0C.0 */
    0x00080, //0b00000000000010000000, /* 0B.7 */
    0x01000, //0b00000001000000000000, /* 0C.4 */
    0x00800, //0b00000000100000000000, /* 0C.3 */
    0x10000, //0b00010000000000000000, /* 0D.0 */
    0x08000, //0b00001000000000000000, /* 0C.7 */
    0x00001, //0b00000000000000000001, /* 0B.0 */
    0x00000, //0b00000000000000000000, /* xxxx */
    0x00000  //0b00000000000000000000  /* xxxx */
};

const uint32_t mUintArrBitPattern30_1[30] = {
    0x00000200, //0b00000000000000000000001000000000, /* 17.1 */
    0x00000020, //0b00000000000000000000000000100000, /* 16.5 */
    0x02000000, //0b00000010000000000000000000000000, /* 19.1 */
    0x00000000, //0b00000000000000000000000000000000, /* 1A.3 */
    0x00000000, //0b00000000000000000000000000000000, /* 1A.5 */
    0x00000080, //0b00000000000000000000000010000000, /* 16.7 */
    0x40000000, //0b01000000000000000000000000000000, /* 19.6 */
    0x01000000, //0b00000001000000000000000000000000, /* 19.0 */

    0x04000000, //0b00000100000000000000000000000000, /* 19.2 */
    0x00000000, //0b00000000000000000000000000000000, /* 1A.4 */
    0x00010000, //0b00000000000000010000000000000000, /* 18.0 */
    0x00000000, //0b00000000000000000000000000000000, /* 1A.2 */
    0x00400000, //0b00000000010000000000000000000000, /* 18.6 */
    0x00000010, //0b00000000000000000000000000010000, /* 16.4 */
    0x00200000, //0b00000000001000000000000000000000, /* 18.5 */
    0x00080000, //0b00000000000010000000000000000000, /* 18.3 */

    0x00004000, //0b00000000000000000100000000000000, /* 17.6 */
    0x00000000, //0b00000000000000000000000000000000, /* 1A.6 */
    0x00020000, //0b00000000000000100000000000000000, /* 18.1 */
    0x00100000, //0b00000000000100000000000000000000, /* 18.4 */
    0x00008000, //0b00000000000000001000000000000000, /* 17.7 */
    0x00000040, //0b00000000000000000000000001000000, /* 16.6 */
    0x00001000, //0b00000000000000000001000000000000, /* 17.4 */
    0x00000400, //0b00000000000000000000010000000000, /* 17.2 */

    0x00000001, //0b00000000000000000000000000000001, /* 16.0 */
    0x80000000, //0b10000000000000000000000000000000, /* 19.7 */
    0x00000008, //0b00000000000000000000000000001000, /* 16.3 */
    0x00000002, //0b00000000000000000000000000000010, /* 16.1 */
    0x00040000, //0b00000000000001000000000000000000, /* 18.2 */
    0x10000000  //0b00010000000000000000000000000000, /* 19.4 */
};

const uint32_t mUintArrBitPattern30_2[30] = {
    0x00, //0b00000000,  /* 17.1 */
    0x00, //0b00000000,  /* 16.5 */
    0x00, //0b00000000,  /* 19.1 */
    0x08, //0b00001000,  /* 1A.3 */
    0x20, //0b00100000,  /* 1A.5 */
    0x00, //0b00000000,  /* 16.7 */
    0x00, //0b00000000,  /* 19.6 */
    0x00, //0b00000000,  /* 19.0 */

    0x00, //0b00000000,  /* 19.2 */
    0x10, //0b00010000,  /* 1A.4 */
    0x00, //0b00000000,  /* 18.0 */
    0x04, //0b00000100,  /* 1A.2 */
    0x00, //0b00000000,  /* 18.6 */
    0x00, //0b00000000,  /* 16.4 */
    0x00, //0b00000000,  /* 18.5 */
    0x00, //0b00000000,  /* 18.3 */

    0x00, //0b00000000,  /* 17.6 */
    0x40, //0b01000000,  /* 1A.6 */
    0x00, //0b00000000,  /* 18.1 */
    0x00, //0b00000000,  /* 18.4 */
    0x00, //0b00000000,  /* 17.7 */
    0x00, //0b00000000,  /* 16.6 */
    0x00, //0b00000000,  /* 17.4 */
    0x00, //0b00000000,  /* 17.2 */

    0x00, //0b00000000,  /* 16.0 */
    0x00, //0b00000000,  /* 19.7 */
    0x00, //0b00000000,  /* 16.3 */
    0x00, //0b00000000,  /* 16.1 */
    0x00, //0b00000000,  /* 18.2 */
    0x00  //0b00000000,  /* 19.4 */
};

const uint32_t mUintArrBitPattern20[20] = {
    0x000004, //0b000000000000000000000100, /* 1C.2 */
    0x002000, //0b000000000010000000000000, /* 1E.5 */
    0x008000, //0b000000001000000000000000, /* 1E.7 */
    0x400000, //0b010000000000000000000000, /* 1D.6 */
    0x000100, //0b000000000000000100000000, /* 1E.0 */
    0x100000, //0b000100000000000000000000, /* 1D.4 */
    0x000400, //0b000000000000010000000000, /* 1E.2 */
    0x800000, //0b100000000000000000000000, /* 1D.7 */

    0x040000, //0b000001000000000000000000, /* 1D.2 */
    0x020000, //0b000000100000000000000000, /* 1D.1 */
    0x000008, //0b000000000000000000001000, /* 1C.3 */
    0x000200, //0b000000000000001000000000, /* 1E.1 */
    0x004000, //0b000000000100000000000000, /* 1E.6 */
    0x000002, //0b000000000000000000000010, /* 1C.1 */
    0x001000, //0b000000000001000000000000, /* 1E.4 */
    0x080000, //0b000010000000000000000000, /* 1D.3 */

    0x000800, //0b000000000000100000000000, /* 1E.3 */
    0x200000, //0b001000000000000000000000, /* 1D.5 */
    0x010000, //0b000000010000000000000000, /* 1D.0 */
    0x000001  //0b000000000000000000000001  /* 1C.0 */
};

/// <summary>
/// bit pattern for 12-15 from 16-19 (1/3)
/// </summary>
const uint8_t mByteArrLookupTable1C_1[] = {
    0xBB, 0x0E, 0x22, 0xC5, 0x73, 0xDF, 0xF7, 0x6D, 0x90, 0xE9, 0xA1, 0x38, 0x1C, 0x84, 0x4A, 0x56,
    0x64, 0x8D, 0x28, 0x0B, 0xD1, 0xBA, 0x93, 0x52, 0x1C, 0xC5, 0xA7, 0xF0, 0xE9, 0x7F, 0x36, 0x4E,
    0xC1, 0x77, 0x3D, 0xB3, 0xAA, 0xE0, 0x0C, 0x6F, 0x14, 0x88, 0xF6, 0x2B, 0xD2, 0x99, 0x5E, 0x45,
    0x1F, 0x70, 0x96, 0xD3, 0xB3, 0x0B, 0xFC, 0xEE, 0x81, 0x42, 0xCA, 0x34, 0xA5, 0x58, 0x29, 0x67
};

/// <summary>
/// bit pattern for 12-15 from 16-19 (2/3)
/// </summary>
const uint8_t mByteArrLookupTable1C_2[] = {
    0xAB, 0x3D, 0xFC, 0x74, 0x65, 0xE6, 0x0E, 0x4F, 0x97, 0x11, 0xD8, 0x59, 0x83, 0xC2, 0xBA, 0x20,
    0xC5, 0x1B, 0xD2, 0x58, 0x49, 0x37, 0x01, 0x7D, 0x93, 0xFA, 0xE0, 0x2F, 0x66, 0xB4, 0xAC, 0x8E,
    0xB7, 0xCC, 0x43, 0xFF, 0x58, 0x66, 0xEB, 0x35, 0x82, 0x2A, 0x99, 0xDD, 0x00, 0x71, 0x14, 0xAE,
    0x4E, 0xB1, 0xF7, 0x70, 0x18, 0x52, 0xAA, 0x9F, 0xD5, 0x6B, 0xCC, 0x3D, 0x04, 0x83, 0xE9, 0x26
};

/// <summary>
/// bit pattern for 12-15 from 16-19 (3/3)
/// </summary>
const uint8_t mByteArrLookupTable1C_3[] = {
    0x0A, 0x02, 0x00, 0x0F, 0x06, 0x07, 0x0D, 0x08, 0x03, 0x0C, 0x0B, 0x05, 0x09, 0x01, 0x04, 0x0E,
    0x02, 0x09, 0x05, 0x0D, 0x0C, 0x0E, 0x0F, 0x08, 0x06, 0x07, 0x0B, 0x01, 0x00, 0x0A, 0x04, 0x03,
    0x08, 0x00, 0x0D, 0x0F, 0x01, 0x0C, 0x03, 0x06, 0x0B, 0x04, 0x09, 0x05, 0x0A, 0x07, 0x02, 0x0E,
    0x03, 0x0D, 0x00, 0x0C, 0x09, 0x06, 0x0F, 0x0B, 0x01, 0x0E, 0x08, 0x0A, 0x02, 0x07, 0x04, 0x05
};

static uint64_t to_bcd(uint8_t n) { return (((n / 10) % 10) << 4) | (n % 10); }
static uint64_t parity(uint64_t d, uint8_t from, uint8_t to_including) {
  uint8_t result = 0;
  for (int bit = from; bit <= to_including; ++bit) {
    if (d & (1LL << bit)) result++;
  }
  return result & 0x1;
}

void GetPlainFromMeteo(uint8_t* result, uint32_t meteo)
{
    //byte[] result = new byte[5];
    meteo <<= 4;
    result[1] = 0x50;            
    result[2] = (uint8_t)((meteo & 0xF0) | 0x02);
    //Console.WriteLine("meteo to 2 0x{0:X}", meteo);
    //Console.WriteLine("result 2 0x{0:X}", result[2]);
    meteo >>= 8;
    result[3] = (uint8_t)(meteo & 0xFF);
    //Console.WriteLine("meteo to 3 0x{0:X} ", meteo);
    //Console.WriteLine("result 3 0x{0:X}", result[3]);
    meteo >>= 8;
    result[4] = (uint8_t)(meteo & 0xFF);
    //Console.WriteLine("meteo to 4 0x{0:X} ", meteo);
    //Console.WriteLine("result 4 0x{0:X}", result[4]);
    //Hay un error en el montaje del plain text para ponerle el checksum 0x2501
    meteo >>= 8;
    result[0] = (uint8_t)(meteo & 0x0F | 0x10);
    //Console.WriteLine("meteo to 0 x{0:X} ", meteo);
    //Console.WriteLine("result 0 0x{0:X}", result[0]);            
    //return result;
}

void CopyTimeToByteUint(uint8_t* data, uint8_t* key, DataContainer *container)
{
    for (int i = 0; i < 4; i++)
    {
        container->mUintLowerTime <<= 8;
        container->mUintLowerTime |= (uint32_t) ( key[3 - i] );
    }
    container->mByteUpperTime2 =  (uint32_t) key[4];

    // copy R
    container->mByteUint3.Byte0 = (uint8_t) data[2];
    container->mByteUint3.Byte1 = (uint8_t) data[3];
    container->mByteUint3.Byte2 = (uint8_t) data[4];
    container->mByteUint3.FullUint >>= 4;
 
    // copy L
    container->mByteUint2.Byte0 = (uint8_t) data[0];
    container->mByteUint2.Byte1 = (uint8_t) data[1];
    container->mByteUint2.Byte2 = (uint8_t)(data[2] & 0x0F);
}

void ExpandL(DataContainer *container)
{
    uint32_t tmp;

    container->mByteUint2.FullUint &= 0x000FFFFF;			// clear 0D(4-7),0E
    tmp = 0x00100000;					// and set bits form 0B-0D(0-3)
    for (int i = 0; i < 12; i++)
    {
        if ((container->mByteUint2.FullUint & mUintArrBitPattern12[i]) != 0)
            container->mByteUint2.FullUint |= tmp;
        tmp <<= 1;
    }
}

void CompressKey(DataContainer *container)
{
    uint32_t tmp;

    container->mByteUint1.FullUint = 0;					// clear 12-15
    tmp = 0x00000001;					// and set bits from 16-1A (time)
    for (int i = 0; i < 30; i++)
    {
        if ((container->mUintLowerTime & mUintArrBitPattern30_1[i]) != 0 || (container->mByteUpperTime2 & mUintArrBitPattern30_2[i]) != 0)
            container->mByteUint1.FullUint |= tmp;
        tmp <<= 1;
    }
}

void DoSbox(DataContainer *container)
{
    uint8_t tmp, helper; //mByteR1B;

    helper = container->mByteUint1.Byte3;					      // R1B = R15;
    container->mByteUint1.Byte3 = container->mByteUint1.Byte2;			// R15 = R14

    // INNER LOOP
    for (int i = 5; i > 0; i--)
    {
        if ((i & 1) == 0) // round 4,2
        {
            tmp = (uint8_t)(container->mByteUint1.Byte0 >> 4);		// swap R12
            tmp |= (uint8_t)((container->mByteUint1.Byte0 & 0x0f) << 4);
            container->mByteUint1.Byte0 = tmp;
        }
        container->mByteUint1.Byte3 &= 0xF0;						// set R1C
        tmp = (uint8_t)((container->mByteUint1.Byte0 & 0x0F) | container->mByteUint1.Byte3);

        if ((i & 4) != 0)
            tmp = mByteArrLookupTable1C_1[(tmp & 0x3F)];
        if ((i & 2) != 0)
            tmp = mByteArrLookupTable1C_2[(tmp & 0x3F)];

        else if (i == 1)
            tmp = mByteArrLookupTable1C_3[(tmp & 0x3F)];
 
        if ((i & 1) != 0)
            container->mByteUint4.Byte0 = tmp & 0x0F;
        else
            container->mByteUint4.Byte0 |= tmp & 0xF0;

        if ((i & 1) == 0)							// copy 14->13->12, 1C->1E->1D
        {
            tmp = container->mByteUint1.Byte3;
            container->mByteUint1.FullUint >>= 8;
            container->mByteUint1.Byte3 = tmp;
            container->mByteUint4.FullUint <<= 8;
        }

        container->mByteUint1.Byte3 >>= 1;					// rotate R1B>R15 twice
        if ((helper & 1) != 0)
            container->mByteUint1.Byte3 |= 0x80;
        helper >>= 1;

        container->mByteUint1.Byte3 >>= 1;
        if ((helper & 1) != 0)
            container->mByteUint1.Byte3 |= 0x80;
        helper >>= 1;
    } // end of inner loop
}

void DoPbox(DataContainer *container)
{
    uint32_t tmp;

    container->mByteUint1.FullUint = 0xFF000000;			// clear 12-14
    tmp = 0x00000001;					// and set bits from 1C-1E (result from F)
    for (int i = 0; i < 20; i++)
    {
        if ((container->mByteUint4.FullUint & mUintArrBitPattern20[i]) != 0)
            container->mByteUint1.FullUint |= tmp;
        tmp <<= 1;
    }
}

void ShiftTimeLeft(int round, DataContainer *container)
{
    int count;
    int8_t tmp;

    if ((round == 16) || (round == 8) || (round == 7) || (round == 3))
        count = 2;
    else
        count = 1;

    while (count-- != 0)
    {
        tmp = 0;
        if ((container->mByteUpperTime2 & 0x80) != 0)
            tmp = 1;
        container->mByteUpperTime2 <<= 1;

        if ((container->mUintLowerTime & 0x80000000) != 0)
            container->mByteUpperTime2 |= 1;

        container->mUintLowerTime <<= 1;
        if ((container->mUintLowerTime & 0x00100000) != 0)
            container->mUintLowerTime |= 1;

        container->mUintLowerTime &= 0xFFEFFFFF;
        if (tmp != 0)
            container->mUintLowerTime |= 0x00100000;
    }
}

void Encrypt(uint8_t* cipher, uint8_t* plain, uint8_t* key)
{
    DataContainer container = {0,0,0,0,0,0};
    //uint ulTemp;
    //uiCnt, uiILCnt, , uiOL2Cnt, uiBitCnt
    //byte ucOL2Pat, ucTemp;

    CopyTimeToByteUint(plain, key, &container);

    // OUTER LOOP 1
    for (int i = 1; i < 17; i++)
    {
        ExpandL(&container);
        CompressKey(&container);

        // expR XOR compr.Key
        container.mByteUint1.FullUint ^= container.mByteUint2.FullUint;	   // L' XOR compr.Key
        container.mByteUint3.Byte2 &= 0x0F;				// clear 0D(4-7)

        DoSbox(&container);
        DoPbox(&container);

        // L XOR P-Boxed Round-Key (L')
        container.mByteUint1.FullUint ^= container.mByteUint3.FullUint;
        // L = R
        container.mByteUint3.FullUint = container.mByteUint2.FullUint & 0x00FFFFFF;
        // R = L'
        container.mByteUint2.FullUint = container.mByteUint1.FullUint & 0x00FFFFFF;

        ShiftTimeLeft(i, &container);
    } // end of outer loop 1

    container.mByteUint3.FullUint <<= 4;
    container.mByteUint2.Byte2 &= 0x0F;
    container.mByteUint2.Byte2 |= (uint8_t)(container.mByteUint3.Byte0 & 0xF0);

    //R0B0C0D0E.byte.R0D |= (R08090A.byte.R08 & 0xF0);
    cipher[0] = container.mByteUint2.Byte0;
    cipher[1] = container.mByteUint2.Byte1;
    cipher[2] = container.mByteUint2.Byte2;
    cipher[3] = container.mByteUint3.Byte1;
    cipher[4] = container.mByteUint3.Byte2;

    /*
        R08090A.ulFull <<= 4;
        R0B0C0D0E.byte.R0D |= (R08090A.byte.R08 & 0xF0);
        pucCipher[0] = R0B0C0D0E.byte.R0B;
        pucCipher[1] = R0B0C0D0E.byte.R0C;
        pucCipher[2] = R0B0C0D0E.byte.R0D;
        pucCipher[3] = R08090A.byte.R09;
        pucCipher[4] = R08090A.byte.R0A;
    */
}

void EncodeDataset(uint32_t plain, uint8_t* key, uint8_t* result)
{
    //byte[] result; // = new byte[10];
    //CipherKeyContainer InOut = new CipherKeyContainer();
    CipherKeyContainer InOut;

    for (int i = 0; i < 5; i++)
        InOut.KeyBytes[i] = key[i];

    uint8_t plainToEncrypt[5] = {0,0,0,0,0};
    GetPlainFromMeteo(plainToEncrypt, plain); //nur 22 Bit

    for (int i = 0; i < 5; i++)
        InOut.CipherBytes[i] = plainToEncrypt[i];

    Encrypt(result, InOut.CipherBytes, InOut.KeyBytes);
    for (int i = 0; i < 5; i++)
        InOut.CipherBytes[i] = result[i];

    //result = new byte[10];
    for (int i = 0; i < 10; i++)
        result[i] = InOut.AllBytes[i];

}

uint8_t int_to_bcd(const uint8_t value) {
    uint8_t tens = value / 10;
    uint8_t units = value % 10;
    return (tens << 4) | units;
}

void EncodeDataset(uint32_t plain, KeyPackage keyPackage, uint8_t* result) 
{

    uint8_t ucTemp;
    uint8_t momentTemp;
    uint32_t uiBitCnt, uiCnt, uiMoment;
    uint8_t dataKey[] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t ucTemp2;

    ucTemp = 0;
    uiBitCnt = 0;
    for (uiMoment = 0; uiMoment < 6; uiMoment++) {
        ucTemp2 = keyPackage.moment[uiMoment];
        momentTemp = int_to_bcd(ucTemp2);
        for (uiCnt = 0; uiCnt < 8; uiCnt++)
        {
            if ((( uiMoment != 3 | uiCnt <= 4 ) & ( uiMoment != 4 | uiCnt <= 2)) | (uiMoment != 3 & uiMoment != 4))
            {
                ucTemp = (uint8_t)(ucTemp >> 1);
                if ((momentTemp & 1) == 1)
                    ucTemp |= 0x80;
                uiBitCnt++;
                if ((uiBitCnt & 7) == 0)
                    dataKey[(uiBitCnt >> 3) - 1] = ucTemp;
            }
            momentTemp =(uint8_t)(momentTemp >> 1);
        }
    }
    EncodeDataset(plain, dataKey, result);

}

MeteoFrames GetMeteoFrames(uint8_t* dataCiperKey) {

    uint8_t uiFrame, uiCnt, uiKey, tempKey;
    MeteoFrames meteoframes;
    uint16_t uiCntFrame;

    // 0101111000.1010  1101.01101001.11  101001.10010011

    uiFrame = 0;
    uiCntFrame = 0;
    meteoframes.meteoFrame[0] = 0;
    meteoframes.meteoFrame[1] = 0;
    meteoframes.meteoFrame[2] = 0;
    for (uiKey = 0; uiKey < 5; uiKey++) {
        tempKey = dataCiperKey[uiKey];
        for(uiCnt = 0; uiCnt < 8; uiCnt++) {
            if (uiKey == 0 & (uiCnt == 0 | uiCnt == 6)) {
                meteoframes.meteoFrame[uiFrame] =(uint16_t)(meteoframes.meteoFrame[uiFrame] >> 1);    
                uiCntFrame++;
            }             
            if ((tempKey & 1) == 1) {
                meteoframes.meteoFrame[uiFrame] |= 0x2000;
            }
            
            uiCntFrame++;
            tempKey = (uint8_t)(tempKey >> 1);
            if (uiCntFrame == 14) {
                uiCntFrame = 0;
                uiFrame++;
            } else {
                meteoframes.meteoFrame[uiFrame] =(uint16_t)(meteoframes.meteoFrame[uiFrame] >> 1);
            }
        }
    }
    return meteoframes;
}

uint16_t getFrame(uint32_t plain, KeyPackage keyPackage) {

  uint8_t seccion = keyPackage.minute % 3;
  keyPackage.minute = keyPackage.minute + 2 - seccion;  

  uint8_t keys[10];
  EncodeDataset(plain, keyPackage, keys);
  MeteoFrames meteoframes = GetMeteoFrames(keys);

  return meteoframes.meteoFrame[seccion];
}

void DCF77TimeSignalSource::PrepareMinute(time_t t) {
  t += 60;  // We're sending the _upcoming_ minute.
  struct tm breakdown;
  localtime_r(&t, &breakdown);

  KeyPackage keyPackage;
  keyPackage.year = breakdown.tm_year;
  keyPackage.dow = breakdown.tm_wday ? breakdown.tm_wday : 7;
  keyPackage.month = breakdown.tm_mon;
  keyPackage.day = breakdown.tm_mday;
  keyPackage.hour = breakdown.tm_hour;
  keyPackage.minute = breakdown.tm_min;
  uint16_t frame = getFrame(6512678, keyPackage);

  // https://de.wikipedia.org/wiki/DCF77
  // Little endian bits. So we store big-endian bits and start transmitting
  // from bit 0
  time_bits_ = 0;
  time_bits_ |= frame << 1;
  time_bits_ |= (breakdown.tm_isdst ? 1 : 0) << 17;
  time_bits_ |= (breakdown.tm_isdst ? 0 : 1) << 18;
  time_bits_ |= (1<<20);  // start time bit.
  time_bits_ |= to_bcd(breakdown.tm_min)  << 21;
  time_bits_ |= to_bcd(breakdown.tm_hour) << 29;
  time_bits_ |= to_bcd(breakdown.tm_mday) << 36;
  time_bits_ |= to_bcd(breakdown.tm_wday ? breakdown.tm_wday : 7) << 42;
  time_bits_ |= to_bcd(breakdown.tm_mon + 1) << 45;
  time_bits_ |= to_bcd(breakdown.tm_year % 100) << 50;

  time_bits_ |= parity(time_bits_, 21, 27) << 28;
  time_bits_ |= parity(time_bits_, 29, 34) << 35;
  time_bits_ |= parity(time_bits_, 36, 57) << 58;
}

TimeSignalSource::SecondModulation
DCF77TimeSignalSource::GetModulationForSecond(int second) {
  if (second >= 59)
    return {{CarrierPower::HIGH, 0}};  // Synchronization
  const bool bit = time_bits_ & (1LL << second);
  return {{CarrierPower::LOW, bit ? 200 : 100}, {CarrierPower::HIGH, 0}};
}
