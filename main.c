#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdint.h>

#include "BitmapReader.h"
#include "benchmark.h"


uint8_t * convert_to_bayer(const int w,const int h,uint8_t * bmp){
    int i,j;
    int bgSelector[] = {2,1};
    int rgSelector[] = {1,0};
    int * selector = bgSelector;
    int * nextSelector = rgSelector;
    int * tmpSelector;
    uint8_t* bayer = calloc(h*w*3,1);

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            bayer[(i*w+j)*3 + selector[j&1]] = bmp[(i*w+j)*3 + selector[j&1]];
        }
        tmpSelector = selector;
        selector = nextSelector;
        nextSelector = tmpSelector;
    }

    return bayer;
}

uint8_t * bayer_to_ycbcr(const int w,const int h,uint8_t * bayer){

}


int main() {

    BITMAPINFOHEADER bmpInfoHeader;
    unsigned char* bitmapData;

    char picture[] = "apple.bmp";

    bitmapData = LoadBitmapFile(picture, &bmpInfoHeader);

    if(bitmapData == NULL){
        printf("Error reading bitmap file: %s\n", picture);
        return -1;
    }

    int height = bmpInfoHeader.biHeight;
    int width = bmpInfoHeader.biWidth;

    uint8_t * bayer;
    uint8_t * test;
    uint8_t * testSet = calloc(10000*10000*3,1);
    double before = get_time();
    bayer = convert_to_bayer(width, height, bitmapData);
    test = convert_to_bayer(10000,10000,testSet);
    double after = get_time();
    free(test);


    printf("Time Elapsed: %f \n",after-before);

    writeBMP("apple_bayer.bmp",width,height,bayer);

    free(bayer);
    return 0;
}