//
// Created by Joseph on 2/22/2019.
//

#ifndef IMAGECONVERSION_BITMAPREADER_H
#define IMAGECONVERSION_BITMAPREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
void writeBMP(char* filename,int w,int h,uint8_t * img);


#endif //IMAGECONVERSION_BITMAPREADER_H
