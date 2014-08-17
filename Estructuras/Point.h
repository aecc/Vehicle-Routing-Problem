/*
 * Point.h
 *
 *  Created on: 23/04/2011
 *      Author: aecc
 */

#ifndef POINT_H_
#define POINT_H_
#include <math.h>

class Point {
public:
	Point();
	Point(int,int);
	static double calcDistance(Point&, Point&);
	int x;
	int y;
	virtual ~Point();
};

#endif /* POINT_H_ */
