#include "header_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <pthread.h>

#define DEFAULT_MODE "send"
#define DEFAULT_PROTOCOL "file"
#define VERSION "0.2"
#define DATE "2023-04-15"
#define DEVELOPER "Varga Levente"
#define NEPTUN "EW33EZ"

void Help(){

    printf("Usage: chart [options]\n");
    printf("Options:\n");
    printf("  -send           set mode to send \033[1m(default)\033[0m\n");
    printf("  -receive        set mode to receive\n");
    printf("  -file           use file protocol \033[1m(default)\033[0m\n");
    printf("  -socket         use socket protocol\n");
    printf("  --version       print version information\n");
    printf("  --help          print this help message\n");
}

void* printVersion(){
    printf("Chart version \t%s\n", VERSION);
    return NULL;
}

void* printDate(){
    printf("Built on \t%s\n", DATE);
    return NULL;
}

void* printDeveloper(){
    printf("Developed by \t%s | %s\n", DEVELOPER, NEPTUN);
    return NULL;
}

void Version(){
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, printVersion, (void*) VERSION);
    pthread_create(&thread2, NULL, printDate, (void*) DATE);
    pthread_create(&thread3, NULL, printDeveloper, (void*) DEVELOPER);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}

int cmds(int* send_mode, int* file_mode, int argc, char *argv[])
{
    char *arg[] = {"--version", "--help", "-send", "-receive", "-file", "-socket"};
    //arg=48
    //size 6
    //arg0 = 8
    //karaktertömb 8 bit
    //Kapcsoló lehetséges számai
    int size = sizeof arg / sizeof arg[0];
    
    int check_count = 0;
    
        if (argc > 1)
            {
                for (int k = 0; k < size; ++k)
                    {   
                        for (int i = 1;  i < argc; ++i)
                            {
                                if (strcmp(arg[k], argv[i]) == 0){
                                        //check_count mindig nő egyel, ha jó kapcsolót adok le, ha nem jó akkor else ág
                                        check_count++;
                                    }
                                else{
                                    *send_mode = -1;
                                    *file_mode = -1;
                                }
                            }
                    }
                    //Hibás név esetén Help()
                    if (check_count != argc-1){
                            Help();
                
                            *send_mode = -1;
                            *file_mode = -1;
                     }
                    //1től indul 2ig
                    for (int i = 1;  i < argc; ++i)
                    {
                        //strcmp megvizsgálja az argumentumot, ha az argv[i] --version és, ha 0 akkor jó 
                        if (strcmp(argv[i],"--help") == 0){   
                                Help();
                                *send_mode = -1;
                                *file_mode = -1;
                            }
                        if (strcmp(argv[i],"--version") == 0){
                                Version();                               
                                *send_mode = -1;
                                *file_mode = -1;
                            }
                        ///////////-------------///////////
                        if (strcmp(argv[i],"-send") == 0){   
                                *send_mode = 0;
                            }
                        if (strcmp(argv[i],"-receive") == 0){   
                                *send_mode = 1;
                            }
                        if (strcmp(argv[i],"-file") == 0){   
                                *file_mode = 0;
                            }
                        if (strcmp(argv[i],"-socket") == 0){   
                                *file_mode = 1;
                            }
                    }
            }
        else
        {
            return EXIT_FAILURE;
        }
}

//--2. feladat
//1-2. lépés
int Measurement(int **Values)
{
    //Idő kezelés
    //=074i
    time_t Time;
    struct tm *T2;

    time(&Time);
    T2 = localtime(&Time);
    //Measuerment szám
    int meres_szam = 0;

    int perc = (*T2).tm_min;
    int masodperc = (*T2).tm_sec;

    if (perc == 0){
        meres_szam = 100;
    }
    else if(perc < 15){
         if((perc * 60) + masodperc < 100){
            meres_szam = 100;
        }else{
            meres_szam = (perc * 60) + masodperc;
        }
    }
    //Kivonok belőle, átalakítom másodpercé és száznak a maximuma
    else if(perc < 30){
         if((((perc - 15) * 60) + masodperc) < 100){
            meres_szam = 100;
        }else{
            meres_szam = ((perc - 15) * 60) + masodperc;
        }
    }
    else if(perc < 45){
         if((((perc - 30) * 60) + masodperc) < 100){
            meres_szam = 100;
        }else{
            meres_szam = ((perc - 30) * 60) + masodperc;
        }
    }
    else if(perc < 59){
         if((((perc - 45) * 60) + masodperc) < 100){
            meres_szam = 100;
        }else{
            meres_szam = ((perc - 45) * 60) + masodperc;
        }
    }

    srand(time(NULL)); 

    /*"Ebben a függvényben foglalj le dinamikus
    memóriafoglalás segítségével egy megfelelő méretű tömböt, ebben legyen tárolva az előállított
    véletlenszerű adatsorozat és a terület címe legyen elmentve azon a memóriacímen, amit
    paraméterként kap a függvény"*/
    *Values = (int *)malloc(meres_szam * sizeof(int));
        //(int *)realloc(*Values, ms_szam * sizeof(int));
    //Vissza nullára
    *(*Values + 0) = 0;

    double rnd;

  for(int i = 1; i < meres_szam; i++){
    
    //0-1 kozott
    rnd = (double)rand()/((unsigned)RAND_MAX+1);
    if(rnd<=0.428571){
        //i-edik érték a for ciklusban, egyenlő az előző érték előző értékével és plusz egy
        *(*Values + i) = *(*Values + (i - 1)) + 1;
    }
    // 11/31+0.428571
    else if (rnd <= 0.783409){
        *(*Values + i) = *(*Values + (i - 1)) - 1;
    }
    else{
        *(*Values + i) = *(*Values + (i - 1));
    }
  }
    return meres_szam;
}


