#include "PointQuadTree.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

int main()
{
	string line;
	string line2;

	ifstream file2("queries5.txt"); //change the name of the txt file 
	ifstream file1("cities.txt");
	stringstream ss;
	stringstream queries;

	string city;
	int x_cord, y_cord;
	PointQuadTree mytree;

	
	int searchx, searchy,x_limit, y_limit;
	unsigned int radius;
	//insert
	if (file1.is_open())
	{
		getline(file1, line);
		ss << line;
		ss >> x_limit >> y_limit; 
		mytree.insert("",x_limit,y_limit);
		while (getline(file1, line))
		{
			istringstream ss(line);
			ss >> city >> x_cord >> y_cord;
			mytree.insert(city, x_cord,y_cord);
		}
	
	}

	else { cout << "Unable to open file"; }

	mytree.pretty_print(); //prints the tree




	if (file2.is_open())
	{ 
		//search
		while (getline(file2, line2))
		{
			string str2(line2.size(), '0');
			str2.erase(remove_copy(line2.begin(), line2.end(), str2.begin(), ','), str2.end());

			istringstream queries(str2);
			queries >> searchx >> searchy >> radius;
			mytree.search(searchx, searchy, radius);
			
			
		}
		file2.close();
	}

	else  
	cout << "Unable to open file"; 

	return 0;
}