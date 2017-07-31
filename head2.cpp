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
int main(const int argc, const char * argv []) {

     int numLines=10;
     const char * filename = argv[1];
     int fd = open(filename, O_RDONLY);

     const int BUFF_SIZE = 8192; // size of data buffer
     char buffer [BUFF_SIZE];    // data buffer
     read(fd, buffer, BUFF_SIZE);
     int i=0;
     int j=0;
     while(j<numLines) {
          for(i;i<BUFF_SIZE-1;i++){
               cout<<buffer[i]<<endl;
               if(buffer[i]=='\n'){

                         j++;
                    
               }
          }
     } // while
     if (write(i, buffer, 8192) == -1) perror("write");

     close(fd); // should check return value here

     return EXIT_SUCCESS;
} // main
