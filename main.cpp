#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
//#include <windows.h>
//#include <conio.h>

#include "fonks.cpp"

#define MaxProbe 50000    // define the maximum number of probset.
#define MaxLink 30000000  // define the maximum number of links.

using namespace std;

int sub_etap, seuilMin, timeSave;  // global variable


#include "Ram_puce.cpp"
RAM_Puce Puce;

#include "zerezo.cpp"
REZO Rezo;

#include "zegeometrik.cpp"
UNIVERS Univers;




int initialisation()
{
    int i, timeDebut, timeActu;
    int asav = 1;

    printf("loading network ... \n\n");
    i = Rezo.preparer();
    if(i) cout << "network loaded : " << i << " nodes\n\n" << endl;

    Rezo.deselect_all();

    if(!Univers.initer2(&Rezo)) return 0;
    cout << "Universe initiate ("<< i <<") ("<<Rezo.n_lien<<") \n\n"<<endl; // display the number of nodes and links

    Univers.activer_lien(0);  // activate links

    cout << "Calculate coordinates ... \n\n" << endl;

    Univers.initer(&Rezo);  // initiate the universe

    timeDebut = time(NULL);  // save time t=0;
    sub_etap = 1;
    while (sub_etap != 0)
    {

        timeActu = time(NULL);  // save actual time (refresh every loop)
        if (timeActu - timeDebut > timeSave)  // autosave
        {
            timeDebut = timeActu;
            Rezo.auto_sauver(asav);
            asav++;
        }

        sub_etap = Univers.marcher(sub_etap); // start up calculation
    }

    Rezo.sauver();  // save the result
    cout << "Done. File " << Rezoout << " created \n\n" << endl;


    return 1;
}

int initialisation_var(int a,char *b[])   // initiate console variable
{
    char seuilMinBuff[128];
    char timeSaveBuff[128];


    if(a != 5) exit(0); // if not enough/too much arguments in the console, stop the program.

    sprintf(Rezoin,"%s.txt", b[1]);   // save the full name (with extension) of the input file
    sprintf(Rezoout,"%s_sav.txt", b[1]); // create a buffer for the name of the output file
    sprintf(fileName,"%s", b[1]); // save the name (without extension) of the input file
    sprintf(seuilMinBuff,"%s", b[2]);  // Save the minimum threshold in str
    sprintf(coordNulBuff,"%s", b[3]);  // To know if the program create randomly the coord. at the begining or if it takes the coord from the input file
    sprintf(timeSaveBuff, "%s", b[4]); // To know the time between two autosave (in minute) (str)

    seuilMin = atoi(seuilMinBuff); // convert the minimum threshold as an int.
    timeSave = 60*atoi(timeSaveBuff); // convert the time between two autosave in second and as an int.


    return 1;
}

int
main(int argc, char *argv[])
{
    float timeStart, timeEnd;
    float timeInterval;
    timeStart=time(NULL);
    initialisation_var(argc,argv);
    initialisation();
    timeEnd=time(NULL);
    timeInterval=(timeEnd-timeStart)/3600;
    printf("%.2f h\n",timeInterval);

    return EXIT_SUCCESS;
}
