/*
 * Point.cpp
 *
 * Clase que representa un punto en el mapa cartesiano con coordenadas X e Y
 *
 *  Created on: 23/04/2011
 *      Author: aecc
 */

#include "Point.h"

Point::Point(int x1, int y1) {
	x = x1;
	y = y1;
}

Point::Point(){}

/*
 * Calcula distancia euclediana entre dos puntos
 */
double Point::calcDistance(Point& p1, Point& p2) {
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}


Point::~Point() {
}
