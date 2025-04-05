//
//  CollatzClass.cpp
//  Collatz
//
//  Created by DiCode77.
//

#include "CollatzClass.hpp"

void Collatz::start(){
    if (!this->total_threads || !this->total_val)
        return;
    
    if (this->arr != nullptr)
        delete [] this->arr;
    
    this->arr = CreateNewI64(this->total_val);
    
    GetCalculationRange();
    
    this->isThreadStop = true;
    this->isBreak_to = true;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int64_t i = 0; i < this->total_threads; i++){
        this->thed[i] = std::thread(&Collatz::CollatzResult, this, std::ref(this->arr), std::ref(this->a_tomac), var{this->vec_var[i].from, this->vec_var[i].to}, std::ref(this->isBreak_to));
    }
    
    for (int64_t i = 0; i < this->total_threads; i++){
        this->thed[i].join();
        if (i == this->total_threads -1)
            this->isThreadStop = false;
    }
    
    auto end = std::chrono::high_resolution_clock::now() - start;
    this->working_hours = std::chrono::duration_cast<std::chrono::milliseconds>(end).count();
}

void Collatz::setTotalVal(int64_t val){
    this->total_val = val;
}

void Collatz::setTotalThreads(int64_t val){
    this->total_threads = (val) ? val : 1;
}

void Collatz::clear(){
    if (this->arr != nullptr)
        delete [] this->arr;

    this->total_val = 0;
    this->total_threads = 0;
    this->arr = nullptr;
    this->a_tomac.val = 0;
    this->a_tomac.inc = 0;
    this->working_hours = 0;
}

void Collatz::stop(){
    this->isBreak_to = false;
}
bool Collatz::isStatus(){
    return this->isThreadStop;
}

Atomic *Collatz::getResult(){
    return &this->a_tomac;
}

int64_t Collatz::getTimeWorking(){
    return this->working_hours;
}

void Collatz::CollatzResult(int64_t *cache, Atomic &atomic_m, var start_end, std::atomic_bool &isBreak){
    for (int64_t val = start_end.from; val <= start_end.to && isBreak; val++){
        int64_t val_local = val;
        int64_t itr = 0;
        
        while (val_local != 1 && isBreak){
            if (!(val_local >= val)){
                if (cache[val_local])
                    break;
            }
            
            if (val_local & 1){
                val_local = ((val_local * 3) + 1) >> 1;
                itr += 2;
            }
            else{
                val_local = (val_local >> 1);
                itr++;
            }
        }

        cache[val] = itr + cache[val_local];
        
        if (cache[val] > atomic_m.inc){
            atomic_m.inc = cache[val];
            atomic_m.val = val;
        }
    }
    return;
}

int64_t *Collatz::CreateNewI64(const int64_t size){
    return new int64_t[size +1]{};
}

void Collatz::GetCalculationRange(){
    if (!this->vec_var.empty())
        this->vec_var.clear();
    
//    if (this->total_threads > this->total_val){
//        this->total_threads = this->total_val;
//    }
    
    int64_t res_p     = this->total_val / this->total_threads;
    int64_t remainder = this->total_val % this->total_threads;
    
    
    for (int64_t i = 0; i < this->total_threads; i++){
        int64_t from   = i * res_p + std::min(i, remainder);
        int64_t to_end = from + res_p + (i < remainder ? 1 : 0);
        
        if (from == 0){
            from = 1;
        }
        
        this->vec_var.push_back(var{from +1, to_end});
    }
    SetResizeCacheVector(this->total_threads);
}

void Collatz::SetResizeCacheVector(const int64_t size){
    this->thed.resize(size);
}
