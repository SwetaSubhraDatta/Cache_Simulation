//
// Created by Rob Datta on 4/29/21.
//

#include "cache.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>

#define KB 1024

using namespace std;

/* Test case for cache simulator */

/* DO NOT MODIFY */

int main(int argc, char **argv)
{
    ofstream out;
    out.open("testcases/testcase6_matmul.out");


    //WRITE-BACK, WRITE-ALLOCATE

    cache *mycache = new cache(256* KB,        //size
                               1,            //associativity
                               256,            //cache line size
                               WRITE_THROUGH,        //write hit policy
                               NO_WRITE_ALLOCATE,    //write miss policy
                               5,            //hit time
                               100,            //miss penalty
                               32            //address width
    );

    out << "FIRST CONFIGURATION" << endl << "------------------------" << endl << endl;

    mycache->print_configuration(&out);
    out << endl;

    mycache->load_trace("traces/Matmul_size_M0_size_N512.t");

    out << "ONE MEMORY ACCESS AT A TIME for 500 times" << endl;
    out << "===========================" << endl;

//        cout << "MEMORY ACCESS #" << dec << i << endl;
        mycache->run(500);
        mycache->print_tag_array();
        mycache->print_tag_array(&out);
        out << endl;
        cout<<endl;


    mycache->print_statistics(&out);
    mycache->print_statistics();
    out << endl;

    delete mycache;
//    out << "=================================" << endl ;
//    out << "SECOND CONFIGURATION" << endl << "------------------------" << endl << endl;
//
//    mycache = new cache(128*KB,              //size
//                        2,                    //associativity
//                        256,                  //cache line size
//                        WRITE_THROUGH,           //write hit policy
//                        NO_WRITE_ALLOCATE,       //write miss policy
//                        5,                    //hit time
//                        100,                  //miss penalty
//                        32                    //address width
//    );
//
//    mycache->print_configuration(&out);
//    out << endl;
//
//    mycache->load_trace("traces/Matmul_size_M0_size_N5.t");
//
//    out << "ONE MEMORY ACCESS AT A TIME for 20 runs" << endl;
//    for (int i=0; i<20; i++){
//        out << "MEMORY ACCESS #" << dec << i << endl;
//        mycache->run(1);
//        mycache->print_tag_array(&out);
//        out << endl;
//    }
//
//    mycache->print_statistics(&out);
//    mycache->print_statistics();
//    cout << endl;
//
//    delete mycache;
}
