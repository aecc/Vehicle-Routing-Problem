/*
 * Quicksort.h
 *
 *  Created on: 11/02/2011
 *      Author: aecc
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

class Quicksort {
public:
	static void runQuicksortCosts(int * const arreglo, double * costs, int first, int last);
	static int particion( int * const arreglo,  double * costs, int izquierda, int derecha );
	static void swap( int * const ptr1, int * const ptr2, double * const ptr11, double * const ptr22 );
	static void runQuicksortRank(int * const arreglo, int * rank, int first, int last);
	static int particionRank( int * const arreglo,  int * rank, int izquierda, int derecha );
	static int particionRank( double * const arreglo,  int * rank, int izquierda, int derecha );
	static void runQuicksortRank(double * const arreglo, int * rank, int first, int last);
	static void swapRank( int * const ptr1, int * const ptr2, int * const ptr11, int * const ptr22 );
	static void swapRank( double * const ptr1, double * const ptr2, int * const ptr11, int * const ptr22 );

	Quicksort();
	virtual ~Quicksort();
};

#endif /* QUICKSORT_H_ */
