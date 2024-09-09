#include        <stdio.h>
#include        <errno.h>
#include        <stdlib.h>
#include        <arpa/inet.h>
#include        <netinet/in.h>
#include        <string.h>
#include        <sys/socket.h>
#define MAXLINE                  4096
#define LISTENQ                  1024    
#define SERV_PORT                9877
#define SA      struct sockaddr
#include        <unistd.h>

static int sockfd;
void DownLoad(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg);
void UpLoad(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg);

int main(int argc, char ** argv) {

    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error.\n");
        exit(1);
    }


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                      
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);       
    servaddr.sin_port = htons(SERV_PORT);             

    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    char instruction[2][10] = { "download\n", "upload\n" };

    int op = 0;
    int i = 0, j = 0;
    int n = 0;
    char mesg[MAXLINE];
    socklen_t len = sizeof(cliaddr);
    while (1) {
        memset(mesg, 0, sizeof(mesg));
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (SA *) &cliaddr, &len);
        op = 0;

        for (i = 1; i <= 2; i++)
            if (strcmp(mesg, instruction[i - 1]) == 0) {
                op = i;
                break;
            }  

        printf("op = %d\n", op);
        switch (op) {
            case 1: {
            	fputs("!!!download!!!\n",stdout);
                DownLoad(sockfd, (SA *) &cliaddr, sizeof(cliaddr), mesg);
                break;
            }
            case 2: {
            	fputs("!!!upload!!!\n",stdout);
                UpLoad(sockfd, (SA *) &cliaddr, sizeof(cliaddr), mesg);
                break;
            }
            default: {
            	fputs("!!!echo!!!\n",stdout);
                sendto(sockfd, mesg, strlen(mesg) + 1, 0, (SA *) &cliaddr,sizeof(cliaddr);
                break;
            }
        }
    }
}




void DownLoad(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg) {
    char text[MAXLINE] = "Please input the file name:\n";
    char sendline[100 * MAXLINE];
    char down[] = "download\n";
    int n = 0;
    char pa[]="/home/songjiahang/桌面/";
    while (strcmp(mesg, down) == 0) {

        printf("start download\n");
        sendto(sockfd, text, strlen(text) + 1, 0, pcliaddr, clilen);
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &clilen);
    }
    FILE *Downloadfp;
    fputs(mesg, stdout);


    for (int j = 0; j <= n; j++) {
        if (mesg[j] == '\n')
            mesg[j] = '\0';
    }
    strcat(pa,mesg);
    Downloadfp = fopen(pa, "r");
    int i = 0;
    char strlin[1000][MAXLINE];
    if (!Downloadfp) {
        printf("Download file failed!\n");

    } else {
        while (1) {
            if (NULL == fgets(strlin[i], MAXLINE, Downloadfp))
                break;  //文件尾结束
            i++;    
        }
        memset(sendline, 0, sizeof(sendline));
        for (int n = 0; n < i; n++) {
            strcat(sendline, strlin[n]);
        }
        fputs(sendline, stdout);
        sendto(sockfd, sendline, strlen(sendline) + 1, 0, pcliaddr, clilen);
    }

    fclose(Downloadfp);

    printf("Download file success!\n");
}

void UpLoad(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg) {
    char text[MAXLINE] = "Please input the file name:\n";
    char recvline[100 * MAXLINE];
    char up[] = "upload\n";
    int n = 0;
    FILE *CopyToServer;
    while (strcmp(mesg, up) == 0) {
        printf("start upload\n");
        sendto(sockfd, text, strlen(text) + 1, 0, pcliaddr, clilen);
		mesg="";
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &clilen);
        
    }  

    char path[MAXLINE] = "";
    strcat(path, mesg);
    //printf("%s\n not luan",path);
    CopyToServer = fopen(path, "w");
    if (!CopyToServer)
        printf("Upload file failed!\n");
    else {
        recvfrom(sockfd, recvline, 100 * MAXLINE, 0, pcliaddr, &clilen);
        fputs(recvline, CopyToServer);
    }
    fclose(CopyToServer);
    printf("Upload file success!\n");
}

