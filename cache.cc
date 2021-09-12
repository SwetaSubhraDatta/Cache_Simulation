#include "cache.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <math.h>
#include "sstream"

using namespace std;

#define KB 1024

cache::cache(unsigned size, 
      unsigned associativity,
      unsigned line_size,
      write_policy_t wr_hit_policy,
      write_policy_t wr_miss_policy,
      unsigned hit_time,
      unsigned miss_penalty,
      unsigned address_width
)
{
    cache_var.Cache_size=size;
    cache_var.Cache_Associativity=associativity;
    cache_var.cache_line_size=line_size;
    cache_var.write_hit_policy=wr_hit_policy;
    cache_var.write_miss_policy=wr_miss_policy;
    cache_var.cache_hit_time=hit_time;
    cache_var.cache_miss_penalty=miss_penalty;
    cache_var.address_width=address_width;
    cache_var.sets=size/(associativity*line_size);
    cache_table=vector<vector<cache_entry>>(cache_var.sets,vector<cache_entry>(cache_var.Cache_Associativity));
    for(int i=0;i<cache_var.sets;i++)
    {
        for(int  j=0;j<cache_var.Cache_Associativity;j++)
        {
            cache_table[i].at(j).tag=UNDEFINED;
            cache_table[i].at(j).valid_bit=0;
            cache_table[i].at(j).dirty_bit=0;
            cache_table[i].at(j).block_offset=UNDEFINED;
            cache_table[i].at(j).index=UNDEFINED;
            cache_table[i].at(j).timestamp=UNDEFINED;

        }
    }

}
/*
 * Tis function prints out the whole configuration in the cache
 * for the first time that is sent through the constructor
 * @return: void
 */
void cache::print_configuration()
{
    cout<<"CACHE CONFIGURATION"<<endl;
    cout<<"size = "<<cache_var.Cache_size/KB<<" KB"<<endl;
    cout<<"associativity = "<<cache_var.Cache_Associativity<<"-way"<<endl;
    cout<<"cache line size = "<<cache_var.cache_line_size<<" B"<<endl;
    cout<<"write hit policy = "<<cache::enums_to_string(cache_var.write_hit_policy)<<endl;
    cout<<"write miss policy = "<<cache::enums_to_string(cache_var.write_miss_policy)<<endl;
    cout<<"cache hit time = "<<cache_var.cache_hit_time<<" CLK"<<endl;
    cout<<"cache miss penalty = "<<cache_var.cache_miss_penalty<<" CLK"<<endl;
    cout<<"memory address width = "<<cache_var.address_width<<" bits"<<endl;
}

void cache::print_configuration(ofstream* out)
{
    *out<<"CACHE CONFIGURATION"<<endl;
    *out<<"size = "<<cache_var.Cache_size/KB<<" KB"<<endl;
    *out<<"associativity = "<<cache_var.Cache_Associativity<<"-way"<<endl;
    *out<<"cache line size = "<<cache_var.cache_line_size<<" B"<<endl;
    *out<<"write hit policy = "<<cache::enums_to_string(cache_var.write_hit_policy)<<endl;
    *out<<"write miss policy = "<<cache::enums_to_string(cache_var.write_miss_policy)<<endl;
    *out<<"cache hit time = "<<cache_var.cache_hit_time<<" CLK"<<endl;
    *out<<"cache miss penalty = "<<cache_var.cache_miss_penalty<<" CLK"<<endl;
    *out<<"memory address width = "<<cache_var.address_width<<" bits"<<endl;
}

cache::~cache()
{
   cache_table.clear();
  cache_var.Cache_size=UNDEFINED;
  cache_var.Cache_Associativity=UNDEFINED;
  cache_var.cache_line_size=UNDEFINED;
  cache_var.write_hit_policy=write_policy_t (UNDEFINED);
  cache_var.write_miss_policy=write_policy_t (UNDEFINED);
  cache_var.cache_hit_time=UNDEFINED;
  cache_var.cache_miss_penalty=UNDEFINED;
  cache_var.address_width=UNDEFINED;
  cache_var.memory_writes=UNDEFINED;
  number_memory_accesses=0;
  cache_var.memory_acccess=0;


	/* edit here */
}

void cache::load_trace(const char *filename){
   stream.open(filename);
}

