#ifndef _PRE_PROCESSING_H_
#define _PRE_PROCESSING_H_

struct projectNode { int imageNum; double length; };

void checkEndian();
bool readFromDataset();
void boxMuller(double* data, int count);
void normalize(double* data, int count);
void geneRandProjVects();
void projection();
void sortVector();
void bulkLoading();

void preProcessing(double (&projectData_)[50][784], bool & isLow_);

#endif