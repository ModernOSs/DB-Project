#include "headers.h"
#define HeaderSize sizeof(int)

BTree::BTree() {
    h_key_ = NULL; l_key_ = NULL;
    h_son_ = NULL; l_son_ = NULL;
    min_h_ = 0; max_l_ = 0;
}

BTree::BTree(BNode *root_ptr)       // constructor
{
    root_ptr_ = root_ptr;
    h_key_ = NULL; l_key_ = NULL;
    h_son_ = NULL; l_son_ = NULL;
    min_h_ = 0; max_l_ = 0;
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
        double *key = new double[vectorsInNode];
        int *son = new int[vectorsInNode];
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
        double *key = new double[vectorsInNode];
        int n;

        if (i == nodesInLevel1 - 1) n = nodesInLevel0 - vectorsInNode * (nodesInLevel1 - 1);
        else n = vectorsInNode;
        for (int j = 0; j < n; j++)
            key[j] = bNode[i * vectorsInNode + j].get_key_of_node();
        
        bNode[nodesInLevel0 + i].init(1, this, n, key, NULL);
    }

    // level 2
    double *key = new double[vectorsInNode];

    for (int i = 0; i < nodesInLevel1; i++)
        key[i] = bNode[nodesInLevel0 + i].get_key_of_node();

    bNode[nodesInLevel0 + nodesInLevel1].init(2, this, nodesInLevel1, key, NULL);

    root_ptr_ = new BNode[nodesInLevel0 + nodesInLevel1 + 1];
    root_ptr_[0] = bNode[nodesInLevel0 + nodesInLevel1];
    for (int i = 1; i < nodesInLevel1 + 1; i++)
        root_ptr_[i] = bNode[nodesInLevel0 + i - 1];
    for (int i = 1 + nodesInLevel1; i < nodesInLevel0 + nodesInLevel1 + 1; i++)
        root_ptr_[i] = bNode[i - 1 - nodesInLevel1];

    for (int i = 0; i < nodesInLevel0 + nodesInLevel1 + 1; i++) {
        root_ptr_[i].set_left_sibling(i - 1);
        root_ptr_[i].set_right_sibling(i + 1);
    }
    root_ptr_[nodesInLevel0 + nodesInLevel1].set_right_sibling(-1);
    tree_size_ = nodesInLevel0 + nodesInLevel1;
    delete []bNode;
}

void BTree::getNextH(double searchKey) {
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));

    // left bound
    if (min_h_) {
        min_h_ = -1;  // do nothing
    } else {
        // first attemp
        if (h_key_ == NULL) {
            // level 2 -> level 1
            int level1 = root_ptr_[0].get_num_entries();
            for (int i = 0; i < root_ptr_[0].get_num_entries(); i++)
                if ((root_ptr_[0].get_key())[i] > searchKey) {
                    if (i == 0)
                        level1 = 1;
                    else
                        level1 = i;
                    break;
                }

            // level 1 -> level 0
            h_level0_ = vectorsInNode * (level1 - 1) + root_ptr_[level1].get_num_entries()
                     + root_ptr_[0].get_num_entries();
            for (int i = 0; i < root_ptr_[level1].get_num_entries(); i++)
                if ((root_ptr_[level1].get_key())[i] > searchKey) {
                    if (i == 0)
                        h_level0_ = 1 + root_ptr_[0].get_num_entries();
                    else
                        h_level0_ = vectorsInNode * (level1 - 1) + i +
                                 root_ptr_[0].get_num_entries();
                    break;
                }

            // search level 0
            h_key_ = (root_ptr_[h_level0_].get_key()) + (root_ptr_[h_level0_].get_num_entries() - 1);
            h_son_ = (root_ptr_[h_level0_].get_son()) + (root_ptr_[h_level0_].get_num_entries() - 1);
            for (int i = 0; i < root_ptr_[h_level0_].get_num_entries(); i++)
                if ((root_ptr_[h_level0_].get_key())[i] > searchKey) {
                    if (i == 0) {
                        min_h_ = 1;
                        h_key_ = (root_ptr_[h_level0_].get_key());
                        h_son_ = (root_ptr_[h_level0_].get_son());
                    } else {
                        h_key_ = (root_ptr_[h_level0_].get_key()) + (i - 1);
                        h_son_ = (root_ptr_[h_level0_].get_son()) + (i - 1);
                    }
                    break;
                }
        } else {
            if (h_key_ == root_ptr_[h_level0_].get_key()) {
                h_level0_--;
                h_key_ = root_ptr_[h_level0_].get_key() +
                         root_ptr_[h_level0_].get_num_entries() - 1;
                h_son_ = root_ptr_[h_level0_].get_son() +
                         root_ptr_[h_level0_].get_num_entries() - 1;
            } else {
                h_key_--;
                h_son_--;
                if (h_key_ == root_ptr_[root_ptr_[0].get_num_entries() + 1].get_key())
                    min_h_ = 1;
            }
        }
    }
}

