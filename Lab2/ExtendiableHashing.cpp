// Omar Hamed Mohamed - 1170023
#include "ExtendiableHashing.h"
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

void displayNotFound(int key)
{
	std::cout << "\t\t\t key:  " << key << " \t value : \t not found" << std::endl;
}

// Display one record entry, please don't change this function
void displayItem(DataItem *dataItem)
{

	if (dataItem != 0 && dataItem->valid)
	{
		std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
	}
	else
	{
		std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
	}
}

void displayBucket(Bucket &currentBucket, string &depths, string &values, int verbose)
{
	depths.append(std::to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");
	if (verbose)
		std::cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << std::endl;
	if (currentBucket.currentEntries == 0)
	{
		if (verbose)
			std::cout << "\t\t\tNo Data yet\n";
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			values.append("null");
			values.append(",");
		}
	}
	else
	{
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid)
			{
				values.append(std::to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
			}
			else
			{
				values.append("null");
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
			}
		}
	}
	values.pop_back();
	values.append("]");
}

void displayDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket, int verbose)
{

	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	string values = "(";
	string depths = "(";
	int count = 0;
	// string locations = "(";
	if (globaldirectory.length == 0)
	{
		count++;
		std::cout << "\tNo Directory yet\n";
		displayBucket(currentBucket, depths, values, verbose);
	}
	else
	{

		for (int i = 0; i < globaldirectory.length; i++)
		{
			if (i == 0)
			{
				count++;
			}
			else
			{
				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i])
					count++;
			}
			if (verbose)
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				std::cout << "-----------------------------------------------\n\n";
		}
		// values.pop_back();
		depths.pop_back();
	}

	values.append(")");
	depths.append(")");
	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	std::cout << "values:\t" << values << endl;
	std::cout << "depths:\t" << depths << endl;
	std::cout << "=========================\n";
}

// Hashing function and getting directory Index, please don't change this function
int getCurrentHash(int key, int depth)
{

	int hashedKey = key & ((1 << depth) - 1);
	return hashedKey;
}

// TODO1: Implement this function, Don't change the interface please
//  functionlity: try to insert item into a bucket
//  return:   1 if succedded
//			 0 if failed
//  input:   currentBucket, dataItem to be inserted (check ExtendiableHashing.h for the content of the file)
//  Hint1: don't forget to update currentEntries upon insertion, you will need it later

int insertItemIntoBucket(Bucket &currentBucket, DataItem data)
{
	// check if there's enough space in the bucket
	if (currentBucket.currentEntries < RECORDSPERBUCKET)
	{

		// insert the data in the first empty slot
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid == 0)
			{

				currentBucket.dataItem[i] = data;
				// edit valid to 1 to indicate that the slot is filled
				currentBucket.dataItem[i].valid = 1;
				currentBucket.currentEntries++;
				return 1;
			}
		}
	}
	return 0;
}

// TODO2: Implement this function, Don't change the interface please
//  functionlity: look for an item in a bucket using key, if found call displayItem(..), if not found call displayNotFound()
//  return:   nothing
//  input:   currentBucket, key value to search for
//  Hint: use displayNotFound & displayItem functions

void findItemInBucket(Bucket &currentBucket, int key)
{
	// loop on the bucket to find the key
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			displayItem(&currentBucket.dataItem[i]);
			return;
		}
	}
	displayNotFound(key);
}

// TODO3: Implement this function, Don't change the interface please
//  functionlity: try to Delete item based on a key value from a bucket
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   currentBucket, key to be inserted (check ExtendiableHashing.h)
//  Hint1:   don't forget to update currentEntries upon deletion, you will need it later

int deleteItemFromBucket(Bucket &currentBucket, int key)
{
	// loop on the bucket to find the key
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			// edit valid to 0 to indicate that the slot is empty
			currentBucket.dataItem[i].valid = 0;
			currentBucket.currentEntries--;
			return 1;
		}
	}
	return 0;
}

