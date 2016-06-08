#include "headers.h"

BTree::BTree(BNode *root_ptr)       // constructor
{
    root_ptr_ = root_ptr;
}

BTree::~BTree()                     // destructor
{

}

                                    // bulkload a tree from memory
void BTree::bulkLoading(projectNode projectVector[60000])
{
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));
    
    // level 0
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    for (int i = 0; i < nodesInLevel0; i++) {
        BNode *bNode = new BNode;
        double key[vectorsInNode];
        int son[vectorsInNode];
        int n;

        if (i == nodesInLevel0 - 1) n = 60000 - vectorsInNode * (nodesInLevel0 - 1);
        else n = vectorsInNode;
        for (int j = 0; j < n; j++) {
            key[j] = projectVector[i * vectorsInNode + j].length;
            son[j] = projectVector[i * vectorsInNode + j].imageNum;
        }

        bNode->init(0, this, n, key, son);
        bNode->set_left_sibling(i - 1);
        bNode->set_right_sibling(i + 1);
    }

    // level 1
    exit(0);
}
