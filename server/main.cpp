/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include "functions.h"
#define BUF 1024
#define PORT 6543

using namespace std;


int main ()
{
    char message[1000];
    string _empty = "";
    int create_socket, new_socket;
    socklen_t addrlen;
    char buffer[BUF];
    ssize_t size;
    struct sockaddr_in address, cliaddress;

    create_socket = socket (AF_INET, SOCK_STREAM, 0);

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (PORT);

    if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0)
    {
        perror("bind error");
        return EXIT_FAILURE;
    }
    listen (create_socket, 5);

    addrlen = sizeof (struct sockaddr_in);
    while (true)
    {
        printf("Waiting for connections...\n");
        new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );
        if (new_socket > 0)
        {
            printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
            /*strcpy(buffer,"Welcome to myserver, Please enter your command:\n"
                          "choose the task: \n"
                          "1) SEND \n"
                          "2) LIST\n"
                          "3) READ \n"
                          "4) DELETE \n"
                          "5) QUIT \n");
            send(new_socket, buffer, strlen(buffer),0);
            memset(&buffer, 0, BUF);*/
        }
        while (true)
        {

            memset(&buffer, 0, BUF);
            strcpy(buffer,"Welcome to myserver, Please enter your command:\n"
                          "choose the task: \n"
                          "1) SEND \n"
                          "2) LIST\n"
                          "3) READ \n"
                          "4) DELETE \n"
                          "0) QUIT \n");
            send(new_socket, buffer, strlen(buffer),0);
            memset(&buffer, 0, BUF);
            size = recv (new_socket, buffer, BUF-1, 0);
            if( size > 0)
            {
                buffer[size] = '\0';
                printf ("Message received: %s\n", buffer);

                switch (buffer[0])
                {
                    case '1':
                    {
                        string message = "SEND\n";
                        strcpy(buffer, message.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);
                        //Empty the ARRAY
                        memset(&buffer, 0, BUF);
                        size = recv (new_socket, buffer, BUF-1, 0);
                        if(size > 0)
                        {
                            string emp = getEmpf(buffer);
                            Save(emp, buffer);
                            cout << "Buffer is:" << buffer << endl;
                        }

                        break;
                    }

                    case '2':
                    {
                        string result;
                        string message = "LIST\n";
                        strcpy(buffer, message.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);
                        memset(&buffer, 0, BUF);
                        size = recv(new_socket, buffer, BUF - 1, 0);

                        if (size > 0)
                        {
                            buffer[size] = '\0';
                            string username = buffer;
                            result = List(username);
                            cout << "Result: " << result  << endl;
                            memset(&buffer, 0, BUF);
                            strcpy(buffer, result.c_str());
                            send(new_socket, buffer, strlen(buffer), 0);

                        }
                        else
                        {

                            cout << "Error";
                        }

                        break;

                    }
                    case '3':
                    {
                        string message = "READ\n";
                        string username;
                        string number;
                        string output;
                        int in = 1;
                        int bufsize;
                        strcpy(buffer, message.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);
                        memset(&buffer, 0, BUF);
                        while(in != 3)
                        {

                            if(in == 1)
                            {
                                size = recv (new_socket, buffer, BUF-1, 0);
                                if(size > 0)
                                {

                                    buffer[size] = '\0';
                                    username = buffer;
                                    memset(&buffer, 0, BUF);
                                    bufsize = recv(new_socket, buffer, BUF - 1, 0);
                                    in++;
                                    continue;
                                }
                            }

                            else if ( in == 2)
                            {
                                size = recv (new_socket, buffer, BUF-1, 0);
                                if(size > 0)
                                {
                                    buffer[size] = '\0';

                                    number = buffer;
                                    memset(&buffer, 0, BUF);
                                    in++;
                                    continue;
                                }
                            }

                        }
                        output = read(username, number);
                        cout << output << endl;
                        strcpy(buffer, output.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);

                        break;
                    }



                    case '4':
                    {
                        string message = "DELETE\n";
                        string username;
                        string number;
                        string output;
                        int in = 1;
                        int bufsize;
                        strcpy(buffer, message.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);
                        memset(&buffer, 0, BUF);
                        while(in != 3)
                        {

                            if(in == 1)
                            {
                                size = recv (new_socket, buffer, BUF-1, 0);
                                if(size > 0)
                                {

                                    buffer[size] = '\0';
                                    username = buffer;
                                    memset(&buffer, 0, BUF);
                                    bufsize = recv(new_socket, buffer, BUF - 1, 0);
                                    in++;
                                    continue;
                                }
                            }

                            else if ( in == 2)
                            {
                                size = recv (new_socket, buffer, BUF-1, 0);
                                if(size > 0)
                                {
                                    buffer[size] = '\0';

                                    number = buffer;
                                    memset(&buffer, 0, BUF);
                                    in++;
                                    continue;
                                }
                            }

                        }
                        output = del(username, number);
                        strcpy(buffer, output.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);

                        break;
                    }

                    case '0':
                    {
                        string message = "QUIT\n";
                        strcpy(buffer, message.c_str());
                        send(new_socket, buffer, strlen(buffer), 0);


                        break;
                    }

                }

            }
            if (buffer=="QUIT\n")
            {
                printf("Client closed remote socket\n");
                break;
            }

            else if (size == 0)
            {
                printf("Client closed remote socket\n");
                break;
            }
            else{
                continue;
            }
        }
        close (new_socket);
    }
    close (create_socket);
    return EXIT_SUCCESS;
}
