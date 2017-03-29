#pragma once
#include <vector>
using namespace std;
int randint(int, int); // random number from particular range
bool procent(int, int);//returns true with probability of first parameter

struct vec3 {
	typedef const vec3& i;
	typedef vec3& io;

	float x, y, z;
	vec3();
	vec3(float, float, float);
	vec3(float, float, float, float);
	
};

inline vec3 operator +  (vec3::i, vec3::i);//addition
inline vec3 operator - (vec3::i, vec3::i);//substraction
inline vec3 operator * (vec3::i, float);//scalar multiplication 
inline vec3 operator / (vec3::i , float);//scalar division
inline vec3 invert(vec3::i a);//inversion of vector
inline vec3 unitise(vec3::i a);//unitising vector
inline float length(vec3::i a);//return lenght of vector
inline float distance(vec3::i p1, vec3::i p2);//calculates distance between points
inline float dotProd(vec3::i a, vec3::i b);// dot product
inline vec3 uCrossProd(vec3::i a, vec3::i b);// cross product

struct Ray
{
	vec3 origin; //point, where ray starts
	vec3 dir;//unitasied vector of direction
	Ray(vec3, vec3);
	Ray();

};
struct Sfera
{
	int sign;//0 - equals; 1-greater; -1 - less
	vec3 o;//coordinants of center
	float radius;
	Sfera(vec3, float, int);
	Sfera();
	vec3 intersect(Ray, bool&);
	bool is_inside(vec3);
};



class Photon : public Ray
{
public:
	float length; //using nanometers;
	Photon(vec3, vec3);
	Photon();
};
	
void* intersect(Sfera, Ray);

struct Plane
{
	int sign;//0 - equals; 1-greater(behind plane); -1 - less
	float a, b, c, d;
	vec3 normal;
	vec3 par1;//parallel vector of direction
	vec3 par2;//parallel vector of direction
	Plane(float, float, float, float, int);//using plain formula  Ax+By+Cz+D=0
	Plane();
	vec3 intersect(Ray, bool&);
	bool is_inside(vec3);
	vec3 rzutuj(vec3, bool&);//projects point according to the normal vecotr
	
};
class Shape
{
public:
	Plane plane;
	Sfera sfera;
	float index;//refraction index
				
public:
	Shape(Sfera, Plane, float); //common part of sphere and plane
	bool is_inside(vec3);//checks, whether point is inside the shape
	vec3 intersect(Ray, bool&);
	Shape();
};
class Retina:public Shape
{
	
public:
	Plane rzutnia;
	Retina(Sfera, Plane);
	vec3 rzutuj(vec3, bool&);//projects point on plain, conservating the distance between points according to the center of plain
};


class Oko
{
	
public:
	Oko();
	vector <Shape> elementy;
	bool trace_routte(Photon&, int);//returns true when hitted retina and false when missed retina - in case of hit, photon at the end will represent information about point of intersection
	
};
struct Cords
{
	int x, y;
	Cords(int, int);
	Cords();
};
struct Krata: public Plane
{
	vec3 par1, par2;//vectors parallel to plain
	vec3 origin;//origin, according to which we calculate coordinants
	float density;//distance beetween following  grilles
	Krata();
	Krata(float, vec3, Plane);
	Cords wkratuj(vec3 p);//returns coordinants of grill which is the nearest grill from point p
};
struct Funkcja
{
	vector <double> wsp;//array of coefficients
	float wartosc(float);
	void calkuj();//integrates 
};

//int xfun,yfun;
struct Tree //structure to keep information about flashes in particular photocell
{
	int x, y, left, right, ile;
	Tree(int xpod, int ypod)
	{
		x = xpod;
		y = ypod;
		left = 0;
		right = 0;
		ile = 1;
	}

};
bool CREATE(int, string);
void Dodaj(int, int, int n = 0); //adding photon hit to particular photocell
int Wypisz(int); //printing information about number of visible flashes
