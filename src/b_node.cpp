// from TA's files

#include "headers.h"

BNode::BNode()						// constructor
{
	level_ = -1;
	num_entries_ = -1;
	left_sibling_ = right_sibling_ = -1;
	key_ = NULL;
	son_ = NULL;

	block_ = capacity_ = -1;
	dirty_ = false;
	btree_ = NULL;
}

BNode::~BNode()						// destructor
{
	key_ = NULL;
	btree_ = NULL;
}

void BNode::init(					// init a new node, which not exist
	int level,						// level (depth) in b-tree
	BTree* btree, 					// b-tree of this node
	int num_entries,
	double* key,
	int *son)
{
	btree_ = btree;					// init <btree_>
	level_ = (char) level;			// init <level_>
	num_entries_ = num_entries;
	key_ = key;
	son_ = son;

	dirty_ = true;					// init <dirty_>
	left_sibling_ = -1;				// init <left_sibling_>
	right_sibling_ = -1;			// init <right_sibling_>

	block_ = -1;					// init <block_>
	capacity_ = -1;					// init <capacity_>
}

int BNode::get_block()				// get <block_> (address of this node)
{
	return block_;
}

int BNode::get_num_entries()		// get <num_entries_>
{
	return num_entries_;
}

int BNode::get_level()				// get <level_>
{
	return level_;
}

double BNode::get_key_of_node()		// get key of this node
{
	return key_[0];
}

bool BNode::isFull()				// whether is full?
{
	if (num_entries_ >= capacity_) return true;
	else return false;
}

void BNode::set_left_sibling(		// set addr of left sibling node
	int left_sibling)				// addr of left sibling node
{
	left_sibling_ = left_sibling;
}

void BNode::set_right_sibling(		// set addr of right sibling node
	int right_sibling)				// addr of right sibling node
{
	right_sibling_ = right_sibling;
}


