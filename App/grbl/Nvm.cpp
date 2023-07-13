/*
  Nvm.c - Non-volatile-memory interface.
  Part of Grbl-Advanced

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2019-2020 Patrick F.

  Grbl-Advanced is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl-Advanced is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl-Advanced.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Nvm.h"
#include "Config.h"
#include "lib_digini.h"


void Nvm_Initialize(void)
{
  #ifdef USE_EXT_EEPROM
    // M24C0X_Initialize();  nothing to do with Digini
  #endif
}

// TODO maybe replace this by smarter method using database
uint8_t Nvm_ReadByte(uint16_t Address)
{
  #ifdef USE_EXT_EEPROM
    uint8_t Data;

    myGBRL_NVM.Read(uint32_t(Address), &Data, 1);
    return Data;
    //   return M24C0X_ReadByte(Address);
  #else
    return  0;
  #endif
}

void Nvm_WriteByte(uint16_t Address, uint8_t Data)
{
#ifdef USE_EXT_EEPROM
    myGBRL_NVM.Write(uint32_t(Address), &Data, 1);
//    M24C0X_WriteByte(Address, Data);
#else
   // TODO AR  EE_WriteByte(Address, Data);
#endif
}

uint8_t Nvm_Read(uint8_t *DataOut, uint16_t Address, uint16_t size)
{
#ifdef USE_EXT_EEPROM
    myGBRL_NVM.Read(uint32_t(Address), &DataOut, size);
    return size;// M24C0X_ReadByteArray(Address, DataOut, size);
#else
   return  0;
#endif
}

uint8_t Nvm_Write(uint16_t Address, uint8_t *DataIn, uint16_t size)
{
#ifdef USE_EXT_EEPROM
    myGBRL_NVM.Write(uint32_t(Address), &DataIn, size);
    return size; //M24C0X_WriteByteArray(Address, DataIn, size);
#else
   // TODO AR  EE_WriteByteArray(Address, DataIn, size);
    return 0;
#endif
}

void Nvm_Update(void)
{
#ifdef USE_EXT_EEPROM
    // Do nothing
#else
   // TODO AR  EE_Program();
#endif
}
