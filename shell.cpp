#include "filesys.h"
#include "sdisk.h"
#include "block.h"
#include "shell.h"


Shell::Shell(string diskname, int numberofblocks, int blocksize):Filesys(diskname, numberofblocks, blocksize)
{
}

int Shell::add(string file, string buffer)
{
	//checking all possible states that would cause an error
	if(file.empty() || buffer.empty())
	{
		cout << "tried to pass an empty string to the add function" << endl;
		return 0;
	}
	int code = getfirstblock(file);
	if(code != 0)
	{
		cout << "file already exists cannot add" << endl;
		return 0;
	}
	code = newfile(file);
	if(code == 0)
	{
		cout << "failed to create the file" << endl;
		return 0;
	}
	
	vector<string> blocks = block(buffer,getblocksize());
	for(int i = 0; i < blocks.size(); i++)
	{
		code = addblock(file,blocks[i]);
		if (code == 0)
		{
			cout << "Encountered an error trying to add a file" << endl;
			return 0;
		}
	}
	return 1;
}

int Shell::del(string file)
{
	//checks if the the file exists
	int block = getfirstblock(file);
	if(block == 0)
	{
		cout << "file does not exist so it cannot be deleted" << endl;
		return 0;
	}
	
	/*
	this should be the same as doing a rmfile
	while(block != 0)
	{
		delblock(file,block);
		block = getfirstblock(file);
	}*/
	rmfile(file);
	return 1;
}



int Shell::copy(string file1, string file2)
{
	//error checking
	if(file1.empty() || file2.empty())
	{
		cout << "tried to pass an empty string to the copy function" << endl;
		return 0;
	}
	int block = getfirstblock(file1);
	if( block == 0)
	{
		cout << "source file does not exist" << endl;
		return 0;
	}
	
	if( newfile(file2) == 0)
	{
		cout << "destination file already exists" << endl;
		return 0;
	}
	
	while( block != 0)
	{
		string buffer;
		readblock(file1, block, buffer);
		int code = addblock(file2, buffer);
		block = nextblock(file1,block);
		if( code == 0)
		{
			cout << "There is no space left in the disk" << endl;
			return 0;
		}
	}
	return 1;
}

int Shell::dir()
{ 
  vector<string> flist=ls();
  for (int i=0; i<flist.size(); i++)
      {
        cout << flist[i] << endl;
      }
  return 1;
}

int Shell::type(string file)
{
	string buffer;
	int block = getfirstblock(file);
	if( block == 0 )
	{
		cout << "file does not exist" << endl;
		return 0;
	}
	
	while (block > 0)
  	{
   		string b;
   		readblock(file, block, b);
   		buffer += b;
   		block = nextblock(file, block);
  	}
 	for (int i = 0; i < buffer.size(); i++)
 	{
     		if (buffer[i] != '#')
     		{
         		cout << buffer[i];
     		}
 	}
 	cout << endl;
 	return 1;
}
