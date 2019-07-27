#include <func.h>
void tackle(char *p,unsigned int privi){
	char mark[3]={'r','w','x'};
	int k=2;
	for(int i=8;i>=0;i--){
		if(privi&1==1){
			p[i]=mark[k];
		}else{
			p[i]='-';
		}
		privi=privi>>1;
		k--;
		if(k<0){
			k=2;
		}
	}
}
int main(int argc,char *argv[]){
	ARGS_CHECK(argc,2);
	char power[10]={0};
	DIR *dir=opendir(argv[1]);
	struct dirent *p;
	int ret;
	while(p=readdir(dir)){
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")){
			continue;
		}
		char path[1024]={0};
		char is_directory;
		sprintf(path,"%s",p->d_name);
		if(p->d_type==4){
			is_directory='d';
		}else{
			is_directory='-';
		}
		struct stat buf;
		ret=stat(path,&buf);
		ERROR_CHECK(ret,-1,"stat");
		tackle(power,buf.st_mode);
		printf("%c",is_directory);
		printf("%s",power);
		printf(" %ld %s %s %ld %s %s\n",buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime),path);
	}
	ret=closedir(dir);
	ERROR_CHECK(ret,-1,"closedir");
	return 0;
}
