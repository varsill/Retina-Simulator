#pragma once
#include <vector>
using namespace std;
struct vec3 {
	typedef const vec3& i;
	typedef vec3& io;

	float x, y, z;
	vec3();
	vec3(float, float, float);
	vec3(float, float, float, float);
	
};

inline vec3 operator +  (vec3::i, vec3::i);//dodawanie
inline vec3 operator - (vec3::i, vec3::i);//odejmowanie
inline vec3 operator * (vec3::i, float);//mnozenie przez skalar
inline vec3 operator / (vec3::i , float);//dzielenie przez skalar
inline vec3 invert(vec3::i a);//odwracanie wektora
inline vec3 unitise(vec3::i a);//jednostkowanie wektora
inline float length(vec3::i a);//zwracanie dlugosci wektora
inline float distance(vec3::i p1, vec3::i p2);//obliczanie odleglosci miedzy punktami
inline float dotProd(vec3::i a, vec3::i b);//iloczyn skalarny
inline vec3 uCrossProd(vec3::i a, vec3::i b);//iloczyn wektorowy
inline vec3 uChangeSpace(vec3::i V, vec3::i Ex, vec3::i Ey, vec3::i Ez);//zamiana przestrzeni

int randint(int, int); // lowosowa liczba z podanego zakresu
bool procent(int); //ile procent na zwrocenie true

struct Ray
{
	vec3 origin; //poczatek
	vec3 dir;//wektor jednostkowy kierunku
	Ray(vec3, vec3);


};
struct Sfera
{
	int sign;//0 - rowna sie; 1-wiekszy; -1 - mniejszy
	vec3 o;//wspolrzedne srodka
	float radius;
	Sfera(vec3, float, bool);
	Sfera();
	vec3 intersect(Ray, bool&);
	bool is_inside(vec3);
};



class Photon : public Ray
{
	float length; //w nm;
};
void* intersect(Sfera, Ray);

struct Plane
{
	int sign;//0 - rowna sie; 1-wiekszy(za); -1 - mniejszy (przed)
	float a, b, c, d;
	vec3 normal;
	Plane(float, float, float, float, bool);//z rownania plaszczyzny Ax+By+Cz+D=0
	Plane();
	vec3 intersect(Ray, bool&);
	bool is_inside(vec3);
	vec3 rzutuj(vec3, bool&);//rzutuje punkt na plaszczyzne wzgledem wektora normalnego

};
bool Is_behind(Plane, vec3);//sprawdza czy punkt p jest za czy przed plaszczyzna
class Shape
{
protected:
	Plane plane;
	Sfera sfera;
	float index;//wspolczynnik zalamania swiatla
				//Absorbtion abs;//info na temat absorbcji spektrum promieniowania
public:
	Shape(Sfera, Plane, float); //Ksztalt zlozony z czesci wspolnej dwoch struktur
	bool is_inside(vec3);//sprawdza czy punkt nalezy do struktury ksztaltu
	vec3 intersect(Ray, bool&);
	Shape();
};
class Retina:public Shape
{
	Plane rzutnia;
	Retina(Sfera, Plane);
public:
	vec3 rzutuj(vec3, bool&);//rzutuje z zachowaniem odleglosci pomiedzy punkatmi na rzutni
};


class Oko
{
	
public:
	Oko();
	vector <Shape> elementy;
	bool trace_routte(Photon&, int);//zwroci prawde jezeli foton uderzyl w siatkowke lub falsz jezeli stalo sie cos innego - jezeli bedzie prawda, to stan fotonu na wyjsciu bedzie odpowiadał punktowi zderzenia z siatkówką
	
};
