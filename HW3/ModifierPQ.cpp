#include "ModifiedPQ.h"
#include <vector>

using namespace std;


/*ModifiedPQ::ModifiedPQ(int numBuildings) //constructor i didnt use
{
currentSize = 0;
vector<int> temp(numBuildings,-1);
locations = temp;
node temp_kemal(0,0);
//vector<node> MPQ_Vec(numBuildings);
//MPQ_Vec.push_back(temp_kemal);
//vector<int> MPQ_Vec(numBuildings);

} */

ModifiedPQ::~ModifiedPQ() //destructor. no dynamic memory allocations has been made
{



}

void ModifiedPQ ::insert(int value, int label) //for inserting element into the array
{
	node temp(label,value);

	//if(!IsEmpty())
	//{
	int hole = ++currentSize;
	for( ; hole >1 && value > MPQ_Vec[hole/2].value ; hole /=2) //generating maxHeap. percolate up until right index is found for temp
	{
		locations[MPQ_Vec[hole / 2].label] = hole;
		MPQ_Vec[hole] = MPQ_Vec[hole/2];
	}
	MPQ_Vec[hole].value = value;
	MPQ_Vec[hole].label = label;
	locations[label] = hole;
}
int ModifiedPQ::Remove(int label) //removes element, returns the value
{
	if(IsEmpty())
		return -1;
	int hole = locations[label]; //index of the element to be deleted
	int returnVal = MPQ_Vec[hole].value; // value of the element to be deleted. will be returned in the end
	MPQ_Vec[hole] = MPQ_Vec[currentSize--]; //fill the index of the deleted element with the last element in the array;
	node tmp = MPQ_Vec[hole];
	int comparedVal = MPQ_Vec[hole].value;
	int comparedLabel = MPQ_Vec[hole].label;
	int child;
	for( ; hole*2 <= currentSize; hole = child) //percolate up
	{
		child= hole*2;
		if(child!= currentSize && MPQ_Vec[child+1].value > MPQ_Vec[child].value)
		{
			child++;
		}
		if(MPQ_Vec[child].value > comparedVal)
		{
			locations[MPQ_Vec[child].label] = hole;
			MPQ_Vec[hole] = MPQ_Vec[child];
		}
		else
			break;
	}
	MPQ_Vec[hole] = tmp;
	locations[label] = -1;
	//currentSize -=1;
	return returnVal;
}




int ModifiedPQ::GetMax() //returns the max element in the heap. returns 0 if it's empty.
{
	if(currentSize != 0)
		return MPQ_Vec[1].value;
	else
		return 0;
}

bool ModifiedPQ::IsEmpty() //returns true if the MPQ_Vec is empty
{
	if(currentSize == 0)
		return true;
	else
		return false;
}
