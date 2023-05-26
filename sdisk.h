
    #ifndef SDISK_H
    #define SDISK_H
    #include <string>
    #include <vector>
    #include <iostream>
    #include <fstream>
    #include <sstream>
    using namespace std; 

    class Sdisk
    {
    public:
        Sdisk(string diskname, int numberofblocks, int blocksize);
        int getblock(int blocknumber, string& buffer);
        int putblock(int blocknumber, string buffer);
        int getnumberofblocks();
        int getblocksize();

    private:
        string diskname;
        int numberofblocks;
        int blocksize; 
    }; // Sdisk
    #endif




