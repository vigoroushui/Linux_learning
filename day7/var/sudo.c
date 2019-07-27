#include <func.h>

int main(void)  
{  
    const char *file = "/root/rootfile.txt";  

    printf ("The real user ID is: %d\n", getuid());  
    printf ("The effective user ID is :%d\n", geteuid());  

    if (!unlink (file))   
        printf ("I am root, and I can delete the file which in /root directory.\n");  
    else  
        perror ("unlink error");  

    return (0);  

}
