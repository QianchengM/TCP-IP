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



void DownLoad(int sockfd, char *mesg);
void UpLoad(int sockfd, char *mesg);


int main(int argc, char **argv)
{
    int  listenfd, connfd;  
    pid_t  childpid;        
    socklen_t  clilen;
    
    struct sockaddr_in  cliaddr,servaddr;    

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error.\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind error.\n");
        exit(1);
    }

    if (listen(listenfd,LISTENQ) < 0) {   
        printf("listen error.\n");
        exit(1);
    }

    char mesg[MAXLINE];
    int i;
    int op;
    ssize_t size = 0;
    char instruction[2][10] = {"download\n", "upload\n" };

    for( ; ;) {

        if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
            printf("accept error.\n");
            exit(1);
        }

        if((childpid = fork()) == 0) {
            close(listenfd) ; 
            for(;;){
            size = 0;
            op = 0;
            memset(mesg,0,strlen(mesg));
            size = read(connfd,mesg,MAXLINE);
            if(size == 0)
                return 0;
            for (i = 1; i <= 2; i++)
                if (strcmp(mesg, instruction[i - 1]) == 0) {
                    op = i;
                    break;
                }  
            printf("op = %d\n", op);
            fputs(mesg,stdout);
            switch (op) {
                case 1: {
                    fputs("!!!download!!!\n",stdout);
                    DownLoad(connfd, mesg);
                    break;
                }
                case 2: {
                    fputs("!!!upload!!!\n",stdout);
                    UpLoad(connfd, mesg);
                    break;
                }
                default: {
                    fputs("!!!echo!!!\n",stdout);
                    write(connfd,mesg,strlen(mesg+1));
                    break;
                }
            }
        }

            exit(0);
        }
    close(connfd);        
    }
    return 0;
}


void DownLoad(int sockfd,char *mesg) {
    char text[MAXLINE] = "Please input the file name: \n";
    char sendline[100 * MAXLINE];
    char down[] = "download\n";
    int n = 0;
    int i = 0;
    char pa[]="/home/songjiahang/桌面/";
    char named[MAXLINE];
    printf("\n**&**%s**&**\n",mesg);
    while (strcmp(mesg, down) == 0) {

        printf("start download\n");
        write(sockfd, text, strlen(text) + 1);
        mesg="";
        n = read(sockfd, named, MAXLINE);
}
    printf("\n**&&**%s**&&**\n",mesg);
    FILE *Downloadfp;
  //  fputs(mesg, stdout);
    printf("\n**&&&**%s**&&&**\n",named);

  /*  for (int j = 0; j <= n; j++) {
        if (mesg[j] == '\n')
            mesg[j] = '\0';
    }*/
    strcat(pa,named);
    printf("\n*****%s*****\n",pa);
    Downloadfp = fopen(pa, "r");
    
    char strlin[1000][MAXLINE];
    if (!Downloadfp) {
        printf("Download file failed!\n");

    } else {
        while (1) {
  
            if (NULL == fgets(strlin[i], MAXLINE, Downloadfp))
                break;  //文件尾结束

            i++;  
        }
        memset(sendline, 0, strlen(sendline));
        for (int n = 0; n < i; n++) {
            strcat(sendline, strlin[n]);
        }

        fputs(sendline, stdout);
        write(sockfd, sendline, strlen(sendline) + 1);
    }
    fclose(Downloadfp);

    printf("Download file success!\n");
}
void UpLoad(int sockfd,char *mesg) {
    char text[MAXLINE] = "Please input the file name: \n";
    char recvline[100 * MAXLINE];
    char nameu[MAXLINE];
    char up[] = "upload\n";

    int n = 0;
    FILE *CopyToServer;

    while (strcmp(mesg, up) == 0) {
        printf("start upload\n");
        write(sockfd, text, strlen(text) + 1);
        mesg="";
        
        n = read(sockfd, nameu, MAXLINE);
         printf("\n%s\n not luan\n",nameu);
    }   

    char pathu[MAXLINE] ="";
    strcat(pathu, nameu);
   // printf("\n%s\n not luan\n",pathu);
    CopyToServer = fopen(pathu, "w");
    if (!CopyToServer)
        printf("Upload file failed!\n");
    else {
        read(sockfd, recvline, 100 * MAXLINE);
        fputs(recvline, CopyToServer);
    }

    fclose(CopyToServer);
    printf("Upload file success!\n");
}


