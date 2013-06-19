#ifndef ZEGEOMETRIK_H_INCLUDED
#define ZEGEOMETRIK_H_INCLUDED

class FORCE
{
    float va[12000];
    float f_fact;
    int n_va; // nombre de donnees
    int p_va; // entree en entier
    public :
    int ouvrir(char *s);
    int fixer_fact(float a);
    float lire(int a);
    float lire_rep(float a);
    float lire_sun(float a);
};


class UNIVERS
{
    REZO *prezo;
    REZO_ELEM *pla;
    REZO_LIEN *pli;
    REZO_ELEM *planete[MaxProbe];
    float depart[MaxProbe][3]; // copie de REZO :: acces rapide!
    float neo[MaxProbe][3];
    float aff_pos[MaxProbe][3];
    double force[MaxProbe][3];
    double forceI[MaxProbe][3];
    float mouv[MaxProbe][3];
    float pas,npas;
    float msoleil,mrepulsion,minter;
    int n_planete,etap,sub_etap,memo;
    int act_lien,dum;
    int lien_seuil,neg_lien_seuil;
    int parsec[MaxProbe][3];// division de l'espace => voisins!
    int brume[MaxProbe];
    int sel[MaxProbe],act_sel,nsel;
    int repulsif[MaxProbe];
    int caractere[MaxProbe];
    float couleur[100][3];
    public:
    double energie[2]; // 0 depart  // 1 arrivee
    int initer(REZO *pr);
    int initer2(REZO *pr);
    int copier_depart();
    int copier_arrivee();
    int prep_parsec();
    int do_cycle(int a);
    double calc_parsec(int a);
    double calc_inter(int a);
    double calc_force();
    int bouger();
    int act_marcher(int a);
    int activer_lien(int a);
    int marcher(int sub_etap);
    float modifier_pas(float a);
    double calc_energie();
    double calc_energie2();
    int fixe_aff_pos(int a,float ax,float ay,float az);
    int prep_selection();
    int act_selection();
    int neo_lien_seuil(int a);
};



#endif // ZEGEOMETRIK_H_INCLUDED
