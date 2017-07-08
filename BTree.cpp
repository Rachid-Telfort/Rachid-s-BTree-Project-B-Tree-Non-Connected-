#include"BTree.h"

BTree::BTree(int keyLength, int recordLength, int sectorLength, int numberOfSectors)
{
    this->setKeyLength(keyLength);
    this->setRecordLength(recordLength);
    this->setSectorLength(sectorLength);
    this->setNumberOfSectors(numberOfSectors);

    if(this->getSectorLength()<500)
    {
        this->setSectorLength(500);
    }

    else if(this->getSectorLength()>10000)
    {
        this->setSectorLength(10000);
    }

    if(this->getNumberOfSectors()<500)
    {
        this->setNumberOfSectors(500);
    }

    else if(this->getNumberOfSectors()>10000)
    {
        this->setNumberOfSectors(10000);
    }

    if(this->getKeyLength()<=0||this->getKeyLength()>=10||this->getKeyLength()>=this->getSectorLength())
    {
        this->setKeyLength(1);
    }

    if(this->getRecordLength()<=0||this->getRecordLength()<this->getKeyLength()||this->getRecordLength()>=this->getSectorLength())
    {
        this->setRecordLength(this->getKeyLength()+1);
    }

    std::stringstream sectorAddressLengthSS;
    sectorAddressLengthSS<<this->getNumberOfSectors();
    this->setNumberOfRecords((this->getSectorLength()-this->getRecordLength()-2)/this->getRecordLength());//The constant 2 is for extra data that may be useful on some BTree operations. The extra data constant may vary depending on implementation specifications.
    this->setNumberOfBranches((this->getSectorLength()-strlen(sectorAddressLengthSS.str().c_str())-2)/(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str())));//The constant 2 is for extra data that may be useful on some BTree operations. The extra data constant may vary depending on implementation specifications.

    std::stringstream numberOfRecordsLengthSS;
    numberOfRecordsLengthSS<<this->getNumberOfRecords();
    this->setNumberOfRecords((this->getSectorLength()-this->getRecordLength()-strlen(numberOfRecordsLengthSS.str().c_str())-2)/this->getRecordLength());//The minimum amount of records that can be set for all operations to work is 3.

    std::stringstream numberOfBranchesLengthSS;
    numberOfBranchesLengthSS<<this->getNumberOfBranches();
    this->setNumberOfBranches((this->getSectorLength()-strlen(sectorAddressLengthSS.str().c_str())-strlen(numberOfBranchesLengthSS.str().c_str())-2)/(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str())));

    if(this->getNumberOfRecords()%2==0)
    {
        this->setNumberOfRecords(this->getNumberOfRecords()-1);
    }

    if(this->getNumberOfBranches()%2==0)
    {
        this->setNumberOfBranches(this->getNumberOfBranches()-1);
    }

    this->setNumberOfKeys(this->getNumberOfBranches()-1);//The minimum amount of keys that can be set for all operations to work is 4.
    this->setFreeSector(0);
    this->setRoot(-1);

    this->setHDD(new char*[this->getNumberOfSectors()]);
    for(int sectorCount=0; sectorCount<this->getNumberOfSectors(); sectorCount++)
    {
        this->getHDD()[sectorCount]=new char[this->getSectorLength()];
        memset(this->getHDD()[sectorCount], '\0', this->getSectorLength()*sizeof(char));//This sets all the characters of a sector in the HDD to the null character constant. The expression this->getSectorLength()*sizeof(char) is equivalent to sizeof(this->getHDD()[sectorCount].
    }

    std::stringstream numberOfRecordsSS;
    numberOfRecordsSS<<this->getNumberOfRecords();
    int nextFreeSector=1;
    for(int sectorNumber=0; sectorNumber<this->getNumberOfSectors(); sectorNumber++)
    {
        if(sectorNumber==this->getNumberOfSectors()-1)
        {
            nextFreeSector=0;
        }

        this->getHDD()[sectorNumber][0]='0';

        std::stringstream nextFreeSectorSS;
        nextFreeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<nextFreeSector;
        char nextFreeSectorC='\0';
        int nextFreeSectorBit=1;
        int nextFreeSectorBit2=nextFreeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
        while(nextFreeSectorBit<nextFreeSectorBit2)
        {
            nextFreeSectorSS>>nextFreeSectorC;
            this->getHDD()[sectorNumber][nextFreeSectorBit]=nextFreeSectorC;
            nextFreeSectorBit++;
        }

        nextFreeSector++;
    }
}

BTree::~BTree()
{
    this->setSectorLength(0);
    this->setKeyLength(0);
    this->setRecordLength(0);
    this->setNumberOfRecords(0);
    this->setNumberOfKeys(0);
    this->setNumberOfBranches(0);
    this->setFreeSector(0);
    this->setRoot(0);
    for(int clearSector=0; clearSector<this->getNumberOfSectors(); clearSector++)
    {
        delete[] this->getHDD()[clearSector];
        this->getHDD()[clearSector]=NULL;
    }
    delete[] this->getHDD();
    this->setHDD(NULL);
    this->setNumberOfSectors(0);
}

void BTree::setSectorLength(int sectorLength)
{
    this->sectorLength=sectorLength;
}

int BTree::getSectorLength()
{
    return this->sectorLength;
}

void BTree::setNumberOfSectors(int numberOfSectors)
{
    this->numberOfSectors=numberOfSectors;
}

int BTree::getNumberOfSectors()
{
    return this->numberOfSectors;
}

void BTree::setKeyLength(int keyLength)
{
    this->keyLength=keyLength;
}

int BTree::getKeyLength()
{
    return this->keyLength;
}

void BTree::setRecordLength(int recordLength)
{
    this->recordLength=recordLength;
}

int BTree::getRecordLength()
{
    return this->recordLength;
}

void BTree::setNumberOfRecords(int numberOfRecords)
{
    this->numberOfRecords=numberOfRecords;
}

int BTree::getNumberOfRecords()
{
    return this->numberOfRecords;
}

void BTree::setNumberOfKeys(int numberOfKeys)
{
    this->numberOfKeys=numberOfKeys;
}

int BTree::getNumberOfKeys()
{
    return this->numberOfKeys;
}

void BTree::setNumberOfBranches(int numberOfBranches)
{
    this->numberOfBranches=numberOfBranches;
}

int BTree::getNumberOfBranches()
{
    return this->numberOfBranches;
}

void BTree::setFreeSector(int freeSector)
{
    this->freeSector=freeSector;
}

int BTree::getFreeSector()
{
    return this->freeSector;
}

void BTree::setRoot(int root)
{
    this->root=root;
}

int BTree::getRoot()
{
    return this->root;
}

void BTree::setHDD(char** HDD)
{
    this->HDD=HDD;
}

char** BTree::getHDD()
{
    return this->HDD;
}

void BTree::insertRecord(std::string record, int sectorNumber)
{
    int checkKeyLength=0;
    std::stringstream checkKeyLengthSS(record);
    checkKeyLengthSS>>checkKeyLength;
    checkKeyLengthSS.str(std::string());
    checkKeyLengthSS.clear();
    checkKeyLengthSS<<checkKeyLength;

    if((int)strlen(checkKeyLengthSS.str().c_str())!=this->getKeyLength())
    {
        std::cout<<"The record that you are trying to insert into the BTree does not have the specified key length. Please choose the appropriate menu choice to insert a record with the appropriate key length.\n"<<std::endl;
    }

    else if((int)strlen(record.c_str())!=this->getRecordLength())
    {
        std::cout<<"The record that you are trying to insert into the BTree does not have the specified record length. Please choose the appropriate menu choice to insert a record with the appropriate record length.\n"<<std::endl;
    }

    else
    {
        if(this->getRoot()==-1)
        {
            std::stringstream numberOfRecordsSS;
            numberOfRecordsSS<<this->getNumberOfRecords();

            std::stringstream nextFreeSectorSS;
            char nextFreeSectorC='\0';
            int nextFreeSector=0;
            int nextFreeSectorBit=1;
            while(this->getHDD()[this->getFreeSector()][nextFreeSectorBit]!='\0')
            {
                nextFreeSectorC=this->getHDD()[this->getFreeSector()][nextFreeSectorBit];
                nextFreeSectorSS<<nextFreeSectorC;
                nextFreeSectorBit++;
            }

            std::stringstream recordSS(record);
            char recordC='\0';
            int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
            int recordBit2=recordBit+strlen(recordSS.str().c_str());
            while(recordBit<recordBit2)
            {
                recordSS>>recordC;
                this->getHDD()[this->getFreeSector()][recordBit]=recordC;
                recordBit++;
            }
            this->getHDD()[this->getFreeSector()][recordBit]='\0';

            std::stringstream updateNumberOfRecordsSS;
            updateNumberOfRecordsSS<<std::setfill('0')<<std::setw(strlen(numberOfRecordsSS.str().c_str()))<<1;
            char updateNumberOfRecordsC='\0';
            int numberOfRecordsBit=1;
            while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
            {
                updateNumberOfRecordsSS>>updateNumberOfRecordsC;
                this->getHDD()[this->getFreeSector()][numberOfRecordsBit]=updateNumberOfRecordsC;
                numberOfRecordsBit++;
            }

            this->getHDD()[this->getFreeSector()][0]='1';
            this->setRoot(this->getFreeSector());
            nextFreeSectorSS>>nextFreeSector;
            this->setFreeSector(nextFreeSector);
        }

        else if(this->getHDD()[sectorNumber][0]=='0')
        {
            std::stringstream numberOfRecordsSS;
            numberOfRecordsSS<<this->getNumberOfRecords();

            std::stringstream nextFreeSectorSS;
            char nextFreeSectorC='\0';
            int nextFreeSector=0;
            int nextFreeSectorBit=1;
            while(this->getHDD()[sectorNumber][nextFreeSectorBit]!='\0')
            {
                nextFreeSectorC=this->getHDD()[sectorNumber][nextFreeSectorBit];
                nextFreeSectorSS<<nextFreeSectorC;
                nextFreeSectorBit++;
            }

            std::stringstream recordSS(record);
            char recordC='\0';
            int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
            int recordBit2=recordBit+strlen(recordSS.str().c_str());
            while(recordBit<recordBit2)
            {
                recordSS>>recordC;
                this->getHDD()[sectorNumber][recordBit]=recordC;
                recordBit++;
            }
            this->getHDD()[sectorNumber][recordBit]='\0';

            std::stringstream updateNumberOfRecordsSS;
            updateNumberOfRecordsSS<<std::setfill('0')<<std::setw(strlen(numberOfRecordsSS.str().c_str()))<<1;
            char updateNumberOfRecordsC='\0';
            int numberOfRecordsBit=1;
            while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
            {
                updateNumberOfRecordsSS>>updateNumberOfRecordsC;
                this->getHDD()[sectorNumber][numberOfRecordsBit]=updateNumberOfRecordsC;
                numberOfRecordsBit++;
            }

            this->getHDD()[sectorNumber][0]='1';
            nextFreeSectorSS>>nextFreeSector;
            this->setFreeSector(nextFreeSector);
        }

        else if(this->getHDD()[sectorNumber][0]=='1')
        {
            std::stringstream numberOfRecordsSS;
            numberOfRecordsSS<<this->getNumberOfRecords();

            std::stringstream checkNumberOfRecordsSS;
            char checkNumberOfRecordsC='\0';
            int checkNumberOfRecords=0;
            int numberOfRecordsBit=1;
            while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
            {
                checkNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
                checkNumberOfRecordsSS<<checkNumberOfRecordsC;
                numberOfRecordsBit++;
            }
            checkNumberOfRecordsSS>>checkNumberOfRecords;

            if(checkNumberOfRecords<=this->getNumberOfRecords())
            {
                int updateNumberOfRecords=checkNumberOfRecords+1;
                std::stringstream updateNumberOfRecordsSS;
                updateNumberOfRecordsSS<<std::setfill('0')<<std::setw(strlen(numberOfRecordsSS.str().c_str()))<<updateNumberOfRecords;
                char updateNumberOfRecordsC='\0';
                numberOfRecordsBit=1;
                while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
                {
                    updateNumberOfRecordsSS>>updateNumberOfRecordsC;
                    this->getHDD()[sectorNumber][numberOfRecordsBit]=updateNumberOfRecordsC;
                    numberOfRecordsBit++;
                }

                std::stringstream recordSS(record);
                char recordC='\0';
                int recordBit=strlen(numberOfRecordsSS.str().c_str())+(updateNumberOfRecords-1)*this->getRecordLength()+1;
                int recordBit2=recordBit+strlen(recordSS.str().c_str());
                while(recordBit<recordBit2)
                {
                    recordSS>>recordC;
                    this->getHDD()[sectorNumber][recordBit]=recordC;
                    recordBit++;
                }
                this->getHDD()[sectorNumber][recordBit]='\0';
                this->LSort(sectorNumber);
            }

            if(checkNumberOfRecords+1>this->getNumberOfRecords())
            {
                this->LSplit(sectorNumber);
            }
        }

        else if(this->getHDD()[sectorNumber][0]=='2')
        {
            std::stringstream numberOfKeysSS;
            numberOfKeysSS<<this->getNumberOfKeys();

            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            int recordKey=0;
            std::stringstream recordKeySS(record);
            recordKeySS>>recordKey;

            int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            while(this->getHDD()[sectorNumber][branchBit]!='\0')
            {
                std::stringstream leftBranchSS;
                char leftBranchC='\0';
                int leftBranch=0;
                int leftBranchBit=branchBit;
                int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(leftBranchBit<leftBranchBit2)
                {
                    leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                    leftBranchSS<<leftBranchC;
                    leftBranchBit++;
                }
                leftBranchSS>>leftBranch;

                std::stringstream internalKeySS;
                char internalKeyC='\0';
                int internalKey=0;
                int internalKeyBit=leftBranchBit;
                int internalKeyBit2=internalKeyBit+this->getKeyLength();
                while(internalKeyBit<internalKeyBit2)
                {
                    internalKeyC=this->getHDD()[sectorNumber][internalKeyBit];
                    internalKeySS<<internalKeyC;
                    internalKeyBit++;
                }
                internalKeySS>>internalKey;

                std::stringstream rightBranchSS;
                char rightBranchC='\0';
                int rightBranch=0;
                int rightBranchBit=internalKeyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                    rightBranchSS<<rightBranchC;
                    rightBranchBit++;
                }
                rightBranchSS>>rightBranch;

                branchBit=rightBranchBit;
                if(recordKey<internalKey)
                {
                    this->insertRecord(record, leftBranch);
                    break;
                }

                else if(this->getHDD()[sectorNumber][branchBit]=='\0')
                {
                    this->insertRecord(record, rightBranch);
                    break;
                }

                else
                {
                    branchBit-=strlen(sectorAddressLengthSS.str().c_str());
                }
            }
        }
    }
}

