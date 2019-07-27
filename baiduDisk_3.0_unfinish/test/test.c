#include "func.h"

void getRandomSalt(char* salt,const int len)
{   
    int i;
    srand(time(NULL));
    for(i=0;i<len;i++)
    {   
        if(0==i%3)
        {
            salt[i]=rand()%26+'a';
        }
        else if(1==i%3)
        {
            salt[i]=rand()%26+'A';
        }
        else{
            salt[i]=rand()%26+'0';
        }
    }   
    salt[i]='\0';
}

int main(int argc,char* argv[])
{   
    char salt[16]={0};
    getRandomSalt(salt,8);
    puts(salt);
    return 0;
}

