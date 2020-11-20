#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <cstring>
#include <unistd.h>
 
#define CAN_SERVICE "CAN_SERVICE" 
int main(void)  
{ 
    int ret;  
    int socket_fd;   
    char snd_buf[1024];   
    static struct sockaddr_un srv_addr;  
 
// 创建socket 
    socket_fd=socket(PF_UNIX,SOCK_STREAM,0);  
    if(socket_fd<0)  
    {  
        perror("cannot create communication socket");  
        return 1;  
    }     
    srv_addr.sun_family=AF_UNIX;  
    strcpy(srv_addr.sun_path,CAN_SERVICE);  
 
// 连接到服务器  
    ret=connect(socket_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));  
    if(ret==-1)  
    {  
        perror("cannot connect to the server");  
        close(socket_fd);  
        return 1;  
    }  
    for (int i = 0;i<10;i++)
    {
        memset(snd_buf,0,1024);  
        
        strcpy(snd_buf,"message from client");  
        char s[10];
        snprintf(s, sizeof(s), "%d", i);
        strcat(snd_buf, s);
    
    // 读取和写入    
        write(socket_fd,snd_buf,sizeof(snd_buf));  
        sleep(1);
    }
//     memset(snd_buf,0,1024);  
//     strcpy(snd_buf,"message from client");  
 
// // 读取和写入    
//     write(socket_fd,snd_buf,sizeof(snd_buf));  
    close(socket_fd);  
    return 0;  
} 