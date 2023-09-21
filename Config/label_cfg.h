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

#define     LABEL_USE_TIME_AND_DATE             DEF_ENABLED
#define     LABEL_USE_PRODUCT_INFO              DEF_ENABLED

#define LABEL_LANGUAGE_DEF(X_LABEL)  \
    X_LABEL( LBL_OK,                     "Ok",                                          nullptr                                         ) \
    X_LABEL( LBL_OUTPUT,                 "Output",                                      "Sortie"                                        ) \
    X_LABEL( LBL_CONTROLS,               "Controls",                                    "Contr\x93les"                                  ) \
    X_LABEL( LBL_SYSTEM_SETUP,           "System Setup",                                "Configuration Syst\x8ame"                      ) \
    X_LABEL( LBL_DIAGNOSTIC,             "Diagnostic",                                  "Diagnostique"                                  ) \
    X_LABEL( LBL_FIRMWARE_UPGRADE,       "Firmware Upgrade",                            "Mise \x85 Niveau Logiciel"                     ) \
    X_LABEL( LBL_HISTORY_LOG,            "History Log",                                 "Historique"                                    ) \
    X_LABEL( LBL_SYSTEM_INFO,            "System Info",                                 "Info Syst\x8ame"                               ) \
    X_LABEL( LBL_SKIN_LOADING,           "Skin Loading        %%",                      "Th\x82matique          %%"                     ) \
    X_LABEL( LBL_TIME,                   "%2d:%02d:%02d",                               nullptr                                         ) \
    X_LABEL( LBL_DATE,                   "%2d %02d %04d",                               nullptr                                         ) \
    X_LABEL( LBL_PERCENT,                "%2d",                                         nullptr                                         ) \
    X_LABEL( LBL_0,                      "0",                                           nullptr                                         ) \
    X_LABEL( LBL_1,                      "1",                                           nullptr                                         ) \
    X_LABEL( LBL_2,                      "2",                                           nullptr                                         ) \
    X_LABEL( LBL_3,                      "3",                                           nullptr                                         ) \
    X_LABEL( LBL_4,                      "4",                                           nullptr                                         ) \
    X_LABEL( LBL_5,                      "5",                                           nullptr                                         ) \
    X_LABEL( LBL_6,                      "6",                                           nullptr                                         ) \
    X_LABEL( LBL_7,                      "7",                                           nullptr                                         ) \
    X_LABEL( LBL_8,                      "8",                                           nullptr                                         ) \
    X_LABEL( LBL_9,                      "9",                                           nullptr                                         ) \
    X_LABEL( LBL_A,                      "A",                                           nullptr                                         ) \
    X_LABEL( LBL_B,                      "B",                                           nullptr                                         ) \
    X_LABEL( LBL_C,                      "C",                                           nullptr                                         ) \
    X_LABEL( LBL_D,                      "D",                                           nullptr                                         ) \
    X_LABEL( LBL_E,                      "E",                                           nullptr                                         ) \
    X_LABEL( LBL_F,                      "F",                                           nullptr                                         ) \
    X_LABEL( LBL_DOT,                    ".",                                           nullptr                                         ) \
    X_LABEL( LBL_DEL,                    "Del",                                         nullptr                                         ) \
    X_LABEL( LBL_SIGN,                   "-",                                           nullptr                                         ) \
    X_LABEL( LBL_TOGGLE_SIGN,            "+/-",                                         nullptr                                         ) \
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
    X_LABEL( LBL_MAIN_MENU,              "Main Menu",                                   "Menu Principal"                                ) \
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
    /* Network label ( shared with VT100 also)*/ \
    X_LABEL( LBL_NETWORK_INFO,           "Network Information",                         "Information R\x82seau"                         ) \
    X_LABEL( LBL_IP_ADDR,                "IPV4 Address . . . :",                        "Addresse IPV4  . . :"                          ) \
    X_LABEL( LBL_IP_MASK,                "Subnet Masks . . . :",                        "Masque Sous-R\x82seau :"                       ) \
    X_LABEL( LBL_IP_GATEWAY,             "Default Gateway  . :",                        "Passerelle D\x82" "faut  :"                    ) \
    X_LABEL( LBL_IP_DNS,                 "DNS Servers  . . . :",                        "Serveurs DNS . . . :"                          ) \
    X_LABEL( LBL_IP_DHCP_STATE,          "DHCP State . . . . :",                        "Status DHCP  . . . :"                          ) \
    X_LABEL( LBL_IP_LINK_STATE,          "Link State . . . . :",                        "Status Lien  . . . :"                          ) \
    X_LABEL( LBL_IP_LINK_SPEED,          "Link Speed . . . . :",                        "Vitesse du Lien  . :"                          ) \
    X_LABEL( LBL_MAC_ADDRESS,            "MAC Address  . . . :",                        "Addresse MAC . . . :"                          ) \
    X_LABEL( LBL_MAC_ADDRESS_VALUE,      "%02X:%02X:%02X:%02X:%02X:%02X",               nullptr                                         ) \


//-------------------------------------------------------------------------------------------------


//    X_LABEL(    LBL_AXIS,                   ASCII_DEVICE_CTRL2 "%3d.%04d",     ASCII_DEVICE_CTRL2 "%3d.%04d"        )