//--3. feladat
void BMPcreator(int* Values, int NumValues)
{
    unsigned long int fsize = (62 + (NumValues * NumValues));
    unsigned int pad = 0;
    int file = open("chart.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH);
    unsigned char** tomb = (unsigned char**)calloc(NumValues, sizeof(unsigned char*));
    

    unsigned char f_headder[] = { 0x42, 0x4d, 0x00, 0x00, 0x00, 0x00,
                                  0x3e, 0x00, 0x00, 0x00, 0x28, 0x00,
                                  0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x61, 0x0f, 0x00, 0x00,
                                  0x61, 0x0f, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x11, 0xff, 0x00, 0xff, 0x00, 0x00,
                                  0xff, 0xff };

    for (int i = 0; i < NumValues; ++i) {
        tomb[i] = (unsigned char*)calloc(NumValues, sizeof(unsigned char));
    }
    for(int i = 0; i < NumValues; ++i){
        int elem = (NumValues/2) + Values[i];
        if (elem > NumValues){
            elem = NumValues-1;
        }
        else if (elem < -(NumValues/2)){
            elem = 0;
        }
        tomb[elem][i] = 0x01;
    }

    write(file, f_headder, sizeof(unsigned char) * 2);     
    write(file, &fsize, sizeof(int));      
    write(file, &f_headder[2], sizeof(unsigned char) * 12); 
    write(file, &NumValues, sizeof(int));  
    write(file, &NumValues, sizeof(int));  
    write(file, &f_headder[14], sizeof(unsigned char) * 36);    

    for (int i = 0; i < NumValues; i++){
        int x = 7;
        for (int j = 0; j < NumValues; ++j){           
            if (tomb[i][j] == 0x01){
                int ps = 1 << x;
                pad = pad + ps;
            }
            if (x == 24){                
                write(file, &pad, sizeof(int));
                pad = 0;                
                x = 7;
                continue;
            }
            if (x % 8 == 0){
                x = (j % 32) + 9;
            }
            --x;
        }    
        write(file, &pad, sizeof(int));       
        pad = 0;
    }
    free(tomb);
}
//--4. feladat
int FindPID()
{
    DIR *dir, *dir2;
    struct dirent *entry, *entry2;
    FILE *file;
    char line[100];
    char* ask_name = "Name:\tchart\n";
    char* ask_pid = "Pid:";
    int check_bash = 0;
    char tomb[4];

    int current_pid = getpid(); // saját pid lekérdezése

    dir = opendir("/proc");

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit((*entry).d_name[0])) {
            if (atoi(entry->d_name) == current_pid) continue; // saját pid kihagyása

            // pointert ráállítok a d_name-ra
            char *new_path = (char *)malloc(strlen(entry->d_name) + 6);
            sprintf(new_path, "/proc/%s", entry->d_name);

            dir2 = opendir(new_path);

            while ((entry2 = readdir(dir2)) != NULL) {
                if ((strcmp(entry2->d_name, "status") == 0)) {
                    char *new_path2 = (char *)malloc(strlen(new_path) + 8);
                    sprintf(new_path2, "%s/%s", new_path, "status");

                    file = fopen(new_path2, "r");
                    if (file == NULL) {
                        return -1;
                    }

                    while (fgets(line, sizeof(line), file) != NULL) {
                        if (strcmp(line, ask_name) == 0) {
                            check_bash = 1;
                        }

                        if ((strncmp(line, ask_pid, 4) == 0) && (check_bash == 1)) {
                            memcpy(tomb, &line[5], 4);
                            closedir(dir);
                            closedir(dir2);
                            return atoi(tomb);
                        }
                    }
                    fclose(file);
                }
            }
            closedir(dir2);
            free(new_path);
        }
    }
    closedir(dir);
    return -1;
}
//--5. feladat
//3. lépés
void ReceiveFile(int sig)
{
    printf("\n\033[1m\033[36mi\033[0m PID: %d", getpid());
    puts("\n\033[1m\033[36mi\033[0m File fogadása");

    char *filename = "/Measurement.txt";
    char *home_dir = getenv("HOME");
    char *filepath = malloc(strlen(home_dir) + strlen(filename) + 1);

    strncpy(filepath, home_dir, strlen(home_dir) + 1);
    strncat(filepath, filename, strlen(filename) + 1);

    puts("\033[1m\033[36mi\033[0m Várakozás a fájlra...");
    while (access(filepath, F_OK) == -1) {
        sleep(3);
    }

    FILE *file = fopen(filepath, "r");
    char line[100];
    //X számolja a measurement sorait
    int x = 0;

    int *result = (int *)malloc(sizeof(int));

    while (fgets(line, sizeof(int), file) != NULL){
        *(result + x) = atoi(line);
        x++;
        result = (int *)realloc(result, (sizeof(int) * (x + 1)));
    }
    puts("\033[1m\033[36mi\033[0m BMP generálása...");
    BMPcreator(result, x);
    puts("\033[32m✔\033[0m Generálás sikeres!");

    remove(filepath);

    free(result);

}

