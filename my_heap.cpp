#include "my_heap.h"
#include <iostream>

using namespace std;

My_heap::My_heap() //constructor
{
	heap_begin=NULL;
	blk = NULL;
	used_bytes =0 ;
}

memory_block* My_heap::bump_allocate(int num_bytes) 
{
    // pre: takes an int num_bytes
	// post: allocates a memory block to the end of img heap, sets blk to newly created memory block and returns blk.
	// returns NULL if MAX_CAPACITY is exceeded
	if(used_bytes + num_bytes > MAX_CAPACITY)
		return NULL;
	else 
	{
		memory_block* temp = new memory_block;
		temp->starting_address = used_bytes;
		used_bytes += num_bytes;
		temp->used = true;
		temp->size = num_bytes;
		if(blk == NULL)
		{
			heap_begin = temp;
			blk = temp; 
			temp->right = NULL;
			temp->left = NULL;
		}
		else
		{
            if(heap_begin == blk)
			{
				blk= temp;
				heap_begin->right = blk;
				blk->left = heap_begin;
				blk->right = NULL;
			}
			else
			{
			blk->right = temp;
			temp->left = blk;
			temp->right = NULL;
			blk = temp;
			}
		}

		return blk;
	}

}

My_heap::~My_heap() //destructor
{
	int count = 0;
	memory_block *temp = heap_begin, *temp2;
	while(temp != NULL)
	{
		if(temp->used ==true)
		count = count + temp->size;
		temp2 = temp;
		temp=temp->right;
		delete temp2;

	}
	cout<< "At destruction, the heap had a memory leak of " << count << " bytes." << endl;
}

void My_heap::deallocate(memory_block* to_delete)
{ // pre : takes a pointer to a memory block in the img heap
  // post : if a memory block at the right of left of to_delete is not used, merges those memory blocks into a single block. sets used = false
	// if there exists not used block at the right or left side deletes to_delete and memory block that aren't used.
	// if not, sets to_delete -> used = false
	bool leftcheck= true, rightcheck = true;
	used_bytes = used_bytes - to_delete->size;
	if(to_delete != blk && to_delete != heap_begin) //if to_delete is not on the edge of img heap.
	{
		memory_block *temp = new memory_block;
		memory_block *rightptr= to_delete->right , *leftptr = to_delete->left;
		temp->used = false;
		if(rightptr->used == false)
			rightcheck = false;
		if(leftptr->used == false)
			leftcheck = false;
		if( rightcheck == false && leftcheck == false) //condition 1. if both right and left blocks are not used
		{
			temp->starting_address = leftptr->starting_address;
			temp->size = rightptr->size + to_delete->size + leftptr->size;
			temp->left = leftptr->left;
			temp->right = rightptr-> right;
			leftptr->left->right = temp;
			rightptr->right->left = temp;
			delete to_delete;
			delete rightptr;
			delete leftptr;
		}
		else if (rightcheck == true && leftcheck == false) //if left block is not used
		{
			temp->starting_address = leftptr->starting_address;
			temp->size = to_delete->size + leftptr->size;
			temp->left = leftptr->left;
			if(leftptr->left !=NULL)
			leftptr->left->right = temp;
			else
				heap_begin = temp;
			temp->right = rightptr;
			rightptr->left = temp;
			delete leftptr;
			delete to_delete;
		}
		else if ( rightcheck == false && leftcheck == true) //if right block is not used
		{
			temp->starting_address= to_delete->starting_address;
			temp->size = to_delete->size + rightptr->size;
			temp->left = leftptr;
			leftptr->right=temp;
			temp->right = rightptr->right;
			if(rightptr->right != NULL)
			rightptr->right->left = temp;
			else
				blk = temp;
			delete rightptr;
			delete to_delete;

		}
		else
		{
			to_delete->used = false;
		}
	}
	else if ( to_delete == blk) //condition 2 
	{
		memory_block* leftptr = blk->left; 

		if ( leftptr->used == false)
		{
			memory_block* temp = new memory_block;
			temp->used = false;
			temp->starting_address = leftptr->starting_address;
			temp->size = leftptr->size + to_delete->size;
			temp->right = NULL; 
			temp->left = leftptr->left;
			if(leftptr->left != NULL)
			leftptr->left->right = temp;
			blk = temp;
			delete to_delete;
			delete leftptr;
		}
		else
		{
			to_delete->used = false;
		}

	}
	else if(to_delete == heap_begin) //condition 3
	{
		memory_block* rightptr = heap_begin->right;
		if ( rightptr->used == false)
		{
			memory_block* temp = new memory_block;
			temp->used = false;
			temp->starting_address = heap_begin->starting_address;
			temp->size = heap_begin->size + rightptr->size;
			temp->right = rightptr->right;
			if(rightptr->right!= NULL)
			rightptr->right->left = temp;
			temp->left = NULL;
			heap_begin = temp;
			delete to_delete;
			delete rightptr;
		}
		else
		{
			to_delete->used = false;
		}

	}


}

memory_block* My_heap::first_fit_allocate(int num_bytes)
{//post: takes in num_bytes
// pre: checks if there exists unused block with size >= num_bytes exists, 
	// if so, sets used = true and returns the pointer to that block
	// if not, creates a new memory block at the end of img heap and returns the pointer to blk
	memory_block *temp = heap_begin;
	bool check = false;
	while(temp!= NULL)
	{
		if(temp->used == false && temp->size >= num_bytes && temp->size + used_bytes <= MAX_CAPACITY) 
		{
			temp->used = true;
			used_bytes += temp->size;
			check = true;
			return temp;
		}
		temp = temp->right;
	}

	return bump_allocate(num_bytes);
}

