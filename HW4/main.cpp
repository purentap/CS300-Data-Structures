#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <tuple>
using namespace std;

struct node
{
	int blockRank, rowIndex; 
	string blockName;
	node(int blockrank, int rowindex, string blockName) : blockRank(blockrank), rowIndex(rowindex), blockName(blockName) {}
};



void print(unordered_map <string, unordered_map<string, unordered_map<int, string>>> &stadium , vector<string> &block_names, vector<string> & row_names, int col_num, ofstream &txtOut )
	//prints the unordered map.
	//complexity is O(B*R*C)
{

	string a;
	for(int i=0 ;  i< block_names.size() ; i++)
	{
		txtOut<<endl ;
		txtOut<<block_names.at(i)<< endl;
		txtOut << "~~~~~~~" << endl;	
		for(int j = 0; j < row_names.size() ; j++)
		{
			txtOut<< row_names.at(j)<<" :" ;
			for(int k = 0 ; k< col_num ; k++)
			{
				txtOut<<" " ;
				txtOut << stadium[block_names.at(i)][row_names.at(j)][k].substr(0,3) ;
			}
			txtOut<< endl ;
			}
		txtOut << "=======" << endl;
		txtOut<<endl ;
		} 
	
		
	}

void PercolateDown(int hole, vector<node> &rowVec, unordered_map <string, unordered_map<int,int>> &sizeMap)
{
	node swap = rowVec.at(rowVec.size()-1);
	rowVec.at(rowVec.size()-1) = rowVec.at(1);
	rowVec.at(1) = swap; //change the first and last elements in the vector
	node temp = rowVec.at(hole);
	int child; 
	for(; hole*2 <= rowVec.size()-1 ;hole = child) //percolate down
	{
		child = hole *2 ; 
		if(child != rowVec.size()-1 && sizeMap[rowVec.at(child+1).blockName][rowVec.at(child+1).rowIndex] < sizeMap[rowVec.at(child).blockName][rowVec.at(child).rowIndex])
			child++;
		if (child != rowVec.size()-1 &&sizeMap[rowVec.at(child).blockName][rowVec.at(child).rowIndex] == sizeMap[rowVec.at(child+1).blockName][rowVec.at(child+1).rowIndex] && rowVec.at(child+1).blockRank < rowVec.at(child).blockRank)
			child++;
		if(sizeMap[rowVec.at(child).blockName][rowVec.at(child).rowIndex] < sizeMap[temp.blockName][temp.rowIndex])
			rowVec.at(hole) = rowVec.at(child);
		else if (sizeMap[rowVec.at(child).blockName][rowVec.at(child).rowIndex] == sizeMap[temp.blockName][temp.rowIndex] && rowVec.at(child).blockRank < temp.blockRank)
		{
			
				rowVec.at(hole) = rowVec.at(child);
		}
		else
			break;
	}
	rowVec.at(hole) = temp;

}

void reserve_seat_by_row(unordered_map <string, vector<string>> &People,unordered_map <string, unordered_map<int,int>> &sizeMap, unordered_map <string, unordered_map<string, unordered_map<int, string>>> &stadium, vector<vector<node>> &SeatPlan, unordered_map <string, int> &RowMap, int numCols, string customer_name , string row_name, ofstream &txtOut)
{
	//given the row name, reserves seat at the row with least customers in.
	// Complexity O(log B) + c
	int RowIndex = RowMap[row_name]; // finds the index of the row, O(1) complexity
	string blockName;
	vector<node> temp = SeatPlan.at(RowIndex);
	blockName = temp.at(1).blockName;
	int num = sizeMap[blockName][RowIndex];
	if(People.find(customer_name) == People.end() && sizeMap[blockName][RowIndex] != numCols) 
	{
		for(int i = 0 ; i < numCols ; i++) // +c 
		{
			if(stadium[blockName][row_name][i] == "---")
			{
			
				vector<string> loc;
				stadium[blockName][row_name][i] = customer_name;
				txtOut<<customer_name << " has reserved " << blockName << " " << row_name << "-" << i << " by emptiest block" << endl;
				sizeMap[blockName][RowIndex] +=1;
				loc.push_back(blockName);
				loc.push_back(row_name);
				loc.push_back(to_string(i));
				People[customer_name] = loc; //save the customers seat information
				//percolate down
				PercolateDown(1, temp, sizeMap); //O(logB)
				SeatPlan.at(RowIndex) = temp;
				break;
				
			}
		}

	}
	else
		txtOut<< customer_name << " could not reserve a seat!" << endl;

}

void reserve_seat(vector<vector<node>> &SeatPlan, unordered_map <string, unordered_map<int,int>> &sizeMap, unordered_map <string, vector<string>> &People, unordered_map <string, unordered_map<string, unordered_map<int, string>>> &stadium,unordered_map <string, int> &RowMap, string customer_name, string block_name, string row_name, int column_number, ofstream &txtOut )
{
	//reserves seat if the given block name, row name and the column isn't already reserved
	// complexity is O(logB)
	if(People.find(customer_name) == People.end() && stadium[block_name][row_name][column_number] == "---")
	{
		int RowIndex = RowMap[row_name];
		vector<node> temp = SeatPlan.at(RowIndex);
		vector<string> loc;
		stadium[block_name][row_name][column_number] = customer_name;
		sizeMap[block_name][RowIndex] +=1;
		loc.push_back(block_name);
		loc.push_back(row_name);
		loc.push_back(to_string(column_number));
		People[customer_name] = loc; //save the customers seat information
		txtOut<< customer_name << " has reserved " << block_name<< " " << row_name << "-"<< column_number << endl;
		PercolateDown(1,temp,sizeMap); //O(logB)
		SeatPlan.at(RowIndex) = temp;
		
	}
	else
	{
		txtOut<< customer_name << " could not reserve a seat!" << endl;
	}

}

