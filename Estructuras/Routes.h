/*
 * Routes.h
 *
 *  Created on: 19/01/2011
 *      Author: aecc
 */
#include "Input.h"
#include <limits>
#include <algorithm>

#ifndef ROUTES_H_
#define ROUTES_H_

class Routes {
private:
	int * customers;
	int * customers_reverse;
	int * routes;
	int * routes_reverse;
	double * capacity_at_depot_pickup;
	double * capacity_at_depot_delivery;
	double * capacity_pickup;
	double * capacity_delivery;
public:
	double ** costs;
	double * routes_costs;
	double * demands_pickup;
	double * demands_delivery;
	int nroutes;
	int ncustomers;
	int last_route;
	double vehicle_capacity;
	Routes(Input *, int=0);
	void clone(Routes *);
	int calcNRoutes();
	bool isFull();
	void addFeasibleArc(int,int,int);
	void newRoute();
	void reset(int=0);
	int getArc(int,int);
	int getReverseArc(int,int);
	void setArc(int,int,int);
	void setReverseArc(int,int,int);
	void setCompleteArc(int,int,int);
	double getDeliveryCapacity(int, int);
	double getPickupCapacity(int, int);
	void setDeliveryCapacity(int, int, double);
	void setPickupCapacity(int, int, double);
	bool updateRouteCapacity(int, bool=true, int=0, int=0);
	bool checkFeasibility(int, bool=true, int=0, int=0);
	bool updateFullRouteCapacity(int, double=-1.0, double=-1.0);
	double calcTotalRouteCost(int route, int = 0, int = 0);
	double calcTotalCost();
	void calcAccCost(double* acc_cost);
	int getFirstCustomer(int);
	void calcAccCostFrom(double **,double* acc_cost);
	void printRouteDetails(int);
	void printRouteDetailsToFile(int,char*);
	void printAllRoutes();
	void printAllRoutesToFile(char *);
	double getRouteCost(int);
	double getTotalCost();
	void setRouteCost(int,double);
	void invertArcs(int,int=0,int=0);
	virtual ~Routes();
	static void createInitialRoutesNearestNeighbor(Routes *);
	static Routes* createInitialRoutesInsertion(Input *in, int v, double y);
	static Routes* createInitialRoutesInsertionBest(Input *in, int v, double y);
	static Routes* createInitialRoutesGRASP(Input *in, double y,int ri);
	static void createInitialRoutesGTS(Routes *,double=0.05);
	static bool allCustomersAsigned(bool *,int);
};

struct tuple{	int id;
				double cost;
				tuple() : id(id), cost(cost){};
			    ~tuple(){};};
bool sort_tuples(tuple* i, tuple* j);
bool sort_tuples2(tuple* i, tuple* j);
#endif /* ROUTES_H_ */

