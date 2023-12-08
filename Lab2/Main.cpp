//============================================================================
// Name        : hashskeleton.cpp
// Author      : Faculty of Engineering, Cairo University
// Version     :
// Description : Hashing using Extendible hashing
//============================================================================

#include <iostream>
#include "ExtendiableHashing.h"

int main()
{

    double score = 0.0;

    // Initialize File
    GlobalDirectory globalDirectory;
    Bucket initialFile;
    // Empty Directory
    std::cout << "Empty \n";
    displayDirectory(globalDirectory, initialFile, 1);
    std::flush(std::cout);
    //============================================================

    // test insertion without making extra directory
    std::cout << "Insert \n";
    std::flush(std::cout);

    insertItem(DataItem(13, 33), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    std::cout << "Search \n";
    std::flush(std::cout);
    // search
    searchItem(13, initialFile, globalDirectory);
    // delete item
    std::cout << "Delete \n";
    std::flush(std::cout);
    deleteItem(13, initialFile, globalDirectory);
    std::cout << "After Delete \n";
    std::flush(std::cout);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    ////////////////////////////////////////////////////////////

    insertItem(DataItem(1, 20), initialFile, globalDirectory);
    insertItem(DataItem(2, 70), initialFile, globalDirectory);
    insertItem(DataItem(42, 11), initialFile, globalDirectory);
    std::cout << "After insert \n";
    std::flush(std::cout);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    insertItem(DataItem(112, 54), initialFile, globalDirectory);
    insertItem(DataItem(240, 16), initialFile, globalDirectory);
    insertItem(DataItem(241, 99), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    // //============================================================
    insertItem(DataItem(159, 30), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    // //============================================================
    insertItem(DataItem(14, 45), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    insertItem(DataItem(13, 88), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================
    insertItem(DataItem(37, 1), initialFile, globalDirectory);
    searchItem(13, initialFile, globalDirectory);
    searchItem(37, initialFile, globalDirectory);
    searchItem(241, initialFile, globalDirectory);
    searchItem(12, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    // //============================================================
    deleteItem(14, initialFile, globalDirectory);
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    // //============================================================
    insertItem(DataItem(158, 5), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    insertItem(DataItem(3, 9), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    return 0;
}
