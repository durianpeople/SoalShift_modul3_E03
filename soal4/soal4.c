#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *thread1(void *arg)
{
    int gid = *((int *)arg);
    if (gid == 0)
    {
        system("ps -aux | head -n 11 | tail -n 10 > /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/SimpanProses1.txt");
    }
    else if (gid == 1)
    {
        system("ps -aux | head -n 11 | tail -n 10 > /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/SimpanProses2.txt");
    }
}

void *thread2(void *arg)
{
    int gid = *((int *)arg);
    if (gid == 0)
    {
        system("zip -j /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/KompresProses1.zip /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/SimpanProses1.txt");
        system("rm -f /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/SimpanProses1.txt");
    }
    else if (gid == 1)
    {
        system("zip -j /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/KompresProses2.zip /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/SimpanProses2.txt");
        system("rm -f /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/SimpanProses2.txt");
    }
}

void *thread3(void *arg)
{
    int gid = *((int *)arg);
    if (gid == 0)
    {
        system("unzip /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/KompresProses1.zip -d /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1/");
    }
    else if (gid == 1)
    {
        system("unzip /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/KompresProses2.zip -d /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2/");
    }
}

int main()
{
    pthread_t t1, t2;
    int gid0 = 0;
    int gid1 = 1;
    system("mkdir /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses1");
    system("mkdir /home/durianpeople/Documents/Notes/SISOP/REPO/soal4/FolderProses2");
    pthread_create(&t1, NULL, thread1, &gid0);
    pthread_create(&t2, NULL, thread1, &gid1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_create(&t1, NULL, thread2, &gid0);
    pthread_create(&t2, NULL, thread2, &gid1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
    pthread_create(&t1, NULL, thread3, &gid0);
    pthread_create(&t2, NULL, thread3, &gid1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}