//
// Created by lix on 28/9/2019.
//

#pragma once

#include <chrono>


typedef std::chrono::duration<int,std::milli> milliseconds_type;

/**
 * @brief using for Timing
 */
class SteadyTimer{
public:
    SteadyTimer(){Reset();}
    ~SteadyTimer() = default;

    /**
     * @param restart true ~ reset the start time; false ~ dont reset
     * @return time(ms) between this call and the call of Reset
     */
    inline double ElapseMs(bool restart = false) {
        _end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = _end_time-_start_time;
        if(restart)
            this->Reset();
        return diff.count()*1000;
    }

    /**reset time start record**/
    inline void Reset(){
        _start_time = std::chrono::steady_clock::now();
    }

private:
    std::chrono::steady_clock::time_point _start_time;
    std::chrono::steady_clock::time_point _end_time;


};
