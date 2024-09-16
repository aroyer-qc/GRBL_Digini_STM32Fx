//-------------------------------------------------------------------------------------------------
//
//  File :  label_cfg.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

// Notes some default label exist in the file "lib_label.h", some of them can be enable/disable in this configuration
#define     LABEL_USE_DATE                      DEF_ENABLED
#define     LABEL_USE_TIME                      DEF_ENABLED
#define     LABEL_USE_PRODUCT_INFO              DEF_ENABLED
#define     LABEL_USE_DECIMAL_KEY               DEF_ENABLED             // Enable label 0-9 and dot, del, sign, sign inversion
#define     LABEL_USE_HEXADECIMAL_KEY           DEF_ENABLED             // Enable label A-F

#define LABEL_LANGUAGE_DEF(X_LABEL)  \
    X_LABEL( LBL_FIRMWARE_UPGRADE,       "Firmware Upgrade",                            "Mise \x85 Niveau Logiciel"                     ) \
    X_LABEL( LBL_SKIN_LOADING,           "Skin Loading        %%",                      "Th\x82matique          %%"                     ) \
    X_LABEL( LBL_SETTINGS,               "Settings",                                    "Configurations"                                ) \
    X_LABEL( LBL_RANGE,                  "Ranges",                                      "Plages"                                        ) \
    X_LABEL( LBL_LIMITS,                 "Limits",                                      "Limites"                                       ) \
    X_LABEL( LBL_WARNINGS,               "Warnings",                                    "Alertes"                                       ) \
    X_LABEL( LBL_MEASURE,                "Measure",                                     "Mesure"                                        ) \
    X_LABEL( LBL_REFERENCE,              "Reference",                                   "R\x82" "ference"                               ) \
    X_LABEL( LBL_FUNCTIONS,              "Functions",                                   "Fonctions"                                     ) \
    X_LABEL( LBL_MODULES,                "Modules",                                     nullptr                                         ) \
    X_LABEL( LBL_AXIS_MM_BACK,           "-888.888",                                    nullptr                                         ) \
    X_LABEL( LBL_AXIS_INCH_BACK,         "-88.8888",                                    nullptr                                         ) \
    X_LABEL( LBL_DEGREE_BACK,            "-888.88",                                     nullptr                                         ) \
    X_LABEL( LBL_AXIS_X,                 "Axis X",                                      "Axe X"                                         ) \
    X_LABEL( LBL_AXIS_Y,                 "Axis Y",                                      "Axe Y"                                         ) \
    X_LABEL( LBL_AXIS_Z,                 "Axis Z",                                      "Axe Z"                                         ) \
    X_LABEL( LBL_AXIS_A,                 "Axis A",                                      "Axe A"                                         ) \
    X_LABEL( LBL_AXIS_B,                 "Axis B",                                      "Axe B"                                         ) \
    X_LABEL( LBL_AXIS_C,                 "Axis C",                                      "Axe C"                                         ) \
    X_LABEL( LBL_SPINDLE,                "Spindle",                                     "Broche"                                        ) \
    X_LABEL( LBL_ENABLE,                 "Enabled",                                     "Activ\x82"                                     ) \
    X_LABEL( LBL_COOLANT,                "Coolant",                                     "Coolant"                                       ) \
    X_LABEL( LBL_MIST,                   "Mist",                                        "Mist"                                          ) \
    X_LABEL( LBL_SPEED_BACK,             "88888",                                       nullptr                                         ) \
    X_LABEL( LBL_SPEED,                  "%5ld",                                        nullptr                                         ) \
    X_LABEL( LBL_SPEED_FLOAT,            "%5.0f",                                       nullptr                                         ) \
    X_LABEL( LBL_MIN_SPINDLE,            "Minimum Spindle Speed",                       "Vitesse Minimale de la broche"                 ) \
    X_LABEL( LBL_MAX_SPINDLE,            "Maximum Spindle Speed",                       "Vitesse Maximale de la broche"                 ) \
    X_LABEL( LBL_RPM,                    "RPM",                                         nullptr                                         ) \
    X_LABEL( LBL_DRO,                    "DRO",                                         nullptr                                         ) \
    X_LABEL( LBL_TEST,                   "the quick brown fox jumps over the lazy dog", "portez ce vieux whisky au juge blond qui fume" ) \
    X_LABEL( LBL_MAC_SELECT,             "Machine Configuration",                       "Configuration Machine"                         ) \
    X_LABEL( LBL_XY_MAC,                 "XY",                                          nullptr                                         ) \
    X_LABEL( LBL_ZX_MAC,                 "ZX",                                          nullptr                                         ) \
    X_LABEL( LBL_XYZ_MAC,                "XYZ",                                         nullptr                                         ) \
    X_LABEL( LBL_INPUT_CHECK,            "Diagnostic Input",                            "Diagnostique Entr\x82" "e"                     ) \
    X_LABEL( LBL_LIMIT_X,                "Input Limit X",                               "Entr\x82" "e Limite X"                         ) \
    X_LABEL( LBL_LIMIT_Y,                "Input Limit Y",                               "Entr\x82" "e Limite Y"                         ) \
    X_LABEL( LBL_LIMIT_Z,                "Input Limit Z",                               "Entr\x82" "e Limite Z"                         ) \
    X_LABEL( LBL_LIMIT_A,                "Input Limit A",                               "Entr\x82" "e Limite A"                         ) \
    X_LABEL( LBL_LIMIT_B,                "Input Limit B",                               "Entr\x82" "e Limite B"                         ) \
    X_LABEL( LBL_LIMIT_C,                "Input Limit C",                               "Entr\x82" "e Limite C"                         ) \
    X_LABEL( LBL_INPUT_ABORT,            "Input Abort",                                 "Entr\x82" "e Avorter"                          ) \
    X_LABEL( LBL_INPUT_RESUME,           "Input Resume",                                "Entr\x82" "e Resumer"                          ) \
    X_LABEL( LBL_INPUT_HOLD,             "Input Hold",                                  "Entr\x82" "e Maintenir"                        ) \
    X_LABEL( LBL_INPUT_PROBE,            "Input Probe",                                 "Entr\x82" "e Sonde"                            ) \
    X_LABEL( LBL_PAGE_INFO,              "Software Information",                        "Information Logiciel"                          ) \
    X_LABEL( LBL_INFO,                   "Information",                                 nullptr                                         ) \
    X_LABEL( LBL_DIAG,                   "Diagnostic",                                  "Diagnostique"                                  ) \
    X_LABEL( LBL_CNC,                    "G90 G54 G00 X-31.500 Y32.500 S1600 M03",      nullptr                                         ) \
    X_LABEL( LBL_TEST_1,                 "Float",                                       "Flottant"                                      ) \
    X_LABEL( LBL_TEST_2,                 "Decimal",                                     "D\x82" "cimale"                                ) \
    X_LABEL( LBL_TEST_3,                 "Hexa",                                        nullptr                                         ) \
    X_LABEL( LBL_TEST_4,                 "String",                                      nullptr                                         ) \
    X_LABEL( LBL_INPUT_DOOR,             "Input Safety Door",                           "Entr\x82" "e Porte de Suret\x82"               ) \
    X_LABEL( LBL_INPUT_FEED,             "Input Feed",                                  "Entr\x82" "e Feed"                             ) \
    X_LABEL( LBL_INPUT_START,            "Input Start",                                 "Entr\x82" "e D\x82marrer"                      ) \
    X_LABEL( LBL_INPUT_RESET,            "Input Reset",                                 "Entr\x82" "e Remise \x85 z\x82ro"              ) \
    X_LABEL( LBL_INPUT_E_STOP,           "Input E-Stop",                                "Entr\x82" "e E-Stop"                           ) \

//-------------------------------------------------------------------------------------------------


//    X_LABEL(    LBL_AXIS,                   ASCII_DEVICE_CTRL2 "%3d.%04d",     ASCII_DEVICE_CTRL2 "%3d.%04d"        )