void cache::run(unsigned num_entries)
{
    access_type_t hit_or_miss;
   unsigned first_access = number_memory_accesses;
   string line;
   unsigned line_nr=0;

   while (getline(stream,line))
   {

	line_nr++;
        char *str = const_cast<char*>(line.c_str());
	
        // tokenize the instruction
        char *op = strtok (str," ");
	char *addr = strtok (NULL, " ");
	address_t address = strtoull(addr, NULL, 16);
	if(string(op)=="r")
    {
	 hit_or_miss=read(address);
	    cache_var.read++;
	    if(hit_or_miss==MISS)
        {
	        cache_var.read_misses++;
        }
    }
	else
    {
	    hit_or_miss=write(address);

	    cache_var.write++;
	    if(hit_or_miss==MISS)
        {
	        cache_var.write_misses++;
        }
    }

	/* 
		edit here:
		insert the code to process read and write operations
	   	using the read() and write() functions below

	*/

	number_memory_accesses++;
	cache_var.memory_acccess=(number_memory_accesses);

	if (num_entries!=0 && (number_memory_accesses-first_access)==num_entries)
		break;
   }
}

void cache::print_statistics(){
	cout << "STATISTICS" << endl;
	cout<< "memory access = "<<dec<<cache_var.memory_acccess<<endl;
	cout<<"read = "<<dec<<cache_var.read<<endl;
	cout<<"read misses = "<<dec<<cache_var.read_misses<<endl;
	cout<<"write = "<<dec<<cache_var.write<<endl;
	cout<<"write misses = "<<dec<<cache_var.write_misses<<endl;
	cout<<"evictions = "<<dec<<cache_var.evictions<<endl;
	cout<< "memory writes = "<<dec<<cache_var.memory_writes<<endl;
	float rm_plus_wm_by_ma=(((float)cache_var.read_misses+(float)cache_var.write_misses)/(float)number_memory_accesses);
	cache_var.average_memory_access_time=(float)cache_var.cache_hit_time+(rm_plus_wm_by_ma*(float)cache_var.cache_miss_penalty);
	cout<<"average memory access time = "<<dec<<cache_var.average_memory_access_time<<endl;
	cout<<"Miss rate = "<<rm_plus_wm_by_ma;
}

//@Overloaded
void cache::print_statistics(ofstream *out)
{
    *out << "STATISTICS" << endl;
    *out<< "memory access = "<<dec<<cache_var.memory_acccess<<endl;
    *out<<"read = "<<dec<<cache_var.read<<endl;
    *out<<"read misses = "<<dec<<cache_var.read_misses<<endl;
    *out<<"write = "<<dec<<cache_var.write<<endl;
    *out<<"write misses = "<<dec<<cache_var.write_misses<<endl;
    *out<<"evictions = "<<dec<<cache_var.evictions<<endl;
    *out<< "memory writes = "<<dec<<cache_var.memory_writes<<endl;
    float rm_plus_wm_by_ma=(((float)cache_var.read_misses+(float)cache_var.write_misses)/(float)cache_var.memory_acccess);
    cache_var.average_memory_access_time=(float)cache_var.cache_hit_time+(rm_plus_wm_by_ma*(float)cache_var.cache_miss_penalty);
    *out<<"average memory access time = "<<dec<<cache_var.average_memory_access_time<<endl;
}

access_type_t cache::read(address_t address)
{
    unsigned long index = get_Index(address);
    unsigned long long tag = get_tag(address);
    unsigned long offset = get_block_offset(address);
    access_type_t status;

    for (unsigned i = 0; i < cache_var.Cache_Associativity; i++)
    {
        if (cache_table[index][i].tag == tag && cache_table[index][i].valid_bit == 1)
        {
            cache_table[index][i].timestamp = number_memory_accesses;//Accessed
            status = HIT;
            return status;
        }
    }

        //if there is a read miss
    for(int j=0;j<cache_var.Cache_Associativity;j++)
        {
            status=MISS;
            if(cache_table[index].at(j).tag!=UNDEFINED && check_if_set_is_full(index,tag))
            {
                if (cache_table[index][j].index == index)//if there is a conflict
                {
                    if(cache_var.write_hit_policy==WRITE_BACK && cache_table[index][j].dirty_bit==1)
                    {
                        cache_var.memory_writes++;
                    }
                    unsigned block = evict(index);
                    cache_var.evictions++;
                    cache_table[index][block].index = index;
                    cache_table[index][block].tag = tag;
                    cache_table[index][block].timestamp = number_memory_accesses;
                    cache_table[index][block].valid_bit = 1;
                    cache_table[index][block].block_offset = offset;
                    status=MISS;


                    return MISS;
                }

            }
            else
                {
                if (cache_table[index][j].tag == UNDEFINED) {
                    cache_table[index].at(j).tag = tag;
                    cache_table[index].at(j).index = index;
                    cache_table[index].at(j).block_offset = offset;
                    cache_table[index].at(j).dirty_bit = 0;
                    cache_table[index].at(j).valid_bit = 1;
                    cache_table[index][j].timestamp = number_memory_accesses;
                    return MISS;
                }
            }




        }


    return status;
}

