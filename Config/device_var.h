//-------------------------------------------------------------------------------------------------
//
//  File :  device_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
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
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------


#if 0  // example

#ifdef __CLASS_CS43L22__
 extern class CS43L22                    CS43L22_AudioCodec;
 #ifdef LIB_CS43L22_GLOBAL
  class CS43L22                          CS43L22_AudioCodec;
 #endif
#endif

#ifdef __CLASS_DS3502__
 extern class DS3502                     DS3502_Volume;
 #ifdef LIB_DS3502_GLOBAL
  class   DS3502                         DS3502_Volume;
 #endif
#endif

#ifdef __CLASS_MAX4598__
 extern class   MAX4598                  MAX4598_AudioSwitch;
 #ifdef LIB_MAX4598_GLOBAL
  class   MAX4598                        MAX4598_AudioSwitch;
 #endif
#endif

#ifdef __CLASS_SI4703__
 extern class SI4703                     SI4703_Radio;
 #ifdef LIB_SI4703_GLOBAL
  class SI4703                           SI4703_Radio;
 #endif
#endif

//#ifdef __CLASS_QSPI_DRIVER__
//  extern QSPI_Driver                    QSPI;
// #ifdef QSPI_DRIVER_GLOBAL
//  QSPI_Driver                           QSPI;
// #endif
//#endif

#endif

//-------------------------------------------------------------------------------------------------
