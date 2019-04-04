# SoalShift_modul3_E03

## SOAL 1

Mendeklarasikan variabel global `pthread_t threads[500]` untuk menampung ID thread, `int list[500]` untuk menampung daftar angka faktorial yang akan dihitung, dan `int ambil = -1` untuk menandakan angka indeks keberapa dari `list[]` yang telah diambil thread untuk dihitung.

Pada `int main()`: 

Mengambil angka-angka pada argumen program

```c
for (int i = 1; i < argc; i++)
{
    list[i - 1] = atoi(argv[i]);
}
```

Sort angka-angka tersebut, kemudian menjalankan thread sesuai jumlah argumen

```c
sort(list, argc - 1);
for (int i = 1; i < argc; i++)
{
    int err = pthread_create(&(threads[i - 1]), NULL, &thread, NULL);
}
```

Program utama menunggu thread lain selesai

```c
for (int i = 1; i < argc; i++)
{
    pthread_join(threads[i - 1], NULL);
}
```

Thread mengambil salah satu angka dari `list[]` kemudian menghitung faktorialnya. Output akan di-delay sesuai *ambil* detik agar output terurut.

```c
void *thread(void *arg)
{
    int self_ambil = ++ambil;
    int hasil = faktorial(list[ambil]);
    sleep(self_ambil);
    printf("%d\n", hasil);
}
```

## SOAL 4

`int gid` digunakan untuk memisahkan peran thread.

Thread pertama mengambil 10 proses pertama kemudian disimpan dalam file teks sesuai nama file yang telah ditentukan

```c
void *thread1(void *arg)
{
    int gid = *((int *)arg);
    if (gid == 0)
    {
        system("ps -aux | head -n 11 | tail -n 10 > /home/durianpeople/Documents/FolderProses1/SimpanProses1.txt");
    }
    else if (gid == 1)
    {
        system("ps -aux | head -n 11 | tail -n 10 > /home/durianpeople/Documents/FolderProses2/SimpanProses2.txt");
    }
}
```

Thread kedua melakukan kompresi terhadap file yang dibuat di thread pertama

```c
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
        system("zip -j /home/durianpeople/Documents/FolderProses2/KompresProses2.zip /home/durianpeople/Documents/FolderProses2/SimpanProses2.txt");
        system("rm -f /home/durianpeople/Documents/FolderProses2/SimpanProses2.txt");
    }
}
```

Thread ketiga melakukan unzip file

```c
void *thread3(void *arg)
{
    int gid = *((int *)arg);
    if (gid == 0)
    {
        system("unzip /home/durianpeople/Documents/FolderProses1/KompresProses1.zip -d /home/durianpeople/Documents/FolderProses1/");
    }
    else if (gid == 1)
    {
        system("unzip /home/durianpeople/Documents/FolderProses2/KompresProses2.zip -d /home/durianpeople/Documents/FolderProses2/");
    }
}
```

Pada `int main()` membuat folder *FolderProses1* dan *FolderProses2*, kemudian menjalankan masing-masing thread pertama dan thread kedua (tidak serentak) (masing-masing thread dijalankan dua kali dengan dua peran berbeda).

```c
pthread_t t1, t2;
int gid0 = 0;
int gid1 = 1;
system("mkdir /home/durianpeople/Documents/FolderProses1");
system("mkdir /home/durianpeople/Documents/FolderProses2");
pthread_create(&t1, NULL, thread1, &gid0);
pthread_create(&t2, NULL, thread1, &gid1);
pthread_join(t1, NULL);
pthread_join(t2, NULL);
pthread_create(&t1, NULL, thread2, &gid0);
pthread_create(&t2, NULL, thread2, &gid1);
pthread_join(t1, NULL);
pthread_join(t2, NULL);
```

Menunggu 15 detik, kemudian menjalankan thread ketiga

```c
printf("Menunggu 15 detik untuk mengekstrak kembali\n");
sleep(15);
pthread_create(&t1, NULL, thread3, &gid0);
pthread_create(&t2, NULL, thread3, &gid1);
pthread_join(t1, NULL);
pthread_join(t2, NULL);
```

## SOAL 5