access_type_t cache::write(address_t address)
{
    //Parse the address out as usual
    unsigned long index = get_Index(address);
    unsigned long long tag = get_tag(address);
    unsigned long offset = get_block_offset(address);
    //Parsing done

    /**
     *  Steps for Miss:
     *   iter for hit
      if you hit, return
     now is the part for miss
     check_full
    if full, evict one block,  say blk i
     else find a invalid block, say blk i
     refill blk i
     */

    for(unsigned i=0;i<cache_var.Cache_Associativity;i++)
    {
        //if there is a hit
        if (cache_table[index][i].tag == tag && cache_table[index][i].valid_bit == 1)
        {
            if (cache_var.write_hit_policy == WRITE_BACK)
            {
                if (cache_table[index].at(i).dirty_bit != 1)
                {
                    cache_table[index].at(i).dirty_bit = 1;
                }
                cache_table[index][i].timestamp = number_memory_accesses;

            }
            else {

                cache_var.memory_writes++;

            }
            return HIT;

        }

    }


        //If there is a miss
        for(int j=0;j<cache_var.Cache_Associativity;j++) {


            //if write-hit policy is writeback
            if (cache_var.write_miss_policy == WRITE_ALLOCATE)
            {
                //Step1::Chech if there is ny empty block
                //if full evict the block withb LRU
                if(cache_table[index].at(j).tag!=UNDEFINED && check_if_set_is_full(index,tag))
                {
                   //if there is a conflict
                    {
                        if(cache_table[index][j].dirty_bit==1)
                        {
                            cache_var.memory_writes++;
                        }
                        unsigned block = evict(index);
                        cache_var.evictions++;
                        cache_table[index][block].index = index;
                        cache_table[index][block].tag = tag;
                        cache_table[index][block].timestamp = number_memory_accesses;
                        cache_table[index][block].dirty_bit = 1;
                        cache_table[index][block].valid_bit = 1;
                        cache_table[index][block].block_offset = offset;



                        return MISS;
                    }

                }
                else
                    {
                    if (cache_table[index][j].tag == UNDEFINED) {
                        cache_table[index].at(j).tag = tag;
                        cache_table[index].at(j).index = index;
                        cache_table[index].at(j).block_offset = offset;
                        cache_table[index].at(j).dirty_bit = 1;
                        cache_table[index].at(j).valid_bit = 1;
                        cache_table[index][j].timestamp = number_memory_accesses;
                        return MISS;
                    }
                }

            }

                //if its write through
            else {

                cache_var.memory_writes++;
                // cache_table[index][i].timestamp=cache_table[index][i].timestamp+number_memory_accesses+1;
                return MISS;

            }
        }


    return MISS;


}


void cache::print_tag_array()
{
    cout<<"TAG ARRAY"<<endl;
   for(int i=0;i<cache_var.Cache_Associativity;i++)
    {
        print_cache_status_text_only(i);
        for(int j=0;j<cache_var.sets;j++)
        {
            if(cache_table[j].at(i).tag!=UNDEFINED)
            {
                print_cache_status(j,i);
            }
        }
    }
}

//Overloaded
void cache::print_tag_array(ofstream *out)
{
    *out<<"TAG ARRAY"<<endl;
    for(int i=0;i<cache_var.Cache_Associativity;i++)
    {
        print_cache_status_text_only(i,out);
        for(int j=0;j<cache_var.sets;j++)
        {
            if(cache_table[j].at(i).tag!=UNDEFINED)
            {
                print_cache_status(j,i,out);
            }
        }
    }
}


