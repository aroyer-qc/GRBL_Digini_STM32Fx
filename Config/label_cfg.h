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
// Typedef(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define LABEL_LANGUAGE_DEF(X_LABEL)  \
    X_LABEL(    LBL_OK,                     "Ok",                              "Ok"                                 ) \
    X_LABEL(    LBL_OUTPUT,                 "Output",                          "Sortie"                             ) \
    X_LABEL(    LBL_CONTROLS,               "Controls",                        "Contrôles"                          ) \
    X_LABEL(    LBL_SYSTEM_SETUP,           "System Setup",                    "Configuration Système"              ) \
    X_LABEL(    LBL_DIAGNOSTIC,             "Diagnostic",                      "Diagnostique"                       ) \
    X_LABEL(    LBL_FIRMWARE_UPGRADE,       "Firmware Upgrade",                "Mise à Niveau Logiciel"             ) \
    X_LABEL(    LBL_HISTORY_LOG,            "History Log",                     "Historique"                         ) \
    X_LABEL(    LBL_SYSTEM_INFO,            "System Info",                     "Info Système"                       ) \
    X_LABEL(    LBL_SKIN_LOADING,           "Skin Loading        %%",          "Chargement du thèmes        %%"     ) \
    X_LABEL(    LBL_TIME,                   "%2d:%02d:%02d",                   "%2d:%02d:%02d"                      ) \
    X_LABEL(    LBL_DATE,                   "%2d %02d %04d",                   "%2d %02d %04d"                      ) \
    X_LABEL(    LBL_PERCENT,                "%2d",                             "%2d"                                ) \
    X_LABEL(    LBL_STRING,                 "%s",                              "%s"                                 ) \
    X_LABEL(    LBL_0,                      "0",                               "0"                                  ) \
    X_LABEL(    LBL_1,                      "1",                               "1"                                  ) \
    X_LABEL(    LBL_2,                      "2",                               "2"                                  ) \
    X_LABEL(    LBL_3,                      "3",                               "3"                                  ) \
    X_LABEL(    LBL_4,                      "4",                               "4"                                  ) \
    X_LABEL(    LBL_5,                      "5",                               "5"                                  ) \
    X_LABEL(    LBL_6,                      "6",                               "6"                                  ) \
    X_LABEL(    LBL_7,                      "7",                               "7"                                  ) \
    X_LABEL(    LBL_8,                      "8",                               "8"                                  ) \
    X_LABEL(    LBL_9,                      "9",                               "9"                                  ) \
    X_LABEL(    LBL_A,                      "A",                               "A"                                  ) \
    X_LABEL(    LBL_B,                      "B",                               "B"                                  ) \
    X_LABEL(    LBL_C,                      "C",                               "C"                                  ) \
    X_LABEL(    LBL_D,                      "D",                               "D"                                  ) \
    X_LABEL(    LBL_E,                      "E",                               "E"                                  ) \
    X_LABEL(    LBL_F,                      "F",                               "F"                                  ) \
    X_LABEL(    LBL_DOT,                    ".",                               "."                                  ) \
    X_LABEL(    LBL_DEL,                    "Del",                             "Del"                                ) \
    X_LABEL(    LBL_SIGN,                   "-",                               "-"                                  ) \
    X_LABEL(    LBL_TOGGLE_SIGN,            "+/-",                             "+/-"                                ) \
    X_LABEL(    LBL_SETTINGS,               "Settings",                        "Configurations "                    ) \
    X_LABEL(    LBL_RANGE,                  "Ranges",                          "Plages"                             ) \
    X_LABEL(    LBL_LIMITS,                 "Limits",                          "Limites"                            ) \
    X_LABEL(    LBL_WARNINGS,               "Warnings",                        "Avertissements"                     ) \
    X_LABEL(    LBL_MEASURE,                "Measure",                         "Mesure"                             ) \
    X_LABEL(    LBL_REFERENCE,              "Reference",                       "Réference"                          ) \
    X_LABEL(    LBL_FUNCTIONS,              "Functions",                       "Fonctions"                          ) \
    X_LABEL(    LBL_MODULES,                "Modules",                         "Modules"                            ) \
    X_LABEL(    LBL_AXIS_MM_BACK,           "-888.888",                        "-888.888"                           ) \
    X_LABEL(    LBL_AXIS_INCH_BACK,         "-88.8888",                        "-88.8888"                           ) \
    X_LABEL(    LBL_DEGREE_BACK,            "-888.88",                         "-888.88"                            ) \
    X_LABEL(    LBL_AXIS_X,                 "Axis X",                          "Axe X"                              ) \
    X_LABEL(    LBL_AXIS_Y,                 "Axis Y",                          "Axe Y"                              ) \
    X_LABEL(    LBL_AXIS_Z,                 "Axis Z",                          "Axe Z"                              ) \
    X_LABEL(    LBL_AXIS_A,                 "Axis A",                          "Axe A"                              ) \
    X_LABEL(    LBL_AXIS_B,                 "Axis B",                          "Axe B"                              ) \
    X_LABEL(    LBL_AXIS_C,                 "Axis C",                          "Axe C"                              ) \
    X_LABEL(    LBL_SPINDLE,                "Spindle",                         "Broche"                             ) \
    X_LABEL(    LBL_ENABLE,                 "Enabled",                         "Activé"                             ) \
    X_LABEL(    LBL_COOLANT,                "Coolant",                         "Coolant"                            ) \
    X_LABEL(    LBL_MIST,                   "Mist",                            "Mist"                               ) \
    X_LABEL(    LBL_SPEED_BACK,             "88888",                           "88888"                              ) \
    X_LABEL(    LBL_SPEED,                  "%5ld",                            "%5ld"                               ) \
    X_LABEL(    LBL_SPEED_FLOAT,            "%5.0f",                           "%5.0f"                              ) \
    X_LABEL(    LBL_MIN_SPINDLE,            "Minimum Spindle Speed",           "Vitesse Minimal de la broche"       ) \
    X_LABEL(    LBL_MAX_SPINDLE,            "Maximum Spindle Speed",           "Vitesse Maximal de la broche"       ) \
    X_LABEL(    LBL_RPM,                    "RPM",                             "RPM"                                ) \
    X_LABEL(    LBL_DRO,                    "DRO",                             "DRO"                                ) \
    X_LABEL(    LBL_TEST,                   "the quick brown fox jumps over the lazy dog",             "portez ce vieux whisky au juge blond qui fume"          ) \
    X_LABEL(    LBL_MAIN_MENU,              "Main Menu",                       "Menu Principal"                     ) \
    X_LABEL(    LBL_MAC_SELECT,             "Machine Configuration",           "Configuration Machine"              ) \
    X_LABEL(    LBL_XY_MAC,                 "XY",                              "XY"                                 ) \
    X_LABEL(    LBL_ZX_MAC,                 "ZX",                              "ZX"                                 ) \
    X_LABEL(    LBL_XYZ_MAC,                "XYZ",                             "XYZ"                                ) \
    X_LABEL(    LBL_INPUT_CHECK,            "Diagnostic Input",                "Diagnostique Entrée"                ) \
    X_LABEL(    LBL_LIMIT_X,                "Input Limit X",                   "Entrée Limite X"                    ) \
    X_LABEL(    LBL_LIMIT_Y,                "Input Limit Y",                   "Entrée Limite Y"                    ) \
    X_LABEL(    LBL_LIMIT_Z,                "Input Limit Z",                   "Entrée Limite Z"                    ) \
    X_LABEL(    LBL_LIMIT_A,                "Input Limit A",                   "Entrée Limite A"                    ) \
    X_LABEL(    LBL_LIMIT_B,                "Input Limit B",                   "Entrée Limite B"                    ) \
    X_LABEL(    LBL_LIMIT_C,                "Input Limit C",                   "Entrée Limite C"                    ) \
    X_LABEL(    LBL_INPUT_ABORT,            "Input Abort",                     "Entrée Avorter"                     ) \
    X_LABEL(    LBL_INPUT_RESUME,           "Input Resume",                    "Entrée Resumer"                     ) \
    X_LABEL(    LBL_INPUT_HOLD,             "Input Hold",                      "Entrée Maintenir"                   ) \
    X_LABEL(    LBL_INPUT_PROBE,            "Input Probe",                     "Entrée Sonde"                       ) \
    X_LABEL(    LBL_PAGE_INFO,              "Software Information",            "Information Logiciel"               ) \
    X_LABEL(    LBL_INFO,                   "Information",                     "Information"                        ) \
    X_LABEL(    LBL_FW_NAME_INFO,           "Name:",                           "Nom:"                               ) \
    X_LABEL(    LBL_FW_VERSION_INFO,        "Version:",                        "Version:"                           ) \
    X_LABEL(    LBL_GUI_NAME_INFO,          "GUI Name:",                       "Nom GUI:"                           ) \
    X_LABEL(    LBL_GUI_VERSION_INFO,       "GUI Version:",                    "Version GUI:"                       ) \
    X_LABEL(    LBL_HARDWARE_INFO,          "Hardware:",                       "Matériel:"                          ) \
    X_LABEL(    LBL_SERIAL_INFO,            "Serial No.:",                     "No. Série:"                         ) \
    X_LABEL(    LBL_COMPILE_DATE_INFO,      "Built Date:",                     "Date logiciel:"                     ) \
    X_LABEL(    LBL_DIAG,                   "Diagnostic",                      "Diagnostique"                       ) \
    X_LABEL(    LBL_CNC,                    "G90 G54 G00 X-31.500 Y32.500 S1600 M03", " " )\
    X_LABEL(    LBL_TEST_1,                 "Float", " " )\
    X_LABEL(    LBL_TEST_2,                 "Decimal", " " )\
    X_LABEL(    LBL_TEST_3,                 "Hexa", " " )\
    X_LABEL(    LBL_TEST_4,                 "String", " " )\

//    X_LABEL(    LBL_INPUT_DOOR,             "Input Safety Door",               "Entrée Porte de Sureté"             )
//    X_LABEL(    LBL_INPUT_FEED,             "Input Feed",                      "Entrée ???"                         )
//    X_LABEL(    LBL_INPUT_START,            "Input Start",                     "Entrée Démarrer"                    )
//    X_LABEL(    LBL_INPUT_RESET,            "Input Reset",                     "Entrée Reset"                       )
//    X_LABEL(    LBL_INPUT_E_STOP,           "Input E-Stop",                    "Entrée E-Stop"                      )  // if wired on IO instead of RST pin

//-------------------------------------------------------------------------------------------------


//    X_LABEL(    LBL_AXIS,                   ASCII_DEVICE_CTRL2 "%3d.%04d",     ASCII_DEVICE_CTRL2 "%3d.%04d"        )
