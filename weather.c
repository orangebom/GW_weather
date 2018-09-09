#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<fcntl.h>


#define IPSTR "117.25.149.239" //服务器IP地址;
#define PORT 80
#define BUFSIZE 1024

//定义flags:只写，文件不存在那么就创建，文件长度戳为0
#define FLAGS O_WRONLY | O_CREAT | O_TRUNC
//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行
#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH


#define WEATHER_DATA_FEIL "weather_data.txt"

int main(int argc, char **argv)
{
		int fd;

        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char str1[4096], str2[4096], buf[BUFSIZE], *str;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;
		int get_num;
         
         //创建套接字
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("创建网络连接失败,本线程即将终止---socket error!\n");
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("连接到服务器失败,connect error!\n");
                exit(0);
        }
        printf("与远端建立了连接\n");
        memset(str2, 0, 4096);
        strcat(str2, "theDataToPost");
        str=(char *)malloc(128);
        len = strlen(str2);
        sprintf(str, "%d", len);


		
        memset(str1, 0, 4096);
        strcat(str1, "GET /hangzhou/ HTTP/1.1\n");
        strcat(str1, "Host: m.15tianqi.com\n");
        strcat(str1, "Content-Type: text/html\n");

        strcat(str1, "\r\n\r\n");
        printf("%s\n",str1);
 
        ret = write(sockfd,str1,strlen(str1));
        if (ret < 0) {
                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                exit(0);
        }else{
                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
        }

		//打开文件
		 if ((fd = open(WEATHER_DATA_FEIL, FLAGS, MODE)) == -1) {
		    printf("open file error");
		    return 0;
		  }
		 
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);
 
        while(1){
                sleep(2);
                tv.tv_sec= 0;
                tv.tv_usec= 0;
                h= 0;
                printf("--------------->1");
                h= select(sockfd +1, &t_set1, NULL, NULL, &tv);
                printf("--------------->2");
 
                //if (h == 0) continue;
                if (h < 0) {
                        close(sockfd);
                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
                        return -1;
                };
 
                if (h > 0){

						
				
                        memset(buf, 0, 4096);
                        i= read(sockfd, buf, 4095);
                        if (i==0){
                                close(sockfd);
                                printf("读取数据报文时发现远端关闭，该线程终止！\n");
                                return -1;
                        }
						
						get_num++;
						//天气信息写入文件
						/*
						if(ret = write(fd, buf, strlen(buf)) < 0){
							printf("write error\n");
						}
						*/
						
						if(get_num  < 3)
						{
							//天气信息写入文件
							if(ret = write(fd, buf, strlen(buf)) < 0){
								printf("write error\n");
							}
							break;
						}
						
						printf("\n\nstrlen:%d,num:%d\n\n",strlen(buf),get_num);
						if(strlen(buf)<4095){
							break;
						}
						
                        printf("%s\n", buf);

						if(strlen(buf)<4095)
							break;
                }
        }
        close(sockfd);
 		close(fd);
 
        return 0;
}
