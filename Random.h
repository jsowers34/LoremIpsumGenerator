//----------------------------------------------------------------------------
// BEGIN FILE  Random.H
//----------------------------------------------------------------------------
// PURPOSE
//    Provide a Random number package
//----------------------------------------------------------------------------
// DESIGN NOTES
//    Uses the L'Ecuyer algorithm
//----------------------------------------------------------------------------
// REVISION HISTORY
//
//   Initial Code:     Joseph Sowers, PhD                01 JUL 94
//
//   Modifications:
//----------------------------------------------------------------------------
// SAFETY DEFINITION
//
#ifndef _RANDOM_H
#define _RANDOM_H
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//

#pragma once

//----------------------------------------------------------------------------
// DEFINES
//
#define MAX_RANDOM_STREAMS  8

typedef struct ranlist {
	long number;
	struct ranlist *next;
} ranlist_type;

typedef struct {
	long seed_1, seed_2;
} seed_type;

typedef seed_type seed_array[MAX_RANDOM_STREAMS];

//
//----------------------------------------------------------------------------
//	 Begin Code

class Random {

public:
	Random();
	~Random();
	void reset(int stream);
	double duniform(int stream, double lower, double upper);
	long uniform(int stream, long lower, long upper);
	double normal(int stream, double mean, double sigma);
	void create_random_list(int stream, long list_length);
	long deal_random_number(int stream);
	bool random_boolean (int stream, int pcnt);
	bool random_boolean (int stream);

private:
	// Data
    seed_array initial_seed_array;
	seed_array seed;
	long ranlist_top;
	ranlist_type *ranlist_head[MAX_RANDOM_STREAMS];

	// Methods
	double unif(int stream);
	void   init(void);

};

#endif /* RANDOM_H_ */
