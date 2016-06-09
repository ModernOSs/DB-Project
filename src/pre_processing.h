#ifndef _PRE_PROCESSING_H_
#define _PRE_PROCESSING_H_

class BTree;

struct projectNode { int imageNum; double length; };

void checkEndian();
bool readFromDataset();
void boxMuller(double* data, int count);
void normalize(double* data, int count);
void geneRandProjVects();
void projection();
void sortVector();
void bulkLoading();

void preProcessing();

#endif