// TODO4: Implement this function, Don't change the interface please
//  functionlity: try to insert item in the file, if the bucket is full, extend the directory,
//				  if you extended the directory five times but it still doesn't work, return 0
//  return:   1 if succedded
//			 0 if failed (when does it fail to insert??)
//  input:   data:--> dataitem to be inserted, currentBucket --> initialBucket before creating director , globaldirectory
//  Hint: use insertItemIntoBucket,getCurrentHash, extendDirectory functions
//  Hint1:   don't forget to check for corner cases, for example if several entries points to the same bucket and you are going to split it
//  Hint2:   a bucket could be split without expanding the directory (think when this will happen?)
//  Hint3:   don't forget to delete extra data

int insertItem(DataItem data, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	printf("Inserting item with key %d\n", data.key);

	if (currentBucket.currentEntries < RECORDSPERBUCKET)
	{
		printf("inserting in bucket\n");
		insertItemIntoBucket(currentBucket, data);
		return 1;
	}
	// case 0 : inserting when directory depth == 0
	if (currentBucket.currentEntries == RECORDSPERBUCKET && globaldirectory.globalDepth == 0)
	{
		printf("inserting in directory\n");
		createFirstTimeDirectory(globaldirectory, currentBucket);
		int hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth);
		insertItemIntoBucket(*globaldirectory.entry[hashedKey], data);
		return 1;
	}
	// case 1 : inserting when directory depth != 0
	int hashindex = getCurrentHash(data.key, globaldirectory.globalDepth);
	// case 1.1 : inserting in a bucket that is not full
	if (globaldirectory.entry[hashindex]->currentEntries < RECORDSPERBUCKET)
	{
		printf("inserting in bucket\n");
		insertItemIntoBucket(*globaldirectory.entry[hashindex], data);
		return 1;
	}
	// case 1.2 : inserting in a bucket that is full
	if (globaldirectory.entry[hashindex]->currentEntries == RECORDSPERBUCKET)
	{
		if (globaldirectory.globalDepth == globaldirectory.entry[hashindex]->localDepth)
		{
			// extend directory
			printf("extending directory\n");
			extendDirectory(globaldirectory, hashindex);
			hashindex = getCurrentHash(data.key, globaldirectory.globalDepth);
			insertItemIntoBucket(*globaldirectory.entry[hashindex], data);
			return 1;
		}
		else if (globaldirectory.globalDepth > globaldirectory.entry[hashindex]->localDepth)
		{
			// extend bucket
			printf("extending bucket\n");
			globaldirectory.entry[hashindex]->localDepth += 1;
			DataItem new_items[2] = {globaldirectory.entry[hashindex]->dataItem[0], globaldirectory.entry[hashindex]->dataItem[1]};
			globaldirectory.entry[hashindex] = new Bucket(globaldirectory.globalDepth);
			int a = 0;
			while (a < 2)
			{
				// always hash with global depth
				int newKey = getCurrentHash(new_items[a].key, globaldirectory.globalDepth);
				if (newKey == -1)
				{
					return -1;
				}
				insertItemIntoBucket(*globaldirectory.entry[newKey], new_items[a]);
				a++;
			}
			hashindex = getCurrentHash(data.key, globaldirectory.globalDepth);
			insertItemIntoBucket(*globaldirectory.entry[hashindex], data);
			return 1;
		}
	}
	return 0;
}

// TODO5: Implement this function, Don't change the interface please
//  functionlity: search the directory for an item using the key
//  return:   nothing
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint1:   use findItemInBucket & getCurrentHash functions

void searchItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	// hint , handle the case where there's no directory first , if so , use findIteminBucket only
	// then if there's directory, search for it using currentHash and findItem
	std::cout << "Search: " << endl;

	if (globaldirectory.globalDepth == 0)
	{
		findItemInBucket(currentBucket, key);
		return;
	}
	int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	std::cout << "hashedkey: " << hashedKey << endl;
	Bucket *b = globaldirectory.entry[hashedKey];
	findItemInBucket(*b, key);
}

