#include "headers.h"

bool isLow;

void checkEndian() {  
    short int test = 0x1234;
    if (*((char *)&test) == 0x12)
        isLow = 0;  //  high endian
    else
        isLow = 1;  //  low endian
}

// turn high-endian data to low-endian data
#define high2Low(data) ((((uint32_t)(data) & 0xff000000) >> 24) | \
                        (((uint32_t)(data) & 0x00ff0000) >> 8 ) | \
                        (((uint32_t)(data) & 0x0000ff00) << 8 ) | \
                        (((uint32_t)(data) & 0x000000ff) << 24))

bool readFromDataset() {
    checkEndian();
    FILE *file;
    file = fopen("data/Mnist.ds", "rb");
    if (file == NULL)
        return 0;  // dataset is not open correctly
    
    int *temp;
    temp = (int*)malloc(sizeof(int));
    // magic number, number of images, number of rows, number of columns
    for (int i = 0; i < 4; i++) {
        fread(temp, 4, 1, file);
        if (isLow) *temp = high2Low(*temp);
        printf("%d\n", *temp);
    }
    free (temp);

    fclose (file);
    return 1;
}
