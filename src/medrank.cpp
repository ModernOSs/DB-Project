#include "headers.h"

double queryVector[100][784];                   // querySet
double projectQueryVector[100][50];             // after project
double nearest[100];
double realNearest[100];

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
            projectQueryVector[i][j] = 0;
            for (int k = 0; k < 784; k++) {
                projectQueryVector[i][j] += queryVector[i][k] * projectData[j][k];
            }
        }
    }
}


// find the real nearest number
// images[60000][784], query[50][784]
void findNearest(int (*images)[784]) {
    printf("Finding the real nearest numbers...\n");
    double currentLength = 0;
    double minLength;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 60000; j++) {
            for (int k = 0; k < 784; k++) {
                currentLength += (images[j][k] - queryVector[i][k])
                               * (images[j][k] - queryVector[i][k]);
            }
            currentLength = sqrt(currentLength);
            if (j == 0) 
                minLength = currentLength;
            if (currentLength < minLength) {
                minLength = currentLength;
            }
            currentLength = 0;
        }
        realNearest[i] = minLength;
    }
}

// Counter < 100, ImageNum < 60000
double tempLength;
void MEDRANK_nearest(int (*images)[784], int ImageNum, int Counter) {
    tempLength = 0;
    for (int i = 0; i < 784; i++) {
        tempLength += (images[ImageNum][i] - queryVector[Counter][i])
                     *(images[ImageNum][i] - queryVector[Counter][i]);
    }
    nearest[Counter] = sqrt(tempLength);
}

double getRatio() {
    double ratio = 0;
    for (int i = 0; i < 100; i++) {
        ratio += nearest[i] / realNearest[i];
    }
    ratio = ratio / 100;
    return ratio;
}

void MEDRANK(double (&projectData)[50][784], bool & isLow, BTree *bTree, int (*images)[784]) {
    printf("======================= MEDRENK ========================\n");
    if (readFromQuery(isLow) == 0) {
        printf("Dataset is not open correctly\n");
        exit(1);
    }

    findNearest(images);

    bool exist = false;
    int result[60000];
    for (int i = 0; i < 60000; i++)
        result[i] = 0;
    int temp = -1;
    int max_counter = 0;
    int max_image_number = -1;

    projection(projectData);

    for (int t = 0; t < 50; t++)
        bTree[t].resetSearch();

    double sum_IO = 0;
    double sum_time = 0;

    printf("Top search results: \n");
    for (int i = 0; i < 100; i++) {
        clock_t start = clock();
        printf("Voting for query number %d...\n", i + 1);
        while (max_counter <= 25) {
            for (int j = 0; j < 50; j++) {
                temp = bTree[j].searchNextImage(projectQueryVector[i][j]);
                result[temp]++;
                if (result[temp] > max_counter) {
                    max_counter = result[temp];
                    max_image_number = temp;
                }
                if (max_counter > 25)
                    break;
            }
        }

        printf("[Image number: %d]\n", max_image_number);

        MEDRANK_nearest(images, max_image_number, i);
        printf("[ratio: %lf]\n", nearest[i] / realNearest[i]);

        int IOCount = 0;
        for (int t = 0; t < 50; t++) {
            IOCount += bTree[t].num_of_visits_;
            bTree[t].resetSearch();
        }
        printf("[I/O cost: %d pages]\n", IOCount);
        sum_IO += IOCount;

        double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
        printf("[Running time: %lf seconds]\n", end);
        sum_time += end;

        exist = false;
        max_counter = 0;
        max_image_number = -1;
        temp = -1;
        for (int i = 0; i < 60000; i++)
            result[i] = 0;
    }

    double ratio = getRatio();
    printf("\n[MEDRANK's Average Ratio: %lf]\n", ratio);
    printf("[MEDRANK's Average I/O cost: %lf]\n", sum_IO / 100);
    printf("[MEDRANK's Average Running Time cost: %lf]\n", sum_time / 100);
}