// TODO6: Implement this function, Don't change the interface please
//  functionlity: search on an item based on the key and delete it.
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint:    use deleteItemFromBucket & getCurrentHash & checkDirectoryMinimization functions
//  Hint1:   in case the whole bucket is empty, the bucket should be merged again and the pointer should point to the peer bucket
//  Hint2:   in how many steps do we reach our goal?
//  Hint3:   in case of delete success don't forget to call checkDirectoryMinimization to compress directory if needed.
//  Hint4: You might want to loop on checkDirectoryMinimization, not just call it once to continue merging

int deleteItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	// case 0 : deleting when directory depth == 0
	if (globaldirectory.globalDepth == 0)
	{
		// loop on the bucket to find the key
		return deleteItemFromBucket(currentBucket, key);
	}
	return 0;
}

// create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket)
{
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{

		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);

		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}
	return 1;
}

// TODO7: Implement this function, Don't change the interface please
//  functionlity: this expands the directory because we can't find a space anymore in the file,
//                it also redistrubtes data over the split buckets
//  return:   1 if succedded
//			 0 if failed (when does it fail to expand??)
//  input:   Directory, hashKey(bucket index) at which the overflow occurs
//  Hint1:   don't forget todelete unneeded pointers to avoid memory leakage
//  Hint2:   what is the size of the new directory compared to old one? what is the new depth?
//  Hint3:   some entries will point to the same bucket
int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex)
{
	// when we can't expand more ? if global depth = maxkeylength
	if (globaldirectory.length == 0)
	{
		return createFirstTimeDirectory(globaldirectory, *globaldirectory.entry[splitIndex]);
	}
	if (globaldirectory.globalDepth == MAXKEYLENGTH)
		return 0;
	// if the directory is empty, create the first directory
	// here's the tricky case of pointers pointing to the same bucket or a new bucket
	//  we will create a new directory with double the size of the old one
	globaldirectory.globalDepth += 1;
	int oldLength = globaldirectory.length;
	globaldirectory.length *= 2;
	for (int i = oldLength; i < globaldirectory.length; i++)
	{
		if (i != splitIndex + oldLength)
		{
			globaldirectory.entry[i] = globaldirectory.entry[i - oldLength];
		}
		else
		{
			DataItem new_items[2] = {globaldirectory.entry[i - oldLength]->dataItem[0], globaldirectory.entry[i - oldLength]->dataItem[1]};

			globaldirectory.entry[i - oldLength] = new Bucket(globaldirectory.globalDepth);

			globaldirectory.entry[i] = new Bucket(globaldirectory.globalDepth);
			int a = 0;
			while (a < 2)
			{
				// always hash with global depth
				int newKey = getCurrentHash(new_items[a].key, globaldirectory.globalDepth);
				if (newKey == -1)
				{
					return -1;
				}
				insertItemIntoBucket(*globaldirectory.entry[newKey], new_items[a]);
				a++;
			}
		}
	}
	return 1;
}

// If all buckets have depth less than global depth,
//  compress the directory depth by one
//  loop on this function as appropriate to compress the file back
int checkDirectoryMinimization(GlobalDirectory &globaldirectory)
{

	for (int i = 0; i < globaldirectory.length; i++)
	{
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;
	}
	// if its depth =1, there is nothing more we can do, stop here
	if (globaldirectory.length == 1)
		return -1;
	// all are smaller than localdepth, initiate minimization
	//  this is actually too stupid, it should be at least half empty to initiate minimization
	//  but we will keep it like that for simplicity
	int oldGlobalDepth, oldLength;
	Bucket **oldEntry;

	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++)
	{
		globaldirectory.entry[i] = oldEntry[2 * i];
	}

	delete[] oldEntry;
	// delete oldEntry;
	return 1;
}