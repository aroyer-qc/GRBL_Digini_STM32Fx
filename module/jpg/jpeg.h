#ifndef __JPEG_H__
#define __JPEG_H__

#ifdef __cplusplus
extern "C" {
#endif

bool JPEG_Decode(uint8_t* JPG, uint8_t* Decoded, size_t* pImageSize/*, uint16_t xPos, uint16_t yPos*/);

#ifdef __cplusplus
}
#endif

#endif // __JPEG_H__
