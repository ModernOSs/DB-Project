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
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    int nodesInLevel1 = ceil((double)nodesInLevel0 / (double)vectorsInNode);
    BNode *bNode = new BNode[nodesInLevel0 + nodesInLevel1 + 1];
    
    // level 0
    for (int i = 0; i < nodesInLevel0; i++) {
        double key[vectorsInNode];
        int son[vectorsInNode];
        int n;

        if (i == nodesInLevel0 - 1) n = 60000 - vectorsInNode * (nodesInLevel0 - 1);
        else n = vectorsInNode;
        for (int j = 0; j < n; j++) {
            key[j] = projectVector[i * vectorsInNode + j].length;
            son[j] = projectVector[i * vectorsInNode + j].imageNum;
        }

        bNode[i].init(0, this, n, key, son);
    }

    // level 1
    for (int i = 0; i < nodesInLevel1; i++) {
        double key[vectorsInNode];
        int n;

        if (i == nodesInLevel1 - 1) n = nodesInLevel0 - vectorsInNode * (nodesInLevel1 - 1);
        else n = vectorsInNode;
        for (int j = 0; j < n; j++)
            key[j] = bNode[i * vectorsInNode + j].get_key_of_node();
        
        bNode[nodesInLevel0 + i].init(1, this, n, key, NULL);
    }

    // level 2
    double key[vectorsInNode];

    for (int i = 0; i < nodesInLevel1; i++)
        key[i] = bNode[nodesInLevel0 + i].get_key_of_node();

    bNode[nodesInLevel0 + nodesInLevel1].init(2, this, nodesInLevel1, key, NULL);

    root_ptr_ = new BNode[nodesInLevel0 + nodesInLevel1 + 1];
    for (int i = 0; i < nodesInLevel0 + nodesInLevel1 + 1; i++)
        root_ptr_[i] = bNode[nodesInLevel0 + nodesInLevel1 - i];
    for (int i = 0; i < nodesInLevel0 + nodesInLevel1 + 1; i++) {
        root_ptr_[i].set_left_sibling(i - 1);
        root_ptr_[i].set_right_sibling(i + 1);
    }
    root_ptr_[nodesInLevel0 + nodesInLevel1].set_right_sibling(-1);
    tree_size_ = nodesInLevel0 + nodesInLevel1;
    free(bNode);
}

bool BTree::writeFile() {
    return 1;
}

bool BTree::readFile(char *fileName) {
    return 1;
}
