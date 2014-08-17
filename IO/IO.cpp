/*
 * IO.cpp
 *
 *  Created on: 15/01/2011
 *      Author: aecc
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>


using namespace std;

#include "IO.h"

IO::IO() {
}

/*Lee un archivo en formato Salhi&Nagy con el problema VRP y devuelve un tipo Input con todas las variables inicializadas*/
Input * IO::readFileSalhiNagy(char * file) {

	  double capacity;
	  int ncustomers;
	  string line;
	  ifstream myfile (file);

	  if (myfile.is_open())
	  {

		myfile >> ncustomers;
		myfile >> capacity;

		int * x = new int[ncustomers];
		int * y = new int[ncustomers];
		ncustomers--;

		//cout << ncustomers << endl << capacity << endl;

		Input * input = new Input(ncustomers, capacity);

		for (int i=0; i<ncustomers+1; i++) {
			myfile >> x[i];
			myfile >> y[i];
			myfile >> input->deliveries[i];
			myfile >> input->pickups[i];
			input->positions[i] = Point(x[i],y[i]);
			//myfile >> input->cost[i][j];
			//cout << cost[i][j] << endl;
		}
		myfile.close();
		for (int i=0; i<ncustomers+1; i++) {
			for (int j=0; j<ncustomers+1; j++) {
				input->cost[i][j] = sqrt(pow((x[i]-x[j]),2) + pow((y[i]-y[j]),2));
			}
			//myfile >> input->cost[i][j];
			//cout << cost[i][j] << endl;
		}
		delete [] x;
		delete [] y;
		return input;
	  }

	  else cout << "Unable to open file";

	  //delete cost[];
	  return NULL;

}


/*Lee un archivo en formato Dethloff con el problema VRP y devuelve un tipo Input con todas las variables inicializadas*/
Input * IO::readFileDethloff(char * file) {

	  double capacity;
	  int ncustomers;
	  string line;
	  ifstream myfile (file);

	  if (myfile.is_open())
	  {

		myfile >> capacity;
		myfile >> ncustomers;

		Input * input = new Input(ncustomers, capacity);

		for (int i=0; i<ncustomers+1; i++) {

			for (int j=0; j<ncustomers+1; j++) {

				myfile >> input->cost[i][j];
				//cout << cost[i][j] << endl;

			}

		}

		for (int i=1; i<ncustomers+1;i++) {
			myfile >> input->deliveries[i];
			myfile >> input->pickups[i];
			//cout << deliveries[i] << endl;
			//cout << pickups[i] << endl;
			//printf("%f\n",pickups[i]);
		}

	    myfile.close();
	    return input;
	  }

	  else cout << "Unable to open file";

	  //delete cost[];
	  return NULL;
}

IO::~IO() {
}

/*
int main() {
	Input * input = IO::readFileDethloff("ejemplo.txt");
	cout << input->ncustomers << endl;
	return 0;
}*/
