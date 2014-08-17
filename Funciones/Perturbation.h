/*
 * Perturbation.h
 *
 *  Created on: 27/02/2011
 *      Author: rsantana
 */
#include "Routes.h"
#include "Input.h"
#include "Operators.h"

#ifndef PERTURBATION_H_
#define PERTURBATION_H_



class perturbation {
private:
public:
	static int ejection_chain(Input *in, Routes *solution);
	static int route_size(Input *in, Routes *solution, int route);
	static int Double_Bridge(Input *in, Routes *solution);
	static void Double_Bridge_Exec(Input *in, Routes *solution, int route);
	static void swaps(Input* in, Routes *solution);
	static void perturbate(Input* in, Routes* solution);
};

#endif /* PERTURBATION_H_ */
