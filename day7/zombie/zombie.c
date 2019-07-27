#include <func.h>

int main(int argc, char* argv[])
{
    if(!fork())
    {
        exit(10);
    }
    else
    {
        sleep(10);
    }
    return 0;
}

