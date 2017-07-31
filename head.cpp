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
#include <cerrno>

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

void printStuff(char* fileName, int howManyLines){
     const int BUFF_SIZE =1000000;
     int fd=open(fileName,O_RDONLY);
     if (fd==-1){
          cout<<"head: cannot open '"<<fileName<<"' for reading: "<<strerror(errno)<<endl;
     }
     int line_offset=howManyLines;
     int lineCount = 1;
     //    string s[BUFF_SIZE];
     string content;
     // size of data buffer
     char buffer[BUFF_SIZE];    // data buffer

     int n = 0;
     unsigned int i = 0;// number of bytes read


     /*otherwise buffer has biazaaar characters*/
     for (int j = 0; j < BUFF_SIZE; ++j) {
         buffer[j] = '\0';
     }

     while ((n = read (fd, buffer, BUFF_SIZE)) > 0) {
         buffer[n] = '\0';
         content += buffer;

     }

     for (i=0;i<content.size()-1;i++) {
         char letter = content[i];

         if (letter == '\n') {
            lineCount++;
         }
         if (line_offset < lineCount)
            break;
     }
     /* i points to 'n' character and we want everything after that.*/
     content = content.substr (0,i+1);
     char storage[BUFF_SIZE];
     int z = 0;
     for (char &s: content) {
         storage[z] = s;
         z++;
     }

     /* Only way to match shell was to used unbuffered i/o */
     if (write (STDOUT_FILENO, storage, content.size ()) == -1)
        cout<<"head: cannot write '"<<fileName<<"' to standard output: "<<strerror(errno)<<endl;

}

void printStuff(char* fileName){
     printStuff(fileName,10);
}

void printMyStuff(){
     const int BUFF_SIZE = 1000000; // size of data buffer
     char buffer [BUFF_SIZE];    // data buffer
     int n = 0;                  // number of bytes read

     while ((n = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {
       if (write(STDOUT_FILENO, buffer, n) == -1) perror("write");
     } // while
}

int main(int argc, char * argv []){//this my main

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
                         char* fileName=argv[i];
                         char meh='-';
                         char* dash=&meh;
                         if(strcmp(fileName,dash)==0){
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
                    char* fileName=argv[i];
                    char meh='-';
                    char* dash=&meh;
                    if(strcmp(fileName,dash)==0){
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
