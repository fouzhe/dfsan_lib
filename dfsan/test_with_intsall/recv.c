#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <Polar/Polar_label.h>

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    char buf[1024]={0};
    char send_msg[] = "hello, send by send() :\n";
    send(s, send_msg, strlen(send_msg), 0);
    recv(s, buf, 1024, 0);
    printf("recv from recv() : %s\n", buf);
}