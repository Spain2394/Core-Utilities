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
#include <vector>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <cerrno>

using namespace std;

struct stat statbuff;
struct dirent *dname;
DIR *dir;

//Struct representing all the information held by a file
struct myFiles{
     char *name;
     string permissions;
     bool isHidden;
     string NAME;
     nlink_t numLinks;
     string ownerID;
     string groupID;
     off_t fileSize;
     time_t lastMod;
     int position;
};

/**
 * [printDirectory This function is the bulk of the code, opening directories, reading their files, and printing out the contents according to user direction]
 * @param myDirectory [the directory printDirectory is being called upon]
 * @param option      [Supplies the options -a and/or -l]
 */

void printDirectory( char* myDirectory, string option){
     if((dir=opendir(myDirectory))==NULL){
          cout<<"ls: cannot access "<<myDirectory<<": "<<strerror(errno)<<endl;
          closedir(dir);
          exit(0);
     }
     closedir(dir);
     //creating an array of strings representing the file names in a folder sorted alphabetically
          struct dirent **namelist;
          int n;
          string sortedNames="";
          string *sortedd;
          n = scandir(myDirectory, &namelist, NULL, alphasort);
          int totalEntries=n;
          sortedd = new string [n];
          if (n < 0)
               perror("scandir");
          else {
               while (n--) {
                    //cout<<n<<endl;
                    //cout<<namelist[n]->d_name<<endl;
                    string temp(namelist[n]->d_name);
                    string index=to_string(n);
                    sortedd[n]=namelist[n]->d_name;
                    //cout<<sortedd[n]<<endl;
                    free(namelist[n]);
               }
               free(namelist);
          }
     //some really dumb stuff I did to get it sorted



          int numFiles=totalEntries;
          int counter=0;
          if((dir=opendir(myDirectory))==NULL){
               cout<<"Something's wrong"<<endl;
               //return EXIT_FAILURE;
          }

          //creating and filling an array of myFiles struct objects with each respective file's information
          myFiles * FILES;
          FILES=new myFiles[numFiles];
          while((dname=readdir(dir))){
               struct passwd *pw=getpwuid(statbuff.st_uid);
               struct group *gr=getgrgid(statbuff.st_gid);
               stat(dname->d_name,&statbuff);
               //cout<<dname->d_name<<endl;
               FILES[counter].name=dname->d_name;
               FILES[counter].permissions="";
               FILES[counter].permissions+=((S_ISDIR(statbuff.st_mode))?"d":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IRUSR)?"r":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IWUSR)?"w":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IXUSR)?"x":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IRGRP)?"r":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IWGRP)?"w":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IXGRP)?"x":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IROTH)?"r":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IWOTH)?"w":"-");
               FILES[counter].permissions+=((statbuff.st_mode&S_IXOTH)?"x":"-");
               FILES[counter].numLinks=statbuff.st_nlink;
               FILES[counter].ownerID=pw->pw_name;
               FILES[counter].groupID=gr->gr_name;
               FILES[counter].fileSize=statbuff.st_size;
               FILES[counter].lastMod=statbuff.st_mtime;
               if(dname->d_name[0]=='.'){
                    FILES[counter].isHidden=true;
               }
               else{
                    FILES[counter].isHidden=false;
               }
               counter++;
          }//while loop



     //my implementation of the sorting method
     //searches through the sorted string array for file names and matches the order with the myFiles array
          string fileName;
          for(int i=0;i<totalEntries;i++){
               fileName=FILES[i].name;
               for(int j=0;j<totalEntries;j++){
                    if(fileName.compare(sortedd[j])==0){
                         FILES[i].position=j;
                         //cout<<FILES[i].name<<"\t"<<j<<endl;
                         break;
                    }
               }
          }
     //I am the King Midas of spaghetti-code

     //prints out given the option of having no additional arguments
          if (option.compare("none")==0){//No additional arguments
               for(int i=0;i<totalEntries;i++){
                    for(int j=0;j<totalEntries;j++){
                         if(FILES[j].position==i){
                              if(!FILES[j].isHidden){
                                   cout<<FILES[j].name<<endl;
                              }
                              break;
                         }
                    }
               }

          }//No additional arguments
          //prints out given the argument -a for all
          else if(option.compare("-a")==0){//List all
               for(int i=0;i<totalEntries;i++){
                    for(int j=0;j<totalEntries;j++){
                         if(FILES[j].position==i){
                              cout<<FILES[j].name<<endl;
                              break;
                         }
                    }
               }
          }
          //prints out given the argument -l for long list
          else if(option.compare("-l")==0){//List all
               for(int i=0;i<totalEntries;i++){
                    for(int j=0;j<totalEntries;j++){
                         if(FILES[j].position==i){
                              if(FILES[j].isHidden){
                                   continue;
                              }
                              char buff[32];
                              struct tm *timeinfo;
                              timeinfo=localtime(&FILES[j].lastMod);
                              strftime(buff, 32, "%b %d %H:%M", timeinfo);
                              cout<<FILES[j].permissions<<".\t"<<FILES[j].numLinks<<"\t"<<FILES[j].ownerID<<"\t"<<FILES[j].groupID<<"\t"<<FILES[j].fileSize<<"\t";
                              printf("%s",buff);
                              cout<<"\t"<<FILES[j].name<<endl;
                              break;
                         }
                    }
               }
          }

          //prints out given the option for both long list and all
          else if(option.compare("-la")==0||option.compare("-al")==0){
               for(int i=0;i<totalEntries;i++){
                    for(int j=0;j<totalEntries;j++){
                         if(FILES[j].position==i){
                              char buff[32];
                              struct tm *timeinfo;
                              timeinfo=localtime(&FILES[j].lastMod);
                              strftime(buff, 32, "%b %d %H:%M", timeinfo);
                              cout<<FILES[j].permissions<<".\t"<<FILES[j].numLinks<<"\t"<<FILES[j].ownerID<<"\t"<<FILES[j].groupID<<"\t"<<FILES[j].fileSize<<"\t";
                              printf("%s",buff);
                              cout<<"\t"<<FILES[j].name<<endl;
                              break;
                         }
                    }
               }
          }

          //tries to clean up
          delete[] sortedd;
          delete[] FILES;
}

