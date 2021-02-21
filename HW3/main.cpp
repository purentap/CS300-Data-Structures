#include "ModifiedPQ.h" 
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

struct building{ //struct for keeping buildings as an object
	int xVal, label, height;
	bool isLeft;
	building(int xval, int label,int height, bool isLeft) : xVal(xval), label(label),height(height), isLeft(isLeft) {} 
};

int main()
{
	string line1, line2; 
	ifstream file;
	stringstream ss; 
	int xLeft, y , xRight, numBuildings; 
	vector< building> Buildings; //vector for the each x value, its label and if it's the left edge or not.
	vector<int> heights;
	int counter = 0; // to store labels of the buildings

	file.open("input2.txt");//######## change this file name to the txt file name
	if(file.fail())
		cout<<"Unable to open file" << endl;
	else
	{
		getline(file,line1); //get the first line which gives the number of buildings.
		ss<<line1;
		ss >> numBuildings ;
		ModifiedPQ MPQ(numBuildings); //Modified Priority Queue Object
		while(getline(file,line1))
		{
			istringstream ss(line1);
			ss >> xLeft >> y >> xRight;
			building temp(xLeft, counter,y, true);
			building temp1(xRight, counter,y, false);
			Buildings.push_back(temp); 
			Buildings.push_back(temp1);
			//heights.push_back(y);
			counter +=1;
		}


		//sort the buildings vector with insertion sort. 
		int j; 
		for(int p= 1 ; p< Buildings.size() ;p++)
		{
			
			building temp = Buildings[p];
			for(j=p; j>0 && temp.xVal < Buildings[j-1].xVal; j--)
			{ 
				Buildings[j] = Buildings[j-1];
			}
			Buildings[j] = temp;
			int k = j;
			while(k>0 && Buildings[k-1].xVal == temp.xVal) //this loop is for organizing the array if the x values are equal
			{
				if(temp.isLeft == true && Buildings[k-1].isLeft == false) //temp is left and j-1 is right
				{
					building lesser_tmp = Buildings[k-1];
					Buildings[k-1] = Buildings[k];
					Buildings[k] = lesser_tmp;
				}
				else if(temp.isLeft == true && Buildings[k-1].isLeft == true) // both are left points.
				{
					if(temp.height > Buildings[k-1].height) //Buildings[k-1] is bigger
					{
						building lesser_tmp = temp;
						Buildings[k] = Buildings[k-1];
						Buildings[k-1] = lesser_tmp;
					}
				}
				else if(temp.isLeft == false && Buildings[k-1].isLeft == false) //both are right points
				{
					if(temp.height < Buildings[k-1].height) // buildings[k-1] is bigger
					{
						building lesser_tmp = temp;
						Buildings[k] = Buildings[k-1];
						Buildings[k-1] = lesser_tmp;
					}

				}
				k-=1;

			}

		}



		if( Buildings[0].xVal != 0) //if buildings doesn't start at x= 0, shows there are no buildings at 0
		{
			cout<<0 << " " << 0 <<endl;

		}

		int maxBefore =-1;
		for(int i= 0 ; i< Buildings.size() ; i++)
		{
			//	int maxBefore = MPQ.GetMax();


			if(Buildings[i].isLeft == true)
			{
				MPQ.insert(Buildings[i].height, Buildings[i].label);
				int maxAfter = MPQ.GetMax();
			}

			else
			{

				int removed = MPQ.Remove(Buildings[i].label);

			}
			if(MPQ.GetMax() != maxBefore)
			{
				maxBefore = MPQ.GetMax();
				cout<< Buildings[i].xVal << " " << maxBefore << endl;
			}
		}
	}
	cin.get();
	cin.ignore();

	return 0;
}
