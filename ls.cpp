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

using namespace std;

struct stat statbuff;
struct dirent *dname;
DIR *dir;

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

int printDirectory(const char* myDirectory, string option){
     //possibly the stupidest way to sort this
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



          int numFiles=0;
          int counter=0;
          if((dir=opendir(myDirectory))==NULL){
               cout<<"Something's wrong"<<endl;
               return EXIT_FAILURE;
          }

          while((dname=readdir(dir))){
               numFiles++;
          }
          closedir(dir);
          if((dir=opendir(myDirectory))==NULL){
               cout<<"Something's wrong"<<endl;
               return EXIT_FAILURE;
          }

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

          //cout<<totalEntries<<endl;

          /*for (int i=0;i<counter;i++){
               string lower = FILES[i].name;
               cout<<lower<<endl;
               for(int j=0;lower.length();j++){
                    FILES[i].NAME[j]=toupper(lower[j]);

               }
          }*/

     //my implementation of the stupidest sorting method
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
          delete[] sortedd;
          delete[] FILES;
}

int main(const int argc, const char * argv []){//this my main

string option="none";
const char* currentDir=get_current_dir_name();

     if(argc==1){//no options, no selected directories
          printDirectory(currentDir,option);
     }//no options, no selected directories

     else if(argc>1){
          string secArg=argv[1];
          if(secArg.compare("-a")==0){
               if(argc==2){
                    printDirectory(currentDir,"-a");
               }
               else{
                    const char* selDir;
                    for(int i=2;i<argc;i++){
                         selDir=argv[i];
                         string durdurdur(selDir);
                         if((dir=opendir(selDir))==NULL){
                              cout<<"ls: cannot access "<<durdurdur<<": "<<strerror(errno)<<endl;
                         }
                         else{
                              cout<<durdurdur<<":"<<endl;
                              printDirectory(selDir,"-a");
                              if(i!=argc-1){
                                   cout<<endl;
                              }
                         }
                    }
               }
          }
          else if(secArg.compare("-l")==0){
               if(argc==2){
                    printDirectory(currentDir,"-l");
               }
               else{
                    const char* selDir;
                    for(int i=2;i<argc;i++){
                         selDir=argv[i];
                         string durdurdur(selDir);
                         if((dir=opendir(selDir))==NULL){
                              cout<<"ls: cannot access "<<durdurdur<<": "<<strerror(errno)<<endl;
                         }
                         else{
                              cout<<durdurdur<<":"<<endl;
                              printDirectory(selDir,"-l");
                              if(i!=argc-1){
                                   cout<<endl;
                              }
                         }
                    }
               }
          }
          else if(secArg.compare("-al")==0||secArg.compare("-la")==0){
               if(argc==2){
                    printDirectory(currentDir,"-la");
               }
               else{
                    const char* selDir;
                    for(int i=2;i<argc;i++){
                         selDir=argv[i];
                         string durdurdur(selDir);
                         if((dir=opendir(selDir))==NULL){
                              cout<<"ls: cannot access "<<durdurdur<<": "<<strerror(errno)<<endl;
                         }
                         else{
                              cout<<durdurdur<<":"<<endl;
                              printDirectory(selDir,"-al");
                              if(i!=argc-1){
                                   cout<<endl;
                              }
                         }
                    }
               }
          }
          else if(secArg.substr(0,1).compare("-")==0){
               cout<<"ls: invalid option --'";
               for(int i=1;i<secArg.length();i++){
                    if(secArg.substr(i,1).compare("a")!=0&&secArg.substr(i,1).compare("l")!=0){
                         cout<<secArg[i];
                         break;
                    }
               }
               cout<<"'"<<endl;
          }

          else{
               const char* selDir;
               for(int i=1;i<argc;i++){
                    selDir=argv[i];
                    string durdurdur(selDir);
                    if((dir=opendir(selDir))==NULL){
                         cout<<"ls: cannot access "<<durdurdur<<": "<<strerror(errno)<<endl;
                    }
                    else{
                         cout<<durdurdur<<":"<<endl;
                         printDirectory(selDir,"none");
                         if(i!=argc-1){
                              cout<<endl;
                         }
                    }
               }
          }

     }


     return EXIT_SUCCESS;
}//This my main
