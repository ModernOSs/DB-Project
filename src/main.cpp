#include "headers.h"

int main() {
    double projectData[50][784];
    bool isLow;
    preProcessing(projectData, isLow);

    MEDRANK(projectData, isLow);
    
	return 0;
}
