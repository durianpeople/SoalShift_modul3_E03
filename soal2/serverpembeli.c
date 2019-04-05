#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main()
{
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
        int i;
        char b[] = "beli";
        char *berhasil = "transaksi berhasil";
        char *gagal = "transaksi gagal";

        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        if(*value  == 0){
        *value = 100;
        }

        printf("Stock Barang : %d\n", *value);


        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
        }
          
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
          
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
        }

        valread = read( new_socket , buffer, 1024);
        i = strcmp(buffer,b);
        if (*value > 0 && i == 0){

        *value = *value - 1;
        send(new_socket , berhasil , strlen(berhasil), 0 );
        printf("terjadi pengurangan stok barang sebanyak 1 menjadi = %d\n", *value);
        exit(0);
        } 
        else if (*value <= 0 && i == 0){
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , gagal , strlen(gagal), 0 );
        exit(0);
        }
        else {
            exit(0);
        }
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
        return 0;
}