void get_seat(ofstream &txtOut, string customer_name,unordered_map <string, unordered_map<string, unordered_map<int, string>>> &stadium, unordered_map <string, vector<string>> &People,unordered_map<string , unordered_map<int,int>> &sizeMap )
{
	//Finds the seat information of the given customer.
	// O(1) complexity
	
	if(People.find(customer_name) != People.end())
	{
		vector<string> temp = People[customer_name];
		txtOut << "Found that " << customer_name << " has a reservation in "<< temp.at(0) << " " << temp.at(1) <<"-"<< temp.at(2) << endl; 
	}
	else
		txtOut<< "There is no reservation made for " << customer_name <<"!" << endl;
}

void cancel_reservation(ofstream &txtOut,unordered_map<string, int> RowMap, vector<vector<node>> &SeatPlan, string customer_name,unordered_map <string, unordered_map<string, unordered_map<int, string>>> &stadium ,  unordered_map <string, vector<string>> &People,  unordered_map <string, unordered_map<int,int>> &sizeMap)
{
	//cancels reservation of the given customer.
	//O(log B) complexity
	if(People.find(customer_name) != People.end())
	{
	vector<string> temp = People[customer_name];
	string block_name = temp.at(0);
	string row_name = temp.at(1);
	int numColumn = stoi(temp.at(2));
	int RowIndex = RowMap[row_name];
	stadium[block_name][row_name][numColumn] = "---" ; 
	sizeMap[block_name][RowIndex] -=1 ;
	People.erase(customer_name); //delete the customer from the seat info map
	txtOut<< "Cancelled the reservation of "<< customer_name << endl;
	vector <node> temp1 = SeatPlan.at(RowIndex);
	PercolateDown(1,temp1,sizeMap); //O(log B)
	SeatPlan.at(RowIndex) = temp1;

	}
	else
		txtOut<< "Could not cancel the reservation for " << customer_name << "; no reservation found!" << endl ; 
}


int main()
{ 
	string line,name;
	int num_column; 
	ifstream file;
	vector<string> block_names, row_names;
	vector<vector<node>> SeatPlan;
	ofstream txtOut;
	txtOut.open("output.txt");
	unordered_map <string, unordered_map<string, unordered_map<int, string>>> stadium; //[blockname][rowname][column]
	unordered_map<string, int> RowMap; //used in reserve seat by row func.given the name of the row, maps it to the index of that row.
	unordered_map <string, unordered_map<int,int>> sizeMap; //blockName, RowIndex.given the blockname and the rowindex maps to the size of the each row for every block. 
	unordered_map <string, vector<string>>People; //given the customer name, maps it to the seat location of the customer
	file.open("inputs1.txt"); //	#######change the input file name here ########
	if(file.fail())
		cout<<"Unable to open file" << endl;
	else
	{
		//get the first line, BLOCK NAMES
		getline(file, line);
		istringstream ss(line);
		while(ss>>name)
		block_names.push_back(name);
		
		//get second line ROW NAMES
		getline(file, line);
		istringstream ss2(line);
		while(ss2>>name)
		row_names.push_back(name);
		
		//get third line, NUMBER OF COLUMNS
		getline(file, line);
		istringstream ss3(line);
		ss3>>num_column; 

		//construct the stadium as unordered_map
		 
		for (int i =0 ; i < block_names.size() ; i++)
		{
			for (int j = 0; j < row_names.size() ; j++)
			{
				for(int k = 0 ; k < num_column ; k++)
				{
				stadium[block_names.at(i)][row_names.at(j)][k] = "---";
				}
			}
		}

		//constructing the vector SeatPlan, minHeap for each row.
		for(int i = 0 ; i < row_names.size() ; i++)
		{
			vector<node> rows;
			RowMap[row_names.at(i)] = i;
			for(int j = 0 ; j < block_names.size() ; j++)
			{
				node tmp(j, i, block_names.at(j));
				if(j == 0 )
				{
					node filler (-1, -1, "temp");
					rows.push_back(filler);
				}
				rows.push_back(tmp);
				sizeMap[block_names.at(j)][i] = 0; //initalize each row's size to 0 

			}
			SeatPlan.push_back(rows);
		}
		string method;
		while(getline(file,line))
		{
			istringstream ss4(line);
			ss4 >> method;
			if(method == "print")
			{
				print(stadium,block_names, row_names,num_column, txtOut);
				method =""; 
			}
			else if(method == "reserve_seat_by_row")
			{
				string customer_name, row_name;
				ss4 >> customer_name >> row_name ; 
				reserve_seat_by_row( People, sizeMap, stadium, SeatPlan, RowMap, num_column, customer_name,row_name, txtOut);
			}
			else if(method == "reserve_seat")
			{
				string name, block_name, row_name;
				int column_number;
				ss4 >> name >> block_name >> row_name >> column_number;
				reserve_seat(SeatPlan, sizeMap, People, stadium , RowMap, name, block_name, row_name, column_number, txtOut);
			}
			else if(method == "get_seat")
			{
				string name;
				ss4 >> name;
				get_seat(txtOut, name, stadium, People, sizeMap);
			}
			else if(method == "cancel_reservation")
			{
				string name;
				ss4>> name;
				cancel_reservation(txtOut, RowMap, SeatPlan, name,stadium,People,sizeMap);
			}

		}

		
		
	}



	return 0;
}
