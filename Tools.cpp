
#include "Tools.h"
#include <math.h>
#include <typeinfo>
#include <time.h>
#include <cstdlib>
#includ <vector>


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
//mnozenie wektorow przez liczbe
inline vec3 operator*(vec3::i a, float t)
{
	return vec3(a.x * t, a.y *t, a.z *t);
}
//dzielenie wektorow przez liczbe
inline vec3 operator/(vec3::i a, float t)
{
	return vec3(a.x /t, a.y /t, a.z /t);
}
//odwracaie wektora
inline vec3 invert(vec3::i a)
{
	return vec3(-a.x, -a.y, -a.z);
}
//przeksztalcanie na wektor jednostkowy
inline vec3 unitise(vec3::i a)
{
	return vec3(a.x, a.y, a.z, 1.0f);
}
//zwracanie dlugosci wektora
inline float length(vec3::i a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
//zwracanie odleglosci pomiedzy punktami
inline float distance(vec3::i p1, vec3::i p2)
{
	return length(p2 - p1);
}
//iloczyn skalarny
inline float dotProd(vec3::i a, vec3::i b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}
//iloczyn wektorowy
inline vec3 uCrossProd(vec3::i a, vec3::i b)
{
	return vec3(
		a.y * b.z - b.y*a.z,
		a.z*b.x - b.z*a.x,
		a.x*b.y - b.x*a.y,
		1.0f);
}

//przeniesienie wektora V do przestrzeni zbudowanej na wektorach Ex, Ey, Ez oraz jego normalizacja
inline vec3 uChangeSpace(vec3::i V, vec3::i Ex, vec3::i Ey, vec3::i Ez) {
	return vec3(
		V.x*Ex.x + V.y*Ey.x + V.z*Ez.x,
		V.x*Ex.y + V.y*Ey.y + V.z*Ez.y,
		V.x*Ex.z + V.y*Ey.z + V.z*Ez.z,
		1.0f
	);
}
Sfera::Sfera(vec3 a, float r, bool s)
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
	//wspolczynniki rownania kwadratowego
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
Plane::Plane(float x, float y, float z, float w, bool s)
{
	a = x;
	b = y;
	c = z;
d = w;
sign = s;
normal = vec3(a, b, c);
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
		float result = (a*point.x + b*point.y + c*point.z + d) / absolute;//po podstawieniu do wzoru
		if (result == sign) return true;
		else return false;
	}
}
vec3 Plane::intersect(Ray ray, bool& kod)
{
	vec3 buf(a, b, c);
	float r = dotProd(ray.dir, buf);//UWAGA

	float t = (-d) / (r + ray.origin.x + ray.origin.y + ray.origin.z);
	if (t <= 0)
	{
		kod = false;
		return	vec3(0, 0, 0);
	}//UWAGA
	else
	{
		vec3 bufor(a*t, b*t, c*t);
		return bufor;
	}
}

Retina::Retina(Sfera sfera2, Plane powierzchnia)//Mozliwe zrodlo problemow xD
{
	sfera = sfera2;
	plane = powierzchnia;
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
			vec3 wynik = rzut_o + (dir*leng);//punkt na plaszczyznie rzutni
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
		wynik = plane.intersect(ray, kod); //do przemyslenia!
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
	if (elementy[i].is_inside(foton.origin) == false) return false;//poza okiem
	if (typeid(elementy[i]) == typeid(Retina)) return true; //trafilo w siatkowke
	vec3 norigin = elementy[i].intersect(foton, k); //okreslenie zrodla "nowego" fotonu
	vec3 ndir = zalamanie(); //okreslenie kierunku "nowego" fotonu po załamaniu
		float len = length(norigin - foton.origin);
		if(absorbcja(len)==true) return false;//jakas madra funkcja powodująca zniknięcie lub przejscie fotonu
		foton.dir = ndir;//przypisanie nowych wlasnosci fotonowi 
		foton.origin = norigin;
		return trace_routte(foton, i);
}

int randint(int pocz, int kon)
{
    srand( time( NULL ) );
    int amount = kon - pocz+1;

    return ((rand() % amount) + pocz);
}

bool procent(int howmuch,int accuracy = 100) 
{
    int wartosc;
    wartosc = randint(1,accuracy);
    if(wartosc > howmuch) return false;
    else return true;   
}

bool absorbcja(int len, struktura)
{
   int x; //wzorek liczacu prawdopodobienstwo 
   return procent(x,1000); 
}
