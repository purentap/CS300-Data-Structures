struct memory_block
{
	memory_block* right; // node to the right
	memory_block* left; // node to the left
	bool used; // if this memory block is reserved or not
	int size; // the number of bytes reserved in Img_heap
	int starting_address; // the starting address in Img_heap
	// of the memory reserved by this block
};

#define MAX_CAPACITY 512 // the size capacity of Img_heap

class My_heap{
private:
	memory_block* heap_begin; //start block of img heap
	memory_block* blk; // end block of img heap
	int used_bytes; //allocated bytes in img heap
public:
	My_heap();
	~My_heap();
	memory_block* bump_allocate(int num_bytes);
	memory_block* first_fit_allocate(int num_bytes);
	memory_block* best_fit_allocate(int num_bytes);
	memory_block* first_fit_split_allocate(int num_bytes);
	void deallocate(memory_block* block_address);
	void print_heap();
	float get_fragmantation();
};