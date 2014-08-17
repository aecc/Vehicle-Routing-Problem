/*
 * Quicksort.cpp
 *
 *  Created on: 11/02/2011
 *      Author: aecc
 */

#include "Quicksort.h"

void Quicksort::runQuicksortRank(double * const arreglo, int * rank, int first, int last) {
	 int posicionActual;
	 if ( first >= last )
	    return;
	 posicionActual = Quicksort::particionRank( arreglo, rank, first, last );
	 Quicksort::runQuicksortRank( arreglo, rank, first, posicionActual - 1 );
	 Quicksort::runQuicksortRank( arreglo, rank, posicionActual + 1, last );

}

int Quicksort::particionRank( double * const arreglo, int * rank, int izquierda, int derecha )
{
  int posicion = izquierda;
  while ( true ) {
    while ( arreglo[ posicion ] <= arreglo[ derecha ] && posicion != derecha )
      --derecha;
    if ( posicion == derecha )
      return posicion;
    if ( arreglo[ posicion ] > arreglo[ derecha ]) {
    	Quicksort::swapRank( &arreglo[ posicion ], &arreglo[ derecha ], &rank[ posicion ], &rank[ derecha ] );
      posicion = derecha;
    }
    while ( arreglo[ izquierda ] <= arreglo[ posicion ] && izquierda != posicion )
      ++izquierda;
    if ( posicion == izquierda )
      return posicion;
    if ( arreglo[ izquierda ] > arreglo[ posicion ] ) {
    	Quicksort::swapRank( &arreglo[ posicion ], &arreglo[ izquierda ], &rank[ posicion ], &rank[ izquierda ]);
      posicion = izquierda;
    }
  }
}

void Quicksort::runQuicksortRank(int * const arreglo, int * rank, int first, int last) {
	 int posicionActual;
	 if ( first >= last )
	    return;
	 posicionActual = Quicksort::particionRank( arreglo, rank, first, last );
	 Quicksort::runQuicksortRank( arreglo, rank, first, posicionActual - 1 );
	 Quicksort::runQuicksortRank( arreglo, rank, posicionActual + 1, last );

}

int Quicksort::particionRank( int * const arreglo, int * rank, int izquierda, int derecha )
{
  int posicion = izquierda;
  while ( true ) {
    while ( arreglo[ posicion ] <= arreglo[ derecha ] && posicion != derecha )
      --derecha;
    if ( posicion == derecha )
      return posicion;
    if ( arreglo[ posicion ] > arreglo[ derecha ]) {
    	Quicksort::swapRank( &arreglo[ posicion ], &arreglo[ derecha ], &rank[ posicion ], &rank[ derecha ] );
      posicion = derecha;
    }
    while ( arreglo[ izquierda ] <= arreglo[ posicion ] && izquierda != posicion )
      ++izquierda;
    if ( posicion == izquierda )
      return posicion;
    if ( arreglo[ izquierda ] > arreglo[ posicion ] ) {
    	Quicksort::swapRank( &arreglo[ posicion ], &arreglo[ izquierda ], &rank[ posicion ], &rank[ izquierda ]);
      posicion = izquierda;
    }
  }
}

void Quicksort::runQuicksortCosts(int * const arreglo, double * costs, int first, int last) {
	 int posicionActual;
	 if ( first >= last )
	    return;
	 posicionActual = Quicksort::particion( arreglo, costs, first, last );
	 Quicksort::runQuicksortCosts( arreglo, costs, first, posicionActual - 1 );
	 Quicksort::runQuicksortCosts( arreglo, costs, posicionActual + 1, last );

}

int Quicksort::particion( int * const arreglo, double * costs, int izquierda, int derecha )
{
  int posicion = izquierda;
  while ( true ) {
    while ( costs[ posicion ] <= costs[ derecha ] && posicion != derecha )
      --derecha;
    if ( posicion == derecha )
      return posicion;
    if ( costs[ posicion ] > costs[ derecha ]) {
    	Quicksort::swap( &arreglo[ posicion ], &arreglo[ derecha ], &costs[ posicion ], &costs[ derecha ] );
      posicion = derecha;
    }
    while ( costs[ izquierda ] <= costs[ posicion ] && izquierda != posicion )
      ++izquierda;
    if ( posicion == izquierda )
      return posicion;
    if ( costs[ izquierda ] > costs[ posicion ] ) {
    	Quicksort::swap( &arreglo[ posicion ], &arreglo[ izquierda ], &costs[ posicion ], &costs[ izquierda ]);
      posicion = izquierda;
    }
  }
}

void Quicksort::swap( int * const ptr1, int * const ptr2, double * const ptr11, double * const ptr22 )
{
  int temp;
  double temp2;
  temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
  temp2 = *ptr11;
  *ptr11 = *ptr22;
  *ptr22 = temp2;
}

void Quicksort::swapRank( int * const ptr1, int * const ptr2, int * const ptr11, int * const ptr22 )
{
  int temp;
  int temp2;
  temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
  temp2 = *ptr11;
  *ptr11 = *ptr22;
  *ptr22 = temp2;
}

void Quicksort::swapRank( double * const ptr1, double * const ptr2, int * const ptr11, int * const ptr22 )
{
  double temp;
  int temp2;
  temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
  temp2 = *ptr11;
  *ptr11 = *ptr22;
  *ptr22 = temp2;
}

Quicksort::Quicksort() {

}

Quicksort::~Quicksort() {

}
