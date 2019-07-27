#include <stdio.h>

int main()
{
    FILE *fp;
    printf("Hello world\n");
    fp=fopen("noexist","rb+");
    if(NULL==fp)
    {
        perror("fopen");
        return -1;
    }
    printf("I am main1\n");
    return 0;
}