void BTree::printSector(int sectorNumber)
{
    if(sectorNumber>=this->getNumberOfSectors())
    {
        std::cout<<"The sector address requested for printing its contents is outside the range of available sectors in the HDD. Please request a different sector number that is available in the HDD to print its contents by selecting the appropriate menu choice.\n"<<std::endl;
    }

    else
    {
        std::cout<<"The contents of sector "<<sectorNumber<<" are: ";
        int sectorBit=0;
        while(this->getHDD()[sectorNumber][sectorBit]!='\0')
        {
            std::cout<<this->getHDD()[sectorNumber][sectorBit];
            sectorBit++;
        }
        std::cout<<std::endl;
    }
}

void BTree::LSort(int sectorNumber)
{
    std::stringstream numberOfRecordsSS;
    numberOfRecordsSS<<this->getNumberOfRecords();

    std::stringstream sectorNumberOfRecordsSS;
    char sectorNumberOfRecordsC='\0';
    int sectorNumberOfRecords=0;
    int numberOfRecordsBit=1;
    while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
    {
        sectorNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
        sectorNumberOfRecordsSS<<sectorNumberOfRecordsC;
        numberOfRecordsBit++;
    }
    sectorNumberOfRecordsSS>>sectorNumberOfRecords;

    for(int sortCount=0; sortCount<sectorNumberOfRecords; sortCount++)
    {
        int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
        int recordBit2=recordBit+this->getRecordLength();
        while(this->getHDD()[sectorNumber][recordBit2]!='\0')
        {
            std::stringstream firstKeySS;
            char firstKeyC='\0';
            int firstKey=0;
            int firstKeyBit=recordBit;
            int firstKeyBit2=firstKeyBit+this->getKeyLength();
            while(firstKeyBit<firstKeyBit2)
            {
                firstKeyC=this->getHDD()[sectorNumber][firstKeyBit];
                firstKeySS<<firstKeyC;
                firstKeyBit++;
            }
            firstKeySS>>firstKey;

            std::stringstream secondKeySS;
            char secondKeyC='\0';
            int secondKey=0;
            int secondKeyBit=recordBit2;
            int secondKeyBit2=secondKeyBit+this->getKeyLength();
            while(secondKeyBit<secondKeyBit2)
            {
                secondKeyC=this->getHDD()[sectorNumber][secondKeyBit];
                secondKeySS<<secondKeyC;
                secondKeyBit++;
            }
            secondKeySS>>secondKey;

            if(firstKey>secondKey)
            {
                std::stringstream firstRecordSS;
                char firstRecordC='\0';
                int firstRecordBit=recordBit;
                int firstRecordBit2=firstRecordBit+this->getRecordLength();
                while(firstRecordBit<firstRecordBit2)
                {
                    firstRecordC=this->getHDD()[sectorNumber][firstRecordBit];
                    firstRecordSS<<firstRecordC;
                    firstRecordBit++;
                }

                std::stringstream secondRecordSS;
                char secondRecordC='\0';
                int secondRecordBit=recordBit2;
                int secondRecordBit2=secondRecordBit+this->getRecordLength();
                while(secondRecordBit<secondRecordBit2)
                {
                    secondRecordC=this->getHDD()[sectorNumber][secondRecordBit];
                    secondRecordSS<<secondRecordC;
                    secondRecordBit++;
                }

                firstRecordBit-=this->getRecordLength();
                while(firstRecordBit<firstRecordBit2)
                {
                    secondRecordSS>>secondRecordC;
                    this->getHDD()[sectorNumber][firstRecordBit]=secondRecordC;
                    firstRecordBit++;
                }

                secondRecordBit-=this->getRecordLength();
                while(secondRecordBit<secondRecordBit2)
                {
                    firstRecordSS>>firstRecordC;
                    this->getHDD()[sectorNumber][secondRecordBit]=firstRecordC;
                    secondRecordBit++;
                }
            }

            recordBit=recordBit2;
            recordBit2+=this->getRecordLength();
        }
    }
}

