#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#include "fonks.cpp"

#define MaxProbe 50000
#define MaxLink 30000000

using namespace std;

int lancer, sub_etap;
char seuilMinBuff[128];
int seuilMin;


#include "Ram_puce.cpp"
RAM_Puce Puce;

#include "zerezo.cpp"
REZO Rezo;

#include "zegeometrik.cpp"
UNIVERS Univers;




int initialisation()
{
    int i;

    printf("loading network ... \n\n");

    i = Rezo.preparer();
    if(i) cout << "network loaded : " << i << " nodes\n\n" << endl;

    Rezo.deselect_all();


    if(!Univers.initer2(&Rezo)) return 0;
    cout << "Universe initiate ("<< i <<") ("<<Rezo.n_lien<<") \n\n"<<endl;
    Univers.activer_lien(0);

    Univers.activer_lien(0);


    cout << "Calculate coordinates ... \n\n" << endl;

    Univers.initer(&Rezo);

    sub_etap = 1;
    while (sub_etap != 0)
    {
        sub_etap = Univers.marcher(sub_etap);
    }

    Rezo.sauver();
    cout << "Done. File " << Rezoout << " created \n\n" << endl;


    return 1;
}

int initialisation_var(int a,char *b[])
{

    if(a != 3) exit(0);

    sprintf(Rezoin,"%s.txt", b[1]);
    sprintf(Rezoout,"%s_sav.txt", b[1]);
    sprintf(seuilMinBuff,"%s", b[2]);  // Save the minimum threshold in str

    seuilMin = atoi(seuilMinBuff); // convert the minimum threshold in int.

    return 1;
}

int
main(int argc, char *argv[])
{

    initialisation_var(argc,argv);
    initialisation();

    return EXIT_SUCCESS;
}

