#include "headers.h"

bool isLow;

void checkEndian() {
    short int test = 0x1234;
    if (*((char *)&test) == 0x12)
        isLow = 0;  // high endian
    else
        isLow = 1;  // low endian
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
    free(temp);

    fclose (file);
    return 1;
}

// standard normal distribution N(0, 1)
void boxMuller(double* data, int count) {
    static const double epsilon = numeric_limits<double>::min();
    static const double twopi = double(2.0 * 3.14159265358979323846);
    double u1;
    double u2;

    for (int i = 0; i < count; i += 2) {
        do {
            u1 = rand() * (1.0 / RAND_MAX);
            u2 = rand() * (1.0 / RAND_MAX);
        }
        while (u1 <= epsilon);
        double radius = sqrt(-2 * log(u1));
        double theta = twopi * u2;
        data[i] = radius * cos(theta);
        data[i + 1] = radius * sin(theta);
    }
}

void normalize(double* data, int count) {
    double total = 0;
    for (int i = 0; i < count; i++)
    {
       total += data[i]*data[i];
    }
    total = sqrt(total);
    for (int i= 0 ; i< count ; i++) {
        data[i] = data[i] / total;
    } 
}

// generate random projection vectors an
void geneRandProjVects() {
    double projectData[50][784];
    double data[784];
    int count = 784;
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < count; j++) {
            boxMuller(data, count);
            normalize(data, count);
            projectData[i][j] = data[j];
            printf("%lf ", data[j]);
        }
        printf("\n");
    }
}

void preProcessing() {
    if (readFromDataset() == 0) {
        printf("dataset is not open correctly\n");
        exit(1);
    }
    geneRandProjVects();
}
