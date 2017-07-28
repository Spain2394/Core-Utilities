//123456789010


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


using std::cout;
using std::endl;
using std::string;

/* Set buffer size to 4096 bytes */
const int BUFF_SIZE = 4096;


/*function prototypes*/
void writeLines(int fd, int line_offset);

void writeBytes(int fd, int byte_offset);

void errorMess(int err);

void stdInOut();


/*!
 * Simple test function to mimic cat utility.
 * @param argc const int which represents number of parameters entered.
 * @param argv const pointer to char array, represents number of input arguments.
 * @return int 0, if program exits succesfully.
 */
int main(const int argc, char *argv[]) {


// execute -c|-n 10 filename;
    struct stat buf;
    char *filename;

    int offset;// number bytes or lines specified



    /*No arguments*/
    /*Should not be the case in linux*/
    /*files not specified*/
    if (argc != 4) {
        errorMess (errno);
    }
    /*check to make sure regular file*/



    if (stat (argv[3], &buf) == -1) {
        errorMess (errno);
    }


    if (S_ISDIR (buf.st_mode) == 1) /*dir, do not do anything file*/
        return 0; /* jus dont do anything if it is a dir*/

    else if (S_ISREG(buf.st_mode) == 0) /*not regular file*/
        errorMess (errno);



    /* file arguments specified */
    filename = argv[3];// argument 1 is a name of file

    int fd = open (filename, O_RDONLY);/* open file*/
    if (fd == -1) errorMess (errno);

    offset = atoi (argv[2]);
    if (strcmp (argv[1], "-n") == 0) {
        writeLines (fd, offset);
    } else if (strcmp (argv[1], "-c") == 0) {
        writeBytes (fd, offset);
    }
        /* cannot open file */
    else {
        errorMess (errno);
    }

    return EXIT_SUCCESS;
} // main

/*If the input argument is an existing filename*/
/*!
 *Read file to buffer length, write to standard output.
 * @param fd int represents file descriptor, fd is -1 if
 * there was an error opening file.
 */
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
        else break; // break after reading the first c bytes.
    } //


    if (close (fd) == -1)
        errorMess (errno);
}


/*!
 * Function to print error message, and end program.
 * @param missive const char pointer, represnting a string
 * that will precede error message.
 */
void errorMess(int err) {

    perror (strerror (err));
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


    while ((n = read (fd, buffer, BUFF_SIZE)) > 0) { // read the entire buffer
        content += buffer;

    }

    for (i = content.size () - 1; i > 0 && (lineCount <= line_offset); --i) {
        int line_len;
        char letter = content[i];

        if (letter == '\n') {
            lineCount++;
        }
    }

    content = content.substr (i, content.size ());

    cout << content;

    if (close (fd) == -1)
        errorMess (errno);
}