void BTree::LSplit(int sectorNumber)
{
    if(this->getHDD()[this->getFreeSector()][0]!='0')
    {
        std::cout<<"The HDD that contains the BTree is currently full. Please choose the appropriate menu choice to reformat the HDD in order to be able to insert more records into the BTree.\n"<<std::endl;
    }

    else
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        std::stringstream retrieveNumberOfRecordsSS;
        char retrieveNumberOfRecordsC='\0';
        int retrieveNumberOfRecords=0;
        int numberOfRecordsBit=1;
        while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
        {
            retrieveNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
            retrieveNumberOfRecordsSS<<retrieveNumberOfRecordsC;
            numberOfRecordsBit++;
        }
        retrieveNumberOfRecordsSS>>retrieveNumberOfRecords;

        std::stringstream retrieveHalfOfRecordsSS;
        char retrieveHalfOfRecordsC='\0';
        int halfSectorBit=strlen(numberOfRecordsSS.str().c_str())+(retrieveNumberOfRecords/2)*this->getRecordLength()+1;
        while(this->getHDD()[sectorNumber][halfSectorBit]!='\0')
        {
            retrieveHalfOfRecordsC=this->getHDD()[sectorNumber][halfSectorBit];
            retrieveHalfOfRecordsSS<<retrieveHalfOfRecordsC;
            halfSectorBit++;
        }
        halfSectorBit-=((retrieveNumberOfRecords/2)*this->getRecordLength());
        this->getHDD()[sectorNumber][halfSectorBit]='\0';

        std::stringstream resetNumberOfRecordsSS;
        resetNumberOfRecordsSS<<std::setfill('0')<<std::setw(strlen(numberOfRecordsSS.str().c_str()))<<retrieveNumberOfRecords/2;
        char resetNumberOfRecordsC='\0';
        numberOfRecordsBit=1;
        while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
        {
            resetNumberOfRecordsSS>>resetNumberOfRecordsC;
            this->getHDD()[sectorNumber][numberOfRecordsBit]=resetNumberOfRecordsC;
            numberOfRecordsBit++;
        }

        int newLeaf=this->getFreeSector();
        std::stringstream recordSS;
        int recordCount=0;
        int recordCount2=this->getRecordLength();
        while(retrieveHalfOfRecordsSS>>retrieveHalfOfRecordsC)
        {
            recordSS<<retrieveHalfOfRecordsC;
            recordCount++;
            if(recordCount==recordCount2)
            {
                this->insertRecord(recordSS.str(), newLeaf);
                recordCount2+=this->getRecordLength();
                recordSS.str(std::string());
                recordSS.clear();
            }
        }

        if(sectorNumber==this->getRoot())
        {
            std::stringstream nextFreeSectorSS;
            char nextFreeSectorC='\0';
            int nextFreeSector=0;
            int nextFreeSectorBit=1;
            while(this->getHDD()[this->getFreeSector()][nextFreeSectorBit]!='\0')
            {
                nextFreeSectorC=this->getHDD()[this->getFreeSector()][nextFreeSectorBit];
                nextFreeSectorSS<<nextFreeSectorC;
                nextFreeSectorBit++;
            }

            std::stringstream numberOfKeysSS;
            numberOfKeysSS<<this->getNumberOfKeys();

            std::stringstream updateNumberOfKeysSS;
            updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<1;
            char updateNumberOfKeysC='\0';
            int numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                updateNumberOfKeysSS>>updateNumberOfKeysC;
                this->getHDD()[this->getFreeSector()][numberOfKeysBit]=updateNumberOfKeysC;
                numberOfKeysBit++;
            }

            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            std::stringstream leftBranchSS;
            leftBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<sectorNumber;
            char leftBranchC='\0';
            int leftBranchBit=numberOfKeysBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchSS>>leftBranchC;
                this->getHDD()[this->getFreeSector()][leftBranchBit]=leftBranchC;
                leftBranchBit++;
            }

            std::stringstream internalKeySS;
            char internalKeyC='\0';
            int leafKeyBit=strlen(numberOfRecordsSS.str().c_str())+1;
            int leafKeyBit2=leafKeyBit+this->getKeyLength();
            while(leafKeyBit<leafKeyBit2)
            {
                internalKeyC=this->getHDD()[newLeaf][leafKeyBit];
                internalKeySS<<internalKeyC;
                leafKeyBit++;
            }

            int internalKeyBit=leftBranchBit;
            int internalKeyBit2=internalKeyBit+this->getKeyLength();
            while(internalKeyBit<internalKeyBit2)
            {
                internalKeySS>>internalKeyC;
                this->getHDD()[this->getFreeSector()][internalKeyBit]=internalKeyC;
                internalKeyBit++;
            }

            std::stringstream rightBranchSS;
            rightBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<newLeaf;
            char rightBranchC='\0';
            int rightBranchBit=internalKeyBit;
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchSS>>rightBranchC;
                this->getHDD()[this->getFreeSector()][rightBranchBit]=rightBranchC;
                rightBranchBit++;
            }
            this->getHDD()[this->getFreeSector()][rightBranchBit]='\0';
            this->getHDD()[this->getFreeSector()][0]='2';
            this->setRoot(this->getFreeSector());
            nextFreeSectorSS>>nextFreeSector;
            this->setFreeSector(nextFreeSector);
        }

        else
        {
            std::stringstream leafKeySS;
            char leafKeyC='\0';
            int leafKey=0;
            int leafKeyBit=strlen(numberOfRecordsSS.str().c_str())+((retrieveNumberOfRecords/2)-1)*this->getRecordLength()+1;
            int leafKeyBit2=leafKeyBit+this->getKeyLength();
            while(leafKeyBit<leafKeyBit2)
            {
                leafKeyC=this->getHDD()[sectorNumber][leafKeyBit];
                leafKeySS<<leafKeyC;
                leafKeyBit++;
            }
            leafKeySS>>leafKey;

            std::stringstream numberOfKeysSS;
            numberOfKeysSS<<this->getNumberOfKeys();

            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            int parent=this->getRoot();
            int child=this->getRoot();
            int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            while(child!=sectorNumber)
            {
                std::stringstream leftBranchSS;
                char leftBranchC='\0';
                int leftBranch=0;
                int leftBranchBit=branchBit;
                int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(leftBranchBit<leftBranchBit2)
                {
                    leftBranchC=this->getHDD()[parent][leftBranchBit];
                    leftBranchSS<<leftBranchC;
                    leftBranchBit++;
                }
                leftBranchSS>>leftBranch;

                std::stringstream internalKeySS;
                char internalKeyC='\0';
                int internalKey=0;
                int internalKeyBit=leftBranchBit;
                int internalKeyBit2=internalKeyBit+this->getKeyLength();
                while(internalKeyBit<internalKeyBit2)
                {
                    internalKeyC=this->getHDD()[parent][internalKeyBit];
                    internalKeySS<<internalKeyC;
                    internalKeyBit++;
                }
                internalKeySS>>internalKey;

                std::stringstream rightBranchSS;
                char rightBranchC='\0';
                int rightBranch=0;
                int rightBranchBit=internalKeyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchC=this->getHDD()[parent][rightBranchBit];
                    rightBranchSS<<rightBranchC;
                    rightBranchBit++;
                }
                rightBranchSS>>rightBranch;

                branchBit=rightBranchBit;
                if(leafKey<internalKey)
                {
                    parent=child;
                    child=leftBranch;
                    branchBit=strlen(numberOfKeysSS.str().c_str())+1;
                }

                else if(this->getHDD()[parent][branchBit]=='\0')
                {
                    parent=child;
                    child=rightBranch;
                    branchBit=strlen(numberOfKeysSS.str().c_str())+1;
                }

                else
                {
                    branchBit-=strlen(sectorAddressLengthSS.str().c_str());
                }
            }

            std::stringstream checkNumberOfKeysSS;
            char checkNumberOfKeysC='\0';
            int checkNumberOfKeys=0;
            int numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
                checkNumberOfKeysSS<<checkNumberOfKeysC;
                numberOfKeysBit++;
            }
            checkNumberOfKeysSS>>checkNumberOfKeys;

            if(checkNumberOfKeys<=this->getNumberOfKeys())
            {
                int updateNumberOfKeys=checkNumberOfKeys+1;
                std::stringstream updateNumberOfKeysSS;
                updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
                char updateNumberOfKeysC='\0';
                numberOfKeysBit=1;
                while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
                {
                    updateNumberOfKeysSS>>updateNumberOfKeysC;
                    this->getHDD()[parent][numberOfKeysBit]=updateNumberOfKeysC;
                    numberOfKeysBit++;
                }

                std::stringstream internalKeySS;
                char internalKeyC='\0';
                int leafKeyBit=strlen(numberOfRecordsSS.str().c_str())+1;
                int leafKeyBit2=leafKeyBit+this->getKeyLength();
                while(leafKeyBit<leafKeyBit2)
                {
                    internalKeyC=this->getHDD()[newLeaf][leafKeyBit];
                    internalKeySS<<internalKeyC;
                    leafKeyBit++;
                }

                int internalKeyBit=strlen(numberOfKeysSS.str().c_str())+(updateNumberOfKeys*strlen(sectorAddressLengthSS.str().c_str()))+(updateNumberOfKeys-1)*this->getKeyLength()+1;
                int internalKeyBit2=internalKeyBit+this->getKeyLength();
                while(internalKeyBit<internalKeyBit2)
                {
                    internalKeySS>>internalKeyC;
                    this->getHDD()[parent][internalKeyBit]=internalKeyC;
                    internalKeyBit++;
                }

                std::stringstream rightBranchSS;
                rightBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<newLeaf;
                char rightBranchC='\0';
                int rightBranchBit=internalKeyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchSS>>rightBranchC;
                    this->getHDD()[parent][rightBranchBit]=rightBranchC;
                    rightBranchBit++;
                }
                this->getHDD()[parent][rightBranchBit]='\0';
                this->ISort(parent);
            }

            if(checkNumberOfKeys+1>this->getNumberOfKeys())
            {
                this->ISplit(parent);
            }
        }
    }
}

void BTree::ISort(int sectorNumber)
{
    std::stringstream numberOfKeysSS;
    numberOfKeysSS<<this->getNumberOfKeys();

    std::stringstream sectorAddressLengthSS;
    sectorAddressLengthSS<<this->getNumberOfSectors();

    std::stringstream sectorNumberOfKeysSS;
    char sectorNumberOfKeysC='\0';
    int sectorNumberOfKeys=0;
    int numberOfKeysBit=1;
    while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
    {
        sectorNumberOfKeysC=this->getHDD()[sectorNumber][numberOfKeysBit];
        sectorNumberOfKeysSS<<sectorNumberOfKeysC;
        numberOfKeysBit++;
    }
    sectorNumberOfKeysSS>>sectorNumberOfKeys;

    for(int sortCount=0; sortCount<sectorNumberOfKeys; sortCount++)
    {
        int keyBit=strlen(numberOfKeysSS.str().c_str())+strlen(sectorAddressLengthSS.str().c_str())+1;
        int keyBit2=keyBit+strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength();
        while(this->getHDD()[sectorNumber][keyBit2]!='\0')
        {
            std::stringstream firstKeySS;
            char firstKeyC='\0';
            int firstKey=0;
            int firstKeyBit=keyBit;
            int firstKeyBit2=firstKeyBit+this->getKeyLength();
            while(firstKeyBit<firstKeyBit2)
            {
                firstKeyC=this->getHDD()[sectorNumber][firstKeyBit];
                firstKeySS<<firstKeyC;
                firstKeyBit++;
            }
            firstKeySS>>firstKey;

            std::stringstream secondKeySS;
            char secondKeyC='\0';
            int secondKey=0;
            int secondKeyBit=keyBit2;
            int secondKeyBit2=secondKeyBit+this->getKeyLength();
            while(secondKeyBit<secondKeyBit2)
            {
                secondKeyC=this->getHDD()[sectorNumber][secondKeyBit];
                secondKeySS<<secondKeyC;
                secondKeyBit++;
            }
            secondKeySS>>secondKey;

            if(firstKey>secondKey)
            {
                std::stringstream firstKeyBranchSS;
                char firstKeyBranchC='\0';
                int firstKeyBranchBit=keyBit;
                int firstKeyBranchBit2=firstKeyBranchBit+strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength();
                while(firstKeyBranchBit<firstKeyBranchBit2)
                {
                    firstKeyBranchC=this->getHDD()[sectorNumber][firstKeyBranchBit];
                    firstKeyBranchSS<<firstKeyBranchC;
                    firstKeyBranchBit++;
                }

                std::stringstream secondKeyBranchSS;
                char secondKeyBranchC='\0';
                int secondKeyBranchBit=keyBit2;
                int secondKeyBranchBit2=secondKeyBranchBit+strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength();
                while(secondKeyBranchBit<secondKeyBranchBit2)
                {
                    secondKeyBranchC=this->getHDD()[sectorNumber][secondKeyBranchBit];
                    secondKeyBranchSS<<secondKeyBranchC;
                    secondKeyBranchBit++;
                }

                firstKeyBranchBit-=(strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength());
                while(firstKeyBranchBit<firstKeyBranchBit2)
                {
                    secondKeyBranchSS>>secondKeyBranchC;
                    this->getHDD()[sectorNumber][firstKeyBranchBit]=secondKeyBranchC;
                    firstKeyBranchBit++;
                }

                secondKeyBranchBit-=(strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength());
                while(secondKeyBranchBit<secondKeyBranchBit2)
                {
                    firstKeyBranchSS>>firstKeyBranchC;
                    this->getHDD()[sectorNumber][secondKeyBranchBit]=firstKeyBranchC;
                    secondKeyBranchBit++;
                }
            }

            keyBit=keyBit2;
            keyBit2+=(strlen(sectorAddressLengthSS.str().c_str())+this->getKeyLength());
        }
    }
}

