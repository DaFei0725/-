#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> // 提供read(), write(), close()等函数
#include <arpa/inet.h>

#define PORT 8080 // 服务器接收的端口号
#define BUFFER_SIZE 1024 // 定义缓冲区大小

int main() {
    int client_fd; // 定义客户端套接字文件描述符
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in serv_address; 
    
    // 设置地址结构体
    serv_address.sin_family = AF_INET; 
    serv_address.sin_port = htons(PORT);//注意到这边比客户端少了一项 因为服务端需要监听自己所有的ip地址 客户端要绑定IP则不用
    inet_pton(AF_INET, "127.0.0.1", &(serv_address.sin_addr.s_addr));//最重要的一项，指定连接的服务器ip并转化为网络字节序（这里是服务器回环 指定了本机的ip地址）
    
    //处理无效地址
    if (inet_pton <=0)
    {
        printf("\n Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    // 创建客户端这边的套接字文件描述符
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }

    // 连接至服务器
    int ret = connect(client_fd, (struct sockaddr *)&serv_address, sizeof(serv_address));//这边同理服务端 只是定义的是ret 调用的是connect
    if (ret != 0){
	perror("connect error"); // 如果连接失败，打印错误信息
        exit(EXIT_FAILURE); 
    }

    // 向服务器发送数据
    write(client_fd, "MaMaShengDe", 11); // 发送消息 指定几个字符
    read(client_fd, buffer, sizeof(buffer)); // 读取数据
    printf("Message from server: %s\n", buffer); // 打印服务器返回的消息

    close(client_fd); // 关闭与服务器的连接

    return 0; // 程序正常退出
}
