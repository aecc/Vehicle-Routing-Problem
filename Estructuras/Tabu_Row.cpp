/*
 * Clase dedicada a incluir informacion de una fila en la tabla Tabu
 * Tabu_Row.cpp
 *
 *  Created on: 11/04/2011
 *      Author: aecc
 */

#include "Tabu_Row.h"
using namespace std;

Tabu_Row::Tabu_Row() {
	node1 = -1;
	node2 = -1;
	node3 = -1;
	node4 = -1;
	type = -1;
	route1 = -1;
	route2 = -1;
}

Tabu_Row::Tabu_Row(int n1, int n2, int r1, int r2, int t, int n3, int n4) {
	node1 = n1;
	node2 = n2;
	node3 = n3;
	node4 = n4;
	type = t;
	route1 = r1;
	route2 = r2;
}

void Tabu_Row::print() {
	cout << "FILA TABU: node1: " << node1 << " node2: " << node2 << " route1: " << route1 << " route2: " << route2 << " type: " << type << " node3: " << node3 << " node4: " << node4 << endl;
}

Tabu_Row::~Tabu_Row() {
}
