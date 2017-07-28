#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <dirent.h>

using namespace std;

DIR *dir;
struct dirent *ent;

struct stat statbuf;
off_t offset=0;

bool isThisAnInt(string meh){
     for(char& c:meh){
          //cout<<c<<endl;
          if(!isdigit(c)){
               return false;
          }
     }
     return true;
}

void printStuff(string fileName){
     ifstream myfile (fileName);
     string line;
     while (getline(myfile,line)){
          cout<<line<<endl;
     }
}

void printStuff(string fileName, int howManyLines){
     ifstream myfile (fileName);
     int numLines=0;
     string line;
     while(getline(myfile,line)){
          numLines++;
     }
     myfile.close();

     string *allTheLines;
     allTheLines = new string[numLines];
     int counter=0;

     myfile.open(fileName);
     while (getline(myfile,line)){
          allTheLines[counter]=line;
          counter++;
     }
     myfile.close();
     for(int i=0;i<howManyLines;i++){
          cout<<allTheLines[i]<<endl;
     }
}

void printMyStuff(){
     while(!cin.eof()){//checking for ctrl d
          string printThis;
          getline(cin,printThis);
          cout<<printThis<<endl;
     }//while statement
}

int main(const int argc, const char * argv []){//this my main

     if (argc==1){
          printMyStuff();
     }
     if(argc>1){
          if(strcmp(argv[1],"-n")==0){//Given number of lines to print
               if(argc<4){//Failed to give number of lines
                    cout<<"head: option requires an argument -- 'n'"<<endl;
                    return EXIT_FAILURE;
               }
               string temp = argv[2];
               if(isThisAnInt(temp)){
                    int howManyLines=stoi(temp);
                    for (int i = 3;i<argc;i++){
                         string fileName=argv[i];
                         if(fileName=="-"){
                              if(argc>4){
                                   cout<<"==> standard input <=="<<endl;
                              }
                              printMyStuff();
                         }
                         else{
                              stat(argv[i], &statbuf);
                              if(S_ISREG(statbuf.st_mode)){//print stuff out
                                   if(argc>4){
                                        cout<<"==> "<<fileName<<" <=="<<endl;
                                   }
                                   printStuff(fileName,howManyLines);
                              }//print stuff out
                              else if(S_ISDIR(statbuf.st_mode)){//directory
                                   cout<<"head: error reading '"<<fileName<<"': Is a directory"<<endl;
                                   return EXIT_FAILURE;
                              }//Directory
                              else{//Unknown file
                                   //cout<<"I got this far"<<endl;
                                   cout<<"head: error reading '"<<fileName<<"': Unknown file type"<<endl;
                                   return EXIT_FAILURE;
                              }//Unknown file
                         }
                    }//incrementing through argv
               }//valid input for number of lines
               else{//Not a number
                    //cout<<"Why is this showing up"<<endl;
                    cout<<"head: "<<argv[2]<<": invalid number of lines"<<endl;
                    return EXIT_FAILURE;
               }//Not a number
          }//Given -n
          else{//Was not given number of lines to print
               for (int i = 1;i<argc;i++){
                    string fileName=argv[i];
                    if(fileName=="-"){
                         if(argc>2){
                              cout<<"==> standard input <=="<<endl;
                         }
                         printMyStuff();
                    }
                    else{
                         stat(argv[i], &statbuf);
                         if(S_ISREG(statbuf.st_mode)){//print stuff out
                              if(argc>2){
                                   cout<<"==> "<<fileName<<" <=="<<endl;
                              }
                              printStuff(fileName);
                         }//print stuff out
                         else if(S_ISDIR(statbuf.st_mode)){//directory
                              cout<<"head: error reading '"<<fileName<<"': Is a directory"<<endl;
                              return EXIT_FAILURE;
                         }//Directory
                         else{//Unknown file
                              cout<<"head: error reading '"<<fileName<<"': Unknown file type"<<endl;
                              return EXIT_FAILURE;
                         }//Unknown file
                    }
               }//incrementing through argv
          }//was not given number of lines to print
     }
}
