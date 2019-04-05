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

### GAME

Mendeklarasikan variabel global

- Untuk thread:

  ```c
  pthread_t inputThread, displayThread,
      healthStatusHandlerThread, hungerStatusHandlerThread, hygieneStatusHandlerThread,
      bathActionHandlerThread;
  ```

- Mendefinisikan struct untuk status dan tindakan

  ```c
  typedef struct
  {
      int max;
      int value;
      int delta;
      int delay;
  } status;
  
  typedef struct
  {
      int delta;
      int amount;
      int delay;
  } action;
  ```

- Mendefinisikan nilai status dan aksi awal

  ```c
  status hunger = {200, 200, 5, 10};
  status hygiene = {100, 100, 10, 30};
  status health = {300, 300, -5, 10};
  int enemyHealth = 0;
  
  action food = {15, 10, 0};
  action bath = {30, 0, 20};
  action attack = {20, 0, 0};
  ```

- Mendefinisikan variabel untuk stock makanan di shop

  ```c
  int *shopStock;
  key_t shopKey = 76255;
  int shopID;
  ```

- Menentukan scene awal

  ```c
  int current_scene = 0;
  /*
      0 -> standby scene
      1 -> battle scene
      2 -> shop scene
  */
  ```

Membuat implementasi

- Dalam `int main()`, menggunakan key `key_t shopKey` untuk shared memory, kemudian pointer `int *shopStock` diarahkan ke alamat shared memory tersebut. Kemudian menjalankan thread-thread terkait

  ```c
  int main()
  {
      shopID = shmget(shopKey, sizeof(int), IPC_CREAT | 0666);
      shopStock = shmat(shopID, NULL, 0);
      //status handler thread
      pthread_create(&healthStatusHandlerThread, NULL, statusHandler, &health);
      pthread_create(&hungerStatusHandlerThread, NULL, statusHandler, &hunger);
      pthread_create(&hygieneStatusHandlerThread, NULL, statusHandler, &hygiene);
  
      //action handler thread
      pthread_create(&bathActionHandlerThread, NULL, actionHandler, &bath);
  
      //output thread
      pthread_create(&displayThread, NULL, display, NULL);
  
      //input thread
      pthread_create(&inputThread, NULL, input, NULL);
  
      pthread_join(healthStatusHandlerThread, NULL);
      pthread_join(hungerStatusHandlerThread, NULL);
      pthread_join(hygieneStatusHandlerThread, NULL);
  
      pthread_join(bathActionHandlerThread, NULL);
      pthread_join(displayThread, NULL);
      pthread_join(inputThread, NULL);
      return 0;
  }
  ```

- Membuat fungsi thread untuk meng-handle status seperti `status health`, `status hygiene`, dan `status hunger`:

  ```c
  void *statusHandler(void *arg)
  {
      status *temp = ((status *)arg);
      while (1)
      {
          sleep(temp->delay);
          temp->value = temp->value - temp->delta;
          if (temp->value <= 0)
          {
              printf("KALAH\n");
              exit(0);
          }
      }
  }
  ```

- Membuat fungsi thread untuk meng-handle aksi seperti `action bath`

  ```c
  void *actionHandler(void *arg)
  {
      action *temp = ((action *)arg);
      int defaultDelta = temp->delta;
      int defaultDelay = temp->delay;
  
      while (1)
      {
          if (temp->delta <= 0)
          {
              temp->delay--;
              if (temp->delay == 0)
              {
                  temp->delta = defaultDelta;
                  temp->delay = defaultDelay;
              }
          }
          sleep(1);
      }
  }
  ```

