#include <func.h>

int main(int argc,char* argv[])
{
    printf("uid=%d,euid=%d\n",getuid(),seteuid(1001));
    while(1);
    return 0;
}

