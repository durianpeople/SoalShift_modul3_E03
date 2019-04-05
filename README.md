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

## SOAL 2

Untuk soal no 2 ini, menggunakan 4 program, dengan rincian 1 server penjual, 1 server pembeli, 1 client penjual, 1 client pembeli

untuk 2 server mendeklarasikan thread, socket, value yang akan di share, buffer, dan string b untuk pembanding

server penjual
```c
pthread_t tid[2];
int *value;
int new_socket, valread;
char buffer[1024] = {0};
char b[] = "tambah";
```

server pembeli
```c
pthread_t tid[2];
int *value;
int new_socket, valread;
char buffer[1024] = {0};
char b[] = "beli";
```

untuk 2 server menggunakan shared memory untuk membagikan value dari stok barang
dengan cara
```c
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
value = shmat(shmid, NULL, 0);
```
untuk deklarasi socket, address seperti di modul

untuk server pembeli hanya bisa mengurangi stock, jika client pembeli memberikan pesan "beli", dengan code seperti dibawah ini, tetapi dengan syarat jika stock lebih dari 0, maka transaksi berhasil, jika stock kurang dari sama dengan 0, maka transaksi gagal

server pembeli
```c
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
```

untuk sisi client pembeli, user penginput sebuah string, tetapi hanya string "beli" lah yang dikirim ke server, untuk kata selain "beli", akan memberikan pesan error

client pembeli
```c
scanf("%[^\n]%*c", a); 
    i = strcmp(a,b);
    // printf("%d\n", i);
  if(i == 0){
        send(sock , a , strlen(a) , 0 );
    
  }
  if(i != 0 ){
      printf("Error input selain kata beli, coba lagi!\n");
        exit(0);
  }
  
    // printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
```

untuk server penjual menggunakan 2 thread, digunakan untuk menghitung setiap 5 detik untuk menampilkan stok barang, dan untuk mengurangi stok barang yang ada

```c
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
```

code untuk pembuatan thread
```c
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
```

untuk sisi client penjual, user penginput sebuah string, tetapi hanya string "tambah" yang dikirim ke server, untuk kata selain "tambah", akan memberikan pesan error

```c
scanf("%[^\n]%*c", a); 
    i = strcmp(a,b);
    printf("%d\n", i);
    // printf("%d\n", i);
    if(i == 0){
        send(sock , a , strlen(a) , 0 );
        
    }
    if(i != 0 ){
        printf("Error input selain kata tambah, coba lagi!\n");
        exit(0);
    }
```

## SOAL 3

deklarasi variabel

```c
int WakeUp_Status;
int Spirit_Status;
pthread_t tid[3];
int choice;
int flag_agmal;
int flag_iraj;
int flag = 0;
int status_agmal;
int status_iraj;
```

WakeUp_Status digunakan untuk menyimpan nilai WakeUp_Status dari agmal
Spirit_Status digunakan untuk menyimpan nilai Spirit_Status dari iraj
pthread_t untuk mendeklarasikan thread
choice digunakan untuk pemilihan fitur menu
flag_agmal dan flag_iraj digunakan untuk menyimpan sudah berapa kali fungsi WakeUp_Status sama Spirit_Status telah dipanggil
flag digunakan sebagai parameter thread mana yang akan melanjutkan tugasnya
status_agmal dan status_iraj digunakan untuk kondisi sleep sebuah thread

deklarasi 3 fungsi, yaitu status, WakeUp_Status, dan Spirit_Status

```c
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
```

definisi untuk thread 1, 2, dan 3

```c
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
```

proses pembuatan thread

```c
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
```

untuk program ini, saya menganalogikan thread status dengan flag 1, thread WakeUp_Status dengan flag 2, thread Spirit_Status dengan flag 3, dan saya menggunakan menu dengan flag 0 

thread menu

```c
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

