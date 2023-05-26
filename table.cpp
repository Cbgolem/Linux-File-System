#include "table.h"
#include "block.h"

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Shell(diskname,numberofblocks,blocksize)
{
	this->flatfile = flatfile;
	this->indexfile = indexfile;
}

int Table::build_table(string input_file)
{
    int code = newfile(indexfile);
    if(code == -1)
    {
        cout << "file is full" << endl;
       return 0;
    }
    if(code == 0)
    {
        cout << "file already exists" << endl;
        return 0;
    }
    code = newfile(flatfile);
    if(code == -1)
    {
        cout << "file is full" << endl;
        return 0;
    }
    if(code == 0)
    {
        cout << "file already exists" << endl;
        return 0;
    }

    ostringstream ostream;
    ifstream infile;
    infile.open(input_file.c_str());
    string inputline;
    getline(infile,inputline);
    block(inputline,getblocksize());
    while(infile.good())
    {
        string key = inputline.substr(0,5);
        string buffer = "test";
    	vector<string> blocks = block(inputline,getblocksize());
        int blockid = addblock(flatfile, blocks[0]);
        ostream << key << " " << blockid << " ";
        getline(infile,inputline);
        
    }
    ostream << "xxxxx" << " " << 0 << " ";
    string buffer = ostream.str();
    vector<string> iblocks = block(buffer,getblocksize());
    for(int i = 0; i < iblocks.size(); i++)
    {
        addblock(indexfile,iblocks[i]);
    }
    return 1;
}

int Table::indexsearch(string value)
{
    	string buffer;
    	int blockid = getfirstblock(indexfile);
    	while(blockid != 0)
    	{
        	string b;
        	readblock(indexfile,blockid,b);
        	buffer += b;
        	blockid = nextblock(indexfile,blockid);
    	}
    	istringstream istream;
    	istream.str(buffer);
    	string k, b;
    	istream >> k >> b;
    	while(k != "xxxxx")
    	{
        	if( k == value)
        	{
            		//cannot return a string when its expecting a integer value
            		int returnval = stoi(b);
            		return returnval;
        	}
        	istream >> k >> b;
    	}
    	return 0;
}

int Table::search(string value)
{
    	int code = indexsearch(value);
    	if(code == 0)
    	{
        	cout << "record not found" << endl;
        	return 0;
    	}
    	else 
    	{
        	string buffer;
        	code = readblock(flatfile,code,buffer);
        	cout << buffer << endl;
        	return 1;
    }
}
