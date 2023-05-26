#include "block.h"
#include "shell.h"
#include "sdisk.h"
#include "filesys.h"
#include "table.h"

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
 
 //Sdisk sdisk=Sdisk("sdisk.txt",256,128);
 //Filesys fsys=Filesys("sdisk.txt",256,128);
 //Shell shell("testtable",256,128);
 Table table("sdisk.txt",256,128, "flatfile", "indexfile");
 table.build_table("data.txt");

 string s;
 string command="go";
 string op1,op2;

 while (command != "quit")
     {

       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       if (firstblank < s.length())
         op1 = s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2 = s.substr(secondblank+1);
       if (command=="dir")
          {
             table.dir();
          }
       if (command=="add")
          {
            // The variable op1 is the new file and op2 is the file data
	        table.add(op1, op2);
          }

       if (command=="del")
          {
            // The variable op1 is the file
           	table.del(op1);
          }
       if (command=="type")
          {
            // The variable op1 is the file
           table.type(op1);
          }
       if (command=="copy")
          {
            // The variable op1 is the source file and the variable op2 is the destination file.
           table.copy(op1, op2);
          }
       if (command=="search")
          {
            // The variable op1 is the date
            table.search(op1);
          }
      }
 return 0;
}
