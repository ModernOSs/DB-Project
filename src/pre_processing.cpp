#include "headers.h"

bool isLow;
double rand_temp[50];


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

int (*images)[784];

bool readFromDataset() {
    checkEndian();
    FILE *file;
    printf("Reading from dataset...\n");
    file = fopen("data/Mnist.ds", "rb");
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
    
    printf("Reading images from dataset...\n");
    // 60,000 images, each has 784 bytes
    images = (int((*)[784]))malloc(60000 * 784 * sizeof(int));
    for (int i = 0; i < 60000; i++) {
        for (int j = 0; j < 196; j++) {
            fread(temp, 4, 1, file);
            if (isLow) *temp = high2Low(*temp);
            images[i][j * 4 + 3] = (*temp) & 0x000000ff;
            images[i][j * 4 + 2] = ((*temp) & 0x0000ff00) >> 8;
            images[i][j * 4 + 1] = ((*temp) & 0x00ff0000) >> 16;
            images[i][j * 4] = ((*temp) & 0xff000000) >> 24;
        }
    }
    free(temp);

    fclose (file);
    return 1;
}

// standard normal distribution N(0, 1)
void boxMuller(double* data, int count, int number) {
    static const double epsilon = numeric_limits<double>::min();
    static const double twopi = double(2.0 * 3.14159265358979323846);
    double u1;
    double u2;

    srand(rand_temp[number]);

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
double projectData[50][784];

void geneRandProjVects() {
    double data[784];
    int count = 784;
    srand((int)time(0));
    for (int i = 0; i < 50; i++)
        rand_temp[i] = rand() % RAND_MAX;

    printf("Generating random projection vectors...\n");
    for (int i = 0; i < 50; i++) {
        boxMuller(data, count, i);
        normalize(data, count);
        for (int j = 0; j < count; j++) {
            projectData[i][j] = data[j];
        }
    }
}

projectNode (*projectVector)[60000];

void projection() {
    printf("Projecting images to vectors...\n");
    projectVector = (projectNode((*)[60000]))malloc(50 * 60000 * sizeof(projectNode));

    for (int i = 0; i < 60000; i++)
        for (int j = 0; j < 50; j++) {
            projectVector[j][i].imageNum = i;
            projectVector[j][i].length = 0;
            for (int k = 0; k < 784; k++)
                projectVector[j][i].length += (double)images[i][k] * projectData[j][k];
        }

    // free(images);
}

int compare(const void *a, const void *b) {
    projectNode (*node1)[60000] = (projectNode((*)[60000]))a;
    projectNode (*node2)[60000] = (projectNode((*)[60000]))b;
    if ((**node1).length > (**node2).length)
        return 1;
    else
        return -1;
}

void sortVector() {
    printf("Sorting the vectors...\n");
    for (int i = 0; i < 50; i++)
        qsort(projectVector[i], 60000, sizeof(projectNode), compare);
}

BTree *bTree;

void bulkLoading() {
    printf("Bulk-loading...\n");
    if (access("forest/", 0) == -1) {
        if (mkdir("forest/", 0777)) {
            printf("Creating route failed!\n");
            exit(0);
        }
    }
    clock_t start = clock();
    bTree = new BTree[50];
    for (int i = 0; i < 50; i++) {
        bTree[i].bulkLoading(projectVector[i]);
        BNode *root = NULL;
        BTree *bTree = new BTree(root);
        bTree->bulkLoading(projectVector[i]);
        char fileName[128];
        sprintf(fileName, "forest/tree_%d", i);
        bTree->writeFile(fileName);
        bTree->readFile(fileName);
    }
    double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    int nodesInLevel1 = ceil((double)nodesInLevel0 / (double)vectorsInNode);
    printf("[Index size: %d kB]\n", (nodesInLevel0 + nodesInLevel1 + 1) * 50);
    printf("[Indexing time: %lf seconds]\n", end);

    free(projectVector);
}

void preProcessing() {
    clock_t start = clock();
    printf("==================== Pre-processing ====================\n");
    if (readFromDataset() == 0) {
        printf("Dataset is not open correctly\n");
        exit(1);
    }
    geneRandProjVects();
    projection();
    sortVector();
    bulkLoading();

    double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    printf("\n[Pre-processing time: %lf seconds]\n\n", end);

    MEDRANK(projectData, isLow, bTree, images);
}
