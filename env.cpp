
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

/*!
 *
 * @param argc represnts number of arguments in argv.
 * @param argv represents input arguments.
 * @param envp represents environment list.
 * @return 0, for success.
 */
int main(const int argc, const char * argv [], const char * envp [])
{

    for (const char **env = envp; *env != NULL; env++)
    {
//        printf("%s\n", *env);
        cout << *env << endl;
    }
return EXIT_SUCCESS;
}
