#include "headers.h"

BNode::BNode()						// constructor
{
	level_ = -1;
	num_entries_ = -1;
	left_sibling_ = right_sibling_ = -1;
	key_ = NULL;
	son_ = -1;

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
	int level,							// level (depth) in b-tree
	BTree* btree)						// b-tree of this node
{
	btree_ = btree;					// init <btree_>
	level_ = (char) level;			// init <level_>

	dirty_ = true;					// init <dirty_>
	left_sibling_ = -1;				// init <left_sibling_>
	right_sibling_ = -1;				// init <right_sibling_>
	key_ = NULL;					// init <key_>
	son_ = -1;

	num_entries_ = 0;				// init <num_entries_>
	block_ = -1;						// init <block_>
	capacity_ = -1;					// init <capacity_>
}

void BNode::init_restore(			// load an exist node from disk to init
	BTree* btree,						// b-tree of this node
	int block)							// addr of disk for this node
{
	btree_ = btree;					// init <btree_>
	block_ = block;					// init <block_>

	dirty_ = false;					// init <dirty_>
	left_sibling_ = -1;				// init <left_sibling_>
	right_sibling_ = -1;				// init <right_sibling_>
	key_ = NULL;					// init <key_>
	son_ = -1;

	num_entries_ = 0;				// init <num_entries_>
	level_ = -1;						// init <block_>
	capacity_ = -1;					// init <capacity_>
}

BNode* BNode::get_left_sibling()	// get the left-sibling node
{
	BNode* node = NULL;
	if (left_sibling_ != -1) {		// left sibling node exist
		node = new BNode();			// read left-sibling from disk
		node->init_restore(btree_, left_sibling_);
	}
	return node;
}

BNode* BNode::get_right_sibling()	// get the right-sibling node
{
	BNode* node = NULL;
	if (right_sibling_ != -1) {		// right sibling node exist
		node = new BNode();			// read right-sibling from disk
		node->init_restore(btree_, right_sibling_);
	}
	return node;
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

float BNode::get_key_of_node()		// get key of this node
{
	return key_[0];
}

bool BNode::isFull()				// whether is full?
{
	if (num_entries_ >= capacity_) return true;
	else return false;
}

void BNode::set_left_sibling(		// set addr of left sibling node
	int left_sibling)					// addr of left sibling node
{
	left_sibling_ = left_sibling;
}

void BNode::set_right_sibling(		// set addr of right sibling node
	int right_sibling)					// addr of right sibling node
{
	right_sibling_ = right_sibling;
}