//2. lépés
void SendFile(int *Values, int NumValues)
{
    char *filename = "/Measurement.txt";
    char *home_dir = getenv("HOME");
    char *filepath = malloc(strlen(home_dir) + strlen(filename) + 1);
    puts("\033[1m\033[36mi\033[0m Fájl küldése...");

    strncpy(filepath, home_dir, strlen(home_dir) + 1);
    strncat(filepath, filename, strlen(filename) + 1);

    FILE *myFile = fopen(filepath, "w");
    for (int i = 0; i < NumValues; i++){
        fprintf(myFile,"%d\n",*(Values + i));
    }
    fclose(myFile);
    free(filepath);

    pid_t pid = FindPID();
    printf("\033[1m\033[36mi\033[0m PID found: %d\n", pid);
    if(pid == -1){
        puts("\033[31m✖\033[0m Nem találtam fogadó módban működő folyamatot!");
        exit(-1);
    }else{
        puts("\033[1m\033[36mi\033[0m Reciever found");
        //kill(pid, SIGUSR1);
    }
}
//--6. feladat
//1. lépés
void SendSocket(int *Values, int NumValues){
    //=0741
    int s;
    int bytes;
    int flag;
    char on;
    unsigned int server_size;
    struct sockaddr_in server;
    int valasz;

    //Inicializálás
    on   = 1;
    flag = 0;
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port        = htons(3333);
    server_size = sizeof server;

    //Socket készítés
    s = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( s < 0 ) {fprintf(stderr, "\033[31m✖\033[0m Nem sikerült a socketet létrehozni!\n"); exit(2);}
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    //Küldő tömb mérete
    bytes = sendto(s, &NumValues, sizeof(int), flag, (struct sockaddr *) &server, server_size);
    puts("\033[1m\033[36mi\033[0m Tömb méretének küldése...");
    if ( bytes <= 0 ) { fprintf(stderr, "\033[31m✖\033[0m Nem sikerült a küldés!\n"); exit(3); }
    signal(SIGALRM,SignalHandler);
    /*"a az eljárás alarm szignált (SIGALRM) kap, akkor írjon ki egy hibaüzenetet arra vonatkozóan, hogy a
    szerver nem válaszol (időkereten belül), majd a program egy hibakóddal álljon le"*/
    sleep(1);
    signal(SIGALRM, SIG_IGN);

    //Fogadás mérete
    bytes = recvfrom(s, &valasz, sizeof(int), flag, (struct sockaddr *) &server, &server_size);
    puts("\033[1m\033[36mi\033[0m Lefoglalt méret ellenőrzése...");
    if ( bytes < 0 ) {fprintf(stderr, "\033[31m✖\033[0m A fogadás nem sikerült!\n"); exit(3);}

    
    
    //Méret teszt
    if (NumValues != valasz){
        fprintf(stderr, "\033[31m✖\033[0m Méret hiba!\n");
        exit(4);
    }
    puts("\033[32m✔\033[0m A küldött és fogadott méret azonos.");

    //Tömbadatok küldése
    bytes = sendto(s, Values, (NumValues * sizeof(int)), flag, (struct sockaddr *) &server, server_size);
    if ( bytes <= 0 ) {fprintf(stderr, "\033[31m✖\033[0m Nem sikerült a küldés!\n"); exit(5);}
    puts("\033[1m\033[36mi\033[0m Tömb adat küldése...");

    //Küldött fogadási bájtok mérete
    int s_bytes = recvfrom(s, &valasz, sizeof(int), flag, (struct sockaddr *) &server, &server_size);
    if (bytes <= 0)
    {
        fprintf(stderr,"Error size!\n");
        exit(7);
    }
    if (valasz != bytes)
        {
        fprintf(stderr, "\033[31m✖\033[0m Méret hiba!\n");
        exit(5);
    }
    puts("\033[1m\033[36mi\033[0m Az elküldött és fogadott bájtok ellenőrzése...");
    puts("\033[32m✔\033[0m A fájl sikeresen elküldve.");
    
    close(s);
}

