/*

    probeset[0] -> field definition
    probeset[1]-> first probset of the microarray


*/

#include "Ram_puce.h"

int RAM_Puce::prep_filtre(char *requete,int *filtre)
{
    int i,j,k;

    i=k=0;
    while(i<n_probe)
     { j = probeset[i].tester(requete);
       if(j)
        { filtre[i] = 1;
          k++;
        }
       else filtre[i] = 0;
       i++;
     }
    return k;
}
int RAM_Puce::get_definitions(char *s)
{
    probeset[0].lire(s);
    return 1;
}
PROBESET* RAM_Puce::lister(int n)
{
    if(n< n_probe) return &probeset[n];
    return NULL;
}
PROBESET* RAM_Puce::chercher_probeset(char *s)
{
    int i;

    mini_text(s);
    for(i=0;i<n_probe;i++)
     if(probeset[i].tester_probeset(s)) return &probeset[i];
     //if(probeset[i].tester(s)) return &probeset[i];

    return NULL;
}
int RAM_Puce::lister_probe()
{
    FILE *pf;
    char tamp[6096];
    int i;

    pf = fopen("h:/MlpTest/Mus11K/debug.txt","w");
    if(!pf) return 0;
    for(i=0;i<n_probe;i++)
     { probeset[i].lire_d(0,tamp);
       fprintf(pf,"%s\n",tamp);
     }

    fclose(pf);
    return 1;
}

int PROBESET::fixer(char *s)
{ // preparation en ***\t*****\t**...\0
    char *pt,*ps;

    ps = s+1; // on saute le 1er " !!!
    pt = d;
    while(*ps)
     { if(*ps == '"')
        if(ps[1] == ',')
          { ps+= 3;
            *pt++ = '\t';
          }
       if(*ps == '"') ps++;
       *pt++ = *ps++;
        if(ps -s > 4094)
         { *ps = 0;
           return 0;
         }
     }
    *pt = 0;
    return 1;
}
int PROBESET::lire(char *s)
{
    copie_string(d,s);
    return 1;
}
int PROBESET::lire_d(int num, char *s)
{
    char tamp[4096],*pt;
    int i;

    pt = lire_seq(d,tamp,'\t');
    for(i = 0; i< num; i++)
      pt = lire_seq(pt,tamp,'\t');

    copie_string(tamp,s);
    return 1;
}
int PROBESET::tester(char *s)
{
    return chercher_string(s,d);
}
int PROBESET::tester_probeset(char *s)
{
    char tamp[4096],*pt;

    pt = lire_seq(d,tamp,'\t');
    mini_text(tamp);
    if(!strcmp(tamp,s)) return 1;
    return 0;
}

