#include "header_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    if (strcmp(argv[0], "./chart") != 0){
        fprintf(stderr,"\033[31m✖\033[0m A program neve nem megfelelő! (chart)\n");
        return EXIT_FAILURE;
    }

    int send_mode = 0; int file_mode = 0;

    int *ERTEKEK = NULL;
    int meret;

    cmds(&send_mode, &file_mode, argc, argv);    //1.Feladat

    signal(SIGINT,SignalHandler); signal(SIGUSR1,SignalHandler);

    if (send_mode < 0 || file_mode < 0){
        return EXIT_FAILURE;
    }
        //send - file
    if (send_mode == 0 && file_mode == 0){
        meret = Measurement(&ERTEKEK);  //2.Feladat
        printf("\n\033[32m✔\033[0m Tömb generálása sikeres\n");
        SendFile(ERTEKEK, meret);
        printf("\033[32m✔\033[0m Fájl elküldve\n");
        free(ERTEKEK);
        exit(0);
    }
        //send - socket
    else if (send_mode == 0 && file_mode == 1) {
        meret = Measurement(&ERTEKEK);  //2.Feladat 
        printf("\n\033[32m✔\033[0m Tömb generálása sikeres\n");
        SendSocket(ERTEKEK, meret);
        free(ERTEKEK);
        exit(0);
    }
        //receive - file
    else if (send_mode == 1 && file_mode == 0){
        while (1){
            ReceiveFile(signal(SIGUSR1, SignalHandler));
            //pause()
            sleep(2);
        }
        pause();
    }
        //receive - socket
    else if (send_mode == 1 && file_mode == 1) {
        ReceiveSocket();   
    }
    
    return EXIT_SUCCESS;
}