unsigned cache::evict(unsigned index)
{
    int block=0;
    unsigned min_blk_idx=0;
    unsigned min_blk_timestamp = -1;
    for(int i=0;i<cache_var.Cache_Associativity;i++)
    {
        if (cache_table[index][i].timestamp < min_blk_timestamp)
        {
            min_blk_timestamp=cache_table[index][i].timestamp;
            min_blk_idx=index;
            block=i;
        }
 }
    /* edit here */
	return block;
}

/*
 * My function definations are given here
 */

/////These 3 functions are your address parsers
unsigned long long  cache::get_tag(address_t address)
{
    unsigned long index=log2(cache_var.sets);
    unsigned long offset_no=log2(cache_var.cache_line_size);
//    unsigned target=cache_var.address_width-(index+offset_no);
    unsigned long long tag_bits=(address>>(index+offset_no));
    return tag_bits;
}

unsigned long cache::get_Index(address_t address)
{
    unsigned long index=log2(cache_var.sets);
    unsigned long offset_no=log2(cache_var.cache_line_size);
    unsigned  long index_bits=(address>>offset_no)&((1<<index)-1);
    return index_bits;

}

unsigned long cache::get_block_offset(address_t address)
{
    unsigned long offset_no=log2(cache_var.cache_line_size);
    unsigned long offset=(address>>0) &((1<<offset_no)-1);
    return offset;
}
//////End of parsers//////

bool cache::check_if_set_is_full(unsigned  index,unsigned tag)
{
    int count_empty=0;
    for(int i=0;i<cache_var.Cache_Associativity;i++)
    {
        if(cache_table[index].at(i).tag==UNDEFINED &&cache_table[index][i].valid_bit==0)//Empty tag still available
        {
            return false;//Yes there is still an empty block avala
        }
    }
    return true;
}

void cache::print_cache_status_text_only(int counter)
{
    if(cache_var.write_hit_policy==WRITE_BACK && cache_var.write_miss_policy==WRITE_ALLOCATE) {
        cout << "BLOCK " << to_string(counter) << endl;
        cout << setw(7) << "index" << setw(7) << "dirty" << setw(13) << "tag" << endl;
    }
    else
    {
        cout << "BLOCK " << to_string(counter) << endl;
        cout << setw(7) << "index" << setw(11)  << "tag" << endl;
    }
}

//@Overloaded
void cache::print_cache_status_text_only(int counter,ofstream* out)
{
    if(cache_var.write_hit_policy==WRITE_BACK && cache_var.write_miss_policy==WRITE_ALLOCATE) {
        *out << "BLOCK " << to_string(counter) << endl;
        *out << setw(7) << "index" << setw(7) << "dirty" << setw(13) << "tag" << endl;
    }
    else
    {
        *out << "BLOCK " << to_string(counter) << endl;
        *out << setw(7) << "index" << setw(11)  << "tag" << endl;
    }
}


void cache::print_cache_status(int i,int j)

{
    if(cache_var.write_hit_policy==WRITE_BACK && cache_var.write_miss_policy==WRITE_ALLOCATE)
    {

        cout << setw(7) << dec<<cache_table[i].at(j).index << setw(7) << cache_table[i].at(j).dirty_bit << setw(9)
             << hex << "0x" << cache_table[i].at(j).tag << endl;
    }

    else
    {
        cout<<setw(7)<<dec<<cache_table[i].at(j).index<<setw(7)<<"0x"<<hex<<cache_table[i].at(j).tag<<endl;
    }

}

//@Overloaded
void cache::print_cache_status(int i,int j,ofstream* out)

{
    if(cache_var.write_hit_policy==WRITE_BACK && cache_var.write_miss_policy==WRITE_ALLOCATE)
    {

        *out << setw(7) << dec<<cache_table[i].at(j).index << setw(7) << cache_table[i].at(j).dirty_bit << setw(9)
             << hex << "0x" << cache_table[i].at(j).tag << endl;
    }

    else
    {
        *out<<setw(7)<<dec<<cache_table[i].at(j).index<<setw(7)<<"0x"<<hex<<cache_table[i].at(j).tag<<endl;
    }

}

