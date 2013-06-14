#ifndef ZEREZO_H_INCLUDED
#define ZEREZO_H_INCLUDED

class REZO_ELEM;

class REZO_LIEN
{
    friend class REZO_ELEM;
    REZO_ELEM *pe;
    REZO_LIEN *suiv;
    int force;
    int actif;
    public:
    int creer(REZO_ELEM *pa,int fa);
    int lier(REZO_LIEN *pa);
    REZO_LIEN* lire(int *fa,int *aa);
    REZO_ELEM* get_elem();
    REZO_LIEN* get_suiv();
    int get_force();
    int activer();
    int desactiver();
    int is_actif();
    int charger(REZO_ELEM *pa,int fa,int ac);
    REZO_LIEN* chercher(REZO_ELEM *pa);
    REZO_LIEN* get_last();
    int activer_seuil_force(int a);
};

class REZO_ELEM
{
    float x,y,z; // position en 3D!
    int id; // pointeur sur un liste de probe id -> probe -> annot!
    REZO_LIEN *lien;
    PROBESET *prob;
    int lu; // pour la recurrence
    int type; // pour le dessin
    int n_lien; // detection des noeuds
    public:
    int fixer_coord(float ax,float ay,float az);
    int lire_coord(float *ax,float *ay,float *az);
    int fixer_lu(int a);
    int fixer(int a);
    int fixer_id(int a);
    int fixer_lien(REZO_LIEN *pl);
    int fixer_probeset(PROBESET *pr);
    int lire_id();
    int fixer_type(int a);
    int lire_type();
    int sauver(FILE *pf);
    int charger(int a,int b);
    REZO_LIEN* aj_lien(REZO_LIEN *pdef,REZO_ELEM *pe,int fa,int aa);
    REZO_LIEN* get_lien();
    PROBESET* get_probeset();
    int get_voisin(int *a);
    int sortir_h3(FILE *pf,int a);
    int sortir_cytoscape(FILE *pf,int a);
    int tester(char *s);


    int copier(REZO_ELEM *pre);
    };

class REZO
{
    public:
    REZO_LIEN Lien[MaxLink];
    REZO_ELEM Elem[MaxProbe];
    REZO_ELEM *plecture;
    int n_elem;
    int n_lien;
    int act_lien;
    int initier();
    int agreger();
    int sauver();
    int charger();
    REZO_ELEM* transferer(int a);
    int preparer();
    int sauver_pos();
    int charger_pos();


    int lier_puce(RAM_Puce *pu);
    int selection_probeset(char *s);
    int deselect_all();
    int prep_filtre(char *requete,int *filtre);


    int activer_all_links();
    int desactiver_all_links();
    int activer_best_vois();
    int compter_liens_actifs();
    int certifier_aller_retour();
    int sortir_probeset(int a);
    int sortir_probeset_cytoscape(int a);
    int creer_liens_sup();
    int activer_seuil_sup(int a);
    int activer_seuil_inf(int a);
};



#endif // ZEREZO_H_INCLUDED
