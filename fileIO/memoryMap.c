/*************************************************************************
	> File Name: memoryMap.c
	> Author: henjies
	> Mail: heshengjie2011@163.com 
	> Created Time: Wed 13 May 2015 08:58:45 PM CST
 ************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* 内存映射文件的长度*/
#define FILELENGTH 80

int main()
{
	int fd = -1;
	char buf[] = "henjies he shengjie hunan yueyang!";
	char *ptr = NULL;
	struct stat st;

	/*打开文件mmap.txt，并清空文件，若文件不存在则创建它，权限为可读写执行*/
	fd = open("./mmap.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
	if(fd == -1)
		return -1;

	/*将文件相对于开始处偏移89个单位*/
	lseek(fd,FILELENGTH-1,SEEK_SET);
	write(fd,"a",1);

	/*把文件内容映射到内存中去，模式设置成共享模式*/
	ptr = (char*)mmap(NULL,FILELENGTH,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if((char*)-1 == ptr)
	{
		printf("\nmmap mmap.txt failed!\n");
		close(fd);

		return -1;
	}
	memcpy(ptr+16,buf,strlen(buf));

	munmap(ptr,FILELENGTH);

	/*获得创建文件的状态信息*/

	if (stat("./mmap.txt",&st) == -1)
	{
		printf("file state information failed!\n");
	}

	printf("文件的所有者ID：%d\n",(int)st.st_uid);
	printf("文件的大小为：%d\n",(int)st.st_size);
	printf("文件最后访问时间：%s\n",asctime(gmtime(&st.st_atime)));
	printf("文件最后修改时间：%s\n",asctime(gmtime(&st.st_mtime)));
	printf("文件最后状态改变时间：%s\n",asctime(gmtime(&st.st_ctime)));

	close(fd);

	return 0;
}
