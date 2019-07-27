#include <func.h>

int main(int argc, char* argv[])
{
    int fp=open("file",O_RDWR|O_CREAT);
    ERROR_CHECK(fp,-1,"open");
    write(fp,"hello",5);
    return 0;
}

