#include <iostream>
#include <string>
#include <cstdlib> //exit()
#include <cstring> // strpy()
#include <unistd.h> //close()
#include <netinet/in.h> 
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode ) // Исключения
{
    cout << "ErrorCode:"<<exitCode <<endl<< why << endl;
    exit(exitCode);
}
int main()
{
    
    // структура с адресом клиента
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; // интернет протокол IPv4
    selfAddr->sin_port = 0;         // любой порт 
    selfAddr->sin_addr.s_addr = 0;  

    // структура с адресом сервера
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;     // интернет протокол IPv4
    remoteAddr->sin_port = htons(13);  // port 13
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12"); //  адрес сети 
    
    // буфер для передачи и приема данных
    char *buf = new char[256];
    strcpy(buf,"Date and time");   //копирование строки
    int msgLen = strlen(buf);      //вычисление длины строки
   
    // создание сокета
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); //tcp протокол
    if (mySocket == -1) {
        close(mySocket);
        Exception("Error open socket",11);
    }
    //связывание сокета с адресом
    int rc = bind(mySocket,(const sockaddr *) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Error bind socket with local address",12);
        }

    //установка соединения
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) { 
        close(mySocket);
        Exception("Error connect socket with remote server.", 13);
    }

    // передача сообщения из буффера
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) { //проверка
        close(mySocket);
        Exception("Error send message", 14);
    }
    cout << "We send: " << buf << endl; 
      
    // принятие ответа
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
       Exception("Error receive answer.", 15);
    }
    buf[rc] = '\0'; // конец принятой строки
    cout << "We receive: " << buf; 
    // закрытие сокета
    close(mySocket);
//освобождение памяти
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}
