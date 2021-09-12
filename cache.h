#ifndef CACHE_H_
#define CACHE_H_

#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "vector"

using namespace std;

#define UNDEFINED 0xFFFFFFFFFFFFFFFF //constant used for initialization

typedef enum {WRITE_BACK, WRITE_THROUGH, WRITE_ALLOCATE, NO_WRITE_ALLOCATE} write_policy_t; 

typedef enum {HIT, MISS} access_type_t;

typedef long long address_t; //memory address type

class cache{

	/* Add the data members required by your simulator's implementation here */

	/* number of memory accesses processed */
	unsigned int number_memory_accesses;

	/* trace file input stream */	
	ifstream stream;


public:

	/* 
	* Instantiates the cache simulator 
        */
	cache(unsigned cache_size, 		// cache size (in bytes)
       unsigned cache_associativity,     // cache associativity (fully-associative caches not considered)
	      unsigned cache_line_size,         // cache block size (in bytes)
	      write_policy_t write_hit_policy,  // write-back or write-through
	      write_policy_t write_miss_policy, // write-allocate or no-write-allocate
	      unsigned cache_hit_time,		// cache hit time (in clock cycles)
	      unsigned cache_miss_penalty,	// cache miss penalty (in clock cycles)	
	      unsigned address_width            // number of bits in memory address
	);	
	
	// de-allocates the cache simulator
	~cache();

	// loads the trace file (with name "filename") so that it can be used by the "run" function  
	void load_trace(const char *filename);

	// processes "num_memory_accesses" memory accesses (i.e., entries) from the input trace 
	// if "num_memory_accesses=0" (default), then it processes the trace to completion 
	void run(unsigned num_memory_accesses=0);
	
	// processes a read operation and returns hit/miss
	access_type_t read(address_t address);
	
	// processes a write operation and returns hit/miss
	access_type_t write(address_t address);

	//Programmer structs are defined here
	//This struct helps to get the the constructor variables passed from testcases as global varibles
	//To be used later
	struct cache_t
	 {
         unsigned Cache_size=UNDEFINED; 		// cache size (in bytes)
         unsigned Cache_Associativity=UNDEFINED;     // cache associativity (fully-associative caches not considered)
         unsigned cache_line_size=UNDEFINED;         // cache block size (in bytes)
         write_policy_t write_hit_policy;  // write-back or write-through
         write_policy_t write_miss_policy; // write-allocate or no-write-allocate
         unsigned cache_hit_time=UNDEFINED;		// cache hit time (in clock cycles)
         unsigned cache_miss_penalty=UNDEFINED;	// cache miss penalty (in clock cycles)
         unsigned address_width=UNDEFINED;
         unsigned sets=UNDEFINED;
         int memory_acccess=0;
         unsigned read=0;
         unsigned read_misses=0;
         unsigned  write=0;
         unsigned write_misses=0;
         unsigned evictions=0;
         unsigned memory_writes=0;
         float  average_memory_access_time=0;

     };

	//
	class cache_entry
    {
    public:
	    unsigned valid_bit;
	    unsigned dirty_bit;
	    unsigned long long   tag;
	    unsigned block_offset;
	    unsigned index;
        unsigned timestamp;

    };
	cache_t cache_var;


	vector<vector<cache_entry>>cache_table;
	// returns the next block to be evicted from the cache
	unsigned evict(unsigned index);
	
	// prints the cache configuration
	void print_configuration();
	void print_configuration(ofstream*);
	
	// prints the execution statistics
	void print_statistics();
	void print_statistics(ofstream *out);

	//prints the metadata information (including "dirty" but, when applicable) for all valid cache entries  
	void print_tag_array();
	void print_tag_array(ofstream*);




	/*
	 * My functions are defined here
	 */
	string enums_to_string(write_policy_t policy)
    {

	    switch(policy)
        {
            case WRITE_BACK:
                return "write_back";
            case WRITE_ALLOCATE:
                return "write_allocate";
            case WRITE_THROUGH:
                return "write_through";
            case NO_WRITE_ALLOCATE:
                return "no-write-allocate";
            default:
                return "error";
        }
    }



    unsigned long long get_tag(address_t address);
	unsigned long get_Index(address_t address);
	unsigned long get_block_offset(address_t address);
	bool  check_if_set_is_full(unsigned index,unsigned tag);
	void print_cache_status_text_only(int counter);
	void print_cache_status_text_only(int counter,ofstream*);
	void print_cache_status(int,int);
	void print_cache_status(int,int,ofstream*);
};

#endif /*CACHE_H_*/
