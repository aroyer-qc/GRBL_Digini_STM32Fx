//-------------------------------------------------------------------------------------------------
//
//  File : lib_CTimer.h
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
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_typedef.h"
#include "nOS.h"

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class CTimerCallBackInterface
{
    public:

        virtual void TimerCallBack() = 0;
};

class CTimer
{
    public:

                    CTimer                      ();

        void        Create                      (uint32_t          TimerPeriodInTicks,
                                                 bool              bAutoReload,
                                                 void*             pInstance
                                                );
        void        Create                      (uint32_t          TimerPeriodInTicks,
                                                 bool              bAutoReload,
                                                 nOS_TimerCallback CallBack,
                                                 void*             pCallBackArg
                                                );

        bool        IsActive                    ();
        bool        Start                       ();
        bool        Stop                        ();
        bool        Reset                       ();
        bool        Pause                       ();
        bool        Continue                    ();
        bool        ChangePeriod                (uint32_t TimerPeriodInTicks);
        bool        SetCallBack                 (void* CallBack, void* arg);
      #if NOS_CONFIG_SIGNAL_DELETE_ENABLE > 0
        bool        Delete                      ();
      #endif

    private:

        nOS_Timer*          m_pTimerHandle;
        void*               m_pCallBackArg;
};

//-------------------------------------------------------------------------------------------------

// TODO (Alain#1#): need to create callback for timer in C++ for each timer created
//extern "C" void OTime_trCallback_Wrapper(xTimerHandle TimerHandle);
