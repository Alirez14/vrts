Project title:

TWMailer   -   A client-server program to internally exchange mails, relying on socket communication.


Build status:

Project part 1 is finished (10.21.2018), part 2 will start being implemented after review of part 1.
Project specification can be found here: https://moodle.technikum-wien.at/pluginfile.php/423803/mod_resource/content/0/VSYS_Socket_Uebung.pdf


Contact information:

Alireza Esbati
Ervin Sebesten if17b087@technikum-wien.at


General usage notes:

-To open through command shell in linux from subdirectory \client-server\client\cmake-build-debug\ respectively \client-server\server\server\cmake-build-debug\ with 
./server and ./client 127.0.0.1 
-Mails are stored in \client-server\Mail\ and then the name of the receiver of the mail.
-Choose the action you want by typing a number from the menu given, then provide the requested info.
-The delete function renames all of the emails that were sent after the one that was deleted by reducing the count with 1 (Email3 becomes Email2 for example, if Email2/Email1 was deleted)
-If the error "server adress is in use" pops up, wait 30 seconds, then try again.

