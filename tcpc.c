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

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc!= 2) {
    printf("No IPaddress");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error.\n");
        exit(1);
    }
    
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    


    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect error.\n");
        exit(1);
    }
    
    
    int n;
    int i=0;
    int op =0;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    char instruction[2][10] = {
        "download\n",
        "upload\n"
    };

    FILE *CopyToLocal;
    FILE *Uploadfp;

    while (fgets(sendline, MAXLINE,stdin) != NULL) {
        write(sockfd, sendline, MAXLINE);
        n = read(sockfd, recvline, MAXLINE);
        recvline[n] = 0; 
        op = 0;
        for(i = 1;i <= 2;i ++)
        if(strcmp(sendline,instruction[i-1])== 0) {
            op = i;
            break;
        }   
        printf("op = %d\n",op);

        switch(op) {
            case 1: {
                fputs(recvline, stdout);
                fgets(sendline, MAXLINE,stdin);
               // printf("\n&&&&%d&&&&&\n",n);
               // for(int j=0;j<=n;j++)
                
               // {
               // if(sendline[j]='\n'){
               //    sendline[j]='\0';
               //    break;
               // }
               // }
                sendline[strlen(sendline)-1]=0;
                printf("\n&&&&%s&&&&\n",sendline);        
                write(sockfd, sendline, strlen(sendline));
                memset(recvline,0,sizeof(recvline));
                n = read(sockfd, recvline, MAXLINE);
                recvline[n] = 0; 
                char pathd[MAXLINE] = "/home/songjiahang/下载/";
                strcat(pathd,sendline);
                fputs(pathd,stdout);
                printf("\n");
                CopyToLocal = fopen(pathd,"w");
                if(!CopyToLocal)
                printf("download file failed!");
                fputs(recvline, CopyToLocal);
                fclose(CopyToLocal);
                break;
            }

            case 2: {

                memset(sendline,0,sizeof(sendline));
                setbuf(stdin,NULL);
                
                fputs(recvline, stdout);      

                //fgets(sendline, MAXLINE,stdin);
                scanf("%s",sendline);
               // for(int k=0;k<=MAXLINE;k++) {                
                 //   printf("%c",sendline[k]);
                    //break;
               // }
                printf("\n&&&&&&&&&&%s&&&&&&&&&\n",sendline);
                write(sockfd, sendline, strlen(sendline));
                memset(recvline,0,sizeof(recvline));
                
                char pathu[MAXLINE]="";
                printf("Please input the path\n");
                scanf("%s",pathu);               
                getchar();
                
                printf("\n^^^^^^%s^^^^^^^\n",pathu);
                
                for(int j=0;j<=n;j++) {
                    if(pathu[j] == '\n') pathu[j] = '\0';
                }
                //printf("\n");
                
                Uploadfp = fopen(pathu, "r");
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
                    write(sockfd, sendline, strlen(sendline)+1);
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

