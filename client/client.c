
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h" /* goi ham read write*/
#include "string.h"
#include "sys/types.h" /* kieu du lieu co ban trong he thong*/
#include "sys/socket.h" /* cac ham socket*/
#include "netinet/in.h" /* sockaddr_in*/
#include "arpa/inet.h" /* htonl, htons, ...*/
#include "pthread.h"
// #include<errno.h>
#define MAXLEN 1024
// static void *recvMessage(void * arg) {
// 	int connfd;
// 	connfd =  * ( ( int *) arg);
// 	pthread_detach(pthread_self());
// 	char recvBuff[1024];
// 	int n = 0;
// 	memset(recvBuff,0,sizeof(recvBuff));
// 	while((n = read(connfd,recvBuff,sizeof(recvBuff)-1) )> 0){
// 		if(!strstr(recvBuff,"@")){
// 			printf("%s\n", recvBuff);
// 		}else{
// 			break;
// 		}
// 		memset(recvBuff, 0, sizeof(recvBuff));
// 	}
// 	return NULL;
// }
// static void *sendMessage(void *arg) {
// 	int listenfd;
// 	listenfd =  * ( ( int *) arg);
// 	pthread_detach(pthread_self());
// 	fflush(stdin);
// 	char mess[255];

// 	while(1) {
// 		memset(mess, 0, sizeof(mess));
// 		scanf("%s", mess);
// 		write(listenfd, mess, strlen(mess));
// 		if(strcmp(mess, "@") == 0) {
// 			break;
// 		}
// 	}
// 	return NULL;
// }
void recvListUser(int listenfd) {
	char item[30], c;
	int len_structUser, i, j = 0;
	read(listenfd, &len_structUser, sizeof(len_structUser));
	printf("User online: %d\n", len_structUser);
	while (j < len_structUser) {
		memset(item, 0, sizeof(item));
		//Readline
		for(i = 0; i < sizeof(item); i ++) {
			if(read(listenfd, &c, 1) == 1) {
				item[i] = c;
				if(item[i] == '\n') {
					break;
				}
			}
		}
		printf("%s", item);
		j ++;

	}
}
int main(int argc, char ** argv)
 {
 	int *listenfd;
 	listenfd = malloc(sizeof(int));
 	if(argc < 2) {
 		printf("You need import IP host\n");
 		exit(1);
 	}
 	struct sockaddr_in servaddr;  /* Cau truc chua thong tin dia chi server ma client can ket noi*/
 	*listenfd = socket(AF_INET, SOCK_STREAM, 0);
 	// Thiet lap dia chi server
 	bzero(&servaddr, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_port = htons(9846);
 	// thiet lap dia chi ip ket noi den server
 	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	connect(*listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 	char username[30], err[5];
 	// int len_listUser;
 	// memset(listUser, 0, sizeof(listUser));
 	do {
 		memset(err, 0, sizeof(err));
 		printf("Import user login: ");
    	scanf("%s", username);
    	// username[sizeof(username) - 1] = '\0';
		write(*listenfd, username, strlen(username));
		read(*listenfd, err, 1);
		if(strcmp(err, "1") != 0) {
			printf("Duplicate name other user!\n");
		}
 	} while (strcmp(err, "1") != 0);
	recvListUser(*listenfd);
	printf("Chating ...\n");
	// pthread_t tid1, tid2;
	// printf("Enter your messasge (@ to finish) :\n");
	// pthread_create(&tid1, NULL, &recvMessage, (void*)listenfd);	
	// pthread_create(&tid2, NULL, &sendMessage, (void*)listenfd);
	// // pthread_join(tid,NULL);
	// pthread_join(tid2,NULL);
	// pthread_join(tid1,NULL);

	// close (*listenfd);
 	return 0;
 }
