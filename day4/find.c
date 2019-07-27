#include <func.h>

int main(int argc,char *argv[]){
	ARGS_CHECK(argc,2);
	DIR *dir=opendir(argv[1]);
	struct dirent *p;
	char search_aim[1024]={0};
	printf("please input a file name:");
	scanf("%s",search_aim);
	while(p=readdir(dir)){
		if(strcmp(p->d_name,search_aim)==0){
			printf("%s%s%s\n",getcwd(NULL,0),"/",search_aim);
			return 0;
		}
	}
	printf("this file is not existed!\n");
}
