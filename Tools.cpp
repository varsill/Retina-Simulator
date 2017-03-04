
#include "Tools.h"
#include <math.h>
#include <typeinfo>
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
Sfera::Sfera(vec3 a, float r)
{
	o = a;
	radius = r;
}
Sfera::Sfera()
{
	o = vec3();
	radius = 0;
}
Ray::Ray(vec3 a, vec3 b)
{
	origin = a;
	dir = unitise(b);
}

inline void* intersect_sphere(Sfera sfera, Ray ray)
{
	//wspolczynniki rownania kwadratowego
	float b = 2*(ray.dir.x*(ray.origin.x - sfera.o.x) + ray.dir.y*(ray.origin.y - sfera.o.y) + ray.dir.z*(ray.origin.z - sfera.o.z));
	float c = pow(ray.origin.x - sfera.o.x, 2) + pow(ray.origin.y - sfera.o.y, 2) + pow(ray.origin.z - sfera.o.z, 2) - pow(sfera.radius, 2);
	float delta = b*b - 4 * c;
	if (delta <= 0) return (bool*) false;
	else
	{
		float t1 = (-b - sqrt(delta))/2;
		float t2 = (-b + sqrt(delta)) / 2;
		float* r;
		if (distance(vec3(), ray.dir*t1)) r=&t1;
		else r=&t2;
		return r;
	}
}


Plane::Plane(float x, float y, float z, float w)
{
	a = x;
	b = y;
	c = z;
	d = w;
}
Plane::Plane()
{
	a = b = c = d = 0;
}
bool Is_behind(Plane plane, vec3 p)//sprawdza czy punkt leży za płaszczyzną licząć od punktu (0, 0, 0)
{
	float n = p.x*plane.a + p.y*plane.b + p.z*plane.c + plane.d;
	if (n >= 0) return true;//punkt jest za plaszczyzna
	else return false;//punkt jest przed plaszczyzna
}

Retina::Retina(Sfera sfera, Plane powierzchnia)//Mozliwe zrodlo problemow xD
{
	sphere = sfera;
	plane = powierzchnia;
}
void* Retina::intersect(Photon foton)//sprawdzanie przeciecia z siatkowka - zwraca wskaznik na vec3 ze wspolrzednymi przeciecia lub zwraca wskaznik na zmienna boolean z typem false; typeid(result);
{
	void* t = intersect_sphere(this->sphere, (Ray)foton);
	if (typeid(t) == typeid(bool)) return (bool*)false;//zwroc falsz
	else
	{
		vec3 pom = foton.dir * *(float*)t + foton.origin;//punkt p
		if (Is_behind(this->plane, pom) == false) return (bool*)false;//zwroc falsz
		else return (vec3*)&pom;//zwroc wspolrzedne punktu przeciecia
	}
}
