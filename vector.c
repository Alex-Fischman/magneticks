#ifndef VECTOR
#define VECTOR

#include <math.h>

typedef struct Vector {
	double x, y, z;
} Vector;

double distance(Vector a, Vector b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

Vector add(Vector a, Vector b) {
	return (Vector) {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector sub(Vector a, Vector b) {
	return (Vector) {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector scale(Vector v, double s) {
	return (Vector) {v.x * s, v.y * s, v.z * s};
}

double length(Vector v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

Vector normalize(Vector v) {
	return scale(v, 1 / length(v));
}

#endif
