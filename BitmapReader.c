//
// Created by Joseph on 2/22/2019.
//

#include "BitmapReader.h"

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    uint8_t *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    uint8_t tempRGB;  //our swap variable
    int i,width,height;


    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    width = bitmapInfoHeader->biWidth;
    height = bitmapInfoHeader->biHeight;

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = malloc(width*height*3 * sizeof(uint16_t));

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    uint8_t *bmpIndex = bitmapImage + width * (height - 1) * 3;
    uint8_t rowBuffer[(width*3 + 3) & (~3)];
    //read in the bitmap image data
    for(i=0;i<height;i++) {
        fread(rowBuffer, 1, (width*3 + 3) & (~3), filePtr);
        memcpy(bmpIndex,rowBuffer,width*3);
        bmpIndex -= width*3;
    }
    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //change from BGR to RGB pixel ordering
    for(imageIdx = 0;imageIdx<width*height;imageIdx++){
        tempRGB = bitmapImage[imageIdx * 3];
        bitmapImage[imageIdx * 3] = bitmapImage[imageIdx *3 +2];
        bitmapImage[imageIdx * 3 + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

void writeBMP(char* filename,int w,int h,uint8_t* img){
    FILE *f;
    int i,j;
    uint8_t tempRGB;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

    /*
    for(i=0; i<h; i++){
        for(j=0; j<w; j++){
            img[(j+i*w)*3+2] = red[i*w+j];
            img[(j+i*w)*3+1] = green[i*w+j];
            img[(j+i*w)*3+0] = blue[i*w+j];;
        }
    }
     */

    //change from RGB to BGR pixel ordering
    for(i = 0;i<w*h;i++){
        tempRGB = img[i* 3];
        img[i* 3] = img[i*3 +2];
        img[i* 3 + 2] = tempRGB;
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen(filename,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),1,3*w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }
    free(img);
    fclose(f);
}
