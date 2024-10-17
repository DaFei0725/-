#include <stdio.h> // 标准输入输出库，用于打印消息
#include <stdlib.h>// 标准库，用于调用exit()函数，用于退出
#include <string.h>// 字符串的操作库
#include <sys/socket.h>//socket套接字在这里
#include <netinet/in.h>//IP地址和端口号的关键字在这里
#include <unistd.h>// 提供read(), write(), close()等函数（最后用）

#define PORT 8080//服务器开放的端口号
#define BUFFER_SIZE 1024 //缓存大小

int main() {
    int server_fd, new_socket;//定义套接字文件间描述符
    struct sockaddr_in address; //包含服务器信息
    int opt = 1;        // 用于端口复用
    char buffer[BUFFER_SIZE] = {0};//缓冲区并初始化
    socklen_t addrlen = sizeof(address);//获取地址结构体大小

    //创建服务器这边的套接字描述符
    server_fd = socket(AF_INET, SOCK_STREAM, 0);//SOCK_STREAM指此为面向连接的可靠的流式套接字
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR, &opt,sizeof(opt)) < 0){
        //setsockopt:用于设置套接字选项 SOL_SOCKET指接下来的选项针对套接字本身 SO_RUSEADDR:允许套接字绑定在已被使用的端口：允许端口复用
        //&opt,sizeo(opt))这一串指&opt指向数据的大小 <0会失败 所以会启用端口复用
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    //关联套接字和端口号ad
    address.sin_family = AF_INET; //指定地址族IPV4
    address.sin_addr.s_addr = INADDR_ANY;//指定具体地址信息，即任意可用的地址
    address.sin_port = htons(PORT);//指定端口号，并转化为网络字节序
    
    //关联套接字
    if (bind(server_fd, (struct sockaddr *)&address , sizeof(address)) < 0) //默认绑定失败返回-1，故<0
{
    perror("bind failed");
    exit(EXIT_FAILURE);
}
    //监听端口
    if (listen(server_fd,3) <0) { //也是检查报错
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n",PORT);


    //接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen))<0){ //bind和accept都调用了sizeof函数 用于测量地址大小 方便存储。
        perror("accept");      //↑ 这个指针指向address地址，被转换为sockaddr指针，bind函数需要
        exit(EXIT_FAILURE);
    }
    //读取数据(回声服务器部分)
    read(new_socket,buffer,BUFFER_SIZE);//最终读取BUFFER_SIZE字节的数据
    printf("Message from client %s\n",buffer);

    //发送数据（回声）
    send(new_socket, buffer, strlen(buffer), 0);//strlen也是个计算地址长度的函数 0代表send无异常
    printf("Echoed back client. Finish :)\n");

    //结束关键字
    close(new_socket);//关闭与客户端连接（套接字）
    close(server_fd);//关闭监听套接字
    
    return 0;
}





    