void BTree::ISplit(int sectorNumber)
{
    if(this->getHDD()[this->getFreeSector()][0]!='0')
    {
        std::cout<<"The HDD that contains the BTree is currently full. Please choose the appropriate menu choice to reformat the HDD in order to be able to insert more records into the BTree.\n"<<std::endl;
    }

    else
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        std::stringstream retrieveNumberOfKeysSS;
        char retrieveNumberOfKeysC='\0';
        int retrieveNumberOfKeys=0;
        int numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            retrieveNumberOfKeysC=this->getHDD()[sectorNumber][numberOfKeysBit];
            retrieveNumberOfKeysSS<<retrieveNumberOfKeysC;
            numberOfKeysBit++;
        }
        retrieveNumberOfKeysSS>>retrieveNumberOfKeys;

        std::stringstream retrieveHalfOfKeyBranchesSS;
        char retrieveHalfOfKeyBranchesC='\0';
        int halfSectorBit=strlen(numberOfKeysSS.str().c_str())+((retrieveNumberOfKeys+1)/2)*strlen(sectorAddressLengthSS.str().c_str())+((retrieveNumberOfKeys-1)/2)*this->getKeyLength()+1;
        while(this->getHDD()[sectorNumber][halfSectorBit]!='\0')
        {
            retrieveHalfOfKeyBranchesC=this->getHDD()[sectorNumber][halfSectorBit];
            retrieveHalfOfKeyBranchesSS<<retrieveHalfOfKeyBranchesC;
            halfSectorBit++;
        }
        halfSectorBit-=(((retrieveNumberOfKeys+1)/2)*strlen(sectorAddressLengthSS.str().c_str())+((retrieveNumberOfKeys-1)/2+1)*this->getKeyLength());
        this->getHDD()[sectorNumber][halfSectorBit]='\0';

        std::stringstream resetNumberOfKeysSS;
        resetNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<(retrieveNumberOfKeys-1)/2;
        char resetNumberOfKeysC='\0';
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            resetNumberOfKeysSS>>resetNumberOfKeysC;
            this->getHDD()[sectorNumber][numberOfKeysBit]=resetNumberOfKeysC;
            numberOfKeysBit++;
        }

        std::stringstream retrieveParentKeySS;
        char retrieveParentKeyC='\0';
        for(int retrieveParentKey=0; retrieveParentKey<this->getKeyLength(); retrieveParentKey++)
        {
            retrieveHalfOfKeyBranchesSS>>retrieveParentKeyC;
            retrieveParentKeySS<<retrieveParentKeyC;
        }

        std::stringstream nextFreeSectorSS;
        char nextFreeSectorC='\0';
        int nextFreeSector=0;
        int nextFreeSectorBit=1;
        while(this->getHDD()[this->getFreeSector()][nextFreeSectorBit]!='\0')
        {
            nextFreeSectorC=this->getHDD()[this->getFreeSector()][nextFreeSectorBit];
            nextFreeSectorSS<<nextFreeSectorC;
            nextFreeSectorBit++;
        }
        nextFreeSectorSS>>nextFreeSector;

        std::stringstream updateNumberOfKeysSS;
        updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<(retrieveNumberOfKeys-1)/2;
        char updateNumberOfKeysC='\0';
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            updateNumberOfKeysSS>>updateNumberOfKeysC;
            this->getHDD()[this->getFreeSector()][numberOfKeysBit]=updateNumberOfKeysC;
            numberOfKeysBit++;
        }

        int keyBranchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(retrieveHalfOfKeyBranchesSS>>retrieveHalfOfKeyBranchesC)
        {
            this->getHDD()[this->getFreeSector()][keyBranchBit]=retrieveHalfOfKeyBranchesC;
            keyBranchBit++;
        }
        this->getHDD()[this->getFreeSector()][keyBranchBit]='\0';
        this->getHDD()[this->getFreeSector()][0]='2';
        int newInternal=this->getFreeSector();
        this->setFreeSector(nextFreeSector);

        if(sectorNumber==this->getRoot())
        {
            nextFreeSectorSS.str(std::string());
            nextFreeSectorSS.clear();
            nextFreeSector=0;
            nextFreeSectorBit=1;
            while(this->getHDD()[this->getFreeSector()][nextFreeSectorBit]!='\0')
            {
                nextFreeSectorC=this->getHDD()[this->getFreeSector()][nextFreeSectorBit];
                nextFreeSectorSS<<nextFreeSectorC;
                nextFreeSectorBit++;
            }

            updateNumberOfKeysSS.str(std::string());
            updateNumberOfKeysSS.clear();
            updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<1;
            numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                updateNumberOfKeysSS>>updateNumberOfKeysC;
                this->getHDD()[this->getFreeSector()][numberOfKeysBit]=updateNumberOfKeysC;
                numberOfKeysBit++;
            }

            std::stringstream leftBranchSS;
            leftBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<sectorNumber;
            char leftBranchC='\0';
            int leftBranchBit=numberOfKeysBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchSS>>leftBranchC;
                this->getHDD()[this->getFreeSector()][leftBranchBit]=leftBranchC;
                leftBranchBit++;
            }

            int keyBit=leftBranchBit;
            int keyBit2=keyBit+this->getKeyLength();
            while(keyBit<keyBit2)
            {
                retrieveParentKeySS>>retrieveParentKeyC;
                this->getHDD()[this->getFreeSector()][keyBit]=retrieveParentKeyC;
                keyBit++;
            }

            std::stringstream rightBranchSS;
            rightBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<newInternal;
            char rightBranchC='\0';
            int rightBranchBit=keyBit;
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchSS>>rightBranchC;
                this->getHDD()[this->getFreeSector()][rightBranchBit]=rightBranchC;
                rightBranchBit++;
            }
            this->getHDD()[this->getFreeSector()][rightBranchBit]='\0';
            this->getHDD()[this->getFreeSector()][0]='2';
            this->setRoot(this->getFreeSector());
            nextFreeSectorSS>>nextFreeSector;
            this->setFreeSector(nextFreeSector);
        }

        else
        {
            std::stringstream internalChildKeySS;
            char internalChildKeyC='\0';
            int internalChildKey=0;
            int keyBit=strlen(numberOfKeysSS.str().c_str())+(((retrieveNumberOfKeys+1)/2)-1)*strlen(sectorAddressLengthSS.str().c_str())+(((retrieveNumberOfKeys-1)/2)-1)*this->getKeyLength()+1;
            int keyBit2=keyBit+this->getKeyLength();
            while(keyBit<keyBit2)
            {
                internalChildKeyC=this->getHDD()[sectorNumber][keyBit];
                internalChildKeySS<<internalChildKeyC;
                keyBit++;
            }
            internalChildKeySS>>internalChildKey;

            int parent=this->getRoot();
            int child=this->getRoot();
            int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            while(child!=sectorNumber)
            {
                std::stringstream leftBranchSS;
                char leftBranchC='\0';
                int leftBranch=0;
                int leftBranchBit=branchBit;
                int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(leftBranchBit<leftBranchBit2)
                {
                    leftBranchC=this->getHDD()[parent][leftBranchBit];
                    leftBranchSS<<leftBranchC;
                    leftBranchBit++;
                }
                leftBranchSS>>leftBranch;

                std::stringstream internalParentKeySS;
                char internalParentKeyC='\0';
                int internalParentKey=0;
                keyBit=leftBranchBit;
                keyBit2=keyBit+this->getKeyLength();
                while(keyBit<keyBit2)
                {
                    internalParentKeyC=this->getHDD()[parent][keyBit];
                    internalParentKeySS<<internalParentKeyC;
                    keyBit++;
                }
                internalParentKeySS>>internalParentKey;

                std::stringstream rightBranchSS;
                char rightBranchC='\0';
                int rightBranch=0;
                int rightBranchBit=keyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchC=this->getHDD()[parent][rightBranchBit];
                    rightBranchSS<<rightBranchC;
                    rightBranchBit++;
                }
                rightBranchSS>>rightBranch;

                branchBit=rightBranchBit;
                if(internalChildKey<internalParentKey)
                {
                    parent=child;
                    child=leftBranch;
                    branchBit=strlen(numberOfKeysSS.str().c_str())+1;
                }

                else if(this->getHDD()[parent][branchBit]=='\0')
                {
                    parent=child;
                    child=rightBranch;
                    branchBit=strlen(numberOfKeysSS.str().c_str())+1;
                }

                else
                {
                    branchBit-=strlen(sectorAddressLengthSS.str().c_str());
                }
            }

            std::stringstream checkNumberOfKeysSS;
            char checkNumberOfKeysC='\0';
            int checkNumberOfKeys=0;
            numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
                checkNumberOfKeysSS<<checkNumberOfKeysC;
                numberOfKeysBit++;
            }
            checkNumberOfKeysSS>>checkNumberOfKeys;

            if(checkNumberOfKeys<=this->getNumberOfKeys())
            {
                int updateNumberOfKeys=checkNumberOfKeys+1;
                std::stringstream updateNumberOfKeysSS;
                updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
                char updateNumberOfKeysC='\0';
                numberOfKeysBit=1;
                while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
                {
                    updateNumberOfKeysSS>>updateNumberOfKeysC;
                    this->getHDD()[parent][numberOfKeysBit]=updateNumberOfKeysC;
                    numberOfKeysBit++;
                }

                keyBit=strlen(numberOfKeysSS.str().c_str())+(updateNumberOfKeys*strlen(sectorAddressLengthSS.str().c_str()))+(updateNumberOfKeys-1)*this->getKeyLength()+1;
                keyBit2=keyBit+this->getKeyLength();
                while(keyBit<keyBit2)
                {
                    retrieveParentKeySS>>retrieveParentKeyC;
                    this->getHDD()[parent][keyBit]=retrieveParentKeyC;
                    keyBit++;
                }

                std::stringstream rightBranchSS;
                rightBranchSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<newInternal;
                char rightBranchC='\0';
                int rightBranchBit=keyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchSS>>rightBranchC;
                    this->getHDD()[parent][rightBranchBit]=rightBranchC;
                    rightBranchBit++;
                }
                this->getHDD()[parent][rightBranchBit]='\0';
                this->ISort(parent);
            }

            if(checkNumberOfKeys+1>this->getNumberOfKeys())
            {
                this->ISplit(parent);
            }
        }
    }
}

void BTree::searchRecord(std::string record, int sectorNumber)
{
    int checkKeyLength=0;
    std::stringstream checkKeyLengthSS(record);
    checkKeyLengthSS>>checkKeyLength;
    checkKeyLengthSS.str(std::string());
    checkKeyLengthSS.clear();
    checkKeyLengthSS<<checkKeyLength;

    if((int)strlen(checkKeyLengthSS.str().c_str())!=this->getKeyLength())
    {
        std::cout<<"The record that you are trying to search for in the BTree does not have the specified key length. Please choose the appropriate menu choice to search for a record with the appropriate key length.\n"<<std::endl;
    }

    else if((int)strlen(record.c_str())!=this->getRecordLength())
    {
        std::cout<<"The record that you are trying to search for in the BTree does not have the specified record length. Please choose the appropriate menu choice to search for a record with the appropriate record length.\n"<<std::endl;
    }

    else
    {
        if(this->getRoot()==-1)
        {
            std::cout<<"The BTree does not currently contain any records. Please choose the appropriate menu choice and insert some records into the BTree in order to search for them at a later time.\n"<<std::endl;
        }

        else if(this->getHDD()[sectorNumber][0]=='0')
        {
            std::cout<<"The sector that was requested for searching of records does not contain any records. Please choose the appropriate menu choice to request a different sector number to search for records.\n"<<std::endl;
        }

        else if(this->getHDD()[sectorNumber][0]=='1')
        {
            std::stringstream numberOfRecordsSS;
            numberOfRecordsSS<<this->getNumberOfRecords();

            std::stringstream sectorRecordSS;
            char sectorRecordC='\0';
            int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
            int recordBit2=recordBit+this->getRecordLength();
            while(this->getHDD()[sectorNumber][recordBit]!='\0')
            {
                sectorRecordC=this->getHDD()[sectorNumber][recordBit];
                sectorRecordSS<<sectorRecordC;
                recordBit++;

                if(recordBit==recordBit2)
                {
                    if(sectorRecordSS.str()==record)
                    {
                        std::cout<<"The record "<<record<<" has successfully been found in sector "<<sectorNumber<<".\n"<<std::endl;
                        recordBit=0;
                        break;
                    }

                    else
                    {
                        sectorRecordSS.str(std::string());
                        sectorRecordSS.clear();
                        recordBit2+=this->getRecordLength();
                    }
                }
            }

            if(this->getHDD()[sectorNumber][recordBit]=='\0')
            {
                std::cout<<"The record "<<record<<" could not be found in the BTree.\n"<<std::endl;
            }
        }

        else if(this->getHDD()[sectorNumber][0]=='2')
        {
            std::stringstream numberOfKeysSS;
            numberOfKeysSS<<this->getNumberOfKeys();

            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            int recordKey=0;
            std::stringstream recordKeySS(record);
            recordKeySS>>recordKey;

            int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            while(this->getHDD()[sectorNumber][branchBit]!='\0')
            {
                std::stringstream leftBranchSS;
                char leftBranchC='\0';
                int leftBranch=0;
                int leftBranchBit=branchBit;
                int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(leftBranchBit<leftBranchBit2)
                {
                    leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                    leftBranchSS<<leftBranchC;
                    leftBranchBit++;
                }
                leftBranchSS>>leftBranch;

                std::stringstream internalKeySS;
                char internalKeyC='\0';
                int internalKey=0;
                int internalKeyBit=leftBranchBit;
                int internalKeyBit2=internalKeyBit+this->getKeyLength();
                while(internalKeyBit<internalKeyBit2)
                {
                    internalKeyC=this->getHDD()[sectorNumber][internalKeyBit];
                    internalKeySS<<internalKeyC;
                    internalKeyBit++;
                }
                internalKeySS>>internalKey;

                std::stringstream rightBranchSS;
                char rightBranchC='\0';
                int rightBranch=0;
                int rightBranchBit=internalKeyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                    rightBranchSS<<rightBranchC;
                    rightBranchBit++;
                }
                rightBranchSS>>rightBranch;

                branchBit=rightBranchBit;
                if(recordKey<internalKey)
                {
                    this->searchRecord(record, leftBranch);
                    break;
                }

                else if(this->getHDD()[sectorNumber][branchBit]=='\0')
                {
                    this->searchRecord(record, rightBranch);
                }

                else
                {
                    branchBit-=strlen(sectorAddressLengthSS.str().c_str());
                }
            }
        }
    }
}

