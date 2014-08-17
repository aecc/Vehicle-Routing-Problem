/*
 * IO.h
 *
 *  Created on: 15/01/2011
 *      Author: aecc
 */

#ifndef IO_H_
#define IO_H_
#include <string>
#include "Input.h"
#include <math.h>
#include <cmath>

class IO {
public:
	IO();
	virtual ~IO();
	static Input * readFileDethloff(char *);
	static Input * readFileSalhiNagy(char *);
};

#endif /* IO_H_ */
