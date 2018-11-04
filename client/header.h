#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H

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
#include <thread>
#include <cstdio>

using namespace std;

//SEND EMAIL
string Send(string name)
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


#endif //CLIENT_HEADER_H
