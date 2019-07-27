#include <func.h>

int main(int argc, char* argv[])
{
    execl("./add","add","3","4",NULL);
    printf("you can't see me\n");
    return 0;
}

