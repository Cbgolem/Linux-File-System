#ifndef TABLE_H
#define TABLE_H
#include "filesys.h"
#include "shell.h"

class Table: public Shell
{
public:
	Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile);
	int build_table(string input_file);
	int search(string value);
private:
	string flatfile;
	string indexfile;
	int indexsearch(string value);
	
};
#endif
