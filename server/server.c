

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h" /* goi ham read write*/
#include "string.h"
#include "sys/types.h" /* kieu du lieu co ban trong he thong*/
#include "sys/socket.h" /* cac ham socket*/
#include "netinet/in.h" /* sockaddr_in*/
#include "arpa/inet.h" /* htonl, htons, ...*/
#include "pthread.h"

#define MAXLEN 1024
#define MAXUSER 10
#define ON 1
#define OFF 0

int count_user = 0;
struct User{
	char u_name[30];
	int conn;
	int id_user;
}user[MAXUSER];
void sendMessage(int connfd, char *mess_error) {
	write(connfd, mess_error, strlen(mess_error));
}
void addUser(int connfd, struct User *user, int i, char *username) {
	strcpy(user[i].u_name, username);
	user[i].conn = connfd;
	user[i].id_user = i;
}
int checkUser(char *username, struct User *user, int length_User) {
	int i = 0;
	if(length_User == 0) {
		return 1;
	} else {
		for(i = 0; i < length_User; i ++) {
			if(strcmp(username, user[i].u_name) == 0) {
				return 0;
				break;
			}
		}
	}
	return 1;
}

void sendListUser(int connfd, int len_structUser) {
	int i = 0;
	char item[30];
	write(connfd, &len_structUser, sizeof(len_structUser));
	while(i < len_structUser) {
		memset(item, 0, sizeof(item));
		sprintf(item, "Name: %s\n", user[i].u_name);
		item[strlen(item)] = '\0';
		sendMessage(connfd, item);
		i ++;
	}
}
// void sendAll(struct User *user, char *mess, int currentconn) {
// 	int j = 0;
// 	for(j = 0; j < count_user; j ++) {
// 		if(user[j].conn != currentconn)
// 			write(user[j].conn, mess, strlen(mess));
// 	}
// }
void *Sendmessage(void *arg) {
	pthread_detach(pthread_self());
	printf("hello %s!\n",user[0].u_name);
	return(NULL);
}
int main(int argc, char **argv)
 {
 	int listenfd; 
 	struct sockaddr_in servaddr;  
 	listenfd = socket(AF_INET, SOCK_STREAM, 0);
 	bzero(&servaddr, sizeof(servaddr));
 	socklen_t clilen;
 	// Thiet lap dia chi server
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 	servaddr.sin_port = htons(9846);
 	// Bind socket gan cong, dia chi IP
 	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
 		printf("Bindding Error");
 		exit(1);
 	}
 	if(listen(listenfd, 5)) {
 		printf("Listening Error");
        exit(1);
 	}
 	clilen = sizeof(servaddr);
 	int *iptr;
 	pthread_t tid;
 	printf("Connecting...\n");
 	for(;;) {
 		iptr = malloc(sizeof(int));
 		*iptr = accept(listenfd, (struct sockaddr *) &servaddr, &clilen);
 		printf("IP and Port: %s vs %d \n", inet_ntoa(servaddr.sin_addr), servaddr.sin_port);
 		if (*iptr >= 0 ) {
 			char username[30], err[5];
 			memset(err, 0, sizeof(err));
 			memset(username,0, sizeof(username));
 			do {
 				read(*iptr, username, sizeof(username));
 				printf("user: %s\n", username);
 				if(checkUser(username, user, count_user) == 0) {
					memset(err, 0, sizeof(err));
					sprintf(err, "0");
					// err[sizeof(err)-1] = '\0';
					sendMessage(*iptr, err);
				} else {
					memset(err, 0, sizeof(err));
					strcpy(err, "1");
					// err[sizeof(err)-1] = '\0';
					// printf("eror: %s\n", err);
					sendMessage(*iptr, err);
				}
			} while (checkUser(username, user, count_user) == 0);
			if(count_user <= MAXUSER) {
				addUser(*iptr, user, count_user, username);
				count_user ++;
				sendListUser(*iptr, count_user);
			}
			pthread_create(&tid, NULL, &Sendmessage, (void*)iptr);
		}	
 	}
 	return 0;
 }