#include "headers.h"

int queryVector[100][784];                      // querySet
double projectQueryVector[50][100];             // after project

// turn high-endian data to low-endian data
#define high2Low(data) ((((uint32_t)(data) & 0xff000000) >> 24) | \
                        (((uint32_t)(data) & 0x00ff0000) >> 8 ) | \
                        (((uint32_t)(data) & 0x0000ff00) << 8 ) | \
                        (((uint32_t)(data) & 0x000000ff) << 24))

bool readFromQuery(bool isLow) {
    FILE *file;
    printf("Reading from query...\n");
    file = fopen("data/Mnist.q", "rb");
    if (file == NULL)
        return 0;  // dataset is not open correctly

    int *temp;
    temp = (int*)malloc(4);
    // magic number, number of images, number of rows, number of columns
    for (int i = 0; i < 4; i++) {
        fread(temp, 4, 1, file);
        if (isLow) *temp = high2Low(*temp);
        switch (i) {
            case 0: printf("[Magic number: "); break;
            case 1: printf("[Number of images: "); break;
            case 2: printf("[Number of rows: "); break;
            case 3: printf("[Number of columns "); break;
            default: break;
        }
        printf("%d]\n", *temp);
    }

    printf("Reading the first 100 images from query...\n");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 196; j++) {
            fread(temp, 4, 1, file);
            if (isLow) *temp = high2Low(*temp);
            queryVector[i][j * 4 + 3] = (*temp) & 0x000000ff;
            queryVector[i][j * 4 + 2] = ((*temp) & 0x0000ff00) >> 8;
            queryVector[i][j * 4 + 1] = ((*temp) & 0x00ff0000) >> 16;
            queryVector[i][j * 4] = ((*temp) & 0xff000000) >> 24;
        }
    }
    free(temp);

    fclose (file);
    return 1;
}

void projection(double (&projectData)[50][784]) {
    printf("Projecting queryset to vectors...\n");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 50; j++) {
            projectQueryVector[j][i] = 0;
            for (int k = 0; k < 784; k++)
                projectQueryVector[j][i] += queryVector[i][k] * projectData[j][k];
        }
    }
}

void MEDRANK(double (&projectData)[50][784], bool & isLow) {
    clock_t start = clock();
    printf("======================= MEDRENK ========================\n");
    if (readFromQuery(isLow) == 0) {
        printf("Dataset is not open correctly\n");
        exit(1);
    }

    int resultCounter[60000];

    for (int i = 0; i < 60000; i++) {
        resultCounter[i] = 0;
    }

    projection(projectData);
    /*while (true) {
        for (int j = 0; j < 50; j++)
            for (int i = 0; i < 100; i++) {
                int imageNum = search(projectVector[j][i], h[j][i], l[j][i]);
                resultCounter[imageNum] ++;
                if (resultCounter[imageNum] > 25) {
                    break;
                }
            }
    }*/


    double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    printf("\nMEDRENK takes %lf seconds.\n", end);
}
