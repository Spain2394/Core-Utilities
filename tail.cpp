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


using std::cout;
using std::endl;
using std::string;
using std::cerr;

using namespace std;

/* Set buffer size to 4096 bytes */
const int BUFF_SIZE = 8192;


/*function prototypes*/
void writeLines(int fd, int line_offset);

void writeBytes(int fd, int byte_offset);

void errorMess(int err);

void stdInOut(int fd);

int main(const int argc, char *argv[])
{

    struct stat buf;
    char *filename;
    bool useLines;
    string arg;
    int fd;
    int offset;// number bytes or lines specified


    /* command prompt simply takes input when no filename specified*/
    if (argc <= 2) {
        errorMess (1);
    }

        /* If the args are thr right number, make sure they are the right kind*/
    else if (argc > 2) {
        if (strcmp (argv[1], "-n") == 0)/* number of lines option*/
        {

            arg = argv[2];
            for (char &c: arg) {
//                cout << c <<" is a digit: "<< isdigit (c) <<  endl;
                if (isdigit (c) == 0)
                    errorMess (1);
            }
            offset = stoi (argv[2]);
            useLines = true;

        } else if (strcmp (argv[1], "-c") == 0)/* number of bytes option*/
        {
            arg = argv[2];
            for (char &c: arg) {
                if (isdigit (c) == 0)
                    errorMess (1);
            }
            offset = stoi (argv[2]);
            useLines = false;
        } else errorMess (1);
    }

    /*  arguments are provided*/
    /*check to make sure regular file*/

    if (argc <= 3) {
        stdInOut (STDIN_FILENO);/* allows input output functionality if (tail -n 10) is provided*/

    } else if (argc > 3) {
        for (int i = 3; i < argc; i++) {
            if (stat (argv[3], &buf) == -1) {
                errorMess (errno);
            }
            /*check to ensure valid type*/
            if (S_ISDIR (buf.st_mode) == 1) /*dir, do not do anything*/
                return 0;

            else if (S_ISREG(buf.st_mode) == 0) /*not regular file*/
                errorMess (errno);

            filename = argv[3];/*argv[4] is a valid filename*/

            fd = open (filename, O_RDONLY);/* open file to read its content*/
            if (fd == -1) errorMess (errno);

            if (useLines) {
                writeLines (fd, offset);
            } else if (!useLines) {
                writeBytes (fd, offset);
            }
        }
    }

    if (close (fd) == -1)
        errorMess (errno);


    return EXIT_SUCCESS;
} // main

void writeBytes(int fd, int byte_offset) {
    // size of data buffer
    char buffer[BUFF_SIZE];    // data buffer
    int n = 0;                  // number of bytes read

    off_t len = lseek (fd, 0, SEEK_END);
    if (len == -1) perror ("lseek");


    if (byte_offset > len)
        errorMess (errno);


    off_t last_bytes = lseek (fd, -byte_offset, SEEK_END);
    if (last_bytes == -1) errorMess (errno);


    while ((n = read (fd, buffer, last_bytes)) > 0) { // read the entire buffer
        if (write (STDOUT_FILENO, buffer, byte_offset) == -1)
            errorMess (errno);
    } //while

}

void errorMess(int err) {


    cerr << "ERROR " << strerror (errno) << endl;
    exit (EXIT_FAILURE); // exit program
}


void writeLines(int fd, int line_offset) {
    int lineCount = 0;
//    string s[BUFF_SIZE];
    string content;
    // size of data buffer
    char buffer[BUFF_SIZE];    // data buffer

    int n = 0;
    int i = 0;// number of bytes read


    /*otherwise buffer has biazaaar characters*/
    for (int j = 0; j < BUFF_SIZE; ++j) {
        buffer[j] = '\0';
    }

    while ((n = read (fd, buffer, BUFF_SIZE)) > 0) {
        buffer[n] = '\0';
        content += buffer;

    }

    for (i = content.size () - 1; i >= 0; --i) {
        char letter = content[i];

        if (letter == '\n') {
            lineCount++;
        }
        if (line_offset < lineCount)
            break;
    }
    /* i points to 'n' character and we want everything after that.*/
    content = content.substr (i + 1, content.size ());
//    string fn = filename;
    char storage[BUFF_SIZE];
    int z = 0;
    for (char &s: content) {
        storage[z] = s;
        z++;
    }

    /* Only way to match shell was to used unbuffered i/o */
    if (write (STDOUT_FILENO, storage, content.size ()) == -1)
        errorMess (errno);

}

void stdInOut(int fd) {
    int n;
    char buffer[BUFF_SIZE];
    string content;


    while ((n = read (STDIN_FILENO, buffer, BUFF_SIZE)) > 0) { // read the entire buffer
        content += buffer;
    }

    char storage[BUFF_SIZE];
    int z = 0;
    for (char &s: content) {

        storage[z] = s;
        z++;
    }

    /* Only way to match shell was to used unbuffered i/o */
    if (write (STDOUT_FILENO, storage, n) == -1)
        errorMess (errno);
}