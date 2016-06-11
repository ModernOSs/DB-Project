#ifndef _MEDRANK_H_
#define _MEDRANK_H_

bool readFromQuery();

void MEDRANK(double (&projectData)[50][784], bool & isLow, BTree *bTree, int (*images)[784]);
void findNearest(int (*images)[784]);
void MEDRANK_nearest(int (*images)[784], int ImageNum, int Counter);
double Ratio();

#endif