//2. lépés
void ReceiveSocket()
{
    int bytes;
    int err;
    int flag;
    char on;
    unsigned int server_size;
    unsigned int client_size;
    struct sockaddr_in server;
    struct sockaddr_in client;
    int valasz;

    //Inicializálás
    on   = 1;
    flag = 0;
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port        = htons(3333);
    server_size = sizeof server;
    client_size = sizeof client;
    //signal(SIGINT, stop);
    //signal(SIGTERM, stop);

    //Socket készítése
    int s = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( s < 0 ) {
        fprintf(stderr, "\033[31m✖\033[0m Hiba a socket létrehozása közben!\n");
        exit(2);
        }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    //Csatlakozás sockethez
    err = bind(s, (struct sockaddr *) &server, server_size);
    if ( err < 0 ) {
      fprintf(stderr,"\033[31m✖\033[0m Socket csatlakozási hiba!\n");
      exit(3);
    }

    while(1){
        puts("\n\033[1m\033[36mi\033[0m Várakozás az adatokra...\n");

        //Fogadási tömb mérete
        bytes = recvfrom(s, &valasz, sizeof(int), flag, (struct sockaddr *) &client, &client_size );
        if ( bytes < 0 ) {fprintf(stderr, "\033[31m✖\033[0m Fogadási hiba!\n");exit(4);}

        //Tömb lefoglalás
        int* ERTEKEK = malloc(valasz * sizeof(int));
        puts("\033[1m\033[36mi\033[0m A tömb méretének lefoglalása...");

        //Tömbkiosztási méret elküldése
        bytes = sendto(s, &valasz, sizeof(int), flag, (struct sockaddr *) &client, client_size);
        puts("\033[1m\033[36mi\033[0m Lefoglalt tömb méretének ellenőrzése...");
        if ( bytes <= 0 ) {fprintf(stderr,"\033[31m✖\033[0m Küldési hiba!\n");exit(4);}

        //Tömbadatok fogadása
        bytes = recvfrom(s, ERTEKEK, valasz * sizeof(int), flag, (struct sockaddr *) &client, &client_size );
        puts("\033[1m\033[36mi\033[0m Tömb adatok beérkezése...");
        if ( bytes < 0 ){fprintf(stderr, "\033[31m✖\033[0m Fogadási hiba!\n"); exit(4);}

        //Fogadási bájtok küldése
        bytes = sendto(s, &bytes, sizeof(int), flag, (struct sockaddr *) &client, client_size);
        puts("\033[1m\033[36mi\033[0m Érkezett adatok bájtjainak ellenőrzése...");
        if ( bytes < 0 ) {fprintf(stderr,"\033[31m✖\033[0m Küldési hiba!\n");exit(8);}

        //BMP fájl készítés
        puts("\033[1m\033[36mi\033[0m BMP generálása...");
        BMPcreator(ERTEKEK, valasz);
        puts("\033[32m✔\033[0m Generálás kész!");

        free(ERTEKEK);
        puts("");
    }

    close(s);   
}
//--7. feladat
//1. lépés
void SignalHandler(int sig)
{
    if(sig==SIGINT){
        puts("\n\033[1m\033[36mi\033[0m A program leállt.");
        exit(0);
    }
    if(sig==SIGUSR1){
        puts("\033[31m✖\033[0m A fájlon keresztüli küldés szolgáltatás nem elérhető!");
        exit(10);
    /**///return;
    }if(sig==SIGALRM){
        puts("\033[31m✖\033[0m Szerver időtúllépés!");
        exit(11);
    }
}