void BTree::removeRecord(std::string record, int sectorNumber)
{
    int checkKeyLength=0;
    std::stringstream checkKeyLengthSS(record);
    checkKeyLengthSS>>checkKeyLength;
    checkKeyLengthSS.str(std::string());
    checkKeyLengthSS.clear();
    checkKeyLengthSS<<checkKeyLength;

    if((int)strlen(checkKeyLengthSS.str().c_str())!=this->getKeyLength())
    {
        std::cout<<"The record that you are trying to remove from the BTree does not have the specified key length. Please choose the appropriate menu choice to remove a record with the appropriate key length.\n"<<std::endl;
    }

    else if((int)strlen(record.c_str())!=this->getRecordLength())
    {
        std::cout<<"The record that you are trying to remove from the BTree does not have the specified record length. Please choose the appropriate menu choice to remove a record with the appropriate record length.\n"<<std::endl;
    }

    else
    {
        if(this->getRoot()==-1)
        {
            std::cout<<"The BTree does not currently contain any records. Please choose the appropriate menu choice and insert some records into the BTree in order to remove them at a later time.\n"<<std::endl;
        }

        else if(this->getHDD()[sectorNumber][0]=='0')
        {
            std::cout<<"The sector that was requested for the removal of records does not contain any records. Please choose the appropriate menu choice to request a different sector number to remove records.\n"<<std::endl;
        }

        else if(this->getHDD()[sectorNumber][0]=='1')
        {
            std::stringstream numberOfRecordsSS;
            numberOfRecordsSS<<this->getNumberOfRecords();

            std::stringstream sectorRecordSS;
            char sectorRecordC='\0';
            bool isRecordFound=false;
            int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
            int recordBit2=recordBit+this->getRecordLength();
            while(this->getHDD()[sectorNumber][recordBit]!='\0')
            {
                sectorRecordC=this->getHDD()[sectorNumber][recordBit];
                sectorRecordSS<<sectorRecordC;
                recordBit++;

                if(recordBit==recordBit2)
                {
                    if(sectorRecordSS.str()==record)
                    {
                        isRecordFound=true;
                        break;
                    }

                    else
                    {
                        sectorRecordSS.str(std::string());
                        sectorRecordSS.clear();
                        recordBit2+=this->getRecordLength();
                    }
                }
            }

            if(isRecordFound==false)
            {
                std::cout<<"The record "<<record<<" could not be found for removal from the BTree.\n"<<std::endl;
            }

            else
            {
                recordBit-=this->getRecordLength();
                while(this->getHDD()[sectorNumber][recordBit2]!='\0')
                {
                    this->getHDD()[sectorNumber][recordBit]=this->getHDD()[sectorNumber][recordBit2];
                    recordBit++;
                    recordBit2++;
                }
                recordBit2-=this->getRecordLength();
                this->getHDD()[sectorNumber][recordBit2]='\0';

                std::stringstream checkNumberOfRecordsSS;
                char checkNumberOfRecordsC='\0';
                int checkNumberOfRecords=0;
                int numberOfRecordsBit=1;
                while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
                {
                    checkNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
                    checkNumberOfRecordsSS<<checkNumberOfRecordsC;
                    numberOfRecordsBit++;
                }
                checkNumberOfRecordsSS>>checkNumberOfRecords;

                int updateNumberOfRecords=checkNumberOfRecords-1;
                std::stringstream updateNumberOfRecordsSS;
                updateNumberOfRecordsSS<<std::setfill('0')<<std::setw(strlen(numberOfRecordsSS.str().c_str()))<<updateNumberOfRecords;
                char updateNumberOfRecordsC='\0';
                numberOfRecordsBit=1;
                while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
                {
                    updateNumberOfRecordsSS>>updateNumberOfRecordsC;
                    this->getHDD()[sectorNumber][numberOfRecordsBit]=updateNumberOfRecordsC;
                    numberOfRecordsBit++;
                }

                std::cout<<"The record "<<record<<" was successfully removed from the BTree.\n"<<std::endl;

                if(updateNumberOfRecords<(this->getNumberOfRecords()+1)/2)
                {
                    this->LMerge(sectorNumber);
                }
            }
        }

        else if(this->getHDD()[sectorNumber][0]=='2')
        {
            std::stringstream numberOfKeysSS;
            numberOfKeysSS<<this->getNumberOfKeys();

            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            int recordKey=0;
            std::stringstream recordKeySS(record);
            recordKeySS>>recordKey;

            int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            while(this->getHDD()[sectorNumber][branchBit]!='\0')
            {
                std::stringstream leftBranchSS;
                char leftBranchC='\0';
                int leftBranch=0;
                int leftBranchBit=branchBit;
                int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(leftBranchBit<leftBranchBit2)
                {
                    leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                    leftBranchSS<<leftBranchC;
                    leftBranchBit++;
                }
                leftBranchSS>>leftBranch;

                std::stringstream internalKeySS;
                char internalKeyC='\0';
                int internalKey=0;
                int internalKeyBit=leftBranchBit;
                int internalKeyBit2=internalKeyBit+this->getKeyLength();
                while(internalKeyBit<internalKeyBit2)
                {
                    internalKeyC=this->getHDD()[sectorNumber][internalKeyBit];
                    internalKeySS<<internalKeyC;
                    internalKeyBit++;
                }
                internalKeySS>>internalKey;

                std::stringstream rightBranchSS;
                char rightBranchC='\0';
                int rightBranch=0;
                int rightBranchBit=internalKeyBit;
                int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
                while(rightBranchBit<rightBranchBit2)
                {
                    rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                    rightBranchSS<<rightBranchC;
                    rightBranchBit++;
                }
                rightBranchSS>>rightBranch;

                branchBit=rightBranchBit;
                if(recordKey<internalKey)
                {
                    this->removeRecord(record, leftBranch);
                    break;
                }

                else if(this->getHDD()[sectorNumber][branchBit]=='\0')
                {
                    this->removeRecord(record, rightBranch);
                    break;
                }

                else
                {
                    branchBit-=strlen(sectorAddressLengthSS.str().c_str());
                }
            }
        }
    }
}

void BTree::LMerge(int sectorNumber)
{
    if(sectorNumber==this->getRoot())
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        std::stringstream checkNumberOfRecordsSS;
        char checkNumberOfRecordsC='\0';
        int checkNumberOfRecords=0;
        int numberOfRecordsBit=1;
        while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
        {
            checkNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
            checkNumberOfRecordsSS<<checkNumberOfRecordsC;
            numberOfRecordsBit++;
        }
        checkNumberOfRecordsSS>>checkNumberOfRecords;

        if(checkNumberOfRecords==0)
        {
            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            std::stringstream freeSectorSS;
            freeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<this->getFreeSector();
            char freeSectorC='\0';
            int freeSectorBit=1;
            int freeSectorBit2=freeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
            while(freeSectorBit<freeSectorBit2)
            {
                freeSectorSS>>freeSectorC;
                this->getHDD()[sectorNumber][freeSectorBit]=freeSectorC;
                freeSectorBit++;
            }
            this->getHDD()[sectorNumber][freeSectorBit]='\0';
            this->getHDD()[sectorNumber][0]='0';
            this->setFreeSector(sectorNumber);
            this->setRoot(-1);
        }
    }

    else
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        std::stringstream retrieveNumberOfRecordsSS;
        char retrieveNumberOfRecordsC='\0';
        int retrieveNumberOfRecords=0;
        int numberOfRecordsBit=1;
        while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
        {
            retrieveNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
            retrieveNumberOfRecordsSS<<retrieveNumberOfRecordsC;
            numberOfRecordsBit++;
        }
        retrieveNumberOfRecordsSS>>retrieveNumberOfRecords;

        std::stringstream leafKeySS;
        char leafKeyC='\0';
        int leafKey=0;
        int leafKeyBit=strlen(numberOfRecordsSS.str().c_str())+(retrieveNumberOfRecords-1)*this->getRecordLength()+1;
        int leafKeyBit2=leafKeyBit+this->getKeyLength();
        while(leafKeyBit<leafKeyBit2)
        {
            leafKeyC=this->getHDD()[sectorNumber][leafKeyBit];
            leafKeySS<<leafKeyC;
            leafKeyBit++;
        }
        leafKeySS>>leafKey;

        int parent=this->getRoot();
        int child=this->getRoot();
        int leftBranch=0;
        int rightBranch=0;
        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        int branchBit2=0;
        while(child!=sectorNumber)
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[parent][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream internalKeySS;
            char internalKeyC='\0';
            int internalKey=0;
            int internalKeyBit=leftBranchBit;
            int internalKeyBit2=internalKeyBit+this->getKeyLength();
            while(internalKeyBit<internalKeyBit2)
            {
                internalKeyC=this->getHDD()[parent][internalKeyBit];
                internalKeySS<<internalKeyC;
                internalKeyBit++;
            }
            internalKeySS>>internalKey;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranchBit=internalKeyBit;
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[parent][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(leafKey<internalKey)
            {
                parent=child;
                child=leftBranch;
                branchBit2=branchBit;
                branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            }

            else if(this->getHDD()[parent][branchBit]=='\0')
            {
                parent=child;
                child=rightBranch;
                branchBit2=branchBit;
                branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            }

            else
            {
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }

        std::stringstream siblingRecordsSS;
        char siblingRecordsC='\0';
        int siblingRecordsBit=strlen(numberOfRecordsSS.str().c_str())+1;
        while(this->getHDD()[rightBranch][siblingRecordsBit]!='\0')
        {
            siblingRecordsC=this->getHDD()[rightBranch][siblingRecordsBit];
            siblingRecordsSS<<siblingRecordsC;
            siblingRecordsBit++;
        }

        std::stringstream freeSectorSS;
        freeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<this->getFreeSector();
        char freeSectorC='\0';
        int freeSectorBit=1;
        int freeSectorBit2=freeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
        while(freeSectorBit<freeSectorBit2)
        {
            freeSectorSS>>freeSectorC;
            this->getHDD()[rightBranch][freeSectorBit]=freeSectorC;
            freeSectorBit++;
        }
        this->getHDD()[rightBranch][freeSectorBit]='\0';
        this->getHDD()[rightBranch][0]='0';
        this->setFreeSector(rightBranch);

        int keyBranchBit=branchBit2-(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
        int keyBranchBit2=keyBranchBit+this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str());
        while(this->getHDD()[parent][keyBranchBit2]!='\0')
        {
            this->getHDD()[parent][keyBranchBit]=this->getHDD()[parent][keyBranchBit2];
            keyBranchBit++;
            keyBranchBit2++;
        }
        keyBranchBit2-=(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
        this->getHDD()[parent][keyBranchBit2]='\0';

        std::stringstream checkNumberOfKeysSS;
        char checkNumberOfKeysC='\0';
        int checkNumberOfKeys=0;
        int numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
            checkNumberOfKeysSS<<checkNumberOfKeysC;
            numberOfKeysBit++;
        }
        checkNumberOfKeysSS>>checkNumberOfKeys;

        int updateNumberOfKeys=checkNumberOfKeys-1;
        std::stringstream updateNumberOfKeysSS;
        updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
        char updateNumberOfKeysC='\0';
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            updateNumberOfKeysSS>>updateNumberOfKeysC;
            this->getHDD()[parent][numberOfKeysBit]=updateNumberOfKeysC;
            numberOfKeysBit++;
        }

        bool isMergeSector=false;
        if(updateNumberOfKeys<this->getNumberOfKeys()/2)
        {
            if(parent==this->getRoot())
            {
                this->IMerge(parent, leftBranch);
            }

            else
            {
                isMergeSector=true;
            }
        }

        bool isExtraData=false;
        std::stringstream mergeRecordSS;
        int recordBit=0;
        int recordBit2=this->getRecordLength();
        while(siblingRecordsSS>>siblingRecordsC)
        {
            mergeRecordSS<<siblingRecordsC;
            recordBit++;

            if(recordBit==recordBit2)
            {
                if(isExtraData==false)
                {
                    std::stringstream checkNumberOfRecordsSS;
                    char checkNumberOfRecordsC='\0';
                    int checkNumberOfRecords=0;
                    numberOfRecordsBit=1;
                    while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
                    {
                        checkNumberOfRecordsC=this->getHDD()[leftBranch][numberOfRecordsBit];
                        checkNumberOfRecordsSS<<checkNumberOfRecordsC;
                        numberOfRecordsBit++;
                    }
                    checkNumberOfRecordsSS>>checkNumberOfRecords;

                    if(checkNumberOfRecords+1>this->getNumberOfRecords())
                    {
                        rightBranch=this->getFreeSector();
                        isExtraData=true;
                    }

                    this->insertRecord(mergeRecordSS.str(), leftBranch);
                    mergeRecordSS.str(std::string());
                    mergeRecordSS.clear();
                }

                else
                {
                    this->insertRecord(mergeRecordSS.str(), rightBranch);
                    mergeRecordSS.str(std::string());
                    mergeRecordSS.clear();
                }

                recordBit2+=this->getRecordLength();
            }
        }

        if(isMergeSector==true)
        {
            checkNumberOfKeysSS.str(std::string());
            checkNumberOfKeysSS.clear();
            checkNumberOfKeys=0;
            numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
                checkNumberOfKeysSS<<checkNumberOfKeysC;
                numberOfKeysBit++;
            }
            checkNumberOfKeysSS>>checkNumberOfKeys;

            if(checkNumberOfKeys<this->getNumberOfKeys()/2)
            {
                this->IMerge(parent, leftBranch);
            }
        }
    }
}

