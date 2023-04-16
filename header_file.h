#ifndef RKP_header_
#define RKP_header_
//1.feladat
int cmds(int* send_mode, int* file_mode, int argc, char *argv[]);
//2.feladat
int Measurement(int **Values);
//3.feladat
void BMPcreator(int *Values, int NumValues);
//4.feladat
int FindPID();
//5.feladat
void SendFile(int *Values, int NumValues);
//6.feladat
void ReceiveFile(/*int sig*/);
//7.feladat
void SendSocket(int *Values, int NumValues);
//8.feladat
void ReceiveSocket();
//9.feladat
void SignalHandler(int sig);
#endif // RKP_header_
