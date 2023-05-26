#include "filesys.h"
#include "sdisk.h"
#include "block.h"

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname,numberofblocks,blocksize)
{
	string buffer;
	int code = getblock(1,buffer);
	if ( code == 0)
	{
		cout << "bad device" << endl;
		return;
	}
	rootsize = getblocksize() / 12;
	fatsize = (getnumberofblocks()*6) / getblocksize() + 1;
	if( buffer[0] == '#' )
	{
		//no file system
		cout << "We will create a file system for you" << endl;
		buildfs();

	}
	else
	{
		cout << "This file system already exists" << endl;
		readfs();
	}
}

int Filesys::fsclose()
{
	return fssynch();
}

int Filesys::rmfile(string file)
{
	//checks if the file exists or not
	bool file_exist = false;
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		{
			file_exist = true;
			break;
		}
	}
	if( file_exist == false)
	{
		cout << "Tried to remove a file that doesn't exist" << endl;
		return 0;
	}
	
	//checks if the file is already empty
	int blockid = getfirstblock(file);
	if(blockid == 0)
	{
		cout << "File is already empty" << endl;
		return 0;
	}
	
	//removes the file
	while( (getfirstblock(file)) != 0 )
	{
		delblock(file, getfirstblock(file));
	}
	
	//deletes the filename in the root so that it can be used later
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		{
			filename[i] = "xxxxx";
		}
	}
	fssynch();
	return 1;
}

int Filesys::buildfs()
{

	for(int i = 0; i < rootsize; i++)
	{
		filename.push_back("xxxxx");
		firstblock.push_back(0);
	}

	fat.push_back(fatsize + 2);
	fat.push_back(-1);

	for(int i = 0; i < fatsize; i++)
	{
		fat.push_back(-1);
	}

	for(int i = fatsize + 2; i < getnumberofblocks(); i++)
	{
		fat.push_back(i+1);
	}
	fat[fat.size()-1] = 0;
	return fssynch();
}


int Filesys::readfs()
{
	istringstream istream;
	string buffer;
	getblock(1,buffer);
	istream.str(buffer);
	for(int i = 0 ; i < rootsize; i++)
	{
		string s; int t;
		istream >> s;
		istream >> t;
		filename.push_back(s);
		firstblock.push_back(t);
	}
	
	string buffer2;
	istringstream istream2;
	for( int i = 0; i < fatsize; i++)
	{
		string b;
		getblock(i+2,b);
		buffer2 += b;
	}
	istream2.str(buffer2);
	for(int i =0; i <getnumberofblocks();i++)
	{
		int t;
		istream2 >> t;
		fat.push_back(t);
	}
	return 0;
}

int Filesys::fssynch()
{
	ostringstream ostream;
	for(int i = 0; i < rootsize; i++)
	{
		ostream << filename[i] << " " << firstblock[i] << " ";
	}
	string buffer1 = ostream.str();
	vector<string> blocks1 = block(buffer1,getblocksize());
	putblock(1,blocks1[0]);
	ostringstream ostream2;
	for(int i = 0; i < fat.size(); i++)
	{
		ostream2 << fat[i] << " ";
	}
	string buffer2 = ostream2.str();
	vector<string> blocks2 = block(buffer2,getblocksize());
	for(int i = 0; i < blocks2.size(); i++)
	{
		putblock(2 + i, blocks2[i]);
	}
	return 1;
}

int Filesys::newfile(string newname)
{
	for(int i = 0; i < rootsize; i++)
	{
		if(filename[i] == newname)
		{
			cout << "File " << newname << " already exists" << endl;
			return 0;
		}
	}
	
	for(int i = 0; i < rootsize; i++)
	{
		if(filename[i] == "xxxxx")
		{
			filename[i] = newname;
			firstblock[i] = 0;
			fssynch();
			return 1;
		}
	}
	
	cout << "no space in root" << endl;
	return 0;
}

int Filesys::getfirstblock(string file)
{
	for(int i = 0; i < rootsize; i++)
	{
		if(filename[i] == file)
		{
			return firstblock[i];
		}
	}
	return 0;
}

int Filesys::addblock(string file, string block)
{
	int blockid = getfirstblock(file);
	if (blockid == -1)
	{
		cout << "File does not exists in filesystem" << endl;
		return 0;
	}
	int allocate = fat[0];
	if (allocate == 0)
	{
		cout << "There is no space left on the disk" << endl;
		return -1;
	}
	
	fat[0] = fat[fat[0]];
	fat[allocate] = 0;
	if (blockid == 0)
	{
		for(int i = 0; i < rootsize; i++)
		{
			if( filename[i] == file)
			{
				firstblock[i] = allocate;
			}
		}
	}
	else
	{
		while( fat[blockid] != 0 )
		{
			blockid = fat[blockid];
		}
		fat[blockid] = allocate;
	}
	putblock(allocate,block);
	fssynch();
	return allocate;	
}

int Filesys::nextblock(string file, int blocknumber)
{
	if( checkblock(file,blocknumber))
	{
		return fat[blocknumber];
	}
	else
	{
		return -1;
	}
}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
	if (checkblock(file,blocknumber))
	{
		getblock(blocknumber,buffer);
		return 1;
	}
	else
	{
		return 0;
	}
	
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
	if ( checkblock(file,blocknumber))
	{
		putblock(blocknumber,buffer);
		return 1;
	}
	else
	{
		return 0;
	}
}

int Filesys::delblock(string file, int blocknumber)
{

	if(!checkblock(file,blocknumber))
	{
		cout << "Cannot delete the block" << endl;
		return 0;
	}
	
	int block = getfirstblock(file);
	if(block == blocknumber)
	{
		for(int i = 0; i < filename.size(); i++)
		{
			if(filename[i] == file)
			{
				firstblock[i] = fat[blocknumber];
				break;
			}
		}
	}
	else 
	{
		int b = block;
		while( fat[b] != blocknumber )
		{
			b = fat[b];
		}
		
		fat[b] = fat[blocknumber];
	}
	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;
	return fssynch();
}

bool Filesys::checkblock(string file, int block)
{
	int blockid = getfirstblock(file);
	if (blockid == -1)
		return false;
	while(blockid != 0)
	{
		if (blockid == block)
			return true;
		blockid = fat[blockid];
	}
	return false;
}

vector<string> Filesys::ls()
{ 
  vector<string> flist;
  for (int i=0; i<filename.size(); i++)
      {
        if (filename[i] != "xxxxx")
           {
             flist.push_back(filename[i]);
           }
      }
  return flist;
}

