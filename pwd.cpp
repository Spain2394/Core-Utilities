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

/**
 * [main program printing out the present working directory]
 * @return [returns EXIT_SUCCESS if it successfully prints out the pwd]
 */

int main(){
     char* currentDir;
     if((currentDir=get_current_dir_name())==NULL){//gets the pwd to print out
          cout<<"I don't really know what to put for this failure... can pwd even fail? "<<strerror(errno);//I legit have no idea what this displays when it fails
          free(currentDir);
          return EXIT_FAILURE;
     }
     else{//printing out the pwd
          cout<<currentDir<<endl;
          free(currentDir);
     }
     return EXIT_SUCCESS;
}
