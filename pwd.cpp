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

int main(){
     char* currentDir;
     if((currentDir=get_current_dir_name())==NULL){
          cout<<"I don't really know what to put for this failure... can pwd even fail? "<<strerror(errno);
          free(currentDir);
          return EXIT_FAILURE;
     }
     else{
          cout<<currentDir<<endl;
          free(currentDir);
     }
     return EXIT_SUCCESS;
}
