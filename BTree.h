#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include<cstring>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<vector>

class BTree
{
    private:
        int sectorLength;//This denotes the length of a sector that will reside in the virtual HDD that will contain the BTree.
        int numberOfSectors;//This denotes the total number of sectors that the virtual HDD will contain.
        int keyLength;//This denotes the key length of a record that will reside in the BTree.
        int recordLength;//This denotes the record length, which also includes the key length, of a record that will reside in the BTree.
        int numberOfRecords;//This denotes the total number of records that can reside in a leaf node in the BTree.
        int numberOfKeys;//This denotes the total number of keys that an internal node in the BTree can contain.
        int numberOfBranches;//This denotes the total number of branches that an internal node in the BTree can contain.
        int freeSector;//This denotes the current available sector in the virtual HDD that has no content in it.
        int root;//This denotes the sector address in the virtual HDD that is the current root of the BTree residing in it.
        char** HDD;//This is the pointer to the 2D array that is the virtual HDD of the program.

    public:
        BTree(int, int, int, int);
        ~BTree();
        void setSectorLength(int);
        int getSectorLength();
        void setNumberOfSectors(int);
        int getNumberOfSectors();
        void setKeyLength(int);
        int getKeyLength();
        void setRecordLength(int);
        int getRecordLength();
        void setNumberOfRecords(int);
        int getNumberOfRecords();
        void setNumberOfKeys(int);
        int getNumberOfKeys();
        void setNumberOfBranches(int);
        int getNumberOfBranches();
        void setFreeSector(int);
        int getFreeSector();
        void setRoot(int);
        int getRoot();
        void setHDD(char**);
        char** getHDD();
        void insertRecord(std::string, int);
        void printSector(int);
        void LSort(int);//This function sorts the records in a leaf node in key order.
        void LSplit(int);
        void ISort(int);//This function sorts the keys in an internal node.
        void ISplit(int);
        void searchRecord(std::string, int);
        void removeRecord(std::string, int);
        void LMerge(int);
        void IMerge(int, int);
        bool isDuplicateKey(std::string, int);
        void printAllRecords(int);
        void retrieveBTreeSectors(int, int, std::vector<std::vector<int> >&, std::vector<std::vector<int> >&);
        void printBTree(int);//This function prints the BTree residing in the HDD in height-level order.
        void printHDD();//This function prints all the characters, not including the null character, in the HDD.
        int getHeight(int, int);//This function returns the current height of the BTree residing in the HDD.
        void retrieveRecordCount(int, std::vector<int>&);
        int getRecordCount(int);
        void retrieveAllRecords(int, std::stringstream&);//This function retrieves all the records in the BTree and stores them in a stringstream buffer.
        void inputFromFile(std::string);
        void outputToFile(std::string);
};

#endif//BTREE_H_INCLUDED
