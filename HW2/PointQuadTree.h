#include <iostream>
#include <vector>
using namespace std;
struct quadrant 
{
  quadrant *SW, *NW, *SE, *NE;
  string city_name;
  float x_cord, y_cord;
  quadrant (quadrant* SW, quadrant *NW, quadrant *SE, quadrant *NE, string city, float x, float y) : 
	  SW(SW), NW(NW), SE(SE), NE(NE), city_name(city) , x_cord(x), y_cord(y) {}

};


class PointQuadTree{	
private:
	quadrant *root;
	int x_length, y_length;
	void MakeEmpty(quadrant *&t);
	void pretty_print(quadrant *t);
	void compare( quadrant *&t, string city, int x, int y);
	void search_compare(quadrant *&t , int x, int y , int radius, vector<string> &been, vector<string> &found);
	
public: 
	PointQuadTree();
	~PointQuadTree();		
	void pretty_print();
	void insert(string city, int x, int y);
	void search (int x, int y, int radius);
	

	

};