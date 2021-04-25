// DynamicHashing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <random>
#include "Complexity_Recorder.hpp"
#include "Hash.hpp"
#include<vector>
#include <limits>
#include <unordered_map>
#include "Complexity_Timer.hpp"
#include "Hopscotch.hpp"
using std::string;
using std::vector;
using std::is_same_v;
using std::max;

using std::ofstream;
using std::cout;
using std::endl;
using std::setw;
using std::flush;
using std::unordered_map;
typedef int dataType;

const int number_of_structures = 2;
const int number_of_trials = 1;

const int FACTOR = 1000;
//This will be used to change the table size
//This algorithm was found on here https://gist.github.com/alabombarda/f3944cd68dda390d25cb 
int isPrime(int value)
{
    for (int i = 2; i <= value / 2; i++)
    {
        if (value % i == 0) {
            return 0;
        }
    }
    return true;
}
int nextPrime(int value)
{
    if (!isPrime(value))
    {
        for (int nextPrime = value + 1; ; ++nextPrime)
        {
            if (isPrime(nextPrime))
            {
                return nextPrime;
            }
        }
    }
    return value;
}
//This will be used to generate random number
template<typename T>
class RANDOMGEN {
private:
    std::random_device rand_dev;
    T lowerBound, upperBound;
public:
    RANDOMGEN<T>() : lowerBound(0), upperBound(0) {}
    T getRand(T startFrom, T endHere) {
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<T> distributeIt(startFrom, endHere);
        return distributeIt(generator);
    }
};
class BASE {
protected:
    int N0, N1, N2, N;
    ofstream ofs;
    vector<recorder<timer> > stats;
    vector<int> numContainer;
    timer timer1;
    int repetitions;
    //These are the structures that will be tested
    DynamicHash myHash;
    unordered_map<int,int> stdHash;
    HopscotchHash myHop;
    bool shuffled;
    RANDOMGEN<int> myGen;
    virtual void loadThemUp()
    {
        if (!numContainer.empty())
        {
            numContainer.erase(numContainer.begin(), numContainer.end());
        }
        for (int i = 0; i < N; ++i)
        {
            numContainer.push_back(myGen.getRand(0,std::numeric_limits<int>::max()));
        }
        mixNumbers();
        myHash.resize(N);
        for (int i = 0; i < N; ++i)
        {
            myHash.insert(numContainer[i]);
        }
        /*
        myHop.clear();
        myHop.resize(nextPrime(N));
        for (int i = 0; i < N; ++i)
        {
            myHop.insertValue(numContainer[i]);
        }
        */
        stdHash = unordered_map<int, int>(N); //we want to initialize it by giving it a hint.
        for (int i = 0; i < N; ++i)
        {
            stdHash.insert(std::make_pair(numContainer[i],numContainer[i]));
        }
    }
    void displayHeadings(const char* message[])
    {
        cout << "____";
        for (int i = 0; i < number_of_structures; ++i)
            cout << *message++;
        cout << endl;

        cout << "Size";
        for (int i = 0; i < number_of_structures; ++i)
            cout << "|      Time     ";
        cout << endl;
    }
    void mixNumbers()
    {
        std::random_device myRand;
        std::mt19937 randomDevice(myRand());
        std::shuffle(numContainer.begin(), numContainer.end(), myRand);
    }
    void resetStat()
    {
        for (int i = 0; i < number_of_structures; ++i)
            stats[i].reset();
    }
    void fillVecOnly()
    {
        std::random_device randomDev;
        std::mt19937 rng(randomDev());
        for (int i = 0; i < N; ++i)
        {
            numContainer.push_back(myGen.getRand(0,std::numeric_limits<int>::max()));
        }
        std::shuffle(numContainer.begin(), numContainer.end(), rng);
    }
    void report()
    {
        for (int i = 0; i < number_of_structures; ++i)
        {
            stats[i].report(cout, repetitions);
            stats[i].report(ofs, repetitions);
        }
        cout << endl;
        ofs << endl;
    }
    virtual void testIt() = 0; //derived class must implement this function
    virtual void loop() = 0;
public:
    using uni_distribution = std::uniform_int_distribution<std::mt19937::result_type>;
    BASE() :stats(number_of_structures)
    {
        N1 = 1;
        N2 = 1000;
        N0 = 0;
        N = 0;
        repetitions = max((32 / N1), 1);
        //stats.resize(number_of_structures);
        shuffled = false;
    }
};
class INSERT :public BASE {
private:
    void loop()
    {
        for (N0 = N1; N0 <= N2; N0 *= 2)
        {
            N = N0 * FACTOR;
            cout << setw(4) << N0 << flush;
            ofs << setw(4) << N0;
            fillVecOnly();
            int k;
            for (int j = 0; j < number_of_trials; ++j)
            {
                for (int i = 0; i < number_of_structures; ++i)
                {
                    timer1.restart();
                    for (k = 0; k < repetitions; ++k)
                    {
                        if (i == 0)
                        {  
                            myHash.resize(N);
                            for (int i = 0; i < N; ++i)
                            {
                                myHash.insert(numContainer[i]);
                            }
                            myHash.clear();
                        }
                        else if (i == 1)
                        {
                            stdHash = unordered_map<int, int>(N);
                            for (int i = 0; i < N; ++i)
                            {
                                stdHash.insert(std::make_pair(numContainer[i],numContainer[i]));
                            }
                            stdHash.clear();
                        }
                        //other expirementer's code is causing seg fault(read access violation)
                        /*
                        else if (i == 2)
                        {
                            myHop.clear();
                            myHop.resize(nextPrime(N));
                            for (int i = 0; i < N; ++i)
                            {
                                myHop.insertValue(numContainer[i]);
                            }
                        }
                        */
                    }
                    timer1.stop();
                    //stats[i].record(timer1.count<std::chrono::milliseconds>());
                   stats[i].record(timer1);
                   myHash.clear();
                   myHop.clear();
                   stdHash.erase(stdHash.begin(),stdHash.end());
                }
            }
            for (int i = 0; i < number_of_structures; ++i)
            {
                stats[i].report(cout, repetitions);
                stats[i].report(ofs, repetitions);
            }
            numContainer.erase(numContainer.begin(), numContainer.end());
            cout << endl;
            ofs << endl;
            if (repetitions > 1)
                repetitions /= 2;
        }
    }
public:
    INSERT()
    {
        ofs.open("insert.txt");
    }
    void testIt()
    {
        const char* messages[number_of_structures] =
        { "|DynHash insert",
         " |Map insert "};
        displayHeadings(messages);
        loop();
    }
};
class SEARCH :public BASE {
private:
    void loop()
    {
        for (N0 = N1; N0 <= N2; N0 *= 2)
        {
            N = N0 * FACTOR;
            cout << setw(4) << N0 << flush;
            ofs << setw(4) << N0;
            loadThemUp();
            int k;
            for (int j = 0; j < number_of_trials; ++j)
            {
                for (int i = 0; i < number_of_structures; ++i) //only 2 structures for this
                {
                    timer1.restart();
                    for (k = 0; k < repetitions; ++k)
                    {
                        if (i == 0)
                        {
                            for (int i = 0; i < N; ++i)
                            {
                                myHash.find(numContainer[i]);
                            }  
                        }
                        else if (i == 1)
                        {  
                            for (int i = 0; i < N; ++i)
                            {
                                stdHash.find(numContainer[i]);
                            }
                        }
                    }
                    timer1.stop();    
                    stats[i].record(timer1);
                }
            }
            for (int i = 0; i < number_of_structures; ++i)
            {
                stats[i].report(cout, repetitions);
                stats[i].report(ofs, repetitions);
            }
            numContainer.erase(numContainer.begin(), numContainer.end());
            cout << endl;
            ofs << endl;
            if (repetitions > 1)
                repetitions /= 2;
        }
    }
public:
    SEARCH()
    {
        ofs.open("search.txt");
    }
    void testIt()
    {
        const char* messages[number_of_structures] =
        { "|DynHash Search",
         "|map Search " };
        displayHeadings(messages);
        loop();
    }
};
class REMOVE :public BASE {
private:
    void loop()
    {
        for (N0 = N1; N0 <= N2; N0 *= 2)
        {
            N = N0 * FACTOR;
            cout << setw(4) << N0 << flush;
            ofs << setw(4) << N0;
            loadThemUp();
            int k;
            for (int j = 0; j < number_of_trials; ++j)
            {
                for (int i = 0; i < number_of_structures; ++i)
                {
                    timer1.restart();
                  
                    if (i == 0)
                    {
                        for (int i = 0; i < N; ++i)
                        {
                            myHash.remove(numContainer[i]);
                        }
                    }
                    else if (i == 1)
                    {
                        for (int i = 0; i < N; ++i)
                        {
                            stdHash.erase(numContainer[i]);
                        }
                    }
                    timer1.stop();
                    stats[i].record(timer1);
                }
                numContainer.erase(numContainer.begin(), numContainer.end());
                loadThemUp();
            }
            for (int i = 0; i < number_of_structures; ++i)
            {
                stats[i].report(cout, repetitions);
                stats[i].report(ofs, repetitions);
            }
            numContainer.erase(numContainer.begin(), numContainer.end());
            cout << endl;
            ofs << endl;
            if (repetitions > 1)
                repetitions /= 2;
        }
    }
public:
    REMOVE()
    {
        ofs.open("Remove.txt");
        this->repetitions = 1;
    }
    void testIt()
    {
        const char* messages[number_of_structures] =
        { "|DynHash Remove",
         "|map Remove " };
        displayHeadings(messages);
        loop();
    }
};
int main()
{
   SEARCH myInsert;
    myInsert.testIt();
}

