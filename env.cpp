//
// Created by Allen Spain on 7/29/17.
//

#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

int main(const int argc, const char * argv [], const char * envp [])
{

    for (const char **env = envp; *env != NULL; env++)
    {
//        printf("%s\n", *env);
        cout << *env << endl;
    }
return EXIT_SUCCESS;
}
