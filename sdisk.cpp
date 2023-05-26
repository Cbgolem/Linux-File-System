

    #include "sdisk.h"
    
    // constructor
    Sdisk::Sdisk(string diskname,int numberofblocks, int blocksize)
    {

	this->diskname = diskname;
	this->numberofblocks = numberofblocks;
	this->blocksize = blocksize;
        fstream filestream;
       //if file exists it will be opened and do nothing
 	filestream.open(diskname.c_str(), ios::in | ios::out);
	if(filestream)
	{
		cout << "disk " << diskname << " exists" << endl;
	}
	else
	{
		cout << "disk does not exist will create a new one" << endl;
		fstream filestream2;
		filestream2.open(diskname.c_str(), ios::in | ios::out | ios::app);
    		for(int i = 0; i < blocksize * numberofblocks; i++)
    		{
    			filestream2 << "#";
    		}

	}
    } 
    
    int Sdisk::getblock(int blocknumber, string& buffer)
    {
    	fstream iofile;
    	iofile.open(diskname.c_str(), ios::in | ios::out);
    	if(blocknumber >= 0 && blocknumber < numberofblocks * blocksize)
    	{
    		char x;
    		iofile.seekg(blocknumber*blocksize);
    		for(int i =0; i < blocksize; i++)
    		{
    			iofile.get(x);
    			buffer.push_back(x);  			
    		}
    		return 1;
    	}
    	else
    	{
    		return 0;
    	}
    	
    }
    
    int Sdisk::putblock(int blocknumber, string buffer)
    {
     	fstream iofile;
    	iofile.open(diskname.c_str(), ios::in | ios::out);
    	if(blocknumber >= 0 && blocknumber < numberofblocks * blocksize)
    	{
    		iofile.seekg(blocknumber*blocksize);
    		for(int i =0; i < blocksize; i++)
    		{
    			iofile.put(buffer[i]);
    		}
    		return 1;
    		
    	}
    	else
    	{
    		return 0;
    	}
    	
    }
    //accessor function
    int Sdisk::getblocksize()
    {
	return this->blocksize;    	
    }
    
    //accessor function
    int Sdisk::getnumberofblocks()
    {
    	return this->numberofblocks;
    }
