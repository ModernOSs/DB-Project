#include "headers.h"

double queryVector[100][784];                   // querySet
double projectQueryVector[50][100];             // after project

bool readFromQuery() {
    return 1;
}

void projection(double (&projectData)[50][784]) {
    printf("Projecting queryset to vectors...\n");
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 50; j++) {
            projectQueryVector[j][i] = 0;
            for (int k = 0; k < 784; k++)
                projectQueryVector[j][i] += queryVector[i][k] * projectData[j][k];
        }
}

void MEDRANK(double (&projectData)[50][784], bool & isLow) {
    clock_t start = clock();
    printf("======================= MEDRENK ========================\n");
    if (readFromQuery() == 0) {
        printf("Dataset is not open correctly\n");
        exit(1);
    }

    // projection(projectData);
    // for (int j = 0; j < 50; j++)
    //     for (int i = 0; i < 100; i++) {
    //         int imageNum = search(projectVector[j][i]);
    //     }

    double end = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    printf("\nMEDRENK takes %lf seconds.\n", end);
}
