/*

Creating and representing the probset network

*/

#include "zerezo.h"

char Rezoin[128];
char Rezoout[128];
char fileName[128];





int REZO_LIEN::creer(REZO_ELEM *pa,int fa)
{
    pe = pa;
    force = fa;
    suiv = NULL;
    return 1;
}
int REZO_LIEN::charger(REZO_ELEM *pa,int fa,int ac)
{
    pe = pa;
    force = fa;
    actif = ac;
    suiv = NULL;
    return 1;
}
int REZO_LIEN::lier(REZO_LIEN *pa)
{
    suiv = pa;
    return 1;
}
int REZO_LIEN::activer()
{
    actif = 1;
    return actif;
}
int REZO_LIEN::desactiver()
{
    actif = 0;
    return actif;
}
REZO_ELEM* REZO_LIEN::get_elem()
{
    return pe;
}
REZO_LIEN* REZO_LIEN::get_suiv()
{
    return suiv;
}
int REZO_LIEN::get_force()
{
    return force;
}
REZO_LIEN* REZO_LIEN::lire(int *fa,int *aa)
{
    *fa = force;
    *aa = actif;

    return suiv;
}
REZO_LIEN* REZO_LIEN::chercher(REZO_ELEM *pa)
{
    REZO_LIEN *ps;

    ps = this;
    while(ps)
     { if(ps->pe == pa) return ps;
       ps = ps->suiv;
     }
    return ps;
}
REZO_LIEN* REZO_LIEN::get_last()
{
    REZO_LIEN *ps;

    ps = this;
    while(ps->suiv) ps = ps->suiv;

    return ps;
}
int REZO_LIEN::is_actif()
{
    return actif;
}
int REZO_LIEN::activer_seuil_force(int a)
{
    if (force > a) actif = 1;
    return 1;
}




int REZO_ELEM::copier(REZO_ELEM *pre)
{
    id = pre->id;
    type = pre->type;
    lien = NULL;
    n_lien = 0;
    lu = 0;
    x = pre->x;
    y = pre->y;
    z = pre->z;
    prob = pre->prob;

    return 1;
}
int REZO_ELEM::fixer_id(int a)
{
    id = a;
    return id;
}
int REZO_ELEM::fixer(int a)
{
    id = a;
    type = 0;
    lien = NULL;
    n_lien = 0;
    return 1;
}
int REZO_ELEM::fixer_coord(float ax,float ay,float az)
{
    x = ax;
    y = ay;
    z = az;

    return 1;
}
int REZO_ELEM::lire_coord(float *ax,float *ay,float *az)
{
    *ax = x;
    *ay = y;
    *az = z;

    return 1;
}
int REZO_ELEM::fixer_lu(int a)
{
    if(a != -1) lu = a;
    return lu;
}
int REZO_ELEM::fixer_type(int a)
{
    if(a != -1) type = a;
    return type;
}
int REZO_ELEM::charger(int a,int b)
{
    id = a;
    type = b;
    lien = NULL;
    lu = 0;
    n_lien = 0;
    return 1;
}
int REZO_ELEM::fixer_lien(REZO_LIEN *pl)
{
    lien = pl;
    return 1;
}
int REZO_ELEM::fixer_probeset(PROBESET *pr)
{
    prob = pr;
    return 1;
}
PROBESET* REZO_ELEM::get_probeset()
{
    return prob;
}
int REZO_ELEM::lire_id()
{
    return id;
}
int REZO_ELEM::lire_type()
{
    return type;
}
int REZO_ELEM::sauver(FILE *pf)
{

    fprintf(pf,"%d\t%6.3f\t%6.3f\t%6.3f\n",id, x,y,z);   // save id probset and coordinates in an output file

    return 1;
}
REZO_LIEN * REZO_ELEM::aj_lien(REZO_LIEN *pdef,REZO_ELEM *pe,int fa,int aa)
{
    REZO_LIEN *pl;

    pl = lien->chercher(pe);
    if(pe)
     { if(!aa)pl->desactiver();
       if(aa) pl->activer();
       return pl;
     }
    // new link !
    pl = pdef;
    pl->creer(pe,fa);
    if(!aa)pl->desactiver();
    if(aa) pl->activer();

    return pl;
}
int REZO_ELEM::get_voisin(int *a)
{
    REZO_LIEN *pl;
    int i,z;

    for(i=0;i<32000;i++) a[i] = 0;

    pl = lien;
    a[id]=1;

    while(pl)
     { if(pl->actif)
        { z = pl->pe->lire_id();
          if(z < 32000) a[z] = 1;
        }
       pl = pl->get_suiv();
     }
    return 1;
}
REZO_LIEN * REZO_ELEM::get_lien()
{
    return lien;
}
int REZO_ELEM::sortir_h3(FILE *pf,int a)
{
    REZO_ELEM *psn;
    REZO_LIEN *psl;

    fprintf(pf,"%d %d %d html\n",a,id,1);
    if(lu) return 0; // deja vu!
    lu = 1;

    psl = lien;
    while(psl)
     { if(psl->is_actif())
        { psn = psl->get_elem();
          if(psn) psn->sortir_h3(pf,a+1);
        }
       psl = psl->get_suiv();
     }
    return a;
}
int REZO_ELEM::sortir_cytoscape(FILE *pf,int a)
{
    REZO_ELEM *psn;
    REZO_LIEN *psl;

    if(lu) return 0; // deja vu!
    lu = 1;

    psl = lien;
    while(psl)
     { if(psl->is_actif())
        { psn = psl->get_elem();
          fprintf(pf,"n%d\tpp\tn%d\n",id,psn->id);
          if(psn) psn->sortir_cytoscape(pf,a);
        }
       psl = psl->get_suiv();
     }
    return a;
}
int REZO_ELEM::tester(char *s)
{
    if(!s[0]) return 0;
    type = 0;
    if(prob->tester(s)) type = 1;
    return type;
}

