//
// Created by Allen Spain on 7/29/17.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include <ctype.h>

using namespace std;

/* Set buffer size to 4096 bytes */
const int BUFF_SIZE = 256;
static int numLines = 0;/*-l*/
static int numWords = 0;/*-w*/
static int numBytes = 0;/*-c*/
static int totalLines = 0;
static int totalWords = 0;
static int totalBytes = 0;


/* Prototypes */
void printSum(int l, int w, int c, char *filename);

string fileContent(int fd);

void errorMess(const char *missive);

void sum(string content, string option);

//string stdInOut(int fd);

int main(int argc, char *argv[]) {

    struct stat buf;
    char *filename;
    bool isFile = true;
    string option = "empty";
    string arg_;
//    bool good_option = false;

//    if (argc <= 2) {
    if (argc == 1) {
        filename = (char *) "";
        sum (fileContent (STDIN_FILENO), "empty");
        printSum (numLines, numWords, numBytes,filename);
        return 0;

    } else if (argc >= 2) {
        if (stat (argv[1], &buf) == -1) isFile = false;
        if (!isFile) {
            arg_ = argv[1];
            if (arg_[0] == '-') {
                for (char &a: arg_) {
                    if (a == 'l' || a == 'w' || a == 'c' || a == '-')
                        continue;

                    else
                        errorMess ("invalid option");
                }
                option = arg_;
                if (argc == 2) {
                    /* only two inputs of valid type*/
                    sum (fileContent (STDIN_FILENO), option);/* dash is input*/
                    if (strcmp (option.c_str (), "-") == 0) {
                        filename = (char *) "-";
                        printSum (numLines, numWords, numBytes, filename);

                    } else
                    {
                        filename = (char *) "";
                        printSum (numLines, numWords, numBytes, filename);

                    }
                    return 0;// exit success
                }

            } else errorMess ("invalid option");
        }
    }
//    }*/


    /* this should iterate through the list of files*/
    /* check their type for validity and compute its content's sum */
    int fileCount = 0;
    for (int i = 1; i < argc; i++) {

        int fd;
        /*skip option index*/
        if (i == 1 && isFile == false) {
            i++;
        }

        if (stat (argv[i], &buf) == -1) errorMess ("Invalid file:");
        /*check to ensure valid type*/
        if (S_ISDIR (buf.st_mode) == 0 && S_ISREG (buf.st_mode) == 0) errorMess ("Ivalid file");


        filename = argv[i];
        fd = open (filename, O_RDONLY);/* open file to read its content*/
        if (fd == -1) errorMess ("Cannot open file");

        sum (fileContent (fd), option);
        printSum (numLines, numWords, numBytes, filename);
        fileCount++;

    }
    /* more then one file will include total*/
    filename = (char *) "total";
    if (fileCount > 1)printSum (numLines, numWords, numBytes, filename);


    return EXIT_SUCCESS;
}

void errorMess(const char *missive) {

    perror (missive);
    exit (EXIT_FAILURE); // exit program
}


void sum(string content, string option)/* returns a string containing the sum */
{
    string line;
    int countL;
    int countW;
    bool space = false;


    // number of lines;
    // number of words
    for (char a: content) {
        if (a == -1)
            break;

        if (a == '\n')
            countL++;

        if (isspace (a)) {
            space = true;
        } else if (space) {
            countW++;
            space = false;
        }

    }
    numBytes = content.size ();
    numLines = countL;
    numWords = countW;

    // handle options
    if (strcmp (option.c_str (), "-c") == 0)
        numLines = numWords = 0;

    else if (strcmp (option.c_str (), "-l") == 0)
        numWords = numBytes = 0;

    else if (strcmp (option.c_str (), "-w") == 0)
        numLines = numBytes = 0;


    totalBytes += numBytes;
    totalLines += numLines;
    totalWords += numWords;
}

string fileContent(int fd) {
    string content = " ";

    int n;
    char buffer[BUFF_SIZE];


    /* strings are '\0' terminating*/
    while ((n = read (fd, buffer, BUFF_SIZE)) > 0) { // read the entire buffer

        if(n != -1 ){
            buffer[n] = '\0';
            content += buffer;
            }
        else errorMess ("ERROR");


    }
    return content;

}

void printSum(int l, int w, int c, char *filename) {

    cout << numLines << " " << numWords << " " << numBytes << " " << filename << "\n";
}
//void printSumTotal()
//{
//    cout << numLines <<" "<<numWords<<" "<<numBytes<< " total" <<"\n";
//}
//string stdInOut(int fd)
//{
//    int n;
//    char buffer[BUFF_SIZE];
//
//    string content = "";
//
//    /* strings are '\0' terminating*/
//    while ((n = read (fd, buffer, BUFF_SIZE)) > 0) { // read the entire buffer
//        buffer[n] = '\0';
//        content += buffer;
//    }
//
//    char storage[BUFF_SIZE];
//    int z = 0;
//    for (char &s: content) {
//
//        storage[z] = s;
//        z++;
//    }
//    storage[z] = '\0';
//
//    /* Only way to match shell was to used unbuffered i/o */
//    if (write (STDOUT_FILENO, storage, content.size ()) == -1)
//        errorMess ("write");
//
//    for(char c: storage)
//    {
//        content += storage;
//    }
//
//}