/**
 * [main driver for the program taking in user input]
 * @param  argc [number of arguments supplied by the user]
 * @param  argv [array of the arguments supplied by the user]
 * @return      [returns EXIT_SUCCESS if everything is successfully printed and EXIT_FAILURE otherwise]
 */

int main( int argc,  char * argv []){//this my main

     //default to having no additional flags and uses the current directory
     string option="none";
      char* currentDir=get_current_dir_name();


     //prints out using the default settings
     if(argc==1){//no options, no selected directories
          printDirectory(currentDir,option);
          free(currentDir);
     }//no options, no selected directories

     else if(argc>1){
          string secArg=argv[1];

          //Looks for the -a flag
          if(secArg.compare("-a")==0){
               if(argc==2){
                    //prints the current directory with the flag
                    printDirectory(currentDir,"-a");
                    free(currentDir);
               }
               else{
                    //prints out each directory supplied by the user with the -a flag
                    for(int i=2;i<argc;i++){
                         if(argc>3){
                              cout<<argv[i]<<":"<<endl;
                         }
                         printDirectory(argv[i],"-a");
                         if(i!=argc-1){
                              cout<<endl;
                         }
                    }
               }
          }

          //looking for the -l flag
          else if(secArg.compare("-l")==0){
               if(argc==2){
                    //prints out the current directory with the -l flag
                    printDirectory(currentDir,"-l");
                    free(currentDir);
               }
               else{
                    //prints out each directory supplied by the user with the -l flag
                    for(int i=2;i<argc;i++){
                         if(argc>3){
                              cout<<argv[i]<<":"<<endl;
                         }
                         printDirectory(argv[i],"-l");
                         if(i!=argc-1){
                              cout<<endl;
                         }
                    }
               }
          }
          //looks for the presence of both the -a and the -l flags
          else if(secArg.compare("-al")==0||secArg.compare("-la")==0){
               if(argc==2){
                    //prints current directory with the flags
                    printDirectory(currentDir,"-la");
                    free(currentDir);
               }
               else{
                    //prints user supplied directories with the flags
                    for(int i=2;i<argc;i++){
                         if(argc>3){
                              cout<<argv[i]<<":"<<endl;
                         }
                         printDirectory(argv[i],"-al");
                         if(i!=argc-1){
                              cout<<endl;
                         }
                    }
               }
          }
          //looks for improper flags
          else if(secArg.substr(0,1).compare("-")==0){
               cout<<"ls: invalid option --'";
               for(unsigned int i=1;i<secArg.length();i++){
                    if(secArg.substr(i,1).compare("a")!=0&&secArg.substr(i,1).compare("l")!=0){
                         cout<<secArg[i];
                         break;
                    }
               }
               cout<<"'"<<endl;
          }
          //prints out the user supplied directories with no flags
          else{
               for(int i=1;i<argc;i++){
                    if(argc>1){
                         cout<<argv[i]<<":"<<endl;
                    }
                    printDirectory(argv[i],"none");
                    closedir(dir);
                    if(i!=argc-1){
                         cout<<endl;
                    }
               }
          }

     }
     free(currentDir);

//done
     return EXIT_SUCCESS;
}//This my main
