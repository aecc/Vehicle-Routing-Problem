/*
 * Tabu_Queue.h
 *
 *  Created on: 11/04/2011
 *      Author: aecc
 */

#ifndef TABU_QUEUE_H_
#define TABU_QUEUE_H_
#include <list>
#include "Tabu_Row.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Tabu_Queue {
public:
	static const int relocate = 1;
	static const int swap = 2;
	static const int interchange1 = 3;
	static const int interchange2 = 4;
	list<Tabu_Row> queue;
	Tabu_Queue(int);
	void push(Tabu_Row);
	void print();
	bool find(Tabu_Row);
	int getSize();
	virtual ~Tabu_Queue();
private:
	int tabu_tenure;
};

#endif /* TABU_QUEUE_H_ */
