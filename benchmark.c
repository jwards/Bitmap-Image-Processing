//
// Created by Joseph on 2/23/2019.
//

#include "benchmark.h"


double get_time(){
    LARGE_INTEGER t,f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return(double) t.QuadPart/(double)f.QuadPart;
}