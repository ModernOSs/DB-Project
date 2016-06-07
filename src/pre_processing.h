#ifndef _PRE_PROCESSING_H_
#define _PRE_PROCESSING_H_

void checkEndian();
bool readFromDataset();
void boxMuller(double* data, int count);
void normalize(double* data, int count);
void geneRandProjVects();

void preProcessing();

#endif