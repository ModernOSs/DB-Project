#ifndef _B_TREE_H_
#define _B_TREE_H_

class BNode;

class BTree {
  public:
    // BlockFile* file_;            // file in disk to store
    BNode* root_ptr_;               // pointer of root
    int tree_size_;

    BTree(BNode *root_ptr);         // constructor
    ~BTree();                       // destructor
    void bulkLoading(projectNode projectVector[60000]);
    bool writeFile();
    bool readFile(char *fileName);
};

#endif