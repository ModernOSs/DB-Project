#ifndef _MEDRANK_H_
#define _MEDRANK_H_

bool readFromQuery();

void MEDRANK(double (&projectData)[50][784], bool & isLow, BTree *bTree);

#endif