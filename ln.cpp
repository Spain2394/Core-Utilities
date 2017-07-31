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

const char* target;
const char* destination;
int main(const int argc, const char * argv []){//this my main

     if(strcmp(argv[1],"-s")==0){
          if(symlink(argv[2],argv[3])==-1){
               cout<<"ln: '"<<argv[2]<<"': "<<strerror(errno)<<endl;
               return EXIT_FAILURE;
          }
     }
     else{
          if(link(argv[1],argv[2])==-1){
               cout<<"ln: '"<<argv[1]<<"': "<<strerror(errno)<<endl;
               return EXIT_FAILURE;
          }
     }

}// this my main
