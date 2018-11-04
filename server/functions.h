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
#include <time.h>
#include <pthread.h>
#include <ldap.h>
#include <ldif.h>

#define BUF 1024
#define PORT 6543
#define LDAP_URI "ldap://ldap.technikum-wien.at:389"
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
using namespace std;

int ldap(string user, string pwd)
{
    LDAP *ld;			/* LDAP resource handle */
    LDAPMessage *result, *e;	/* LDAP result handle */
    BerElement *ber;		/* array of attributes */
    char *attribute;
    BerValue **vals;

    char BIND_USER[64];
    sprintf(BIND_USER, "uid=%s,ou=People,%s", user.c_str(), SEARCHBASE);

    BerValue *servercredp;
    BerValue cred;
    cred.bv_val = (char *)pwd.c_str();
    cred.bv_len=strlen(pwd.c_str());
    int i,rc=0;

    const char *attribs[] = { "uid", "cn", NULL };		/* attribute array for search */

    int ldapversion = LDAP_VERSION3;

    /* setup LDAP connection */
    if (ldap_initialize(&ld,LDAP_URI) != LDAP_SUCCESS)
    {
        fprintf(stderr,"ldap_init failed");
        return 0;
    }

    printf("connected to LDAP server %s\n",LDAP_URI);

    if ((rc = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &ldapversion)) != LDAP_SUCCESS)
    {
        fprintf(stderr, "ldap_set_option(PROTOCOL_VERSION): %s\n", ldap_err2string(rc));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 0;
    }
    if ((rc = ldap_start_tls_s(ld, NULL, NULL)) != LDAP_SUCCESS)
    {
        fprintf(stderr, "ldap_start_tls_s(): %s\n", ldap_err2string(rc));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 0;
    }

    /* anonymous bind */
    cout <<  "USER" << user << endl;
    rc = ldap_sasl_bind_s(ld,BIND_USER,LDAP_SASL_SIMPLE,&cred,NULL,NULL,&servercredp);

    if (rc != LDAP_SUCCESS)
    {
        cout << user << endl;
        fprintf(stderr,"LDAP bind error: %s\n",ldap_err2string(rc));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 0;
    }
    else
    {
        printf("bind successful\n");
    }

    /* perform ldap search */
    rc = ldap_search_ext_s(ld, SEARCHBASE, SCOPE, "(uid=if16b086)", (char **)attribs, 0, NULL, NULL, NULL, 500, &result);

    if (rc != LDAP_SUCCESS)
    {
        fprintf(stderr,"LDAP search error: %s\n",ldap_err2string(rc));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 0;
    }

    printf("Total results: %d\n", ldap_count_entries(ld, result));

    for (e = ldap_first_entry(ld, result); e != NULL; e = ldap_next_entry(ld,e))
    {
        printf("DN: %s\n", ldap_get_dn(ld,e));

        /* Now print the attributes and values of each found entry */

        for (attribute = ldap_first_attribute(ld,e,&ber); attribute!=NULL; attribute = ldap_next_attribute(ld,e,ber))
        {
            if ((vals=ldap_get_values_len(ld,e,attribute)) != NULL)
            {
                for (i=0;i < ldap_count_values_len(vals);i++)
                {
                    printf("\t%s: %s\n",attribute,vals[i]->bv_val);
                }
                ldap_value_free_len(vals);
            }
            /* free memory used to store the attribute */
            ldap_memfree(attribute);
        }
        /* free memory used to store the value structure */
        if (ber != NULL) ber_free(ber,0);

        printf("\n");
    }

    /* free memory used for result */
    ldap_msgfree(result);
    printf("LDAP search suceeded\n");

    ldap_unbind_ext_s(ld, NULL, NULL);
    return 1;
}


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
        Filename.append("../Mails/" + sname);

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
    string path = "../Mails/";
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
    string emptymail = "This User has no Email No Email\n";
    string path = "../Mails/";
    string username = name;
    string message_read;
    string output;
    string fileExists = path + username;

    //GET THE EMAILS COUNTS
    countemail = CountEmails(name);
    if(countemail == 0)
    {
        return "0";
    }
    output.append("Number of Emails: ");
    output.append(to_string(countemail) + "\n");
    output.append("======================");
    output.append("\n");

    int emailnum = 1;

    //WHILE EXISTS EMAILS, LIST ALL THE SUBJECTS
    while(true)
    {
        string Filename;
        fstream File;
        string result[100];
        string num = to_string(emailnum);

        Filename.append(path + username + "/" + "Email" + num + ".txt");
        cout << "Filename:" << Filename << endl;
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
            output.append("Email number " + num + " ");
            output.append("---> ");
            output.append("Subject: ");
            output.append(result[2]);
            output.append("\n");
            emailnum++;
        }
        else
            break;

    }
    output.append("======================");
    output.append("\n");

    return output;
}


bool fexists(string& filename) {
    ifstream ifile(filename.c_str());
    return (bool)ifile;
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
    Filename.append("../Mails/" + name);

    folder = Filename;
    struct stat st;
    num = emailnum;


        Filename = folder + "/" + email + emailnum + ".txt";

        bool is_true = fexists(Filename);

        if (is_true == false)
        {
            return "ERROR!";
        }

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

    Filename.append("../Mails/" + name);

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
            string File = "../Mails/" + name + "/Email" + number + ".txt";

            string newnum = to_string(i);
            string newfile = "../Mails/" + name + "/Email" + newnum + ".txt";

            int newname = rename(File.c_str(), newfile.c_str());
            if(rename == 0)
            {
                cout << "The file sucessfully renamed" << endl;
            }
            cout << File << endl;
        }
        output.append("The File " + Filename + " has beed deleted Successfully" + "\n");
    }
    else
    {
        output.append("Unable to delete the File " + Filename + "\n" + "The File does Not Exists!" + "\n\n");
    }

    return output;
}


#endif //SERVER_FUNCTIONS_H