void BTree::IMerge(int sectorNumber, int mergeSector)
{
    if(sectorNumber==this->getRoot())
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream checkNumberOfKeysSS;
        char checkNumberOfKeysC='\0';
        int checkNumberOfKeys=0;
        int numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            checkNumberOfKeysC=this->getHDD()[sectorNumber][numberOfKeysBit];
            checkNumberOfKeysSS<<checkNumberOfKeysC;
            numberOfKeysBit++;
        }
        checkNumberOfKeysSS>>checkNumberOfKeys;

        if(checkNumberOfKeys==0)
        {
            std::stringstream sectorAddressLengthSS;
            sectorAddressLengthSS<<this->getNumberOfSectors();

            std::stringstream freeSectorSS;
            freeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<this->getFreeSector();
            char freeSectorC='\0';
            int freeSectorBit=1;
            int freeSectorBit2=freeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
            while(freeSectorBit<freeSectorBit2)
            {
                freeSectorSS>>freeSectorC;
                this->getHDD()[sectorNumber][freeSectorBit]=freeSectorC;
                freeSectorBit++;
            }
            this->getHDD()[sectorNumber][freeSectorBit]='\0';
            this->getHDD()[sectorNumber][0]='0';
            this->setFreeSector(sectorNumber);
            this->setRoot(mergeSector);
        }
    }

    else
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        std::stringstream retrieveNumberOfKeysSS;
        char retrieveNumberOfKeysC='\0';
        int retrieveNumberOfKeys=0;
        int numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            retrieveNumberOfKeysC=this->getHDD()[sectorNumber][numberOfKeysBit];
            retrieveNumberOfKeysSS<<retrieveNumberOfKeysC;
            numberOfKeysBit++;
        }
        retrieveNumberOfKeysSS>>retrieveNumberOfKeys;

        std::stringstream internalChildKeySS;
        char internalChildKeyC='\0';
        int internalChildKey=0;
        int keyBit=strlen(numberOfKeysSS.str().c_str())+(retrieveNumberOfKeys)*strlen(sectorAddressLengthSS.str().c_str())+(retrieveNumberOfKeys-1)*this->getKeyLength()+1;
        int keyBit2=keyBit+this->getKeyLength();
        while(keyBit<keyBit2)
        {
            internalChildKeyC=this->getHDD()[sectorNumber][keyBit];
            internalChildKeySS<<internalChildKeyC;
            keyBit++;
        }
        internalChildKeySS>>internalChildKey;

        int parent=this->getRoot();
        int child=this->getRoot();
        int leftBranch=0;
        int rightBranch=0;
        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        int branchBit2=0;
        while(child!=sectorNumber)
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[parent][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream internalParentKeySS;
            char internalParentKeyC='\0';
            int internalParentKey=0;
            keyBit=leftBranchBit;
            keyBit2=keyBit+this->getKeyLength();
            while(keyBit<keyBit2)
            {
                internalParentKeyC=this->getHDD()[parent][keyBit];
                internalParentKeySS<<internalParentKeyC;
                keyBit++;
            }
            internalParentKeySS>>internalParentKey;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranchBit=keyBit;
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[parent][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(internalChildKey<internalParentKey)
            {
                parent=child;
                child=leftBranch;
                branchBit2=branchBit;
                branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            }

            else if(this->getHDD()[parent][branchBit]=='\0')
            {
                parent=child;
                child=rightBranch;
                branchBit2=branchBit;
                branchBit=strlen(numberOfKeysSS.str().c_str())+1;
            }

            else
            {
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }

        std::stringstream mergeKeySS;
        char mergeKeyC='\0';
        keyBit=branchBit2-(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
        keyBit2=keyBit+this->getKeyLength();
        while(keyBit<keyBit2)
        {
            mergeKeyC=this->getHDD()[parent][keyBit];
            mergeKeySS<<mergeKeyC;
            keyBit++;
        }

        std::stringstream siblingKeyBranchesSS;
        char siblingKeyBranchesC='\0';
        while(mergeKeySS>>siblingKeyBranchesC)
        {
            siblingKeyBranchesSS<<siblingKeyBranchesC;
        }
        int siblingKeyBranchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[rightBranch][siblingKeyBranchBit]!='\0')
        {
            siblingKeyBranchesC=this->getHDD()[rightBranch][siblingKeyBranchBit];
            siblingKeyBranchesSS<<siblingKeyBranchesC;
            siblingKeyBranchBit++;
        }

        std::stringstream freeSectorSS;
        freeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<this->getFreeSector();
        char freeSectorC='\0';
        int freeSectorBit=1;
        int freeSectorBit2=freeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
        while(freeSectorBit<freeSectorBit2)
        {
            freeSectorSS>>freeSectorC;
            this->getHDD()[rightBranch][freeSectorBit]=freeSectorC;
            freeSectorBit++;
        }
        this->getHDD()[rightBranch][freeSectorBit]='\0';
        this->getHDD()[rightBranch][0]='0';
        this->setFreeSector(rightBranch);

        int keyBranchBit=branchBit2-(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
        int keyBranchBit2=keyBranchBit+this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str());
        while(this->getHDD()[parent][keyBranchBit2]!='\0')
        {
            this->getHDD()[parent][keyBranchBit]=this->getHDD()[parent][keyBranchBit2];
            keyBranchBit++;
            keyBranchBit2++;
        }
        keyBranchBit2-=(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
        this->getHDD()[parent][keyBranchBit2]='\0';

        std::stringstream checkNumberOfKeysSS;
        char checkNumberOfKeysC='\0';
        int checkNumberOfKeys=0;
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
            checkNumberOfKeysSS<<checkNumberOfKeysC;
            numberOfKeysBit++;
        }
        checkNumberOfKeysSS>>checkNumberOfKeys;

        int updateNumberOfKeys=checkNumberOfKeys-1;
        std::stringstream updateNumberOfKeysSS;
        updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
        char updateNumberOfKeysC='\0';
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            updateNumberOfKeysSS>>updateNumberOfKeysC;
            this->getHDD()[parent][numberOfKeysBit]=updateNumberOfKeysC;
            numberOfKeysBit++;
        }

        bool isMergeSector=false;
        if(updateNumberOfKeys<this->getNumberOfKeys()/2)
        {
            if(parent==this->getRoot())
            {
                this->IMerge(parent, leftBranch);
            }

            else
            {
                isMergeSector=true;
            }
        }

        retrieveNumberOfKeysSS.str(std::string());
        retrieveNumberOfKeysSS.clear();
        retrieveNumberOfKeys=0;
        numberOfKeysBit=1;
        while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
        {
            retrieveNumberOfKeysC=this->getHDD()[leftBranch][numberOfKeysBit];
            retrieveNumberOfKeysSS<<retrieveNumberOfKeysC;
            numberOfKeysBit++;
        }
        retrieveNumberOfKeysSS>>retrieveNumberOfKeys;
        updateNumberOfKeys=retrieveNumberOfKeys;

        bool isExtraData=false;
        keyBranchBit=0;
        keyBranchBit2=this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str());
        int keyBranchBit3=strlen(numberOfKeysSS.str().c_str())+(retrieveNumberOfKeys+1)*strlen(sectorAddressLengthSS.str().c_str())+(retrieveNumberOfKeys)*this->getKeyLength()+1;
        while(siblingKeyBranchesSS>>siblingKeyBranchesC)
        {
            if(isExtraData==false)
            {
                this->getHDD()[leftBranch][keyBranchBit3]=siblingKeyBranchesC;
                keyBranchBit++;
                keyBranchBit3++;
            }

            else
            {
                this->getHDD()[rightBranch][keyBranchBit3]=siblingKeyBranchesC;
                keyBranchBit++;
                keyBranchBit3++;
            }

            if(keyBranchBit==keyBranchBit2)
            {
                if(isExtraData==false)
                {
                    updateNumberOfKeys++;
                    updateNumberOfKeysSS.str(std::string());
                    updateNumberOfKeysSS.clear();
                    updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
                    numberOfKeysBit=1;
                    while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
                    {
                        updateNumberOfKeysSS>>updateNumberOfKeysC;
                        this->getHDD()[leftBranch][numberOfKeysBit]=updateNumberOfKeysC;
                        numberOfKeysBit++;
                    }
                    this->getHDD()[leftBranch][keyBranchBit3]='\0';

                    if(updateNumberOfKeys>this->getNumberOfKeys())
                    {
                        rightBranch=this->getFreeSector();
                        this->ISplit(leftBranch);
                        keyBranchBit3=strlen(numberOfKeysSS.str().c_str())+((updateNumberOfKeys+1)/2)*strlen(sectorAddressLengthSS.str().c_str())+((updateNumberOfKeys-1)/2)*this->getKeyLength()+1;
                        updateNumberOfKeys=(updateNumberOfKeys-1)/2;
                        isExtraData=true;
                    }
                }

                else
                {
                    updateNumberOfKeys++;
                    updateNumberOfKeysSS.str(std::string());
                    updateNumberOfKeysSS.clear();
                    updateNumberOfKeysSS<<std::setfill('0')<<std::setw(strlen(numberOfKeysSS.str().c_str()))<<updateNumberOfKeys;
                    numberOfKeysBit=1;
                    while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
                    {
                        updateNumberOfKeysSS>>updateNumberOfKeysC;
                        this->getHDD()[rightBranch][numberOfKeysBit]=updateNumberOfKeysC;
                        numberOfKeysBit++;
                    }
                    this->getHDD()[rightBranch][keyBranchBit3]='\0';
                }

                keyBranchBit2+=(this->getKeyLength()+strlen(sectorAddressLengthSS.str().c_str()));
            }
        }

        if(isMergeSector==true)
        {
            checkNumberOfKeysSS.str(std::string());
            checkNumberOfKeysSS.clear();
            checkNumberOfKeys=0;
            numberOfKeysBit=1;
            while(numberOfKeysBit<=(int)strlen(numberOfKeysSS.str().c_str()))
            {
                checkNumberOfKeysC=this->getHDD()[parent][numberOfKeysBit];
                checkNumberOfKeysSS<<checkNumberOfKeysC;
                numberOfKeysBit++;
            }
            checkNumberOfKeysSS>>checkNumberOfKeys;

            if(checkNumberOfKeys<this->getNumberOfKeys()/2)
            {
                this->IMerge(parent, leftBranch);
            }
        }
    }
}