void BTree::getNextL(double searchKey) {
  if (max_l_) {
    max_l_ = -1;  // do nothing
  } else {
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    int nodesInLevel1 = ceil((double)nodesInLevel0 / (double)vectorsInNode);
    int last = nodesInLevel0 + nodesInLevel1;
    if (l_key_ == NULL) {
      if (h_key_ == root_ptr_[last].get_key() + (root_ptr_[last].get_num_entries() - 1)) {
          l_key_ = root_ptr_[last].get_key() + (root_ptr_[last].get_num_entries() - 1);
          l_son_ = root_ptr_[last].get_son() + (root_ptr_[last].get_num_entries() - 1);
          max_l_ = 1;
          l_level0_ = h_level0_;
          getNextH(searchKey);
      }
      else if (h_key_ == root_ptr_[h_level0_].get_key() + (root_ptr_[h_level0_].get_num_entries() - 1)) {
        l_level0_ = h_level0_ + 1;
        l_key_ = root_ptr_[l_level0_].get_key();
        l_son_ = root_ptr_[l_level0_].get_son();
      } else {
        l_level0_ = h_level0_;
        l_key_ = h_key_ + 1;
        l_son_ = h_son_ + 1;
      }
    } else {
        if (l_key_ == root_ptr_[l_level0_].get_key() + (root_ptr_[l_level0_].get_num_entries() - 1)) {
            l_level0_++;
            l_key_ = root_ptr_[l_level0_].get_key();
            l_son_ = root_ptr_[l_level0_].get_son();
        } else {
            l_key_++;
            l_son_++;
            if (l_key_ == root_ptr_[last].get_key() + (root_ptr_[last].get_num_entries() - 1))
                max_l_ = 1;
        }
    }
  }
}

int BTree::searchNextImage(double searchKey) {
    if (h_key_ == NULL) {
        getNextH(searchKey);
        getNextL(searchKey);
    }

    int returnValue = -1;

    if (min_h_ == -1) {
        returnValue = *l_son_;
        getNextL(searchKey);
    } if (max_l_ == -1) {
        returnValue = *h_son_;
        getNextH(searchKey);
    }

    if (returnValue != -1)
        return returnValue;

    if (fabs(searchKey - *h_key_) < fabs(*l_key_ - searchKey)) {
        returnValue = *h_son_;
        getNextH(searchKey);
    } else if (fabs(searchKey - *h_key_) > fabs(*l_key_ - searchKey)) {
        returnValue = *l_son_;
        getNextL(searchKey);
    } else if (*h_key_ != *l_key_) {
        returnValue = *h_son_;
        getNextH(searchKey);
    } else {
        getNextH(searchKey);
        returnValue = *l_son_;
    }
    return returnValue;
}

void BTree::resetSearch() {
    h_key_ = NULL; l_key_ = NULL;
    h_son_ = NULL; l_son_ = NULL;
    min_h_ = 0; max_l_ = 0;
}

bool BTree::writeFile(char *fileName) {
    FILE * pFile;
    pFile = fopen (fileName,"wb");
    if (pFile == NULL) {
        printf("Open file %s failed.\n", fileName);
        return false;
    }
    //set header
    fwrite (&tree_size_ , sizeof(int), sizeof(int), pFile);
    fseek (pFile, 1024 ,SEEK_SET);

    //set nodes
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    int nodesInLevel1 = ceil((double)nodesInLevel0 / (double)vectorsInNode);
    for (int i = 0; i < nodesInLevel0 + nodesInLevel1 + 1; ++i)
    {
        char* temp = (char*)malloc(1024);
        root_ptr_[i].write_to_buffer(temp);
        fwrite(temp, sizeof(char), 1024, pFile);
        free(temp);
    }
    fclose(pFile);
    return 1;
}

bool BTree::readFile(char *fileName) {
    FILE * pFile;
    pFile = fopen (fileName,"rb");
    if (pFile == NULL) {
        printf("Open file %s failed.\n", fileName);
        return false;
    }
    
    //read header
    fread(&tree_size_, sizeof(int), sizeof(int), pFile);
    fseek (pFile, 1024 ,SEEK_SET);

    //read nodes
    int vectorsInNode = floor((double)(1024 - sizeof(char) - sizeof(int) * 3) /
                              (double)(sizeof(double) + sizeof(int)));
    int nodesInLevel0 = ceil((double)60000 / (double)vectorsInNode);
    int nodesInLevel1 = ceil((double)nodesInLevel0 / (double)vectorsInNode);
    if (root_ptr_ != NULL) delete []root_ptr_;
    root_ptr_ = new BNode[nodesInLevel0 + nodesInLevel1 + 1];
    for (int i = 0; i < nodesInLevel0 + nodesInLevel1 + 1; ++i)
    {
        char* temp = (char*)malloc(1024);
        fread(temp, sizeof(char), 1024, pFile);
        root_ptr_[i].read_from_buffer(temp);
        free(temp);
    }

    fclose (pFile);
    return 1;
}
