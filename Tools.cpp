
#include "Tools.h"
#include <math.h>
#include <typeinfo>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#define EPS 0.001
#define PHOTOCELLSIZE 1
#define ACTIVATIONNUMBER 10 
using namespace std;
vector <Tree> tab;
vec3::vec3()
{
	x = y = z = 0.0f;
}

vec3::vec3(float x2, float y2, float z2) : x(x2), y(y2), z(z2) 
{

}

vec3::vec3(float x2, float y2, float z2, float len) :x(x2), y(y2), z(z2)
{
	float newLen = len / sqrt(x*x + y*y + z*z);
	x *= newLen; y *= newLen; z *= newLen;
}

inline vec3 operator+(vec3::i a, vec3::i b)
{
	return vec3(a.x+b.x,a.y+b.y, a.z+b.z);
}


inline vec3 operator-(vec3::i a, vec3::i b)
{
	return vec3(a.x- b.x, a.y - b.y, a.z - b.z);
}
inline vec3 operator*(vec3::i a, float t)
{
	return vec3(a.x * t, a.y *t, a.z *t);
}

inline vec3 operator/(vec3::i a, float t)
{
	return vec3(a.x /t, a.y /t, a.z /t);
}

inline vec3 invert(vec3::i a)
{
	return vec3(-a.x, -a.y, -a.z);
}

inline vec3 unitise(vec3::i a)
{
	return vec3(a.x, a.y, a.z, 1.0f);
}

