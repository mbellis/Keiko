#ifndef RAM_PUCE_H_INCLUDED
#define RAM_PUCE_H_INCLUDED

class PROBESET
{
    char d[4096];
    public:
    int fixer(char *s);
    int lire(char *s);
    int lire_d(int num,char *s);
    int tester(char *s);
    int tester_probeset(char *s);
};

class RAM_Puce
{
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


#endif // RAM_PUCE_H_INCLUDED
