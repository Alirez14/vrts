//
// Created by alirez on 23.10.18.
//

#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H




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

#define BUF 1024
#define PORT 6543

using namespace std;

// GET THE MESSAGE FROM THE CLIENT AND SAVE IT INTO NEW ORDNER AND CREATE A EMAIL FILE FOR EACH MAIL
void Save(string sname, char message[BUF])
{

    while (true)
    {
        string folder;
        static int emailnum = 1;
        string num;
        string Filename;
        ofstream File;
        string email = "Email";

        num = to_string(emailnum);
        Filename.append("../../Mails/" + sname);

        folder = Filename;
        struct stat st;

        //IF THE DIR DOESN'T EXISTS -> CREATE ONE ELSE MAKE A .TXT FILE AND SAVE THE EMAIL IN IT
        if (opendir(folder.c_str()) == NULL)
        {

            mkdir(folder.c_str(), 0777);
        }
        else
        {
            //CREATE A TEXT FILE
            Filename = folder + "/" + email + num + ".txt";

            if (stat(Filename.c_str(), &st) == 0)
            {
                emailnum++;
                continue;

            }
            else
            {
                File.open(Filename);
                //File << emailnum << "\n";
                File << message;
                File.close();
                break;
            }
        }

    }
}

// GET THE EMPFÄNGER FROM THE EMAIL IN CASE TO CREATE A ORDNER FOR IT
string getEmpf(char buffer[BUF])
{
    stringstream buff(buffer);
    string emp[8];

    for (int i = 0; i < 2; ++i)
    {
        buff >> emp[i];
    }
    // FIRST INDEX IS THE
    return emp[1];
}

//THIS FUNCTION COUNT THE SUM OF THE USERS MAILS AND RETURNS IT
int CountEmails(string name)
{
    int countemail = 0;
    string path = "../../Mails/";
    string username = name;
    string message_read;
    string output;
    int emailnum = 1;
    //WHILE EXISTS TEXT FILE, COUNT THE MAILS
    while(true)
    {
        string Filename;
        fstream File;
        string result[100];
        string num = to_string(emailnum);

        Filename.append(path + username + "/" + "Email" + num + ".txt");
        File.open(Filename);

        if(File.is_open())
        {
            countemail++;
            emailnum++;
        }
        else
            break;
    }

    return countemail;
}

// FOR LIST THE EMAILS
string List(string name)
{
    int countemail;
    string emptymail = "No Email\n";
    string path = "../../Mails/";
    string username = name;
    string message_read;
    string output;

    //GET THE EMAILS COUNTS
    countemail = CountEmails(name);
    output.append("Number of Emails: ");
    output.append(to_string(countemail) + "\n");

     int emailnum = 1;

    //WHILE EXISTS EMAILS, LIST ALL THE SUBJECTS
    while(true)
    {
        string Filename;
        fstream File;
        string result[100];
        string num = to_string(emailnum);

        Filename.append(path + username + "/" + "Email" + num + ".txt");
        cout << "Filename" << Filename << endl;
        File.open(Filename);
        cout << "Ok" << endl;

        // LIST THE SUBJECTS
        if(File.is_open())
        {
            int i = 0;
            while (File.good() && i != 3)
            {
                File >> message_read;
                result[i].append(message_read);
                i++;
            }
            output.append("Email number " + num + "\n");
            output.append("Subject: ");
            output.append(result[2]);
            output.append("\n");
            emailnum++;
        }
        else
            break;

    }

    return output;
}

//READ FUNCTION
string read(string name, string emailnum)
{
    string folder;
    string num;
    string Filename;
    fstream File;
    string email = "Email";
    string output;
    string result;
    string emptymail = "No Email\n";


    num = emailnum;
    Filename.append("../../Mails/" + name);

    folder = Filename;
    struct stat st;
    num = emailnum;

    // iF THERE IS NO FOLDER, IT MEANS THE USER HAS NO EMAILS
    if (opendir(folder.c_str()) == NULL)
    {
        return emptymail;
    }

        //ELSE SEND IT TO AN ARRAY AND RETURN IT
    else
    {

        Filename = folder + "/" + email + emailnum + ".txt";
        cout << Filename;

        File.open(Filename);

        if (File.is_open())
        {
            while (File.good())
            {

                File >> output;
                result.append(output);
                result.append("\n");
            }
        }

        File.close();

    }
    return result;
}

//THE FUNCTION DELETE A GIVEN EMAIL AND RENAME THE NEXT EMAILS BASE ON THE DELETED EMAIL
string del(string name, string emailnum)
{
    string folder;
    string num;
    string Filename;
    fstream File;
    string email = "Email";
    string output;
    int countemails = CountEmails(name);

    string::size_type sz;
    int mailnum = stoi(emailnum, &sz);

    Filename.append("../../Mails/" + name);

    folder = Filename;
    num = emailnum;

    if (opendir(folder.c_str()) == NULL)
    {

        output.append("This User has No Email No Email");

    }

    Filename = folder + "/" + email + emailnum + ".txt";

    int status= remove(Filename.c_str());

    //IF THE FILE SUCESSFULLY DELETE THEN THE NEXT EMAIL WILL BE FROM FOR EXAMPLE EMAIL3 TO EMAIL2
    if (status == 0)
    {
        for (int i = mailnum; i <= countemails; ++i )
        {
            string number = to_string(i + 1);
            string File = "../../Mails/" + name + "/Email" + number + ".txt";

            string newnum = to_string(i);
            string newfile = "../../Mails/" + name + "/Email" + newnum + ".txt";

            int newname = rename(File.c_str(), newfile.c_str());
            if(rename == 0)
            {
                cout << "The file sucessfully renamed" << endl;
            }
            cout << File << endl;
        }
        output.append("The File " + Filename + " has beed deleted Successfully");
    }
    else
    {
        output.append("Unable to delete the File " + Filename);
    }

    return output;
}










#endif //SERVER_FUNCTIONS_H
