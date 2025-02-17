// from TA's files

#ifndef _B_NODE_H
#define _B_NODE_H

class BTree;

class BNode {
  public:
	BNode();
	virtual ~BNode();

	virtual void init(					// init a new node, which not exist
		int level,						// level (depth) in b-tree
		BTree* btree,  					// b-tree of this node
		int num_entries,
		double* key,
		int *son);
									       
	void set_left_sibling(			    // set <left_sibling>
		int left_sibling);				// addr of left sibling node
	void set_right_sibling(			    // set <right sibling>
		int right_sibling);				// addr of right sibling node

	int get_block();				    // get <block_>
	int get_num_entries();			    // get <num_entries_>
	int get_level();				    // get <level_>
	int  get_header_size();			    // get header size in b-node
	double get_key_of_node();		    // get key of this node
	bool isFull();					    // is full?
	int get_left_sibling();
	int get_right_sibling();
	int* get_son();
	double* get_key();
	void write_to_buffer(char* buf);
	void read_from_buffer(char* buf);

  protected:
	char level_;						// level of b-tree (level > 0)
	int num_entries_;			    	// number of entries in this node
	int left_sibling_;			    	// addr in disk for left sibling
	int right_sibling_;			    	// addr in disk for right sibling
	double* key_;						// keys
	int* son_;

	bool dirty_;						// if dirty, write back to file
	int block_;					    	// addr of disk for this node
	int capacity_;						// max num of entries can be stored
	BTree* btree_;						// b-tree of this node
};

#endif
