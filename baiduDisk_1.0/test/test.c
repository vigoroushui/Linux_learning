#include "func.h"

int main(int argc,char* argv[])
{   
    char *pwd=getcwd(NULL,0);
    puts(pwd);
    return 0;
}

