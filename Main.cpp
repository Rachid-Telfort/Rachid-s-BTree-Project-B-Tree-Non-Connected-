#include"BTree.h"

int main(int argc, char* argv[])
{
    std::cout<<"Welcome to the BTree (B+Tree Non-Connected) manipulator program.\n"<<std::endl;
    std::cout<<"Now you will enter your desired specifications of key length, record length, sector length, and number of sectors that will be used to create a BTree that you will be able to manipulate.\n"<<std::endl;

    int keyLength=0;
    int recordLength=0;
    int sectorLength=0;
    int numberOfSectors=0;
    std::string BTreeSpecs=std::string();
    std::getline(std::cin, BTreeSpecs);
    std::cout<<std::endl;
    std::stringstream BTreeSpecsSS(BTreeSpecs);
    BTreeSpecsSS>>keyLength>>recordLength>>sectorLength>>numberOfSectors;
    BTree* BT=new BTree(keyLength, recordLength, sectorLength, numberOfSectors);

    std::cout<<"Now that the BTree has been created with your desired or default specifications, a menu will now be created to show you the ways you will be able to manipulate the BTree.\n"<<std::endl;
    std::cout<<"1. Insert a record/number of records into the BTree."<<std::endl;
    std::cout<<"2. Print the contents of a sector/number of sectors in the BTree's HDD."<<std::endl;
    std::cout<<"3. Print out the current specification details of the BTree."<<std::endl;
    std::cout<<"4. Search for a record/number of records in the BTree."<<std::endl;
    std::cout<<"5. Remove a record/number of records from the BTree."<<std::endl;
    std::cout<<"6. Print out all the records currently in the BTree."<<std::endl;
    std::cout<<"7. Print out the BTree in level order."<<std::endl;
    std::cout<<"8. Print out all the bits of the HDD (This also contains the BTree bits)."<<std::endl;
    std::cout<<"9. View the current height of the BTree."<<std::endl;
    std::cout<<"10. View the current amount of records in the BTree."<<std::endl;
    std::cout<<"11. Insert records into the BTree from a file."<<std::endl;
    std::cout<<"12. Store the records in the BTree to a file for future use."<<std::endl;
    std::cout<<"13. Clear the BTree for the insertion of a new set/new sets of records."<<std::endl;
    std::cout<<"14. Reformat the HDD to change the BTree specifications as well as the visual representation of the BTree."<<std::endl;
    std::cout<<"15. If running the program on command line and the program has arguments, perform a set/sets of operations on the BTree provided by the command line argument(s)."<<std::endl;
    std::cout<<"16. Quit the BTree (B+Tree Non-Connected) manipulator program.\n"<<std::endl;

    std::cout<<"Now you will choose which operation you want the program to perform on the BTree by entering the number associated with its description.\n"<<std::endl;
    std::string menuChoice=std::string();
    std::getline(std::cin, menuChoice);
    std::cout<<std::endl;
    std::stringstream menuChoiceSS(menuChoice);
    int menuChoice2=0;
    menuChoiceSS>>menuChoice2;
    while(menuChoice2!=16)
    {
        switch(menuChoice2)
        {
            case 1:
            {
                std::cout<<"Now you will type the record(s) that you want to insert into the BTree.\n"<<std::endl;
                std::string recordsInsert=std::string();
                std::getline(std::cin, recordsInsert);
                std::cout<<std::endl;
                std::stringstream recordsInsertSS(recordsInsert);
                while(recordsInsertSS>>recordsInsert)
                {
                    std::stringstream recordsInsert2(recordsInsert);
                    if(BT->isDuplicateKey(recordsInsert2.str(), BT->getRoot())==false)
                    {
                        BT->insertRecord(recordsInsert2.str(), BT->getRoot());
                    }
                }

                break;
            }

            case 2:
            {
                std::cout<<"Now you will type the sector/number of sectors that you want to see its/their content(s) printed.\n"<<std::endl;
                std::string sectorsPrint=std::string();
                std::getline(std::cin, sectorsPrint);
                std::cout<<std::endl;

                int sectorNumber=0;
                std::stringstream sectorsPrintSS(sectorsPrint);
                while(sectorsPrintSS>>sectorsPrint)
                {
                    std::stringstream sectorsPrint2(sectorsPrint);
                    sectorsPrint2>>sectorNumber;
                    BT->printSector(sectorNumber);
                }
                std::cout<<std::endl;

                break;
            }

            case 3:
            {
                std::cout<<"Now the current specification details of the BTree will be displayed to you.\n"<<std::endl;
                std::cout<<"Sector Length: "<<BT->getSectorLength()<<std::endl;
                std::cout<<"Number of Sectors: "<<BT->getNumberOfSectors()<<std::endl;
                std::cout<<"Key Length: "<<BT->getKeyLength()<<std::endl;
                std::cout<<"Record Length "<<BT->getRecordLength()<<std::endl;
                std::cout<<"Number of Records: "<<BT->getNumberOfRecords()<<std::endl;
                std::cout<<"Number of Keys: "<<BT->getNumberOfKeys()<<std::endl;
                std::cout<<"Number of Branches: "<<BT->getNumberOfBranches()<<std::endl;
                std::cout<<"Current Free Sector: "<<BT->getFreeSector()<<std::endl;
                std::cout<<"Current Root: "<<BT->getRoot()<<std::endl;
                std::cout<<std::endl;

                break;
            }

            case 4:
            {
                std::cout<<"Please type the record(s) that you would like to search for in the BTree.\n"<<std::endl;
                std::string recordsSearch=std::string();
                std::getline(std::cin, recordsSearch);
                std::cout<<std::endl;
                std::stringstream recordsSearchSS(recordsSearch);
                while(recordsSearchSS>>recordsSearch)
                {
                    BT->searchRecord(recordsSearch, BT->getRoot());
                }

                break;
            }

            case 5:
            {
                std::cout<<"Please type the record(s) that you would like to remove from the BTree.\n"<<std::endl;
                std::string recordsRemove=std::string();
                std::getline(std::cin, recordsRemove);
                std::cout<<std::endl;
                std::stringstream recordsRemoveSS(recordsRemove);
                while(recordsRemoveSS>>recordsRemove)
                {
                    BT->removeRecord(recordsRemove, BT->getRoot());
                }

                break;
            }

            case 6:
            {
                std::cout<<"Now all the records currently in the BTree will be printed.\n"<<std::endl;
                BT->printAllRecords(BT->getRoot());

                if(BT->getRoot()!=-1)
                {
                    std::cout<<"\n"<<std::endl;
                }

                break;
            }

            case 7:
            {
                std::cout<<"Now the BTree will be printed in level order.\n"<<std::endl;
                BT->printBTree(BT->getRoot());

                if(BT->getRoot()!=-1)
                {
                    std::cout<<std::endl;
                }

                break;
            }

            case 8:
            {
                std::cout<<"Now all the bits of the HDD will be printed.\n"<<std::endl;
                BT->printHDD();
                std::cout<<std::endl;

                break;
            }

            case 9:
            {
                std::cout<<"Now the current height of the BTree will be printed.\n"<<std::endl;
                std::cout<<"The current height of the BTree is "<<BT->getHeight(BT->getRoot(), 0)<<"\n"<<std::endl;

                break;
            }

            case 10:
            {
                std::cout<<"Now the current BTree record count will be printed.\n"<<std::endl;
                std::cout<<"There are currently "<<BT->getRecordCount(BT->getRoot())<<" records in the BTree.\n"<<std::endl;

                break;
            }

            case 11:
            {
                std::cout<<"Please type the file you would like to insert records from.\n"<<std::endl;
                std::string fileInput=std::string();
                std::getline(std::cin, fileInput);
                std::cout<<std::endl;
                BT->inputFromFile(fileInput);

                break;
            }

            case 12:
            {
                std::cout<<"Now you will type the file that you want to store the BTree records to.\n"<<std::endl;
                std::string fileOutput=std::string();
                std::getline(std::cin, fileOutput);
                std::cout<<std::endl;
                BT->outputToFile(fileOutput);

                break;
            }

            case 13:
            {
                std::cout<<"Now the BTree will be cleared.\n"<<std::endl;
                keyLength=BT->getKeyLength();
                recordLength=BT->getRecordLength();
                sectorLength=BT->getSectorLength();
                numberOfSectors=BT->getNumberOfSectors();

                delete BT;
                BT=NULL;
                BT=new BTree(keyLength, recordLength, sectorLength, numberOfSectors);

                break;
            }

            case 14:
            {
                std::cout<<"Please enter the key length, record length, sector length, number of sectors, number of records, and number of keys specifications that you would like for the reformatted BTree and HDD.\n"<<std::endl;
                std::string BTreeSpecs=std::string();
                std::getline(std::cin, BTreeSpecs);
                std::cout<<std::endl;

                int numberOfRecords=0;
                int numberOfKeys=0;
                std::stringstream BTreeSpecsSS(BTreeSpecs);
                BTreeSpecsSS>>keyLength>>recordLength>>sectorLength>>numberOfSectors>>numberOfRecords>>numberOfKeys;
                std::stringstream recordBuffer;
                BT->retrieveAllRecords(BT->getRoot(), recordBuffer);
                delete BT;
                BT=NULL;
                BT=new BTree(keyLength, recordLength, sectorLength, numberOfSectors);

                if(numberOfRecords<=0)
                {
                    numberOfRecords=3;
                }

                else if(numberOfRecords>BT->getNumberOfRecords())
                {
                    numberOfRecords=BT->getNumberOfRecords();
                }

                else if(numberOfRecords%2==0)
                {
                    numberOfRecords--;
                }

                if(numberOfKeys<=0)
                {
                    numberOfKeys=4;
                }

                else if(numberOfKeys>BT->getNumberOfKeys())
                {
                    numberOfKeys=BT->getNumberOfKeys();
                }

                else if(numberOfKeys%2!=0)
                {
                    numberOfKeys--;
                }

                std::stringstream numberOfRecordsSS;
                numberOfRecordsSS<<BT->getNumberOfRecords();

                std::stringstream sectorAddressLengthSS;
                sectorAddressLengthSS<<BT->getNumberOfSectors();

                BT->setNumberOfRecords(numberOfRecords);
                BT->setNumberOfKeys(numberOfKeys);
                BT->setNumberOfBranches(numberOfKeys+1);

                int nextFreeSector=1;
                for(int sectorNumber=0; sectorNumber<BT->getNumberOfSectors(); sectorNumber++)
                {
                    if(sectorNumber==BT->getNumberOfSectors()-1)
                    {
                        nextFreeSector=0;
                    }

                    BT->getHDD()[sectorNumber][0]='0';

                    std::stringstream nextFreeSectorSS;
                    nextFreeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<nextFreeSector;
                    char nextFreeSectorC='\0';
                    int nextFreeSectorBit=1;
                    int nextFreeSectorBit2=nextFreeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
                    while(nextFreeSectorBit<nextFreeSectorBit2)
                    {
                        nextFreeSectorSS>>nextFreeSectorC;
                        BT->getHDD()[sectorNumber][nextFreeSectorBit]=nextFreeSectorC;
                        nextFreeSectorBit++;
                    }
                    BT->getHDD()[sectorNumber][nextFreeSectorBit]='\0';

                    nextFreeSector++;
                }

                std::string recordsInsert=std::string();
                while(recordBuffer>>recordsInsert)
                {
                    BT->insertRecord(recordsInsert, BT->getRoot());
                }

                break;
            }

            case 15:
            {
                if(argc>=2)
                {
                    std::cout<<"Now all the arguments to the program shall be processed so their specified set(s) of operations can be performed on the BTree.\n"<<std::endl;

                    for(int argCount=1; argCount<argc; argCount++)
                    {
                        std::fstream BTfileStream(argv[argCount]);
                        if(BTfileStream.is_open())
                        {
                            std::string BTfileStreamInfo=std::string();
                            while(std::getline(BTfileStream, BTfileStreamInfo))
                            {
                                std::string BTfileOperation=std::string();
                                std::stringstream BTfileStreamInfoSS(BTfileStreamInfo);
                                BTfileStreamInfoSS>>BTfileOperation;

                                if(BTfileOperation=="insert:")
                                {
                                    std::cout<<"Now the records provided by the argument file will be inserted into the BTree.\n"<<std::endl;
                                    std::string recordsInsert=std::string();
                                    while(BTfileStreamInfoSS>>recordsInsert)
                                    {
                                        std::stringstream recordsInsert2(recordsInsert);
                                        if(BT->isDuplicateKey(recordsInsert2.str(), BT->getRoot())==false)
                                        {
                                            BT->insertRecord(recordsInsert2.str(), BT->getRoot());
                                        }
                                    }
                                }

                                else if(BTfileOperation=="print-sector:")
                                {
                                    std::cout<<"Now all the sector numbers provided by the argument file will have their contents printed.\n"<<std::endl;
                                    int sectorNumber=0;
                                    std::string sectorsPrint=std::string();
                                    while(BTfileStreamInfoSS>>sectorsPrint)
                                    {
                                        std::stringstream sectorsPrint2(sectorsPrint);
                                        sectorsPrint2>>sectorNumber;
                                        BT->printSector(sectorNumber);
                                    }
                                    std::cout<<std::endl;
                                }

                                else if(BTfileOperation=="specs:")
                                {
                                    std::cout<<"Now the current specification details of the BTree will be displayed to you.\n"<<std::endl;
                                    std::cout<<"Sector Length: "<<BT->getSectorLength()<<std::endl;
                                    std::cout<<"Number of Sectors: "<<BT->getNumberOfSectors()<<std::endl;
                                    std::cout<<"Key Length: "<<BT->getKeyLength()<<std::endl;
                                    std::cout<<"Record Length "<<BT->getRecordLength()<<std::endl;
                                    std::cout<<"Number of Records: "<<BT->getNumberOfRecords()<<std::endl;
                                    std::cout<<"Number of Keys: "<<BT->getNumberOfKeys()<<std::endl;
                                    std::cout<<"Number of Branches: "<<BT->getNumberOfBranches()<<std::endl;
                                    std::cout<<"Current Free Sector: "<<BT->getFreeSector()<<std::endl;
                                    std::cout<<"Current Root: "<<BT->getRoot()<<std::endl;
                                    std::cout<<std::endl;
                                }

                                else if(BTfileOperation=="search:")
                                {
                                    std::cout<<"Now the records provided by the argument file will be searched for in the BTree.\n"<<std::endl;
                                    std::string recordsSearch=std::string();
                                    while(BTfileStreamInfoSS>>recordsSearch)
                                    {
                                        BT->searchRecord(recordsSearch, BT->getRoot());
                                    }
                                }

                                else if(BTfileOperation=="remove:")
                                {
                                    std::cout<<"Now the records provided by the argument file will be removed from the BTree.\n"<<std::endl;
                                    std::string recordsRemove=std::string();
                                    while(BTfileStreamInfoSS>>recordsRemove)
                                    {
                                        BT->removeRecord(recordsRemove, BT->getRoot());
                                    }
                                }

                                else if(BTfileOperation=="print-records:")
                                {
                                    std::cout<<"Now all the records in the BTre will be printed.\n"<<std::endl;
                                    BT->printAllRecords(BT->getRoot());
                                    if(BT->getRoot()!=-1)
                                    {
                                        std::cout<<"\n"<<std::endl;
                                    }
                                }

                                else if(BTfileOperation=="print-BTree:")
                                {
                                    std::cout<<"Now the BTree will be printed in level order.\n"<<std::endl;
                                    BT->printBTree(BT->getRoot());
                                    if(BT->getRoot()!=-1)
                                    {
                                        std::cout<<std::endl;
                                    }
                                }

                                else if(BTfileOperation=="print-HDD:")
                                {
                                    std::cout<<"Now all the bits of the HDD will be printed.\n"<<std::endl;
                                    BT->printHDD();
                                    std::cout<<std::endl;
                                }

                                else if(BTfileOperation=="height:")
                                {
                                    std::cout<<"Now the current height of the BTree will be printed.\n"<<std::endl;
                                    std::cout<<"The current height of the BTree is "<<BT->getHeight(BT->getRoot(), 0)<<"\n"<<std::endl;
                                }

                                else if(BTfileOperation=="record-count:")
                                {
                                    std::cout<<"Now the current BTree record count will be printed.\n"<<std::endl;
                                    std::cout<<"There are currently "<<BT->getRecordCount(BT->getRoot())<<" records in the BTree.\n"<<std::endl;
                                }

                                else if(BTfileOperation=="file-input:")
                                {
                                    std::cout<<"Now the file path provide by the argument file shall be processed to retrieve records from it for insertion.\n"<<std::endl;
                                    std::string fileInput=std::string();
                                    std::string fileInput2=std::string();
                                    while(BTfileStreamInfoSS>>fileInput)
                                    {
                                        fileInput2+=fileInput+" ";
                                    }
                                    fileInput2=fileInput2.substr(0, fileInput2.find_last_of(" "));
                                    BT->inputFromFile(fileInput2);
                                }

                                else if(BTfileOperation=="file-output:")
                                {
                                    std::cout<<"Now the file path provide by the argument file shall be processed to store records to it for future use.\n"<<std::endl;
                                    std::string fileOutput=std::string();
                                    std::string fileOutput2=std::string();
                                    while(BTfileStreamInfoSS>>fileOutput)
                                    {
                                        fileOutput2+=fileOutput+" ";
                                    }
                                    fileOutput2=fileOutput2.substr(0, fileOutput2.find_last_of(" "));
                                    BT->outputToFile(fileOutput2);
                                }

                                else if(BTfileOperation=="clear:")
                                {
                                    std::cout<<"Now the BTree will be cleared.\n"<<std::endl;

                                    keyLength=BT->getKeyLength();
                                    recordLength=BT->getRecordLength();
                                    sectorLength=BT->getSectorLength();
                                    numberOfSectors=BT->getNumberOfSectors();

                                    delete BT;
                                    BT=NULL;
                                    BT=new BTree(keyLength, recordLength, sectorLength, numberOfSectors);
                                }

                                else if(BTfileOperation=="reformat:")
                                {
                                    std::cout<<"Now the HDD and BTree will be reformatted with the specifications from the argument file.\n"<<std::endl;

                                    int numberOfRecords=0;
                                    int numberOfKeys=0;
                                    BTfileStreamInfoSS>>keyLength>>recordLength>>sectorLength>>numberOfSectors>>numberOfRecords>>numberOfKeys;
                                    std::stringstream recordBuffer;
                                    BT->retrieveAllRecords(BT->getRoot(), recordBuffer);
                                    delete BT;
                                    BT=NULL;
                                    BT=new BTree(keyLength, recordLength, sectorLength, numberOfSectors);

                                    if(numberOfRecords<=0)
                                    {
                                        numberOfRecords=3;
                                    }

                                    else if(numberOfRecords>BT->getNumberOfRecords())
                                    {
                                        numberOfRecords=BT->getNumberOfRecords();
                                    }

                                    else if(numberOfRecords%2==0)
                                    {
                                        numberOfRecords--;
                                    }

                                    if(numberOfKeys<=0)
                                    {
                                        numberOfKeys=4;
                                    }

                                    else if(numberOfKeys>BT->getNumberOfKeys())
                                    {
                                        numberOfKeys=BT->getNumberOfKeys();
                                    }

                                    else if(numberOfKeys%2!=0)
                                    {
                                        numberOfKeys--;
                                    }

                                    std::stringstream numberOfRecordsSS;
                                    numberOfRecordsSS<<BT->getNumberOfRecords();

                                    std::stringstream sectorAddressLengthSS;
                                    sectorAddressLengthSS<<BT->getNumberOfSectors();

                                    BT->setNumberOfRecords(numberOfRecords);
                                    BT->setNumberOfKeys(numberOfKeys);
                                    BT->setNumberOfBranches(numberOfKeys+1);

                                    int nextFreeSector=1;
                                    for(int sectorNumber=0; sectorNumber<BT->getNumberOfSectors(); sectorNumber++)
                                    {
                                        if(sectorNumber==BT->getNumberOfSectors()-1)
                                        {
                                            nextFreeSector=0;
                                        }

                                        BT->getHDD()[sectorNumber][0]='0';

                                        std::stringstream nextFreeSectorSS;
                                        nextFreeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<nextFreeSector;
                                        char nextFreeSectorC='\0';
                                        int nextFreeSectorBit=1;
                                        int nextFreeSectorBit2=nextFreeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
                                        while(nextFreeSectorBit<nextFreeSectorBit2)
                                        {
                                            nextFreeSectorSS>>nextFreeSectorC;
                                            BT->getHDD()[sectorNumber][nextFreeSectorBit]=nextFreeSectorC;
                                            nextFreeSectorBit++;
                                        }
                                        BT->getHDD()[sectorNumber][nextFreeSectorBit]='\0';

                                        nextFreeSector++;
                                    }

                                    std::string recordsInsert=std::string();
                                    while(recordBuffer>>recordsInsert)
                                    {
                                        BT->insertRecord(recordsInsert, BT->getRoot());
                                    }
                                }

                                else
                                {
                                    std::cout<<"The operation "<<BTfileOperation<<" is not a recognizable operation that can be done on the BTree.\n"<<std::endl;
                                }
                            }
                        }

                        else
                        {
                            std::cout<<"There was an error opening the file "<<argv[argCount]<<". Please try to re-format or re-pass the file as an argument to the program to see if it can open then.\n"<<std::endl;
                        }
                    }
                }

                else
                {
                    std::cout<<"You have not provided any arguments to the program to perform a set/sets of operations on it. Please run the program via command line in order to pass arguments to the program that can specify which set(s) of operations to be done on the BTree.\n"<<std::endl;
                }

                break;
            }

            default:
            {
                break;
            }
        }

        std::cout<<"Now a menu will be re-created to show you the ways you are able to manipulate the BTree for reminding purposes.\n"<<std::endl;
        std::cout<<"1. Insert a record/number of records into the BTree."<<std::endl;
        std::cout<<"2. Print the contents of a sector/number of sectors in the BTree's HDD."<<std::endl;
        std::cout<<"3. Print out the current specification details of the BTree."<<std::endl;
        std::cout<<"4. Search for a record/number of records in the BTree."<<std::endl;
        std::cout<<"5. Remove a record/number of records from the BTree."<<std::endl;
        std::cout<<"6. Print out all the records currently in the BTree."<<std::endl;
        std::cout<<"7. Print out the BTree in level order."<<std::endl;
        std::cout<<"8. Print out all the bits of the HDD (This also contains the BTree bits)."<<std::endl;
        std::cout<<"9. View the current height of the BTree."<<std::endl;
        std::cout<<"10. View the current amount of records in the BTree."<<std::endl;
        std::cout<<"11. Insert records into the BTree from a file."<<std::endl;
        std::cout<<"12. Store the records in the BTree to a file for future use."<<std::endl;
        std::cout<<"13. Clear the BTree for the insertion of a new set/new sets of records."<<std::endl;
        std::cout<<"14. Reformat the HDD to change the BTree specifications as well as the visual representation of the BTree."<<std::endl;
        std::cout<<"15. If running the program on command line and the program has arguments, perform a set/sets of operations on the BTree provided by the command line argument(s)."<<std::endl;
        std::cout<<"16. Quit the BTree (B+Tree Non-Connected) manipulator program.\n"<<std::endl;

        std::cout<<"Please enter your next choice of operation on the BTree by entering the number associated with its description.\n"<<std::endl;
        std::getline(std::cin, menuChoice);
        std::cout<<std::endl;
        menuChoiceSS.str(std::string());
        menuChoiceSS.clear();
        menuChoiceSS<<menuChoice;
        menuChoiceSS>>menuChoice2;
    }

    delete BT;
    BT=NULL;

    return 0;
}
