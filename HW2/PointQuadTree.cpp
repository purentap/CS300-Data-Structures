#include "PointQuadTree.h"
#include <iostream>
#include <sstream>
#include<vector>
using namespace std;

PointQuadTree::PointQuadTree() //constructor
{
	root = NULL;
	x_length = 0;
	y_length =0;

}
void PointQuadTree::MakeEmpty(quadrant *&t) //recursively deletes elements of the tree
{
	if (t!=NULL)
	{
		MakeEmpty(t->NE);
		MakeEmpty(t->NW);
		MakeEmpty(t->SE);
		MakeEmpty(t->SW);
		delete t;
	}
	t = NULL;
}

PointQuadTree::~PointQuadTree() //destructor
{
	MakeEmpty(root);

}

void PointQuadTree::pretty_print()
{
	pretty_print(root);
}
void PointQuadTree::pretty_print(quadrant *t) //prints the tree recursively
{
	if(t!=NULL)
	{
		cout << t->city_name << endl ;
		pretty_print(t->SE);
		pretty_print(t->SW);
		pretty_print(t->NE);
		pretty_print(t->NW);
	
	}
	
}

void PointQuadTree::compare( quadrant *&t, string city, int x, int y) 
{//inserts the given city with x and y values to the available place.
  
	
	if(t!=NULL)
	{
	if(x< t->x_cord && y<t->y_cord)
		{quadrant *father = t;
		compare(t->SW,city, x, y);
	}

	else if(x< t->x_cord && y> t->y_cord)
	{
		quadrant *father = t;
		compare(t->NW,city, x,y);
	}
	
	else if(x> t->x_cord && y< t->y_cord)
	{
	    quadrant *father = t;
		compare(t->SE ,city, x,y);
	}
	
	else if(x > t->x_cord && y> t->y_cord)
	{
		quadrant *father = t;
		compare(t->NE,city, x, y);
	}
	else if ( x== t->x_cord && y < t->y_cord)
	{
	
		compare(t->SE,city,x,y);
	}
	else if( x == t->x_cord && y> t->y_cord)
	{
		compare(t->NE,city,x,y);
	}
	else if (x< t->x_cord && y == t->y_cord)
	{
		compare(t-> NW,city,x,y);
	}
	else if ( x> t->x_cord && y== t->y_cord)
	{
		compare(t-> NE,city,x,y);
	}
	}
	else
		  t = new quadrant(NULL,NULL,NULL,NULL, city, x,y);
		

}

void PointQuadTree::insert(string city,int x,int y)
{
	if(x_length == 0 && y_length == 0 ) // str length check et
	{
		x_length = x;
		y_length = y;
	}
	else if( x <= x_length && y<= y_length)
	{
		
		if(root == NULL)
			root = new quadrant(NULL,NULL,NULL,NULL,city, x,y);

		else
		{

			compare(root, city, x, y);
			//pretty_print();
		}
		
	

	}
}
void PointQuadTree::search_compare(quadrant *&t , int x, int y , int radius, vector<string> &been, vector<string> &found)
{
	float distance = 0;
	float x_lower = x-radius; // boundaries of the square
	float x_upper = x+radius;
	float y_upper = y+radius;
	float y_lower = y-radius;
	if(t!=NULL)
	{
		been.push_back(t->city_name);
		distance = sqrt((t->x_cord - x)*(t->x_cord - x) + (t->y_cord -y )*(t->y_cord -y ));
	
		if(distance <= radius )
		found.push_back(t->city_name);
	    if(distance > radius)
		{
		if(t->x_cord < x_lower )
		{ if(t->y_cord > y_upper)//1 
			search_compare(t->SE , x,y,radius, been,found); 
		else if( t->y_cord< y_upper && t->y_cord > y_lower) //4
		{
			search_compare(t->SE, x,y,radius,been,found);
			search_compare(t->NE , x,y, radius,been, found);
		}
		else if(t->y_cord < y_lower) //6
			search_compare(t->NE, x,y,radius,been, found);
		}
		else if (t->x_cord > x_upper)
		{
			if(t->y_cord > y_upper) //3
				search_compare(t->SW, x,y,radius,been, found);
			else if( t->y_cord > y_lower && t->y_cord < y_upper) //5 
			{
				search_compare(t->SW, x,y,radius,been,found);
				search_compare(t->NW, x,y,radius, been, found);
			}
			else if(t->y_cord < y_lower)  //8
				search_compare(t->NW, x,y,radius,been,found);
		}
		else if ( t->x_cord >x_lower && t->x_cord < x_upper)
		{
			 
			 
			 if(t->y_cord > y_upper) //2
			{
				search_compare(t->SE, x,y,radius,been,found);
				search_compare(t->SW, x,y,radius,been,found);
			}
			else if( t->y_cord < y_lower) //7
			{
				search_compare(t->NE, x,y,radius,been,found);
				search_compare(t->NW, x,y,radius,been,found);
			}
			else if(t->y_cord > y && distance > radius) 
			{
				if(t->x_cord< x)//9
				{
					search_compare(t->SE, x,y,radius,been,found);
					search_compare(t->SW, x,y,radius,been,found);
					search_compare(t->NE, x,y,radius,been,found);
				
				
				}
				else if (t->x_cord> x) //10
				{
					search_compare(t->SE, x,y,radius,been, found);
					search_compare(t->SW, x,y,radius,been,found);
					search_compare(t->NW, x,y,radius,been,found);
					
					
				}

			}
			else if(t->y_cord < y && distance > radius)
			{
				if ( t->x_cord < x) //11
				{
					search_compare(t->SE, x,y,radius,been, found);
					search_compare(t->NE, x,y,radius,been, found);
					search_compare(t->NW, x,y,radius, been, found);
					
				}
				if ( t->x_cord > x ) // 12
				{
					search_compare(t->SW, x,y,radius,been,found);
					search_compare(t->NE, x,y,radius,been, found);
					search_compare(t->NW, x,y,radius, been, found);
					
				}

			}
		}
		}
		else //13
		{
			search_compare(t->SE, x,y,radius,been, found);
			search_compare(t->SW, x,y,radius,been,found);
			search_compare(t->NE, x,y,radius,been, found);
			search_compare(t->NW, x,y,radius, been, found);
			
			
		}
	}
}
void PointQuadTree::search(int x, int y, int radius)
{
	vector<string> been;
	vector<string> found;
	search_compare(root,x,y,radius,been,found);
	cout<< "\n\n";
	if(found.size() == 0)
		cout<<"<None>" << endl;
	else
	{
		for (int i = 0 ; i < found.size() ; i++) //prints the found cities
		{
			cout << found.at(i) ;
			if(i != found.size()-1)
				cout<<", " ; 
		}
		cout<< endl;
	}
	for(int i= 0; i< been.size() ; i++) //prints the been cities.
		{
			cout<< been.at(i) ;
			if(i != been.size()-1)
				cout<<", " ; 
		}
	
}