memory_block* My_heap::best_fit_allocate(int num_bytes)
{ //post: takes in num_bytes
  // pre: finds the memory block in img heap which is not used and will waster the least memory. (size-num_bytes should be smallest)
	// returns pointer to the memory block if such block exists
	//else creates a new memory block at the end of img heap and returns blk
	memory_block *temp, *check ; 
	int num=0, size = 0;
	bool found = false;
	temp = heap_begin;
	while(temp!= NULL)
	{
		if(temp->used == false && temp->size >= num_bytes && used_bytes + temp->size <= MAX_CAPACITY)
		{
			if (found == false) //found the first nonused such memory block
			{
				check = temp;
				num = temp->size - num_bytes;
				found = true;
			}
			else
			{
				if ( num >= temp->size - num_bytes) //finding the memory block which will waste the least memory
				{
					check = temp;
					num = temp->size - num_bytes;
				}

			}

		}
		temp = temp->right;
	}
	if(found == true)
	{
		check->used = true;
		used_bytes += check->size;
		return check; 
	}
	else
	{
		return bump_allocate(num_bytes);
	}


}

memory_block * My_heap::first_fit_split_allocate(int num_bytes)
{ //pre: takes int num_bytes
  // post: searches for nonused block in img heap, if it exists and has size > num_bytes, SPLITS the block into two, creates a new block with num_bytes, returns the pointer.
if(used_bytes + num_bytes > MAX_CAPACITY) // returns NULL if max capacity is exceeded
	return NULL;
memory_block* temp = heap_begin, *next;
while(temp!= NULL )
{
	if(temp->used == false && temp->size == num_bytes) //if size = num_bytes, just set used = true
	{
		temp->used = true;
		used_bytes+= num_bytes;
		return temp;
	}
	else if(temp->used == false && temp->size > num_bytes) // if size > num_bytes, create 2 new memory blocks and delete the old one.
	{                                                      //left block will be the block we want to assign. right block will be the free block with size temp->size - num_bytes
		next = temp->right;
		memory_block *left_block = new memory_block;
		memory_block* right_block = new memory_block;
		left_block->used = true;
		left_block->size = num_bytes;
		left_block->starting_address = temp->starting_address;
		left_block ->left = temp->left;
		temp->left->right = left_block;
		right_block -> used = false;
		right_block->size = temp->size - num_bytes;
		right_block->starting_address = left_block->starting_address + num_bytes;
		right_block -> right = next;
		next->left = right_block;
		right_block->left = left_block; 
		left_block -> right = right_block;
		used_bytes += num_bytes;
		delete temp;
		return left_block;
	}

	temp = temp->right;
}
 return bump_allocate(num_bytes);
}

float My_heap::get_fragmantation()
{ // fragmantation function. calculates the fragmentation percentage of the img heap and returns the value as float. 
	//fragmentation% = (free_memory - biggest_free_block)/free_memory *100
	float free_memory = MAX_CAPACITY - used_bytes;
	float biggest_free_block = 0;
	float check = 0, total_size=0;
	float fragmentation = 0;
	bool found = false;
	memory_block *temp = heap_begin;
	while(temp!=NULL)
	{
		total_size += temp->size;
		if(temp->used == false )
		{
			if(found == false)
			{
				check = temp->size;
				found = true;
			}
			else
			{
				if(temp->size > check)
					check = temp->size;
			}

		}
		temp = temp->right;
	}
	if(check> MAX_CAPACITY-total_size)
		biggest_free_block = check;
	else
		biggest_free_block = MAX_CAPACITY - total_size;

	fragmentation = (free_memory-biggest_free_block) / free_memory * 100 ;
	return fragmentation;

}


void My_heap::print_heap()
{//prints img heap with used_bytes, total amount of memory blocks, amount of used memory blocks, amount of free memory blocks and fragmentation percentage.
	// prints each memory block with it's size, if it's used or not and starting address
	memory_block *temp = heap_begin;
	int used_count = 0, free_count = 0, index=0, total=0;
	string check;
	while(temp!=NULL) // loop to count the total amount of blocks, used blocks and free blocks in the img heap
	{
		total +=1;
		if(temp->used == true)
		{
			used_count +=1;
		}
		else
		{
			free_count +=1;
		}
		temp = temp->right;

	}
	cout<<"Maximum capacity of heap: " << MAX_CAPACITY <<"B"<< endl;
	cout<<"Currently used memory (B): " << used_bytes << endl;
	cout<<"Total memory blocks: "<< total << endl;
	cout<<"Total used memory blocks: " << used_count << endl;
	cout<<"Total free memory blocks: " << free_count << endl;
	cout<<"Fragmentation " << get_fragmantation()<<"%" <<endl;
	cout<< "------------------------------" << endl;
	temp = heap_begin; 
	while(temp!= NULL)
	{
		if(temp->used == true)
		{
			cout<<"Block "<<index << "\t\tUsed: True" <<"\tSize (B): "<< temp->size ;
			cout <<"\tStarting Address: 0x" << hex << temp->starting_address<< dec<<endl;
		}
		else
		{
			cout<<"Block "<<index << "\t\tUsed: False" <<"\tSize (B): "<< temp->size  ;
			cout <<"\tStarting Address: 0x"<< hex << temp->starting_address<< dec<< endl;
		}
		temp = temp->right;
		index +=1;

	}
	cout<<"------------------------------" << endl;
	cout<< "------------------------------"<< endl;

}
