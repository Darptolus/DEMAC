#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

int main (int argc, char* argv[]) {
    int id = atoi(argv[1]);
    int max_id = 24;
    string ip_b = "192.168.10.1";

    const char *path1="/etc/hostname";
    ofstream myfile1;
    myfile1.open (path1);
    //cout << "ID number:";
    //cin >> id;
    if (id<10) {
        myfile1 << "NOPA0" << id;
    }else{
        myfile1 << "NOPA" << id;
    }
    myfile1.close();
    
    const char *path2="/etc/hosts";
    ofstream myfile2;
    myfile2.open (path2);
    myfile2 << "127.0.0.1       localhost\n";
    myfile2 << "::1             localhost ip6-localhost ip6-loopback\n";
    myfile2 << "fe00::0         ip6-localnet\n";
    myfile2 << "ff00::0         ip6-mcastprefix\n";
    myfile2 << "ff02::1         ip6-allnodes\n";
    myfile2 << "ff02::2         ip6-allrouters\n";
    myfile2 << "\n";
    myfile2 << ip_b << "90" << " Darptolus" << "\n";
    for(int i=1; i <= max_id; i++){
        if (i<10) {
            myfile2 << ip_b << "0" << i << " " << "NOPA0" << i <<"\n";
        }else{
            myfile2 << ip_b << i << " " << "NOPA" << i << "\n";
    }
    }
    myfile2.close();

    const char *path3="/etc/network/interfaces.d/eth0";
    ofstream myfile3;
    //myfile.open (path, ios::out | ios::app );
    myfile3.open (path3);
    myfile3 << "auto eth0\n";
    myfile3 << "iface eth0 inet static\n";
    if (id<10) {
        myfile3 << "address " << ip_b << "0" << id << "\n";
    }else{
        myfile3 << "address " << ip_b << id << "\n";
    }
    myfile3 << "netmask 255.255.255.0\n";
    myfile3 << "gateway 192.168.10.1\n";
    myfile3 << "nameserver 8.8.8.8\n";
    myfile3 << "nameserver 8.8.4.4\n";
    //myfile << "\n";
    myfile3.close();

return 0;
}
