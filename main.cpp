#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "fonks.cpp"

#define MaxProbe 50000    // define the maximum number of probset.
#define MaxLink 110000000  // define the maximum number of links.


using namespace std;

int sub_etap, mSeuil, aSeuil, coordFlag, timeSave;  // global variable


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
    if (i>MaxProbe){
        cout << "too many nodes : " << i << " nodes (MaxProbe allowed)\n\n" << endl;
        return 0;
    }
    if(i) cout << "network loaded : " << i << " nodes\n\n" << endl;

    Rezo.deselect_all();

    if(!Univers.initer2(&Rezo)){
        if (Rezo.n_lien>MaxLink){
            cout << "too many links : " << Rezo.n_lien << " nodes (MaxLink allowed)\n\n" << endl;
        }
        return 0;
     }
    cout << "Universe initiate ("<< i <<" nodes) ("<<Rezo.n_lien<<" links) \n\n"<<endl; // display the number of nodes and links

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
    char seuilMaxBuff[128];
    char seuilMinBuff[128];
    char coordNulBuff[128];
    char timeSaveBuff[128];


    if(a != 6) exit(0); // if not enough/too much arguments in the console, stop the program.

    sprintf(Rezoin,"%s.txt", b[1]);   // save the full name (with extension) of the input file
    sprintf(Rezoout,"%s_end.txt", b[1]); // create a buffer for the name of the output file
    sprintf(fileName,"%s", b[1]); // save the name (without extension) of the input file
    sprintf(seuilMinBuff,"%s", b[2]);  // Save the minimal force to be considered (>0)
    sprintf(seuilMaxBuff,"%s", b[3]);  // Save the threshold for activate links at start in str (<10 000)
    sprintf(coordNulBuff,"%s", b[4]);  // To know if the program create randomly the coord. at the begining or if it takes the coord from the input file
    sprintf(timeSaveBuff, "%s", b[5]); // To know the time between two autosave (in minute) (str)

    mSeuil = atoi(seuilMaxBuff); // convert the minimum force threshold as an int.
    aSeuil = atoi(seuilMinBuff); // convert the activation threshold as an int.
    coordFlag=atoi(coordNulBuff);
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
