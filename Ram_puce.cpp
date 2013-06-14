/*

    RAM gerant les fichiers *.csv des puces Affymetrix.
    probeset[0] -> definitions des champs
    probeset[1]-> le premier probeset de la puce!!!!!!!


*/
class PROBESET {
    char d[4096];
    public:
    int fixer(char *s);
    int lire(char *s);
    int lire_d(int num,char *s);
    int tester(char *s);
    int tester_probeset(char *s);
    };

class RAM_Puce {
    PROBESET probeset[MaxProbe+1];
    int n_probe; // nombre de puces utilisés!
    char nom[64]; // nom complet du fichier *csv;
    char data_ID[64][16]; // descrition des données accessibles
    public:
    int initier();
    int lire_proteasom();
    int get_definitions(char *s);
    PROBESET* lister(int n);
    PROBESET* chercher_probeset(char *s);
    int lister_probe();
    int prep_filtre(char *requete,int *filtre);
    };
/***********************************************/
char Rezoannot[128];
/***********************************************/
int RAM_Puce::initier()
{
    FILE *pf;
    char tamp[4096];
    int l;


   // pf = fopen("g:/travaux/U95/HG_U95red.csv","r");
    //pf = fopen("g:/travaux/U133/HG-U133A_red.csv","r");
    pf = fopen(Rezoannot,"r");
    if(!pf) return 0;
    n_probe = 0;
    while(fgets(tamp,4096,pf))
     { Ctexte(tamp);
       tamp[4094] = 0;
       probeset[n_probe++].fixer(tamp);
       //probeset[n_probe-1].lire(tamp);
       if(n_probe >= MaxProbe) n_probe = MaxProbe;
     }
    fclose(pf);
    return n_probe;
}
int RAM_Puce::lire_proteasom()
{
    FILE *pf;
    char tamp[6096];
    int i,j;

    pf = fopen("h:/tempo/mouse_prot.txt","w");
    if(!pf) return 0;

    i = 0;
    probeset[i++].lire(tamp);
    fprintf(pf,"%s\n",tamp);
    while(i<n_probe)
     { j = probeset[i].tester("proteasome");
       if(j)
        { probeset[i].lire(tamp);
          fprintf(pf,"%s\n",tamp);
        }
       i++;
     }
     fclose(pf);
     return 1;
}
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
    int i,j;

    pf = fopen("h:/MlpTest/Mus11K/debug.txt","w");
    if(!pf) return 0;
    for(i=0;i<n_probe;i++)
     { probeset[i].lire_d(0,tamp);
       fprintf(pf,"%s\n",tamp);
     }

    fclose(pf);
    return 1;
}
/**********************************************/
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

    //pt = tamp;
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

/**********************************************/
