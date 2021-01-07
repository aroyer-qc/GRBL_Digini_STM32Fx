//-------------------------------------------------------------------------------------------------
//
//  File : lib_trigono.cpp
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2020 Alain Royer.
// Email: aroyer.qc@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
// AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
// Const(s)
//-------------------------------------------------------------------------------------------------

const uint16_t SinCosTable[91] = // sin(angle) * 10000
{
    0,
    175,  349,  523,  698,  872,
    1045, 1219, 1392, 1564, 1736,
    1908, 2079, 2250, 2419, 2588,
    2756, 2924, 3090, 3256, 3420,
    3584, 3746, 3907, 4067, 4226,
    4384, 4540, 4695, 4848, 5000,
    5151, 5299, 5447, 5592, 5736,
    5878, 6018, 6157, 6294, 6428,
    6561, 6692, 6820, 6947, 7072,
    7193, 7314, 7431, 7547, 7660,
    7772, 7880, 7986, 8090, 8191,
    8290, 8387, 8480, 8571, 8660,
    8746, 8829, 8910, 8988, 9063,
    9135, 9205, 9272, 9336, 9397,
    9455, 9511, 9563, 9613, 9659,
    9703, 9744, 9781, 9816, 9848,
    9877, 9903, 9926, 9945, 9962,
    9976, 9986, 9994, 9998, 10000
};

const uint16_t TanTable[46] = // tan(angle) * 10000
{
    0,
    175,  349,  524,  699,  875,
    1051, 1228, 1405, 1584, 1763,
    1944, 2126, 2309, 2493, 2679,
    2867, 3057, 3249, 3443, 3640,
    3839, 4040, 4245, 4452, 4663,
    4877, 5095, 5317, 5543, 5773,
    6009, 6249, 6494, 6745, 7002,
    7265, 7536, 7813, 8098, 8391,
    8693, 9004, 9325, 9657, 10000
};

//-------------------------------------------------------------------------------------------------
//
//   Function Name: LIB_TriangleToOppositeCoord
//
//   Parameter(s):  uint16_t    Angle                   1 = 1 degree
//                  uint16_t    AdjacentSize
//   Return:        uint16_t    Coordinate in Cartesian_t value
//
//   Description:   Calculate opposite corner of the right angle triangle from angle and adjacent
//                  line
//
//   Note(s):       Used by LIB_AngleToCircleCoordinate
//
//-------------------------------------------------------------------------------------------------

uint16_t LIB_TriangleToOppositeCoord(uint16_t Angle, uint16_t AdjacentSize)
{
    if(Angle > 45) return 0;
    return uint16_t((TanTable[Angle] * uint32_t(AdjacentSize)) / 10000);
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: LIB_SinCos
//
//   Parameter(s):  uint16_t    Angle                   1 = 1 degree
//                  uint16_t    Radius
//   Return:        uint16_t    Return value for each angle
//
//   Description:   Calculate opposite corner of the right angle triangle from angle and adjacent
//                  line
//
//   TODO change for 2 functions Sin and Cos
//
//-------------------------------------------------------------------------------------------------
uint16_t LIB_SinCos(uint16_t Angle, uint16_t Radius)
{
    if(Angle > 90) return 0;
    return uint16_t((SinCosTable[Angle] * Radius) / 10000);
}

//-------------------------------------------------------------------------------------------------

