/* myclient.c */
#include "header.h"

#define BUF 1024
#define PORT 6543


int main (int argc, char **argv)
{
    int create_socket;
    char buffer[BUF];
    string sender, subject, to, message;
    struct sockaddr_in address;
    ssize_t size;
    argc=2;
    argv[0]="./client";
    argv[1]="localhost";

    if (argc < 2)
    {
        printf("Usage: %s ServerAdresse\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return EXIT_FAILURE;
    }


    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_aton(argv[1], &address.sin_addr);

    if (connect(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0)
    {
        printf("Connection with server (%s) established\n", inet_ntoa(address.sin_addr));

        string username;
        string password;
        int tedad = 0;

        for(;;) {

            string username;
            string password;
            memset(&buffer, 0, BUF);
            if( tedad == 3)
            {
                memset(&buffer, 0, BUF);
                size = recv(create_socket, buffer, BUF -1, 0);
                if(size > 0)
                {
                    buffer[size] = '\0';
                    cout << buffer << endl;
                    for(int j = 1; j <= 5; j++)
                    {
                        sleep(60);
                    }
                    tedad++;
                }
                continue;
            }

            size = recv(create_socket, buffer, BUF - 1, 0);
            if(size > 0)
            {
                cout << buffer;
            }
            cin >> username;
            strcpy(buffer, username.c_str());
            send(create_socket, buffer, strlen(buffer), 0);

            memset(&buffer, 0, BUF);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if(size > 0)
            {
                cout << buffer;
            }

            cin >> password;
            strcpy(buffer, password.c_str());
            send(create_socket, buffer, strlen(buffer), 0);
            strcpy(buffer, "----------------------------\n");

            memset(&buffer, 0, BUF);
            size = recv(create_socket, buffer, BUF - 1, 0);

            if (size > 0)
            {
                if( strcmp(buffer, "ERR!") != 0) {
                    cout << buffer << endl;
                    break;
                }
                else
                {
                    cout << "ERROR BEIM LDAP, TRY AGAIN!" << endl;
                }
            }
            tedad++;
        }
    }
    else
    {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    do {
        memset(buffer, 0, BUF);
        size = recv(create_socket, buffer, BUF - 1, 0);
        if (size > 0) {
            buffer[size] = '\0';
            printf("%s", buffer);
        }

        int num;
        cout << "> ";
        cin >> num;

        if(cin.fail())
        {
            cout << "Unknown Task! Please Choose the right one" << endl;
            cin.clear();
            cin.ignore();
            cout << "> ";
            cin >> num;
        }
        memset(&buffer, 0, BUF);
        string message = to_string(num);
        strcpy(buffer, message.c_str());

        send(create_socket, buffer, strlen(buffer), 0);
        size = recv(create_socket, buffer, BUF - 1, 0);
        if (size > 0)
        {
            buffer[size] = '\0';
            cout << buffer;
        }
        if (strcmp(buffer, "SEND\n") == 0)
        {
            strcpy(buffer, Send().c_str());
            send(create_socket, buffer, strlen(buffer), 0);

            continue;

        }
        else if (strcmp(buffer, "LIST\n") == 0)
        {

            cout << "Username: ";
            string username;
            cin >> username;
            strcpy(buffer, username.c_str());
            send(create_socket, buffer, strlen(buffer), 0);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0)
            {

                buffer[size] = '\0';
                if(strcmp(buffer, "0") == 0)
                {
                    cout << "................" << endl;
                    cout << "Inbox is Empty!" << endl;
                    cout << "................" << endl;
                    cout << endl;
                }
                else {
                    cout << buffer << endl;
                }
            }

            continue;

        }

        // FIRST SEND METHOD SEND THE USERNAME AND THE SECOND ONE SEND THE EMAIL NUMBER
        else if (strcmp(buffer, "READ\n") == 0)
        {

            cout << "Username: " << endl;
            string username;
            cin >> username;
            strcpy(buffer, username.c_str());

            if(buffer[0] != '\0')
            {
                send(create_socket, buffer, sizeof(buffer), 0);
            }

            cout << "Select the Email Number: ";
            string number;
            cin >> number;
            memset(&buffer, 0, BUF);
            strcpy(buffer, number.c_str());
            send(create_socket, buffer, sizeof(buffer), 0);
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {

                if(strcmp(buffer, "ERROR!") == 0)
                {

                    cout << "\n" << buffer << endl;
                    cout << "THE EMAIL DOES NOT EXISTS OR HAS BEEN DELETED" << endl;
                    cout << endl;
                    continue;

                }

                buffer[size] = '\0';
                stringstream str;
                string sender;
                string empf;
                string subject;
                string message[1000];

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
                int i = 0;
                while(str) {

                    str >> message[i];
                    if(message[i] == ".")
                    {
                        break;
                    }
                    if(message[i] == "\n")
                    {
                        cout << endl;
                    }

                    cout << message[i] << " " ;
                    i++;

                }
                cout << endl;
                cout << "-------------------------------------" << endl;
            }

            continue;

        }
        // FIRST SEND METHOD SEND THE USERNAME AND THE SECOND ONE SEND THE EMAIL NUMBER
        else if (strcmp(buffer, "DELETE\n") == 0)
        {

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
            if (size > 0)
            {
                buffer[size] = '\0';
                cout << "**********************************" << endl;
                cout << buffer;
                cout << "**********************************" << endl;

            }
            memset(buffer, 0, BUF);
            continue;
        }

    }
    while (strcmp(buffer, "QUIT\n") != 0);
    close (create_socket);
    return EXIT_SUCCESS;
}
