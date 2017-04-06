#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"

#define min(x,y) (x<y ? x : y)
#define max(x,y) (x>=y ? x : y)

Point2D PointXY(float x, float y)
{
	Point2D point;
	point.x = x;
	point.y = y;

	return point;
}

Vector2D VectorXY(float x, float y)
{
	Vector2D vector;
	vector.x = x;
	vector.y = y;

	return vector;
}

Vector2D Vector(Point2D A, Point2D B)
{
	Vector2D vector;
	vector.x = B.x - A.x;
	vector.y = B.y - A.y;

	return vector;
}

Point2D PointPlusVector(Point2D P, Vector2D V)
{
	Point2D point;
	point.x = P.x + V.x;
	point.y = P.y + V.y;

	return point;
}

Vector2D AddVectors(Vector2D V, Vector2D W)
{
	Vector2D vector;
	vector.x = V.x + W.x;
	vector.y = V.y + W.y;

	return vector;
}

Vector2D SubVectors(Vector2D V, Vector2D W)
{
	Vector2D vector;
	vector.x = V.x - W.x;
	vector.y = V.y - W.y;

	return vector;
}

Vector2D MultVector(Vector2D V, float scalaire)
{
	Vector2D vector;
	vector.x = V.x * scalaire;
	vector.y = V.y * scalaire;

	return vector;
}

Vector2D DivVector(Vector2D V, float scalaire)
{
	Vector2D vector;
	vector.x = V.x / scalaire;
	vector.y = V.y / scalaire;

	return vector;
}

float DotProduct(Vector2D V, Vector2D W)
{
	return (V.x * W.x) + (V.y * W.y);
}

float Norm(Vector2D V)
{
	return sqrt((V.x * V.x) + (V.y * V.y));
}

Vector2D Normalize(Vector2D V)
{
	float norm = Norm(V);
	Vector2D vector;
	vector.x = V.x / norm;
	vector.y = V.y / norm;

	return vector;
}