bool BTree::isDuplicateKey(std::string record, int sectorNumber)
{
    if(this->getRoot()==-1)
    {
        return false;
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        return false;
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        int recordKey=0;
        std::stringstream recordKeySS(record);
        recordKeySS>>recordKey;

        std::stringstream sectorRecordKeySS;
        char sectorRecordKeyC='\0';
        int sectorRecordKey=0;

        int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
        int recordBit2=recordBit+this->getRecordLength();
        while(this->getHDD()[sectorNumber][recordBit]!='\0')
        {
            sectorRecordKeyC=this->getHDD()[sectorNumber][recordBit];
            sectorRecordKeySS<<sectorRecordKeyC;
            recordBit++;

            if(recordBit==recordBit2)
            {
                sectorRecordKeySS>>sectorRecordKey;
                if(recordKey==sectorRecordKey)
                {
                    return true;
                }

                else
                {
                    sectorRecordKey=0;
                    sectorRecordKeySS.str(std::string());
                    sectorRecordKeySS.clear();
                    recordBit2+=this->getRecordLength();
                }
            }
        }

        return false;
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int recordKey=0;
        std::stringstream recordKeySS(record);
        recordKeySS>>recordKey;

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[sectorNumber][branchBit]!='\0')
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranch=0;
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream internalKeySS;
            char internalKeyC='\0';
            int internalKey=0;
            int internalKeyBit=leftBranchBit;
            int internalKeyBit2=internalKeyBit+this->getKeyLength();
            while(internalKeyBit<internalKeyBit2)
            {
                internalKeyC=this->getHDD()[sectorNumber][internalKeyBit];
                internalKeySS<<internalKeyC;
                internalKeyBit++;
            }
            internalKeySS>>internalKey;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranch=0;
            int rightBranchBit=internalKeyBit;
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(recordKey<internalKey)
            {
                return this->isDuplicateKey(record, leftBranch);
            }

            else if(this->getHDD()[sectorNumber][branchBit]=='\0')
            {
                return this->isDuplicateKey(record, rightBranch);
            }

            else
            {
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }
    }

    return false;//This is here because control may reach the end of this function.
}

void BTree::printAllRecords(int sectorNumber)
{
    if(this->getRoot()==-1)
    {
        std::cout<<"There are no records in the BTree to print. Please choose the appropriate menu choice in order to insert some records into the BTree and print them later.\n"<<std::endl;
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        std::cout<<"There are no records in this sector to print as this sector is a free sector. Please choose the appropriate menu choice to choose a sector that is not a free sector in order to print its contents.\n"<<std::endl;
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        int recordBit=strlen(numberOfRecordsSS.str().c_str())+1;
        int recordBit2=recordBit+this->getRecordLength();
        while(this->getHDD()[sectorNumber][recordBit]!='\0')
        {
            std::cout<<this->getHDD()[sectorNumber][recordBit];
            recordBit++;

            if(recordBit==recordBit2)
            {
                std::cout<<" ";
                recordBit2+=this->getRecordLength();
            }
        }
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[sectorNumber][branchBit]!='\0')
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranch=0;
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranch=0;
            int rightBranchBit=leftBranchBit+this->getKeyLength();
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(this->getHDD()[sectorNumber][branchBit]=='\0')
            {
                this->printAllRecords(leftBranch);
                this->printAllRecords(rightBranch);
            }

            else
            {
                this->printAllRecords(leftBranch);
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }
    }
}

void BTree::retrieveBTreeSectors(int sectorNumber, int height, std::vector<std::vector<int> >& heightBuffer, std::vector<std::vector<int> >& sectorBuffer)
{
    if(this->getRoot()==-1)
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        std::vector<int>subHeightBuffer;
        std::vector<int>subSectorBuffer;
        subHeightBuffer.push_back(height);
        subSectorBuffer.push_back(sectorNumber);
        heightBuffer.push_back(subHeightBuffer);
        sectorBuffer.push_back(subSectorBuffer);
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::vector<int>subHeightBuffer;
        std::vector<int>subSectorBuffer;
        subHeightBuffer.push_back(height);
        subSectorBuffer.push_back(sectorNumber);
        heightBuffer.push_back(subHeightBuffer);
        sectorBuffer.push_back(subSectorBuffer);

        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[sectorNumber][branchBit]!='\0')
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranch=0;
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranch=0;
            int rightBranchBit=leftBranchBit+this->getKeyLength();
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(this->getHDD()[sectorNumber][branchBit]=='\0')
            {
                this->retrieveBTreeSectors(leftBranch, height+1, heightBuffer, sectorBuffer);
                this->retrieveBTreeSectors(rightBranch, height+1, heightBuffer, sectorBuffer);
            }

            else
            {
                this->retrieveBTreeSectors(leftBranch, height+1, heightBuffer, sectorBuffer);
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }
    }
}

void BTree::printBTree(int sectorNumber)
{
    std::vector<std::vector<int> >heightBuffer;
    std::vector<std::vector<int> >sectorBuffer;
    this->retrieveBTreeSectors(sectorNumber, 0, heightBuffer, sectorBuffer);

    if(heightBuffer.empty())
    {
        std::cout<<"The BTree residing in the HDD is currently empty and thus cannot be printed. Please choose the appropriate menu choice to insert some records into the BTree in order to print the BTree later.\n"<<std::endl;
    }

    else
    {
        for(int heightIndex=0; heightIndex<(int)heightBuffer.size(); heightIndex++)
        {
            if(heightBuffer.size()>1)
            {
                for(int heightIndex2=0; heightIndex2<(int)heightBuffer.size(); heightIndex2++)
                {
                    if(heightIndex!=heightIndex2)
                    {
                        if(heightBuffer[heightIndex][0]==heightBuffer[heightIndex2][0])
                        {
                            for(int heightIndex3=0; heightIndex3<(int)heightBuffer[heightIndex2].size(); heightIndex3++)
                            {
                                heightBuffer[heightIndex].push_back(heightBuffer[heightIndex2][heightIndex3]);
                                sectorBuffer[heightIndex].push_back(sectorBuffer[heightIndex2][heightIndex3]);
                            }

                            heightBuffer.erase(heightBuffer.begin()+heightIndex2);
                            sectorBuffer.erase(sectorBuffer.begin()+heightIndex2);
                            heightIndex=0;
                        }
                    }
                }
            }
        }

        for(int heightIndex=0; heightIndex<(int)heightBuffer.size(); heightIndex++)
        {
            for(int heightIndex2=0; heightIndex2<(int)heightBuffer[heightIndex].size(); heightIndex2++)
            {
                int sectorBit=0;
                while(this->getHDD()[sectorBuffer[heightIndex][heightIndex2]][sectorBit]!='\0')
                {
                    std::cout<<this->getHDD()[sectorBuffer[heightIndex][heightIndex2]][sectorBit];
                    sectorBit++;
                }

                std::cout<<" ";
            }

            std::cout<<std::endl;
        }
    }
}

void BTree::printHDD()
{
    for(int sectorCount=0; sectorCount<this->getNumberOfSectors(); sectorCount++)
    {
        int sectorBit=0;
        while(this->getHDD()[sectorCount][sectorBit]!='\0')
        {
            std::cout<<this->getHDD()[sectorCount][sectorBit];
            sectorBit++;
        }
    }
    std::cout<<std::endl;
}

int BTree::getHeight(int sectorNumber, int height)
{
    if(this->getRoot()==-1)
    {
        return -1;
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        return -1;
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        return height;
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;

        std::stringstream leftBranchSS;
        char leftBranchC='\0';
        int leftBranch=0;
        int leftBranchBit=branchBit;
        int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
        while(leftBranchBit<leftBranchBit2)
        {
            leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
            leftBranchSS<<leftBranchC;
            leftBranchBit++;
        }
        leftBranchSS>>leftBranch;

        return this->getHeight(leftBranch, height+1);
    }

    return -1;//This is here because control may reach the end of this function.
}

void BTree::retrieveRecordCount(int sectorNumber, std::vector<int>& recordCountBuffer)
{
    if(this->getRoot()==-1)
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        std::stringstream numberOfRecordsSS;
        numberOfRecordsSS<<this->getNumberOfRecords();

        std::stringstream retrieveNumberOfRecordsSS;
        char retrieveNumberOfRecordsC='\0';
        int retrieveNumberOfRecords=0;
        int numberOfRecordsBit=1;
        while(numberOfRecordsBit<=(int)strlen(numberOfRecordsSS.str().c_str()))
        {
            retrieveNumberOfRecordsC=this->getHDD()[sectorNumber][numberOfRecordsBit];
            retrieveNumberOfRecordsSS<<retrieveNumberOfRecordsC;
            numberOfRecordsBit++;
        }
        retrieveNumberOfRecordsSS>>retrieveNumberOfRecords;

        recordCountBuffer.push_back(retrieveNumberOfRecords);
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[sectorNumber][branchBit]!='\0')
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranch=0;
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranch=0;
            int rightBranchBit=leftBranchBit+this->getKeyLength();
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(this->getHDD()[sectorNumber][branchBit]=='\0')
            {
                this->retrieveRecordCount(leftBranch, recordCountBuffer);
                this->retrieveRecordCount(rightBranch, recordCountBuffer);
            }

            else
            {
                this->retrieveRecordCount(leftBranch, recordCountBuffer);
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }
    }
}

int BTree::getRecordCount(int sectorNumber)
{
    std::vector<int> recordCountBuffer;
    this->retrieveRecordCount(sectorNumber, recordCountBuffer);

    int recordCount=0;
    for(int recordCountIndex=0; recordCountIndex<(int)recordCountBuffer.size(); recordCountIndex++)
    {
        recordCount+=recordCountBuffer[recordCountIndex];
    }

    return recordCount;
}