inline float length(vec3::i a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

inline float distance(vec3::i p1, vec3::i p2)
{
	return length(p2 - p1);
}

inline float dotProd(vec3::i a, vec3::i b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline vec3 uCrossProd(vec3::i a, vec3::i b)
{
	return vec3(
		a.y * b.z - b.y*a.z,
		a.z*b.x - b.z*a.x,
		a.x*b.y - b.x*a.y,
		1.0f);
}

inline vec3 uChangeSpace(vec3::i V, vec3::i Ex, vec3::i Ey, vec3::i Ez) {
	return vec3(
		V.x*Ex.x + V.y*Ey.x + V.z*Ez.x,
		V.x*Ex.y + V.y*Ey.y + V.z*Ez.y,
		V.x*Ex.z + V.y*Ey.z + V.z*Ez.z,
		1.0f
	);
}
Sfera::Sfera(vec3 a, float r, int s)
{
	o = a;
	radius = r;
	sign = s;
}
Sfera::Sfera()
{
	o = vec3();
	radius = 0;
	sign = 0;
}
Ray::Ray(vec3 a, vec3 b)
{
	origin = a;
	dir = unitise(b);
}

vec3 Sfera::intersect(Ray ray, bool &kod)
{
	kod = true;
	//coefficients of quadratic equation
	float b = 2*(ray.dir.x*(ray.origin.x - o.x) + ray.dir.y*(ray.origin.y - o.y) + ray.dir.z*(ray.origin.z - o.z));
	float c = pow(ray.origin.x - o.x, 2) + pow(ray.origin.y - o.y, 2) + pow(ray.origin.z - o.z, 2) - pow(radius, 2);
	float delta = b*b - 4 * c;
	if (delta <= 0) {
		kod = false;
		return vec3(0,0,0);
	}
	else
	{
		float t1 = (-b - sqrt(delta))/2;
		float t2 = (-b + sqrt(delta)) / 2;
		vec3 bufor = ray.origin + (ray.dir*t2);
		return bufor;
	}
}
bool Sfera::is_inside(vec3 point)
{
	float r = pow((o.x - point.x), 2) + pow((o.y - point.y), 2) + pow((o.y - point.y), 2);
	if (r == radius) return true;
	else if (r < radius&&sign == -1) return true;
	else if (r > radius&&sign == 1) return true;
	else return false;
}
Plane::Plane(float x, float y, float z, float w, int s)
{
	a = x;
	b = y;
	c = z;
d = w;
sign = s;
normal = vec3(a, b, c);
}
Photon::Photon(vec3 a, vec3 b)
{
	origin = a;
	dir = unitise(b);
}
Plane::Plane()
{
	a = b = c = d = 0;
	sign = 0;
	normal = vec3(0, 0, 0);
}
vec3 Plane::rzutuj(vec3 p, bool& kod)
{
	kod = true;
	Ray ray(p, unitise(normal));
	intersect(ray, kod);
	if (kod != false) return intersect(ray, kod);
	else
	{
		kod = false;
		return vec3(0, 0, 0);
	}
}
bool Plane::is_inside(vec3 point)
{
	float absolute = abs(a*point.x + b*point.y + c*point.z + d);
	if (absolute == 0) return true;
	else
	{
		float result = (a*point.x + b*point.y + c*point.z + d) / absolute;
		if (result == sign) return true;
		else return false;
	}
}
vec3 Plane::intersect(Ray ray, bool& kod)
{
	vec3 buf(a, b, c);
	float r = dotProd(ray.dir, buf);//!

	float t = (-d) / (r + ray.origin.x + ray.origin.y + ray.origin.z);
	if (t <= 0)
	{
		kod = false;
		return	vec3(0, 0, 0);
	}//!
	else
	{
		vec3 bufor(a*t, b*t, c*t);
		return bufor;
	}
}

Retina::Retina(Sfera sfera2, Plane powierzchnia)
{
	sfera = sfera2;
	plane = powierzchnia;
}
Ray::Ray()
{

}
Photon::Photon()
{

}
vec3 Retina::rzutuj(vec3 p, bool& kod)
{
	vec3 wynik = rzutnia.rzutuj(sfera.o, kod);

	if (kod == false) return vec3(0, 0, 0);
	else
	{
		vec3 rzut_o = wynik;

		wynik = rzutnia.rzutuj(p, kod);
		if (kod == false) return vec3(0, 0, 0);
		else
		{
			float leng = length(p - sfera.o);
			vec3 loc = wynik;
			vec3 dir = unitise(loc - rzut_o);
			vec3 wynik = rzut_o + (dir*leng);
			return wynik;//point on the plain of projector
		}
	}
}
Shape::Shape()
{

}
Shape::Shape(Sfera s, Plane p, float f)
{
	index = f;
	sfera = s;
	plane = p;
}
vec3 Shape::intersect(Ray ray, bool& kod)
{
	vec3 wynik = sfera.intersect(ray, kod);
	if (kod == true) return wynik;
	else
	{
		wynik = plane.intersect(ray, kod); 
		if (kod == true) return wynik;
		else return vec3(0, 0, 0);
	}
}
bool Shape::is_inside(vec3 p)
{
	if ((sfera.is_inside(p) == true) && (plane.is_inside(p) == true)) return true;
	else return false;
}
Oko::Oko()
{

}
bool Oko::trace_routte(Photon& foton, int pop=0)
{
	bool k;
	int i;
	for (i= 0; i < elementy.size(); i++)
	{
		if (elementy[i].is_inside(foton.origin)&&i!=pop) break;
	}
	if (elementy[i].is_inside(foton.origin) == false) return false;//outside of eyeball
	if (typeid(elementy[i]) == typeid(Retina)) return true; //retina was hit
	vec3 norigin = elementy[i].intersect(foton, k); //new source of photon

		float len = length(norigin - foton.origin);
		foton.origin = norigin;
		return trace_routte(foton, i);
}
int randint(int pocz, int kon)
{
	srand(time(NULL));
	int amount = kon - pocz + 1;

	return ((rand() % amount) + pocz);
}

bool procent(int howmuch, int accuracy = 100)
{
	int wartosc;
	wartosc = randint(1, accuracy);
	if (wartosc > howmuch) return false;
	else return true;
}


Krata::Krata()
{

}
Krata::Krata(float density, vec3 o, Plane pl)
{
	a = pl.a;
	b = pl.b;
	c = pl.c;
	density = pl.d;
	normal = pl.normal;
	sign = 0;//always plain
	density = d;
	origin = o;
}
Cords::Cords()
{

}
Cords::Cords(int a, int b)
{
	x = a;
	y = b;
}
Cords Krata::wkratuj(vec3 p)
{
	//Returns coordinants of point 
	/*float W = -1 * ((par1.y*par2.x)+(par1.x*par2.y)); //wyznacznik glowny
		float Wt = (p.x - origin.x)*(-1)*(par2.y) + par2.x*(p.y - origin.y);//wyznacznik na t
		float Wd = par1.x*(p.y - origin.y) - par1.y*(p.x - origin.x);//wyznacznik na d
		int ta = ceil((Wt / W)/density);
		int da = ceil((Wd / W)/density);*/
	int ta = abs(ceil(length(vec3(p.x-origin.x, 0, 0))));
	int da = abs(ceil(length(vec3(p.y-origin.y, 0, 0))));

		return Cords(ta, da);

}
float Funkcja::wartosc(float x)
{
	float wartosc = 0;
	for (int i = 0; i < wsp.size(); i++)
	{
		wartosc += wsp[i] * pow(x, i);
	}
	return wartosc;
}
void Funkcja::calkuj()
{
	
	wsp.push_back(0);
	for (int i = wsp.size(); i > 0; i--)
	{
		wsp[i] = wsp[i - 1] / i;
	}
	wsp[0] = 0;//constant value
}
bool CREATE(int SIZE, string path)//SIZE is radipus of eyball, in adequate unit
{
	fstream file;
	file.open(path, ios::in);
	vector<Photon> photons;
	float a, b, c;//buffors
	while (!file.eof())
	{

		file >> a;
		file >> b;
		file >> c;
		photons.push_back(Photon(vec3(a, b, c), unitise(vec3(0, 0, 1))));
	}
	Oko oko;
	Cords kordy;
	vec3 p;
	float distance;
	Funkcja function;
	function.wsp.push_back(31.732);//interpolating the polynominal function
	function.wsp.push_back(-8.8622);
	function.wsp.push_back(0.7212);
	function.wsp.push_back(-0.0199);
	function.wsp.push_back(0.0002);
	function.wsp.push_back(-0.000001);
	function.wsp.push_back(0.000000003);


	function.calkuj();
	Retina retina(Sfera(vec3(0, 0, 0), SIZE, -1), Plane(0, 1, 0,SIZE/2, 1));//wielkosc w mikrometrach
	Plane plane;
	float prob;//probability of being hitted
	int number;//number of photoreceptors
	Krata krata(10, vec3(0, SIZE/2, SIZE+1), retina.plane);
	retina.rzutnia = Plane(0, 1, 0, SIZE + 1, 0);
	bool kod;
	oko.elementy.push_back(retina);
	for (int i = 0; i < photons.size(); i++)
	{
		if (oko.trace_routte(photons[i])) {

		p =	retina.rzutnia.rzutuj(photons[i].origin, kod);
			if (kod) {
				if (krata.origin.x - p.x > 0)
				{
					distance = -1* length(p - krata.origin);
				}else distance = length(p - krata.origin);
				
				if (function.wartosc(distance) > 0)
				{
					distance = abs(distance);
					number = abs(function.wartosc(distance + EPS) - function.wartosc(distance - EPS));
					prob = number*PHOTOCELLSIZE / (3.14*((2 * distance*EPS)));
					if (procent(ceil(prob)))
					{
						kordy = krata.wkratuj(p);
						Dodaj(kordy.x, kordy.y);
					}
				}
			}
		}
		else return 0;
	}
	cout << Wypisz(ACTIVATIONNUMBER);//
	return 1;
}
void Dodaj(int x, int y, int n)
{
	if (x > tab[n].x)//***************************************************
	{
		if (tab[n].left)
			Dodaj(x, y, tab[n].left);
		else
		{
			tab[n].left = tab.size();
			tab.push_back(Tree(x, y));
		}
	}
	else if (x < tab[n].x)//***************************************************
	{
		if (tab[n].right)
			Dodaj(x, y, tab[n].right);
		else
		{
			tab[n].right = tab.size();
			tab.push_back(Tree(x, y));
		}
	}
	if (x == tab[n].x)//***************************************************
	{
		if (y > tab[n].y)
		{
			if (tab[n].left)
				Dodaj(x, y, tab[n].left);
			else
			{
				tab[n].left = tab.size();
				tab.push_back(Tree(x, y));
			}
		}
		else if (y < tab[n].y)
		{
			if (tab[n].right)
				Dodaj(x, y, tab[n].right);
			else
			{
				tab[n].right = tab.size();
				tab.push_back(Tree(x, y));
			}
		}
		else
			tab[n].ile++;
	}
}


int Wypisz(int lim)
{
	int pom = 0;
	for (unsigned int i = 1; i<tab.size(); i++)
		if (tab[i].ile >= lim)
			pom++;
	return pom;
}
