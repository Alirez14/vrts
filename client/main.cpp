/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#define BUF 1024
#define PORT 6543
/*
******************* CLIENT *******************
*/
using namespace std;

//SEND EMAIL
string Send()
{
    string sender, to, subject;

    //VECTOR REMALOC THE SIZE
    vector<string> mail[100];
    string message;

    cout << "FROM: ";
    cin >> sender;
    cout << "TO: ";
    cin >> to;
    cout << "Subject: ";
    cin >> subject;
    cout << "Message:" << endl;

    mail->push_back(sender + "\n");
    mail->push_back(to + "\n");
    mail->push_back(subject + "\n");

    while (message != ".")
    {
        getline(cin, message);
        mail->push_back(message + "\n");

    }

    auto size = mail->size();
    auto *Email = new string[size] ;

    for(auto i = mail->begin(); i != mail->end(); ++i)
    {
        Email->append(*i);
    }

    return *Email;
    delete[] Email;
}


int main (int argc, char **argv) {
    int create_socket;
    char buffer[BUF];
    string sender, subject, to, message;
    struct sockaddr_in address;
    ssize_t size;
    argc=2;
    argv[0]="./client";
    argv[1]="localhost";



    if (argc < 2) {
        printf("Usage: %s ServerAdresse\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }


    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_aton(argv[1], &address.sin_addr);

    if (connect(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0) {
        printf("Connection with server (%s) established\n", inet_ntoa(address.sin_addr));
//label:
        //memset(&address, 0, sizeof(address));


    } else {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    do {
    	        size = recv(create_socket, buffer, BUF - 1, 0);
        if (size > 0) {
            buffer[size] = '\0';
            printf("%s", buffer);
        }
        char message[100];
        scanf("%s", message);
        memset(&buffer, 0, BUF);
        strcpy(buffer, message);
        send(create_socket, buffer, strlen(buffer), 0);
        //memset(&buffer, 0, BUF);
        size = recv(create_socket, buffer, BUF - 1, 0);
        if (size > 0) {
            //buffer[size] = '\0';
            cout << buffer;
        }
        if (strcmp(buffer, "SEND\n") == 0) {
            strcpy(buffer, Send().c_str());
            send(create_socket, buffer, strlen(buffer), 0);
            /*size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                cout << buffer;
            } */

            continue;

        } else if (strcmp(buffer, "LIST\n") == 0) {
            //memset(&buffer, 0, BUF);
            cout << "Username: ";
            string username;
            cin >> username;
            strcpy(buffer, username.c_str());
            send(create_socket, buffer, strlen(buffer), 0);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                cout << buffer;
            }

            continue;

        }
            // FIRST SEND METHOD SEND THE USERNAME AND THE SECOND ONE SEND THE EMAIL NUMBER
        else if (strcmp(buffer, "READ\n") == 0) {
            cout << "Username: " << endl;
            string username;
            cin >> username;
            strcpy(buffer, username.c_str());
            send(create_socket, buffer, sizeof(buffer), 0);
            cout << "Select the Email Number: ";
            string number;
            cin >> number;
            memset(&buffer, 0, BUF);
            strcpy(buffer, number.c_str());
            send(create_socket, buffer, sizeof(buffer), 0);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                stringstream str;
                string sender;
                string empf;
                string subject;
                string message;
                cout << "-------------------------------------" << endl;
                str << buffer;
                cout << "Sender: ";
                str >> sender;
                cout << sender << endl;

                cout << "To: ";
                str >> empf;
                cout << empf << endl;

                cout << "Subject: ";
                str >> subject;
                cout << subject << endl;

                cout << "Message: ";
                str >> message;
                cout << message << endl;
                cout << "-------------------------------------" << endl;
            }
            continue;

        }
            // FIRST SEND METHOD SEND THE USERNAME AND THE SECOND ONE SEND THE EMAIL NUMBER
        else if (strcmp(buffer, "DELETE\n") == 0) {
            cout << "Username: " << endl;
            string username;
            cin >> username;
            strcpy(buffer, username.c_str());
            send(create_socket, buffer, sizeof(buffer), 0);
            cout << "Select the Email Number: ";
            string number;
            cin >> number;
            memset(&buffer, 0, BUF);
            strcpy(buffer, number.c_str());
            send(create_socket, buffer, sizeof(buffer), 0);
            memset(&buffer, 0, BUF);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                cout << buffer;
            }
            continue;
        }

    } while (strcmp(buffer, "QUIT\n") != 0);
    close (create_socket);
    return EXIT_SUCCESS;
}
