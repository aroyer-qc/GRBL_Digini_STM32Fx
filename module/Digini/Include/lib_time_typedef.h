//-------------------------------------------------------------------------------------------------
//
//  File : lib_time_typedef.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Type definition(s), enum(s) and structure(s)
//-------------------------------------------------------------------------------------------------

struct Time_t
{
    uint8_t     Second;
    uint8_t     Minute;
    uint8_t     Hour;
};

struct Date_t
{
    uint8_t     Day;
    uint8_t     Month;
    uint8_t     Year;
};

struct  Clock_t
{
    Time_t      Time;
    Date_t      Date;
    uint8_t     DayOfWeek;
    uint16_t    DayOfYear;
    uint8_t     WeekOfYear;
    uint8_t     Century;
    uint16_t    MinuteOfDay;
    uint32_t    SecondStamp;
};

enum TimeFormat_e
{
    TIME_FORMAT_24_HOUR = 0,
    TIME_FORMAT_12_HOUR,
};

//-------------------------------------------------------------------------------------------------