void BTree::retrieveAllRecords(int sectorNumber, std::stringstream& recordBuffer)
{
    if(this->getRoot()==-1)
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='0')
    {
        return void();//This is equivalent to return;.
    }

    else if(this->getHDD()[sectorNumber][0]=='1')
    {
        std::stringstream numberOfRecordSS;
        numberOfRecordSS<<this->getNumberOfRecords();

        int recordBit=strlen(numberOfRecordSS.str().c_str())+1;
        int recordBit2=recordBit+this->getRecordLength();
        while(this->getHDD()[sectorNumber][recordBit]!='\0')
        {
            recordBuffer<<this->getHDD()[sectorNumber][recordBit];
            recordBit++;

            if(recordBit==recordBit2)
            {
                recordBuffer<<" ";
                recordBit2+=this->getRecordLength();
            }
        }
    }

    else if(this->getHDD()[sectorNumber][0]=='2')
    {
        std::stringstream numberOfKeysSS;
        numberOfKeysSS<<this->getNumberOfKeys();

        std::stringstream sectorAddressLengthSS;
        sectorAddressLengthSS<<this->getNumberOfSectors();

        int branchBit=strlen(numberOfKeysSS.str().c_str())+1;
        while(this->getHDD()[sectorNumber][branchBit]!='\0')
        {
            std::stringstream leftBranchSS;
            char leftBranchC='\0';
            int leftBranch=0;
            int leftBranchBit=branchBit;
            int leftBranchBit2=leftBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(leftBranchBit<leftBranchBit2)
            {
                leftBranchC=this->getHDD()[sectorNumber][leftBranchBit];
                leftBranchSS<<leftBranchC;
                leftBranchBit++;
            }
            leftBranchSS>>leftBranch;

            std::stringstream rightBranchSS;
            char rightBranchC='\0';
            int rightBranch=0;
            int rightBranchBit=leftBranchBit+this->getKeyLength();
            int rightBranchBit2=rightBranchBit+strlen(sectorAddressLengthSS.str().c_str());
            while(rightBranchBit<rightBranchBit2)
            {
                rightBranchC=this->getHDD()[sectorNumber][rightBranchBit];
                rightBranchSS<<rightBranchC;
                rightBranchBit++;
            }
            rightBranchSS>>rightBranch;

            branchBit=rightBranchBit;
            if(this->getHDD()[sectorNumber][branchBit]=='\0')
            {
                this->retrieveAllRecords(leftBranch, recordBuffer);
                this->retrieveAllRecords(rightBranch, recordBuffer);
            }

            else
            {
                this->retrieveAllRecords(leftBranch, recordBuffer);
                branchBit-=strlen(sectorAddressLengthSS.str().c_str());
            }
        }
    }
}

void BTree::inputFromFile(std::string fileInput)
{
    std::fstream inputFileStream(fileInput.c_str());
    if(inputFileStream.is_open())
    {
        if(this->getRoot()!=-1)
        {
            std::cout<<"The file "<<fileInput<<" was able to be opened to upload records from it. However, you are about to overwrite the current records in the BTree with the potential records from the file.\n"<<std::endl;
            std::cout<<"To give you the option to proceed with overwriting the current records in the BTree or not, a menu will now be displayed that will allow you to pick your choice of whether you will want to overwrite the current records in the BTree or not.\n"<<std::endl;

            std::cout<<"1. Overwrite the current records in the BTree."<<std::endl;
            std::cout<<"2. Do not overwrite the current records in the BTree.\n"<<std::endl;

            std::cout<<"Please enter your choice based on the number associated with its description.\n"<<std::endl;

            std::string overwriteChoice=std::string();
            int overwriteChoice2=0;
            do
            {
                std::getline(std::cin, overwriteChoice);
                std::cout<<std::endl;
                std::stringstream overwriteChoiceSS(overwriteChoice);
                overwriteChoiceSS>>overwriteChoice2;
                switch(overwriteChoice2)
                {
                    case 1:
                    {
                        std::cout<<"Now the records in the BTree will be overwritten with the records from "<<fileInput<<".\n"<<std::endl;

                        std::stringstream sectorAddressLengthSS;
                        sectorAddressLengthSS<<this->getNumberOfSectors();

                        this->setFreeSector(0);
                        this->setRoot(-1);
                        int nextFreeSector=1;
                        for(int sectorNumber=0; sectorNumber<this->getNumberOfSectors(); sectorNumber++)
                        {
                            if(sectorNumber==this->getNumberOfSectors()-1)
                            {
                                nextFreeSector=0;
                            }

                            this->getHDD()[sectorNumber][0]='0';

                            std::stringstream nextFreeSectorSS;
                            nextFreeSectorSS<<std::setfill('0')<<std::setw(strlen(sectorAddressLengthSS.str().c_str()))<<nextFreeSector;
                            char nextFreeSectorC='\0';
                            int nextFreeSectorBit=1;
                            int nextFreeSectorBit2=nextFreeSectorBit+strlen(sectorAddressLengthSS.str().c_str());
                            while(nextFreeSectorBit<nextFreeSectorBit2)
                            {
                                nextFreeSectorSS>>nextFreeSectorC;
                                this->getHDD()[sectorNumber][nextFreeSectorBit]=nextFreeSectorC;
                                nextFreeSectorBit++;
                            }
                            this->getHDD()[sectorNumber][nextFreeSectorBit]='\0';

                            nextFreeSector++;
                        }

                        std::string recordsInsert=std::string();
                        while(inputFileStream>>recordsInsert)
                        {
                            std::stringstream recordsInsert2(recordsInsert);
                            if(this->isDuplicateKey(recordsInsert2.str(), this->getRoot())==false)
                            {
                                this->insertRecord(recordsInsert2.str(), this->getRoot());
                            }
                        }

                        std::cout<<"The records from the file "<<fileInput<<" have been successfully been loaded into the BTree.\n"<<std::endl;

                        break;
                    }

                    case 2:
                    {
                        std::cout<<"The current BTree records will not be overwritten. If you wish to overwrite the current BTree records at a later time, you may do so by choosing to load records from a file again by choosing the appropriate menu choice.\n"<<std::endl;

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                if(overwriteChoice2!=1&&overwriteChoice2!=2)
                {
                    std::cout<<"Now a menu will be re-created showing the possible choices of choosing whether to overwrite the current records in the BTree or not for reminding purposes.\n"<<std::endl;
                    std::cout<<"1. Overwrite the current records in the BTree."<<std::endl;
                    std::cout<<"2. Do not overwrite the current records in the BTree.\n"<<std::endl;
                    std::cout<<"Please enter your next choice by entering the number associated with its description.\n"<<std::endl;
                }

                else
                {
                    break;
                }
            }

            while(overwriteChoice2!=1&&overwriteChoice2!=2);
        }

        else
        {
            std::cout<<"Now the records from "<<fileInput<<" will be loaded into the BTree.\n"<<std::endl;

            std::string recordsInsert=std::string();
            while(inputFileStream>>recordsInsert)
            {
                std::stringstream recordsInsert2(recordsInsert);
                if(this->isDuplicateKey(recordsInsert2.str(), this->getRoot())==false)
                {
                    this->insertRecord(recordsInsert2.str(), this->getRoot());
                }
            }

            std::cout<<"The records from the file "<<fileInput<<" have been successfully been loaded into the BTree.\n"<<std::endl;
        }
    }

    else
    {
        std::cout<<"The file "<<fileInput<<" could not be opened to load records from it. If you wish to attempt to load records from a file again you may do so by choosing the appropriate menu choice.\n"<<std::endl;
    }

    inputFileStream.close();
}

void BTree::outputToFile(std::string fileOutput)
{
    std::fstream outputFileStream(fileOutput.c_str(), std::ios::in);
    if(outputFileStream.is_open())
    {
        if(this->getRoot()!=-1)
        {
            std::cout<<"The file "<<fileOutput<<" was able to be opened to store records to it. However, you are about to overwrite any data that it contains with the current records from the BTree.\n"<<std::endl;
            std::cout<<"To give you the option to proceed with overwriting the file with the current records from the BTree or not, a menu will now be displayed that will allow you to pick your choice of whether you will want to overwrite the file with the current records from the BTree or not.\n"<<std::endl;

            std::cout<<"1. Overwrite the file with the current records from the BTree."<<std::endl;
            std::cout<<"2. Do not overwrite the file with the current records from the BTree.\n"<<std::endl;

            std::cout<<"Please enter your choice based on the number associated with its description.\n"<<std::endl;

            std::string overwriteFileChoice=std::string();
            int overwriteFileChoice2=0;
            do
            {
                std::getline(std::cin, overwriteFileChoice);
                std::cout<<std::endl;
                std::stringstream overwriteFileChoiceSS(overwriteFileChoice);
                overwriteFileChoiceSS>>overwriteFileChoice2;
                switch(overwriteFileChoice2)
                {
                    case 1:
                    {
                        std::cout<<"Now the file "<<fileOutput<<" will be overwritten with the current records from the BTree.\n"<<std::endl;

                        outputFileStream.close();
                        outputFileStream.open(fileOutput.c_str(), std::ios::out|std::ios::trunc);
                        if(outputFileStream.is_open())
                        {
                            std::stringstream recordBuffer;
                            this->retrieveAllRecords(this->getRoot(), recordBuffer);
                            std::string recordOutput;
                            while(recordBuffer>>recordOutput)
                            {
                                 outputFileStream<<recordOutput<<" ";
                            }
                            outputFileStream<<std::endl;
                            std::cout<<"The current records from the BTree were successfully stored to "<<fileOutput<<".\n"<<std::endl;
                        }

                        else
                        {
                            std::cout<<"The file could not be overwritten to store the current records from the BTree to it. If you wish to try to store the current records from the BTree to a file again you may do so by choosing the appropriate menu choice.\n"<<std::endl;
                        }

                        break;
                    }

                    case 2:
                    {
                        std::cout<<"No record data will be stored to "<<fileOutput<<" if you wish to store records to a file at a later time, you may do so by choosing the appropriate menu choice.\n"<<std::endl;

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                if(overwriteFileChoice2!=1&&overwriteFileChoice2!=2)
                {
                    std::cout<<"Now a menu will be re-created showing the possible choices of choosing whether to overwrite the file with the current records from the BTree or not for reminding purposes.\n"<<std::endl;
                    std::cout<<"1. Overwrite the file with the current records from the BTree."<<std::endl;
                    std::cout<<"2. Do not overwrite the file with the current records from the BTree.\n"<<std::endl;
                    std::cout<<"Please enter your next choice by entering the number associated with its description.\n"<<std::endl;
                }

                else
                {
                    break;
                }
            }

            while(overwriteFileChoice2!=1&&overwriteFileChoice2!=2);
        }

        else
        {
            std::cout<<"The BTree is currently empty. Thus no record data could be stored to "<<fileOutput<<".\n"<<std::endl;
        }
    }

    else
    {
        if(this->getRoot()!=-1)
        {
            std::cout<<"Now the file "<<fileOutput<<" will be overwritten with the current records from the BTree.\n"<<std::endl;

            outputFileStream.close();
            outputFileStream.open(fileOutput.c_str(), std::ios::out|std::ios::trunc);
            if(outputFileStream.is_open())
            {
                std::stringstream recordBuffer;
                this->retrieveAllRecords(this->getRoot(), recordBuffer);
                std::string recordOutput;
                while(recordBuffer>>recordOutput)
                {
                     outputFileStream<<recordOutput<<" ";
                }
                outputFileStream<<std::endl;
                std::cout<<"The current records from the BTree were successfully stored to "<<fileOutput<<".\n"<<std::endl;
            }

            else
            {
                std::cout<<"The file could not be overwritten to store the current records from the BTree to it. If you wish to try to store the records to a file again you may do so by choosing the appropriate menu choice.\n"<<std::endl;
            }
        }

        else
        {
            std::cout<<"The BTree is currently empty. Thus no record data could be stored to "<<fileOutput<<".\n"<<std::endl;
        }
    }

    outputFileStream.close();
}
