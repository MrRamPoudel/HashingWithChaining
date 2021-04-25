/*
Hopscotch Hashing class

Supports inserting and deleting values from hash table for testing, no search function implemented.

To run in a test program, just call insertValue() and deleteValue() to use the functions.
Made to work in a test program that inserts a vector or array of values, and delete in a loop.

By: Matt Sluder
*/

#ifndef HOPSCOTCH_HPP_INCLUDED
#define HOPSCOTCH_HPP_INCLUDED

/*
   Maximum distance for the table to search or find new value.
   "hops" each value by this many spaces based on the hash key
   Modifying this value changes how many iterations if insert
   or delete can be done before a segmentation fault. 4 seems to work for the most iterations.
*/
const int DIST = 2;

class HopscotchHash
{
    struct Bucket
    {
        bool empty = true;
        int val = 0;
        int hop[DIST];
    };

public:
    HopscotchHash(int newTableSize);
    HopscotchHash();
    void insertValue(int val);
    void deleteValue(int input);
    void deleteKey(int key);
    void resize(int newTableSize);
    void clear();
    ~HopscotchHash()
    {
        delete bucket;
    }
private:
    // getHash() only used within member functions
    int getHash(int theValue);
    Bucket* bucket;
    int tableSize = 0;
    int key = 0;

};

HopscotchHash::HopscotchHash(int newTableSize) {
    bucket = new Bucket[newTableSize];
    tableSize = newTableSize;
}

inline HopscotchHash::HopscotchHash()
{
    tableSize = 0;
    bucket = nullptr;
}

// Inserts a value into the hash table
void HopscotchHash::insertValue(int input)
{
    key = getHash(input);

    // Gets insert key, exits loop if unsuccessful
    for (int i = 0; i < DIST; ++i) {
        if (bucket[key].hop[i] == 0 && bucket[key + i].empty) {
            key = key + i;
            if (key >= tableSize) {
                // Out of bounds
                key = -2;
            }

            bucket[key - i].hop[i] = 1;
            bucket[key].empty = false;
            bucket[key].val = input;
        }
    }

    // Moves the value with recursion if collision, exits loop if unsuccessful
    for (int i = 0; i < DIST; ++i) {
        if (bucket[key].hop[i] == 0) {
            int parentKey = getHash(bucket[key + i].val);
            int distance = (key + i) - parentKey;

            for (int j = 0; j < tableSize; ++j) {
                if (bucket[parentKey + j].empty) {
                    bucket[parentKey + j].empty = false;

                    bucket[parentKey + j].val = bucket[key + i].val;

                    bucket[parentKey].hop[j] = 1;
                    bucket[parentKey].hop[distance] = 0;

                    deleteKey(key + i);
                    insertValue(input);
                    key += i;
                }
            }
        }
    }

    // If insert fails
    key = -1;
}

void HopscotchHash::deleteValue(int input)
{
    // Get hash value for the input
    key = getHash(input);

    // Check the hash value for the range that fits within the hop distance (4 in this function)
    if (bucket[key].hop[0] == 1 && bucket[key].val == input) {
        bucket[key].empty = true;
        bucket[key].val = 0;
        bucket[key].hop[0] = 0;
    }
    else if (bucket[key].hop[1] == 1 && bucket[key + 1].val == input) {
        bucket[key + 1].empty = true;
        bucket[key + 1].val = 0;
        bucket[key].hop[1] = 0;
        key += 1;
    }
    else if (bucket[key].hop[2] == 1 && bucket[key + 2].val == input) {
        bucket[key + 2].empty = true;
        bucket[key + 2].val = 0;
        bucket[key].hop[2] = 0;
        key += 2;
    }
    else if (bucket[key].hop[3] == 1 && bucket[key + 3].val == input) {
        bucket[key + 3].empty = true;
        bucket[key + 3].val = 0;
        bucket[key].hop[3] = 0;
        key += 3;
    }
    else {
        // Value not found
        key = -1;
    }
}

// Unused, should delete a value at a key value given in the input
// Havent tested if this works properly
void HopscotchHash::deleteKey(int input)
{
    bucket[input].empty = true;
    bucket[input].val = 0;
    bucket[input].hop[0] = 0;
}

void HopscotchHash::resize(int newTableSize)
{
    if (bucket)
    {
        delete bucket;
    }
    tableSize = newTableSize;
    bucket = new Bucket[tableSize];
    key = 0;
}

void HopscotchHash::clear()
{
    if (bucket)
    {
        delete bucket;
        bucket = nullptr;
        tableSize = 0;
        key = 0;
    }
}

// Makes a hash key for an input value
int HopscotchHash::getHash(int input)
{
    return input % tableSize;
}

#endif

