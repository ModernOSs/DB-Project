#include "headers.h"

double queryVector[100][784];                      // querySet
double projectQueryVector[100][50];             // after project
struct find{
        int imageNum;
        int counter;
    };

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
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 50; j++) {
            projectQueryVector[i][j] = 0;
            for (int k = 0; k < 784; k++) {
                projectQueryVector[i][j] += queryVector[i][k] * projectData[j][k];
            }
        }
    }
}



int Compare(const void *a, const void *b) {
    return ((struct find *)b)->counter - ((struct find *)a)->counter;
}

void MEDRANK(double (&projectData)[50][784], bool & isLow, BTree *bTree) {
    clock_t start = clock();
    printf("======================= MEDRENK ========================\n");
    if (readFromQuery(isLow) == 0) {
        printf("Dataset is not open correctly\n");
        exit(1);
    }

    int structSize = 0;
    bool exist = false;
    struct find *result = (struct find*)malloc(60000*sizeof(struct find));
    int temp = -1;
    int max_counter = 0;

    projection(projectData);

    for (int t = 0; t < 50; t++)
        bTree[t].resetSearch();

    for (int i = 0; i < 1; i++) {
        printf("Voting for query number %d...\n", i + 1);
        while (max_counter <= 25) {
            for (int j = 0; j < 50; j++) {
                temp = bTree[j].searchNextImage(projectQueryVector[i][j]);
                //printf("%d\t%lf\n", temp, projectQueryVector[i][j]);
                for (int k = 0; k < structSize; k++) {
                    if (result[k].imageNum == temp) {
                        result[k].counter++;
                        if (max_counter < result[k].counter)
                            max_counter = result[k].counter;
                        exist = true;
                        break;
                    }
                }
                if (exist == false) {
                    result[structSize].imageNum = temp;
                    result[structSize].counter = 1;
                    structSize++;
                } else {
                    exist = false;
                }
                if (max_counter > 25)
                    break;
            }
        }
        for (int t = 0; t < 50; t++)
            bTree[t].resetSearch();
        qsort(result, structSize, sizeof(struct find), Compare);
        printf("Top c search results:\n");
        printf("image number\tVoters\n");
        for (int m = 0; m < 1; m++) {
            printf("%d\t\t%d\n", result[m].imageNum, result[m].counter);
        }
        structSize = 0;
        exist = false;
        max_counter = 0;
        temp = -1;
    }
    
    double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    printf("\nMEDRENK takes %lf seconds.\n", end);
}