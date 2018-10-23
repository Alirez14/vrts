//
// Created by alirez on 09.10.18.
//

#ifndef SOCKET_PR_BEFEHLE_H
#define SOCKET_PR_BEFEHLE_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>



using namespace std;
void Save(string sname , string message) {
    bool next = true;
    while (next == true) {
        string folder;

        static int emailnum = 1;
        string num;

        string Filename;
        ofstream File;
        string email = "Email";

        num = to_string(emailnum);

        Filename.append("/home/alirez/Desktop/VRTS/Mails/" + sname);

        folder = Filename;

        struct stat st;

        if (opendir(folder.c_str()) == NULL) {

            mkdir(folder.c_str(), 0777);
        } else {

            Filename = folder + "/" + email + num + ".txt";

            if (stat(Filename.c_str(), &st) == 0) {
                emailnum++;
                continue;

            } else {
                File.open(Filename);
                File << message;
                File.close();
                break;
            }
        }

    }
}
void send(){
    char sname[8] , rname[8],subject[80];
    string message;
    string a;

cout<<"sender name max 8 char:"<<endl;
cin>>sname;
cout<<"reciever name max 8 cahr:"<<endl;
cin>>rname;
cout<<"subject name max 80 char:"<<endl;
cin>>subject;
cout<<"write the message:"<<endl;

while(a != "."){

        cin >> a;
        message.append(a);
        message.append("\n");



}
string smessage=message;
Save(sname,smessage);

cout<<sname<<rname<<subject<<message<<endl;



}
bool fexists(string filename) {
    ifstream file(filename);
    return (bool)file;
}
/*
void save(){

        int emailnum = 1;
        char num = '0' + emailnum;
        bool if_exists;
        string Filename;
        ifstream File;
        string email = "Email";
        label:
        num = '0' + emailnum;
        Filename.append("/home/alirez/Desktop/VRTS");
        Filename = Filename + email + num + ".txt";
        if_exists = fexists(Filename);
        if (if_exists)
        {
            cout << fexists(Filename) << endl;
            cout << emailnum << endl;
            emailnum++;
            cout << emailnum;
            goto label;
        }
        else
        {
            File.open(Filename);
            File << "HiIII";
            File.close();
        }


}
 */









#endif //SOCKET_PR_BEFEHLE_H
