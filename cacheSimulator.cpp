#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

int cahceSize;   // S: the total cache size in bytes
int lineSize;    // L: the line size in the cache
int cacheCycles; // clock cycles to access the cache
int assoc; // Associativity of the cache for m-associativity cache.
string cacheMode;
ifstream memoryAcessFile;

bool isPowerTwo(int x)
{ // assuming x is positive
    // if x is power of two then x-1 should be all ones. Ow, it won't
    return (x & (x - 1)) == 0;
}

void takeInput()
{
    // I assume the sizes given will fit in an int type
    cout << "Please, enter the cache total size (in bytes), and the cache line size (in bytes) seperated by space: ";
    while (true)
    {
        cin >> cahceSize >> lineSize;
        if (cahceSize <= 0 || lineSize <= 0)
        {
            cout << "The sizes must be positive. Try again!\n";
        }
        else if (!isPowerTwo(cahceSize) || !isPowerTwo(lineSize))
        {
            cout << "The sizes must be powers of two. Try again!\n";
        }
        else if (cahceSize % lineSize)
        {
            cout << "The cache size must be divisible by the line size. Try again!\n";
        }
        else
        {
            break;
        }
    }


    cout<<"Which cache organization you want?\n";
    cout<<"Please, enter D for direct mapping, F for full associativity, or S for set associativity: ";
    cin>>cacheMode;

    while(cacheMode!="D" && cacheMode!="d" && cacheMode!="F" && cacheMode!="f" && cacheMode!="S" && cacheMode!="s"){
        cout<<"Invalid. Please enter the type of the cache correctly: ";
        cin>>cacheMode;
    }
    if (cacheMode =="D" || cacheMode=="d"){
        assoc=1;
    }
    else if (cacheMode == "F" || cacheMode == "f"){
        assoc = cahceSize / lineSize;
    }
    else{
        cout<<"Please, enter the associativity level. It must be a power of two less than or equal the number of lines\n";
        while( true){
            cin>>assoc;
            if (!isPowerTwo(assoc)){
                cout<<"Associativity must be power of two. Try again!\n";
            }
            else if (assoc > cahceSize / lineSize){
                cout<<"Associtivity must be less than or equal number of lines in the cache. Try again!\n";
            }
            else{
                break;
            }
        }
    }

    cout << "Enter the number of cycles needed to access the cache (an integer between 1 and 10): ";
    while (true)
    {
        cin >> cacheCycles;
        if (cacheCycles < 1 || cacheCycles > 10)
            cout << "The cycles must be between 1 and 10. Try again\n";
        else
            break;
    }

    cout << "The memory access file should consist of integers indicating the memory location being accessed seperated by lines\n";
    cout << "Enter the memory access file name with the extension. The file should be in the current direcory.\n";
    string fileName;
    cin >> fileName;
    memoryAcessFile.open(fileName);
    while (!memoryAcessFile.is_open())
    {
        cout << "The file can not be located.\nPlease enter the name again: ";
        cin >> fileName;
        memoryAcessFile.open(fileName);
    }




}

void process()
{

    int numAccesses = 0;
    int numHits = 0;
    int numMisses = 0;
    int numLines = (cahceSize / lineSize) / assoc;
    int bitsOffset = log2(lineSize);
    int bitsLines = log2(numLines); // In full, bitsLines will be zero
    // Each line is valid bit then tag.
   // vector<pair<int, int>> cache(numLines, pair<int, int>(0, 0));

    vector<vector<pair<int, int>>> D_cache(numLines, vector<pair<int, int>>(assoc,  pair<int, int>(0, 0)));
    vector<vector<pair<int, int>>> I_cache(numLines, vector<pair<int, int>>(assoc,  pair<int, int>(0, 0)));
    vector<vector<pair<int, int>>>* ptr;

    char type;
    int addr;
    while (memoryAcessFile >> type >> addr)
    {
//        char type = memLocation[0];
//        int s = memLocation.length();
//        cout<<s<<endl<<memLocation.substr(2, 1)<<endl;
//        int addr = stoi(memLocation.substr(2, s-2));

        if (type == 'i')  ptr = &I_cache;
        else if (type == 'd') ptr = &D_cache;

        numAccesses++;
        int tempLoc = addr;
        tempLoc = tempLoc >> bitsOffset; // removing bits offset
        int index = tempLoc % numLines;
        int tag = tempLoc >> bitsLines;

        int curSet=0;

        for ( ;curSet<assoc;curSet++){
            //hit
            if((*ptr)[index][curSet].first==1 && (*ptr)[index][curSet].second == tag){
                numHits++;
                break;
            }
            else if ((*ptr)[index][curSet].first==0){
                //miss but an empty place
                break;
            }
        }

        if (curSet ==assoc){ //miss and cache sets at that index are full
            numMisses++;
            int randSet= rand()%assoc;
            // random replacement policy
            (*ptr)[index][randSet].first = 1;
            (*ptr)[index][randSet].second = tag;
        }
        else if((*ptr)[index][curSet].first==0){//miss but there is an empty set
            numMisses++;
            (*ptr)[index][curSet].first=1;
            (*ptr)[index][curSet].second =tag;
        }

        if (type == 'i') cout << "Accessing instruction memory location " << addr << "\n";
        else if (type == 'd') cout << "Accessing data memory location " << addr << "\n";

        cout << "Data Cache" << "\n";
        cout << "Index\tValid Bit\tTag\n";
        for (int i = 0; i < numLines; i++)
        {
            for (int j=0; j<assoc;j++){
                cout << i << "\t" << D_cache[i][j].first << "\t\t" << D_cache[i][j].second << "\n";
            }

        }

        cout << "------------\n";
        cout << "Instruction Cache" << "\n";
        cout << "Index\tValid Bit\tTag\n";
        for (int i = 0; i < numLines; i++)
        {
            for (int j=0; j<assoc;j++){
                cout << i << "\t" << I_cache[i][j].first << "\t\t" << I_cache[i][j].second << "\n";
            }

        }
        cout << "------------\n";
        cout << "Total accesses are: " << numAccesses << "\n";
        cout << "Hit ratio is: " << (numHits * 1.0) / numAccesses << "\n";
        cout << "Miss ratio is: " << (numMisses * 1.0) / numAccesses << "\n";
        cout<< "The AMAT is: " <<cacheCycles + ((numMisses * 1.0) / numAccesses) *100<<"\n";

        cout << "---------------------------------------\n";
    }
}

int main()
{

    takeInput();
    process();
    system("pause");
    return 0;
}
