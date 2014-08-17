/*
 * Random.h
 *
 *  Created on: 12/02/2011
 *      Author: aecc
 */

#ifndef RANDOM_H_
#define RANDOM_H_

class Random {
public:
	Random();
	static void init_genrand(unsigned long s);
	static void init_by_array(unsigned long init_key[], int key_length);
	static unsigned long genrand_int32(void);
	static long genrand_int31(void);
	static double genrand_real1(void);
	static double genrand_real2(void);
	static double genrand_real3(void);
	static double genrand_res53(void);
	virtual ~Random();
};

#endif /* RANDOM_H_ */
