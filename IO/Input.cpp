/*
 * Input.cpp
 *
 *  Created on: 16/01/2011
 *      Author: aecc
 */

#include "Input.h"

Input::Input(int n_customers, double capac) {
	capacity = capac;
	ncustomers = n_customers;
	pickups = new double[ncustomers+1];
	deliveries = new double[ncustomers+1];
	deliveries[0] = 0.0;
	pickups[0] = 0.0;
	cost = new double*[ncustomers+1];
			for (int i=0;i<ncustomers+1;i++)
					cost[i] = new double[ncustomers+1];
	positions = new Point[ncustomers+1];

}

Input::~Input() {
}
