#include "Hash.hpp"

DynamicHash::DynamicHash()
{
	actualSize = 0;
	adjustedSize = 5;
	hashTable.resize(adjustedSize);

}

DynamicHash::DynamicHash(int sizeHint) : actualSize(0), adjustedSize(0)
{
	if (sizeHint <= 0)
	{
		return;
	}
	if (!isPrime(sizeHint))
	{
		adjustedSize = nextPrime(sizeHint);
	}
	else
	{
		adjustedSize = sizeHint;
	}
	hashTable.resize(adjustedSize);
}
int DynamicHash::getIndex(int value)
{
	return value % adjustedSize;
}
void DynamicHash::insert(int value)
{
	/*
	if (actualSize == adjustedSize)
	{
		return;
	}
	*/
	if (value < 0)
	{
		return;
	}
	int keyIndex = getIndex(value);
	if (std::find(hashTable[keyIndex].begin(), hashTable[keyIndex].end(), value) != hashTable[keyIndex].end()) //this means the value is already in the table
	{
		return;
	}
	else
	{
		hashTable[keyIndex].push_back(value);
		++actualSize;
	}
}
bool DynamicHash::find(int value)
{
	if (value < 0)
	{
		return false;
	}
	int keyIndex = getIndex(value);
	return (std::find(hashTable[keyIndex].begin(), hashTable[keyIndex].end(), value) != hashTable[keyIndex].end());
}
bool DynamicHash::remove(int value)
{
	if (value < 0)
	{
		return false;
	}
	int keyIndex = getIndex(value);
	auto iter = std::find(hashTable[keyIndex].begin(), hashTable[keyIndex].end(), value);
	if (iter == hashTable[keyIndex].end()) //if it is not in the bucket
	{
		return false;
	}
	hashTable[keyIndex].erase(iter);
	--actualSize;
	return true;
}
int DynamicHash::isPrime(int value)
{
	for (int i = 2; i <= value / 2; i++)
	{
		if (value % i == 0) {
			return 0;
		}
	}
	return true;
}
int DynamicHash::nextPrime(int value)
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
void DynamicHash::print()
{
	if (empty())
		return;
	for (int i = 0; i < hashTable.size(); ++i)
	{
		for (int j = 0; j < hashTable[i].size(); ++j)
		{
			std::cout << hashTable[i][j] << " ";
		}
		std::cout << '\n';
	}
}
void DynamicHash::resize(int sizeHint)
{
	if (sizeHint <= 0)
	{
		return;
	}
	hashTable.erase(hashTable.begin(),hashTable.end());
	if (!isPrime(sizeHint))
	{
		adjustedSize = nextPrime(sizeHint);
	}
	else
	{
		adjustedSize = sizeHint;
	}
	actualSize = 0;
	hashTable.resize(adjustedSize);
}
void DynamicHash::clear()
{
	hashTable.clear();
}
bool DynamicHash::empty() const
{
	return actualSize == 0;
}
