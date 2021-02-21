#include<iostream>
#include <vector>
#include <stdio.h>
#include <string>
using namespace std;



class ModifiedPQ
{

public: 

	struct node {
		int label; //label of the building
		int value; //height of the building
		node(int label, int value) : label(label), value(value){}
		node() : label(0), value(0){}
	};
	//ModifiedPQ(int numBuildings);
	ModifiedPQ(int size ) : MPQ_Vec(size + 1), locations(size + 1),currentSize (0) {}
	~ModifiedPQ();
	void insert(int value, int label); //inserts an item with the given value and label into ModifiedPQ
	int Remove(int label); //removes the value with this label and returns it
	int GetMax(); //returns the max value currently stored
	bool IsEmpty(); //checks if ModifiedPQ is empty

private: 


	int currentSize;
	vector<node> MPQ_Vec; 
	//int MPQ_Vec[100];
	vector<int> locations;

	//void percolateUp(int hole);
};


