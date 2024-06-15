/**--------------------------------------------------------------------------
* BEGIN FILE  Random.cpp
*--------------------------------------------------------------------------
* PURPOSE
*    Provide a Random number package for C++
*--------------------------------------------------------------------------
* DESIGN NOTES
*    Uses the L'Ecuyer algorithm
*--------------------------------------------------------------------------
* REVISION HISTORY
*
*   Initial Code:     Joseph Sowers, PhD     01 JUL 94
*   Modifications:    JL Sowers              11 JUN 24  Added boolean return methods.
*--------------------------------------------------------------------------
* INCLUDE FILES
*/

#include <stddef.h>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include "Random.h"

/**
*--------------------------------------------------------------------------
* DEFINES
*       None
*--------------------------------------------------------------------------
* BEGIN CODE
*/

//  Default Constructor
Random::Random() {
	init();
}

/**
* Default Destructor.
*/
Random::~Random() {
	// Null
}

/**
* Initialize 8 random number streams.
*/
void Random::init() {
	int i;
	initial_seed_array[0].seed_1 = 1878369238;
	initial_seed_array[0].seed_2 = 1021707299;
	initial_seed_array[1].seed_1 = 666082376;
	initial_seed_array[1].seed_2 = 842447242;
	initial_seed_array[2].seed_1 = 444846677;
	initial_seed_array[2].seed_2 = 218288335;
	initial_seed_array[3].seed_1 = 1566698773;
	initial_seed_array[3].seed_2 = 214067549;
	initial_seed_array[4].seed_1 = 578069528;
	initial_seed_array[4].seed_2 = 413166292;
	initial_seed_array[5].seed_1 = 1695327920;
	initial_seed_array[5].seed_2 = 1262893840;
	initial_seed_array[6].seed_1 = 1692528413;
	initial_seed_array[6].seed_2 = 1522062797;
	initial_seed_array[7].seed_1 = 747819450;
	initial_seed_array[7].seed_2 = 1460277304;
	ranlist_top = 0;
	for (i = 0; i < MAX_RANDOM_STREAMS; i++)
		ranlist_head[i] = NULL;
}

/**
*  Reset a stream to its original state.
* @param stream   number of stream (0 to 7) to reset. Default is reset all streams.
*/
void Random::reset(int stream = -1) {
	int i;

	if (stream == -1) {
		for (i = 0; i < MAX_RANDOM_STREAMS; i++) {
			seed[i].seed_1 = initial_seed_array[i].seed_1;
			seed[i].seed_2 = initial_seed_array[i].seed_2;
		}
	} else {
		i = stream;
		seed[i].seed_1 = initial_seed_array[i].seed_1;
		seed[i].seed_2 = initial_seed_array[i].seed_2;
	}
}

/**
* Generate a random double value from a uniform distribution.
* @param stream the stream to select from.
* @param lower  lowest value possible (default 0.0)
* @param upper  highest value possible (default 1.0)
* @return a double value in the interval [0.0, 1.0)
*/
double Random::duniform(int stream = 0, double lower = 0.0, double upper = 1.0) {
	double r = unif(stream);
	return (lower + r * (upper - lower));
}

/**
* Generate a random long value from a uniform distribution.
* @param stream the stream to select from.
* @param lower  lowest value possible (default = 0L)
* @param upper  highest value possible (default = 100L)
* @return a double value in the interval [0L, 100L)
*/
long Random::uniform(int stream = 0, long lower = 0L, long upper = 100L) {
	double r = unif(stream);
	return ((long) (lower + r * (upper - lower)));
}


/**
* Generate a random double value from a Normal distribution.
* @param stream the stream to select from.
* @param mean  average value  (default 0.0)
* @param sigma  standard deviation (default 0.1)
* @return a double value
*/
double Random::normal(int stream = 0, double mean = 0.0, double sigma = 0.1) {
	int j;
	double return_value = 0.0;

	for (j = 0; j < 12; j++)
		return_value += unif(stream);
	return (double) (mean + sigma * (return_value - 6.0));
}

/**
* Generate a TRUE/FALSE boolean value.
* @param stream the stream to select from.
* @param pcnt   the percentage that TRUE being returned is likely (Default = 50%).
*/
bool Random::random_boolean (int stream, int pcnt=50) {
	bool return_value = false;
	if (uniform () < pcnt) return_value = true;
	return return_value;
}

/**
* Generate a TRUE/FALSE boolean value with a 50-50 probability.
* @param stream the stream to select from.
*/
bool Random::random_boolean (int stream) {
	return random_boolean (stream, 50);
}

/**
* Creates a list of numbers from 0 to list_length and assigns it to a stream.
* @see deal_random_number
* @param stream  the stream used.
* @param list_length   last long value in the list (default = 999L).
*/
void Random::create_random_list(int stream = 0, long list_length = 999l) {
	long i;
	ranlist_type *start = NULL;
	ranlist_type *ptr;
	ranlist_type *prior = NULL;

	for (i = 0; i < list_length; i++) {
		ptr = new ranlist_type;
		ptr->number = i;
		ptr->next = NULL;
		if (prior == NULL)
			start = ptr;
		else
			prior->next = ptr;
		prior = ptr;
	}

	ranlist_top = list_length;
	ranlist_head[stream] = start;
}


/**
* Deals a uniformly distributed random number from a random list specified by the stream.
* Note that method create_random_list must be called prior to using this method.
* Note that 'deal' implies selecting without repeat (aka deal the cards)
* So for a card deck, you would initialise the stream used by calling create_random_list(0, 52)
* @param stream stream used to define the list used.
*/
long Random::deal_random_number(int stream = 0) {
	long i;
	long deal;
	long r = uniform(stream, 0L, ranlist_top);
	ranlist_type *prior = NULL;
	ranlist_type *ptr;

	ptr = ranlist_head[stream];

	for (i = 0; i < r; i++) {
		prior = ptr;
		ptr = ptr->next;
	}

	deal = ptr->number;
	ranlist_top--;

	if (prior == NULL)
		ranlist_head[stream] = ptr;
	else
		prior->next = ptr->next;

	return deal;
}

// Private

/**
* Computes a 'random' uniformly distributed value for the input stream.
* Uses the L'Ecuyer algorithm.
* @param stream   stream to be used (default = 0).
*/
double Random::unif(int stream = 0) {
	const long m1 = 2147483563;
	const long a1 = 40014;
	const long q1 = 53668;
	const long r1 = 12211;
	const long m2 = 2147483199;
	const long a2 = 40692;
	const long q2 = 52774;
	const long r2 = 3791;
	const long m1_minus_1 = m1 - 1;
	const double normalize = 4.656613e-10;
	long q;
	double z;

	q = seed[stream].seed_1 / q1;
	seed[stream].seed_1 = a1 * (seed[stream].seed_1 - q * q1) - q * r1;
	if (seed[stream].seed_1 < 0)
		seed[stream].seed_1 += m1;

	q = seed[stream].seed_2 / q1;
	seed[stream].seed_2 = a2 * (seed[stream].seed_2 - q * q2) - q * r2;
	if (seed[stream].seed_2 < 0)
		seed[stream].seed_2 += m2;

	z = seed[stream].seed_1 - seed[stream].seed_2;
	while (z < 1)
		z += m1_minus_1;
	return ((double) z * normalize);
}

//
// END FILE  random
//------------------------------------------------------------------------

