#pragma once


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


struct Sfera
{
	vec3 o;//wspolrzedne srodka
	float radius;
	Sfera(vec3, float);

};

struct Ray
{
	vec3 origin; //poczatek
	vec3 dir;//wektor jednostkowy kierunku
	Ray(vec3, vec3);


};
float intersect(Sfera, Ray);
