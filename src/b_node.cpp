// from TA's files
#define SIZECHAR sizeof(char)
#define SIZEINT sizeof(int)
#define SIZEDOUBLE sizeof(double)

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

int BNode::get_left_sibling() {
	return left_sibling_;
}

int BNode::get_right_sibling() {
	return right_sibling_;
}

int* BNode::get_son() {
	return son_;
}

double* BNode::get_key() {
	return key_;
}

void BNode::write_to_buffer(char* buf) {
	int i = 0;	
	memcpy(&buf[i], &level_, SIZECHAR);
	i += SIZECHAR;

	memcpy(&buf[i], &num_entries_, SIZEINT);
	i += SIZEINT;

	memcpy(&buf[i], &left_sibling_, SIZEINT);
	i += SIZEINT;

	memcpy(&buf[i], &right_sibling_, SIZEINT);
	i += SIZEINT;

	for (int j = 0; j < num_entries_; j++) {
		memcpy(&buf[i], &key_[j], SIZEDOUBLE);
		i += SIZEDOUBLE;

		if (level_ == 0) {
			memcpy(&buf[i], &son_[j], SIZEINT);
		}
		i += SIZEINT;
	}
}

void BNode::read_from_buffer(char* buf) {
	int i = 0;
	memcpy(&level_, &buf[i], SIZECHAR);
	i += SIZECHAR;

	memcpy(&num_entries_, &buf[i], SIZEINT);
	i += SIZEINT;

	memcpy(&left_sibling_, &buf[i], SIZEINT);
	i += SIZEINT;

	memcpy(&right_sibling_, &buf[i], SIZEINT);
	i += SIZEINT;

	key_ = new double[num_entries_];
	if (level_ == 0) son_ = new int[num_entries_];
	else son_ = NULL;
	for (int j = 0; j < num_entries_; j++) {
		memcpy(&key_[j], &buf[i], SIZEDOUBLE);
		i += SIZEDOUBLE;

		if (level_ == 0) memcpy(&son_[j], &buf[i], SIZEINT);
		i += SIZEINT;
	}
}
