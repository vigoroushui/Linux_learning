#include <stdio.h>

int main()
{
    printf("Hello world\n");
    FILE *fp;
    fp=fopen("noexist","rb+");
    if(NULL==fp)
    {
        perror("fopen");
  		return 0;
    }
    return 0;
}

