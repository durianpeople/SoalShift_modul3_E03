#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <pthread.h>
#include <string.h>

int WakeUp_Status;
int Spirit_Status;
pthread_t tid[3];
int choice;
int flag_agmal;
int flag_iraj;
int flag = 0;
int status_agmal;
int status_iraj;

void status(){
	printf("Agmal WakeUp_Status = %d\n", WakeUp_Status);
	printf("Iraj Spirit_Status = %d\n", Spirit_Status);
}

void agmal(){
	WakeUp_Status = WakeUp_Status + 15;
}

void iraj(){
	Spirit_Status = Spirit_Status - 20;
}


void* playandcount(void *arg)
{
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	if(pthread_equal(id,tid[0])) //thread status
	{
		while(1){
			while(flag != 1){

			}
			status();
			flag = 0;
		}
	}
	else if(pthread_equal(id,tid[1])) // thread agmal
	{
		while(1){
			while(flag != 2){

			}
			if (flag == 2){
				if(flag_iraj >= 3){
					flag_iraj = 0;
					flag = 0;
					status_agmal = 1;
					sleep(10);
					status_agmal = 0;
					continue;
				} else {
					agmal();
					if (WakeUp_Status >= 100){
						printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
						exit(0);
					} else {
						flag_agmal++;
						flag = 0;
					}
				}
			}
			if (flag_agmal >= 3){
				flag = 3;
			}
		}
	}
	else if(pthread_equal(id,tid[2])) // thread iraj
	{
		while(1){
			while(flag != 3){

			}
			if (flag == 3){
				if (flag_agmal >= 3){
					flag_agmal = 0;
					flag = 0;
					status_iraj = 1;
					sleep(10);
					status_iraj = 0;
					continue;
				} else {
					iraj();
					if (Spirit_Status <= 0){
						printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
						exit(0);
					}else{
						flag_iraj++;
						flag = 0;
					}
				}
			}
			if (flag_iraj >= 3){
				flag = 2;
			}
		}
	}
	return NULL;
}


int main(){
	WakeUp_Status = 0;
	Spirit_Status = 100;

	int i = 0;
	int err;
	while(i<3) // loop sejumlah thread
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

	while(1){
		while (flag != 0){}
		printf("Pilih salah 1 menu dibawah!\n");
		printf("1. All Status\n");
		printf("2. Agmal Ayo Bangun\n");
		printf("3. Iraj Ayo Tidur\n");
		scanf("%d", &choice);
		if(choice == 1){
			flag = 1;
		} else if(choice == 2) {
			if(status_agmal == 0){
				flag = 2;
			}
			else{ //ketika iraj dijalankan 3x
				printf("Agmal Ayo Bangun disabled 10 s\n");
				flag = 0;
			}
		} else {
			if(status_iraj == 0){
				flag = 3;
			}
			else{ //ketika agmal dijalankan 3x
				printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
				flag = 0;
			}
		}
	}
	return 0;
}