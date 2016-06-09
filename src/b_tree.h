#ifndef _B_TREE_H_
#define _B_TREE_H_

class BNode;

class BTree {
  public:
    // BlockFile* file_;            // file in disk to store
    BNode* root_ptr_;               // pointer of root
    int tree_size_;

    BTree();
    BTree(BNode *root_ptr);         // constructor
    ~BTree();                       // destructor
    void bulkLoading(projectNode projectVector[60000]);

    double *h_key_, *l_key_;
    int *h_son_, *l_son_;
    int min_h_, max_l_;
    int h_level0_, l_level0_;
    void getNextH(double searchKey);
    void getNextL(double searchKey);
    int searchNextImage(double searchKey);
    void resetSearch();
    bool writeFile(char *fileName);
    bool readFile(char *fileName); 
};

#endif