/*

Defines class recorder<Timer> for recording computing times as
measured by objects of class Timer.  See also recorder.hpp, which
defines another recorder class capable of also recording operation
counts.

*/

/*
 * Copyright (c) 1997 Rensselaer Polytechnic Institute
 *
 *
 */

#ifndef COMPLEXITY_RECORDER_HPP
#define COMPLEXITY_RECORDER_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

using std::cout;

template <class Container>
typename Container::value_type median(Container& c)
{
    typename Container::iterator midpoint = c.begin() + (c.end() - c.begin()) / 2;
    nth_element(c.begin(), midpoint, c.end());
    return *midpoint;
}

template <class Timer>
class recorder {
    std::vector<double> times;

public:
    void record(const Timer& t) {
        times.push_back(t.time());
    }
    void report(std::ostream& o, int repeat_factor)
    {
        o << std::fixed << std::setprecision(3) << std::setw(12)
            << median(times) / repeat_factor;
        o << "     ";
    }
    void reset() {
        times.erase(times.begin(), times.end());
    }
};

#endif