int REZO::initier()
{
    plecture = NULL;
    n_elem = 0;
    return n_elem;
}
REZO_ELEM* REZO::transferer(int a)
{
    if(a<n_elem) return &Elem[a];
    return NULL;
}
int REZO::sauver()
{
    FILE *pf;
    int i;

    pf = fopen(Rezoout,"w");  // open the output file created from the console var.

    if(!pf) return 0; // if doesn't open, close the program.

    for(i=0;i<n_elem;i++)
      Elem[i].sauver(pf);  // save coordinates

    fclose(pf); // close the file.
    return 1;
}

int REZO::auto_sauver(int j)
{
    FILE *pf;
    int i;
    char fname[128];

    sprintf(fname,"autosave/%s_asav%d.txt", fileName, j);  // Save the name of the autosaved files into a buffer

    pf = fopen(fname,"w");  // Open (and create) the autosaved file

    if(!pf) return 0;  // If it doen't work, stop the function

    for(i=0;i<n_elem;i++)
      Elem[i].sauver(pf);  // Function to save coordinates.

    fclose(pf); // close the file

    cout << "Coord. autosaved as \"autosaved/" << fileName <<"_asav"<< j << ".txt \n\n" << endl;
    return 1;
}


int REZO::charger()   // open the input file, read the data and attribute them to var.
{
    FILE *pf;
    char tamp[84];
    char tamp2[24],*pt;
    int i,j,k,k_lien;
    float ax,ay,az;


    pf = fopen(Rezoin,"r");
    if(!pf) return 0;

    // pre-define the planet : more secure
    for(i=0;i<MaxProbe;i++) Elem[i].fixer(i);
    n_elem = n_lien = k_lien = 0;

    while(fgets(tamp,84,pf))
     { Ctexte(tamp);
       if(tamp[0] == '>')
        { pt = tamp+1;
          pt = lire_seq(pt,tamp2,'\t');
          i = atoi(tamp2);
          pt = lire_seq(pt,tamp2,'\t');
          j = atoi(tamp2);
          Elem[n_elem++].charger(i,j);
          Elem[n_elem-1].fixer_lien(NULL);
          ax = ay = az = 0.0;
          if(pt)
           {  pt = lire_seq(pt,tamp2,'\t');
              ax = atof(tamp2);
              pt = lire_seq(pt,tamp2,'\t');
              ay = atof(tamp2);
              az = atof(pt);
           }
          Elem[n_elem-1].fixer_coord(ax,ay,az);
          k_lien = 0;
        }
       else
        { pt = tamp;
          pt = lire_seq(pt,tamp2,'\t');
          i = atoi(tamp2);
          pt = lire_seq(pt,tamp2,'\t');
          j = atoi(tamp2);
          k = atoi(pt);
          if(k_lien) Lien[n_lien-1].lier(&Lien[n_lien]);
          else Elem[n_elem-1].fixer_lien(&Lien[n_lien]);
          Lien[n_lien++].charger(&Elem[i],j,k);
          k_lien++;
        }
     }
    fclose(pf);
    //charger_pos();
    return 1;
}
int REZO::sauver_pos()
{
    FILE *pf;
    float x,y,z;
    int i;

    pf = fopen("position_sav.txt","w");
    if(!pf) return 0;

    for(i=0;i<n_elem;i++)
     { Elem[i].lire_coord(&x,&y,&z);
       fprintf(pf,"%f\t%f\t%f\n",x,y,z);
     }
    fclose(pf);
    return 1;
}
int REZO::charger_pos()
{
    FILE *pf;
    char tamp[64],tampbis[64],*pt;
    float x,y,z;
    int i;

    pf = fopen("position_sav.txt","r");
    if(!pf) return 0;
    i = 0;
    while(fgets(tamp,84,pf))
     { Ctexte(tamp);
       pt = lire_seq(tamp,tampbis,'\t');
       x = atof(tampbis);
       pt = lire_seq(pt,tampbis,'\t');
       y = atof(tampbis);
       pt = lire_seq(pt,tampbis,'\t');
       z = atof(tampbis);

       Elem[i++].fixer_coord(x,y,z);
     }
    fclose(pf);
    return 1;
}
int REZO::sortir_probeset(int a)
{
    FILE *pf;
    int i;

    for(i=0;i<n_elem;i++) Elem[i].fixer_lu(0);

    pf = fopen("e:/Programmes/reseau/rezo.txt","w");
    if(!pf) return 0;

    Elem[a].sortir_h3(pf,0);

    fclose(pf);
    return 1;
}
int REZO::sortir_probeset_cytoscape(int a)
{
    FILE *pf;
    int i;

    for(i=0;i<n_elem;i++) Elem[i].fixer_lu(0);


    pf = fopen("e:/Programmes/reseau/rezo.sif","w");
    if(!pf) return 0;

    Elem[a].sortir_cytoscape(pf,0);

    fclose(pf);
    return 1;
}
int REZO::activer_all_links()
{
    int i;

    for(i=0;i<n_lien;i++) Lien[i].activer();
    return 1;
}
int REZO::desactiver_all_links()
{
    int i;

    for(i=0;i<n_lien;i++) Lien[i].desactiver();
    return 1;
}
int REZO::activer_seuil_sup(int a)
{
    int i;

    for(i=0;i<n_lien;i++)
     if(Lien[i].get_force() > a)  Lien[i].activer();

    return 1;
}
int REZO::activer_seuil_inf(int a)
{
    int i;

    for(i=0;i<n_lien;i++)
     if(Lien[i].get_force() < a)  Lien[i].activer();

    return 1;
}
int REZO::activer_best_vois()
{
    REZO_LIEN *pl,*plmax;
    int i,fmax,f;

    for(i=0;i<n_elem;i++)
     { pl = Elem[i].get_lien();
       if(pl)
        { f = 0;
          plmax = NULL;
          while(pl)
           { f = pl->get_force();
             if(f > fmax)
              { fmax = f;
                plmax = pl;
              }
             pl = pl->get_suiv();
           }
          if(plmax) plmax->activer();
        }
     }
    return 1;
}
int REZO::creer_liens_sup()
{
    int i;

    for(i=0;i<n_lien;i++) Lien[i].activer_seuil_force(4000);
    return 1;
}
int REZO::compter_liens_actifs()
{
    REZO_LIEN *pl;
    int i,j,k;

    k = 0;
    for(i=0;i<n_lien;i++)
     { j = Lien[i].is_actif();
       if(j) k++;
     }
    return k;
}

int REZO::selection_probeset(char *s)
{
    int i,k;

    k = 0;
    for(i=0;i<n_elem;i++) k += Elem[i].tester(s);

    return k;
}
int REZO::deselect_all()
{
    int i,k;

    k = 0;
    for(i=0;i<n_elem;i++) k += Elem[i].fixer_type(0);

    return k;
}
int REZO::prep_filtre(char *requete,int *filtre)
{
    int i,j,k;

    i=k=0;
    while(i<n_elem)
     { j = Elem[i].tester(requete);
       if(j)
        { filtre[i] = 1;
          k++;
        }
       else filtre[i] = 0;
       i++;
     }
    return k;
}

int REZO::preparer()
{

    charger();
    desactiver_all_links();
    activer_seuil_sup(2500);
    activer_seuil_inf(-1500);

    return n_elem;
}

