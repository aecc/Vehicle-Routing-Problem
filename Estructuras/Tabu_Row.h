/*
 * Tabu_Row.h
 *
 *  Created on: 11/04/2011
 *      Author: aecc
 */

#ifndef TABU_ROW_H_
#define TABU_ROW_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Tabu_Row {
public:
	static const int size = 7;
	int node1;
	int node2;
	int node3;
	int node4;
	int type;
	int route1;
	int route2;
	void print();
	Tabu_Row();
	Tabu_Row(int,int,int,int,int,int=-1,int=-1);
	virtual ~Tabu_Row();
};

#endif /* TABU_ROW_H_ */
