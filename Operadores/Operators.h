/*
 * Operators.h
 *
 *  Created on: 25/01/2011
 *      Author: aecc
 */
#include "Routes.h"
#include "Input.h"
#include "Tabu_Queue.h"
#ifndef OPERATORS_H_
#define OPERATORS_H_

class Operators {
private:
public:
	static void shift_one(int city, int city_route, int min_pos, int min_route, Routes* solution, Input* in);
	static void shift_two(int city, int route, int min_pos, int min_route, Routes* solution, Input* in);
	static int check_shift_one(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in);
	static int check_shift_two(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in);
	static int check_swap_one(int city, int route, int* min1, int* min2, double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in);
	static void swap_one(int city, int city_route, int min, int min_route, Routes* solution, Input* in);
	static int check_swap_two_one(int city, int route, int* min1, int* min2, double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in);
	static void swap_two_one(int city, int city_route, int min, int min_route, Routes* solution, Input* in);
	static int check_swap_two_two(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in);
	static void swap_two_two(int city, int city_route, int min, int min_route, Routes* solution, Input* in);
	static int check_crossover(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost);
	static void crossover(int city, int route, int min, int min_route, Routes* solution, Input* in, double* acc_cost);
	static int check_Interchange(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost);
	static void Interchange(int city, int route, int min, int min_route, Routes* solution, Input* in, double* acc_cost);
	static void relocateFeasibleCustomerIntra(int, int, int, int, Routes*, bool);
	static void relocateFeasibleCustomerIntraTwo(int, int, int, int, Routes*, bool);
	static void relocateFeasibleCustomerIntraThree(int, int, int, int, Routes*, bool);
	static void exchangeFeasibleCustomersIntra(int, int, int, Routes*, bool);
	static void two_up(int,int,int,Routes*);
	static int check_shift_oneGTS(int city, int route, int* min, double* min_cost, int* min_route, int* pos, Routes* solution, Input* in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double** real_costs);
	static int check_swap_oneGTS(int city, int route, int* min1, int* min2, double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs);
	static int check_crossoverGTS(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs, double * real_acc_cost);
	static int check_InterchangeGTS(int city, int route, int* min1, int* min2 , double* min_cost, int* min_route1, int* min_route2, Routes* solution, Input* in, double* acc_cost, bool ** reduced_neighbourhood, Tabu_Queue * queue, double best_solution, double ** real_costs, double * real_acc_cost);
	static const double epsilon_parameter = 0.001;
	static double diff_parameter;
	static double diff_neg_parameter;

};

#endif /* OPERATORS_H_ */
