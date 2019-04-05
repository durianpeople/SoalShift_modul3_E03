#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 4000

pthread_t tid[2];
int *value;
int new_socket, valread;
char buffer[1024] = {0};
char b[] = "tambah";

int length=5; //inisialisasi jumlah untuk looping
void* playandcount(void *arg)
{
    unsigned long i=0;
    pthread_t id=pthread_self();
    int iter;
    if(pthread_equal(id,tid[0])) //thread untuk menjalankan counter
    {
        while(1){
            printf("Stock Barang = %d\n",*value);
            sleep(5);
        }
    }
    else if(pthread_equal(id,tid[1])) // thread menampilkan gambar
    {
        valread = read( new_socket , buffer, 1024);
        i = strcmp(buffer,b);
        if (i == 0){

        *value = *value + 1;
        printf("terjadi penambahan stok barang sebanyak 1 menjadi = %d\n", *value);
        exit(0);
        } 
        else {
            exit(0);
        }
    }

    return NULL;
}


int main()
{		
		int server_fd;
    	struct sockaddr_in address;
    	int opt = 1;
    	int addrlen = sizeof(address);

        key_t key = 1234;
        

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        printf("Stock Barang 1: %d\n", *value);
        

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

        int i=0;
        int err;
        while(i<2) // loop sejumlah thread
        {
            err=pthread_create(&(tid[i]),NULL,&playandcount,NULL); //membuat thread
            if(err!=0) //cek error
            {
                printf("\n can't create thread : [%s]",strerror(err));
            }
            else
            {
                printf("\n create thread success\n");
            }
            i++;
        }
        pthread_join(tid[0],NULL);
        pthread_join(tid[1],NULL);
        exit(0);

        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
    	return 0;
}