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


uint8_t * bayer_to_ycbcr(int w,int h,uint8_t * bayer){
    int i,j;

    uint8_t *ycbcr = calloc((w-1) * (h-1) * 3,1);
    for(i=0;i<h-1;i++){
        for(j=0;j<w-1;j++){
            int r,g,b;
            r = (int) bayer[(((i+(1-i&1))) * w + j + (1-(j&1))) * 3 + 0];
            g = (int) bayer[((i * w + j + 1 - ((j&1) ^ (i&1))) ) * 3 + 1];
            b = (int) bayer[(((i + (i&1))) * w + j + (j&1)) * 3 + 2];

            //y
            ycbcr[(i*(w-1) + j)*3 + 0] = (uint8_t) (r*0.183+g*0.614+b*0.062)+16;
            //cb
            ycbcr[(i*(w-1)+ j)*3 + 1] = (uint8_t) (r*-0.101+g*-0.338+b*0.439)+128;
            //cr
            ycbcr[(i*(w-1)+ j)*3 + 2] = (uint8_t) (r*0.439+g*-0.399+b*-0.040)+128;
        }
    }
    return ycbcr;
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
    uint8_t * yc;
    double before = get_time();
    bayer = convert_to_bayer(width, height, bitmapData);
    yc = bayer_to_ycbcr(width, height, bayer);
    double after = get_time();

    printf("Time Elapsed: %f \n",after-before);

    writeBMP("apple_yc.bmp",width-1,height-1,yc);

    free(bayer);
    free(yc);
    return 0;
}