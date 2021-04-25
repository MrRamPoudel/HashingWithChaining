#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class DynamicHash {
public:
	//constructor
	//Precondition: none
	//Postcondition: initialize everything to 0
	DynamicHash();
	//Precondition: pass the integer as a size
	//Postcondition: creates a table of given size that is bumped to be the next prime number
	DynamicHash(int);
	//Precondition: must positive integer
	//Postcondition: looks through the table to see if the key exists, 1 if found, 0 if not
	bool find(int);
	//Precondition: takes postive integer
	//Postcondition: add the value to the hash table
	void insert(int);
	//Precondition: takes non negative number
	//Postcondition: removes the key if it is in the table, return true if the removal was sucessful
	bool remove(int);
	//Precondition: none
	//Postcondition: return true if the table has no keys
	bool empty()const;
	//Precondition: hashtable with atlease 1 key
	//Postcondition: prints the table from index 0 to table size
	void print();
	//Precondition: takes positive integer greater than 0
	//Postcondition: reinitialises the table with given size bumped to next prime, should only be used for testing
	void resize(int);
	//Precondition: none
	//Postcondition: sets everything to 0.
	void clear();
private:
	std::vector<std::vector<int>> hashTable; //this is the table
	int adjustedSize; //size after bumping it to next prime
	int actualSize; //keys that are in the table
	//Precondition: must pass the insert condition
	//Postcondition: returns a number indicating the index
	int getIndex(int); 
	/***************************
	* the following algorithm can be found at https://gist.github.com/alabombarda/f3944cd68dda390d25cb 
	* Author: alabombarda
	*/
	//Precondition: takes positive integer
	//Postcondition: determines the primality of the number
	int isPrime(int);
	//Precondition: postive integer
	//Postcondition: changes the given number to nearest prime
	int nextPrime(int);
};
