//
//  CollatzClass.hpp
//  Collatz
//
//  Created by DiCode77.
//

#ifndef CollatzClass_hpp
#define CollatzClass_hpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <vector>

#define SPEED_MODE true // boost^ for false 400ms, but increased danger

using int64_t = long long;

typedef struct var{
    int64_t from;
    int64_t to;
} var;

typedef struct Atomic{
#if SPEED_MODE == true
    std::atomic_int64_t val;
    std::atomic_int64_t inc;
#else
    int64_t val;
    int64_t inc;
#endif
} Atomic;


class Collatz{
    int64_t total_val;
    int64_t total_threads;
    int64_t *arr;
    
    std::vector<var> vec_var;
    std::vector<std::thread> thed;
    
    Atomic a_tomac;
    int64_t working_hours;
    std::atomic_bool isBreak_to;
    bool isThreadStop;
    
public:
    Collatz(){
        this->total_val = 0;
        this->total_threads = 0;
        this->arr = nullptr;
        this->a_tomac.val = 0;
        this->a_tomac.inc = 0;
        this->working_hours = 0;
        this->isBreak_to = true;
        this->isThreadStop = false;
    }
    
    
    ~Collatz(){
        delete [] this->arr;
    }
    
    void start();
    void setTotalVal(int64_t);
    void setTotalThreads(int64_t);
    void clear();
    void stop();
    bool isStatus();
    
    Atomic *getResult();
    
    int64_t getTimeWorking();
    
private:
    void CollatzResult(int64_t*, Atomic&, var, std::atomic_bool&);
    int64_t *CreateNewI64(const int64_t);
    void GetCalculationRange();
    
    void SetResizeCacheVector(const int64_t);
};

#endif /* CollatzClass_hpp */
