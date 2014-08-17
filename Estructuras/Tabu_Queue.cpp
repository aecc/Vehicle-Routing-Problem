/*
 * Clase contenedora de una cola Tabu de tamaño fijo tal que al agregar nueva informacion tabu, se elimine
 * el primer elemento de la cola y se agregue esta nueva informacion al final de la cola
 * Tabu_Queue.cpp
 *
 *  Created on: 11/04/2011
 *      Author: aecc
 */

#include "Tabu_Queue.h"
using namespace std;

/*
 * Se crea la cola con el tamaño como parametro
 */
Tabu_Queue::Tabu_Queue(int tenure) {

	tabu_tenure = tenure;
	Tabu_Row * row;
	for (int i=0; i<tabu_tenure; i++) {
		row = new Tabu_Row();
		queue.push_back(*row);
	}

}

/*
 * Se agrega el elemento al final de la cola y se elimina el primero
 */
void Tabu_Queue::push(Tabu_Row row) {
	queue.pop_front();
	Tabu_Row * new_row = new Tabu_Row();
	new_row->node1 = row.node1;
	new_row->node2 = row.node2;
	new_row->node3 = row.node3;
	new_row->node4 = row.node4;
	new_row->type = row.type;
	new_row->route1 = row.route1;
	new_row->route2 = row.route2;
	queue.push_back(*new_row);
}

/*
 * Obtiene el tamaño de la cola
 */
int Tabu_Queue::getSize() {
	return queue.size();
}

/*
 * Chequea si existe el elemento en la cola tabu
 */
bool Tabu_Queue::find(Tabu_Row row) {
	list<Tabu_Row>::iterator it;
	for ( it=queue.begin() ; it != queue.end(); it++ ) {
		if ((*it).type == row.type)
			if ((*it).node1 == row.node1)
				if ((*it).node2 == row.node2)
					if ((*it).node3 == row.node3)
						if ((*it).node4 == row.node4)
							if ((*it).route1 == row.route1)
								if ((*it).route2 == row.route2)
										return true;
	}
	return false;
}

/*
 * Imprime la cola
 */
void Tabu_Queue::print() {
	list<Tabu_Row>::iterator it;
	for ( it=queue.begin() ; it != queue.end(); it++ ) {
		cout << "node1: " << (*it).node1 << " node2: " << (*it).node2 << " route1: " << (*it).route1 << " route2: " << (*it).route2 << " type: " << (*it).type << " node3: " << (*it).node3 << " node4: " << (*it).node4 << endl;
	}
}

Tabu_Queue::~Tabu_Queue() {
}
