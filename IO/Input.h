/*
 * Input.h
 *
 *  Created on: 16/01/2011
 *      Author: aecc
 */

#ifndef INPUT_H_
#define INPUT_H_
#include "Point.h"

class Input {
public:
	double capacity;
	int ncustomers;
	double * pickups;
	double * deliveries;
	Point * positions;
	double ** cost;
	Input(int, double);
	virtual ~Input();
};

#endif /* INPUT_H_ */