- Fungsi thread untuk menjalankan aksi sesuai input

  ```c
  void *input(void *arg)
  {
      while (1)
      {
          char input = getch();
          switch (current_scene)
          {
          case 0:
              switch (input)
              {
              case '1':
                  if (food.amount > 0)
                  {
                      hunger.value += food.delta;
                      if (hunger.value > hunger.max)
                          hunger.value = hunger.max;
                      food.amount--;
                      refresh_display();
                  }
                  break;
              case '2':
                  hygiene.value += bath.delta;
                  bath.delta = 0;
                  if (hygiene.value > hygiene.max)
                      hygiene.value = hygiene.max;
                  refresh_display();
                  break;
              case '3':
                  enemyHealth = 100;
                  pthread_cancel(healthStatusHandlerThread);
                  pthread_cancel(hygieneStatusHandlerThread);
                  pthread_cancel(hungerStatusHandlerThread);
                  current_scene = 1;
                  refresh_display();
                  break;
              case '4':
                  current_scene = 2;
                  refresh_display();
                  break;
              case '5':
                  exit(0);
                  break;
              }
              break;
          case 1:
              switch (input)
              {
              case '1':
                  if (enemyHealth > 0)
                  {
                      health.value -= attack.delta;
                      enemyHealth -= attack.delta;
                      if (health.value <= 0)
                      {
                          printf("KALAH\n");
                          exit(0);
                      }
                      if (enemyHealth < 0)
                          enemyHealth = 0;
                      refresh_display();
                  }
                  break;
              case '2':
                  pthread_create(&healthStatusHandlerThread, NULL, statusHandler, &health);
                  pthread_create(&hungerStatusHandlerThread, NULL, statusHandler, &hunger);
                  pthread_create(&hygieneStatusHandlerThread, NULL, statusHandler, &hygiene);
                  current_scene = 0;
                  refresh_display();
                  break;
              }
              break;
          case 2:
              switch (input)
              {
              case '1':
                  if (*shopStock > 0)
                  {
                      (*shopStock)--;
                      food.amount++;
                      refresh_display();
                  }
                  break;
              case '2':
                  current_scene = 0;
                  refresh_display();
                  break;
              }
              break;
          }
      }
  }
  ```

- Mendefinisikan fungsi thread untuk menampilkan menu sesuai scene yang ditentukan oleh `int current_scene`

  ```c
  void *display(void *arg)
  {
      int time = 0;
      while (1)
      {
          system("clear");
          printf("Time: %d\n", time++);
          switch (current_scene)
          {
          case 0:
              printf("Standby Mode\n");
              printf("Health: %d\n", health.value);
              printf("Hunger: %d\n", hunger.value);
              printf("Hygiene: %d\n", hygiene.value);
              printf("Food left: %d\n", food.amount);
              if (bath.delta == 0)
                  printf("Bath will be ready in: %d s\n", bath.delta == 0 ? bath.delay : 0);
              else
                  printf("Bath is ready\n");
              printf("Choices\n");
              printf("1. Eat\n");
              printf("2. Bath\n");
              printf("3. Battle\n");
              printf("4. Shop\n");
              printf("5. Exit\n");
              break;
          case 1:
              printf("Battle Mode\n");
              printf("Monster's Health: %d\n", health.value);
              printf("Enemy's Health: %d\n", enemyHealth);
              if (enemyHealth <= 0)
                  printf("MUSUH KALAH\n");
              printf("Choices\n");
              printf("1. Attack\n");
              printf("2. Run\n");
              break;
          case 2:
              printf("Shop Mode\n");
              printf("Shop food stock: %d\n", *shopStock);
              printf("Your food stock: %d\n", food.amount);
              printf("Choices\n");
              printf("1. Buy\n");
              printf("2. Back\n");
              break;
          }
          sleep(1);
      }
  }
  ```

### SHOP

Mendeklarasikan variabel global untuk thread dan shared memory stock makanan

```c
pthread_t inputThread, displayThread;

int *shopStock;
key_t shopKey = 76255;
int shopID;
```

Dalam `int main()`, mereferensi ke shared memory kemudian menjalankan thread terkait

```c
int main()
{
    shopID = shmget(shopKey, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shopID, NULL, 0);

    pthread_create(&inputThread, NULL, input, NULL);
    pthread_create(&displayThread, NULL, display, NULL);

    pthread_join(displayThread, NULL);
    pthread_join(inputThread, NULL);
}
```

Handle input

```c
void *input(void *arg)
{
    while (1)
    {
        char input = getch();
        switch (input)
        {
        case '1':
            (*shopStock)++;
            refresh_display();
            break;
        case '2':
            exit(0);
            break;
        }
    }
}
```

Handle tampilan

```c
void *display(void *arg)
{
    int time = 0;
    while (1)
    {
        system("clear");
        printf("Time: %d\n", time++);
        printf("Shop\n");
        printf("Food stock: %d\n", *shopStock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
        sleep(1);
    }
}
```

