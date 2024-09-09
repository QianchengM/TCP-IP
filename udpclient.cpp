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


void dg_cli(FILE *, int, const SA *, socklen_t);

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        printf("No IPaddress\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error.\n");
        exit(1);
    }

    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    exit(0);
}

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    char instruction[2][10] = {
        "download\n",
        "upload\n"
    };
    int i=0;
    int op =0;
    FILE *CopyToLocal;
    FILE *Uploadfp;

    while (fgets(sendline, MAXLINE,fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = 0; 
        op = 0;
        for(i = 1;i<=2;i++)
        if(strcmp(sendline,instruction[i-1])== 0) {
            op = i;
            break;
        }   
        printf("op=%d\n",op);

        switch(op) {
            case 1: {
                fputs(recvline, stdout);
                fgets(sendline, MAXLINE,fp);    
                sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
                memset(recvline,0,sizeof(recvline));
                n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
                recvline[n] = 0; 
                char path[MAXLINE] = "/home/songjiahang/下载/";
                strcat(path,sendline);
                fputs(path,stdout);
                printf("\n");
                CopyToLocal = fopen(path,"w");
                if(!CopyToLocal)
                printf("download file failed!");
                fputs(recvline, CopyToLocal);
                fclose(CopyToLocal);
                break;
            }

            case 2: {
                memset(sendline,0,sizeof(sendline));
                setbuf(fp,NULL);
                
                fputs(recvline, stdout);      

                //fgets(sendline, MAXLINE,stdin);
                scanf("%s",sendline);
                for(int k=0;k<=MAXLINE;k++) {                
                    printf("%c",sendline[k]);
                    //break;
                }

                sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
                memset(recvline,0,sizeof(recvline));
                
                char path[MAXLINE]="";
                printf("Please input the path\n");
                scanf("%s",path);               
                getchar();
                
                printf("\n^^^^^^%s^^^^^^^\n",path);
                
                for(int j=0;j<=n;j++) {
                    if(path[j] == '\n') path[j] = '\0';
                }
                //printf("\n");
                
                Uploadfp = fopen(path, "r");
                int i =0;
                char strlin[1000][MAXLINE];
                if (!Uploadfp) {
                    printf("upload file failed!\n");

                } else {
                    while (1) {
                        if (NULL==fgets(strlin[i], MAXLINE, Uploadfp))
                        break;//文件尾结束            
                        i++;
                    }

                    memset(sendline,0,sizeof(sendline));
                    
                    for (int n = 0; n < i; n++) {
                        strcat(sendline, strlin[n]);
                    }
                    sendto(sockfd, sendline, strlen(sendline+1), 0, pservaddr, servlen);
                }
               

                fclose(Uploadfp);
            }
            default: {
                fputs(recvline, stdout);
                break;
            }


        }
            printf("\n");
        for(int i=0;i<30;i++) {
            printf("^");
        }
        printf("\n");
    }

    exit(0);
}


