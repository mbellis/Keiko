/*
Creation et presentation du reseau de probeset
membre de glutrezo



*/ 
/*
#define MaxProbe 13000
#define MaxLink 28000000
*/

char Rezoin[128];
char Rezoout[128];
char Rezoout_pos[128];
char Rezoprobindex[128];


class REZO_ELEM; 

class REZO_LIEN{ friend class REZO_ELEM;
    REZO_ELEM *pe;
    REZO_LIEN *suiv;
    int force;
    int actif;
    public:
    int creer(REZO_ELEM *pa,int fa);
    int lier(REZO_LIEN *pa);
    REZO_LIEN* lire(int *fa,int *aa);
   // REZO_LIEN* lire_actif(int *fa,int *aa);
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
class REZO_ELEM{
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
    int sauver_pos(FILE *pf);
    int charger(int a,int b);
    REZO_LIEN* aj_lien(REZO_LIEN *pdef,REZO_ELEM *pe,int fa,int aa);
    REZO_LIEN* get_lien();
    PROBESET* get_probeset();
    int get_voisin(int *a);
    int sortir_h3(FILE *pf,int a);
    int sortir_cytoscape(FILE *pf,int a);
    int tester(char *s);
    /************************/
    int copier(REZO_ELEM *pre);
    };     
 
class REZO{ public:
    REZO_LIEN Lien[MaxLink];
    REZO_ELEM Elem[MaxProbe];
    REZO_ELEM *plecture;
    int n_elem;
    int n_lien;
    int act_lien;
   // public:
    int initier();
    int agreger();
    int sauver();
    int charger();
    REZO_ELEM* transferer(int a);
    int preparer();
    int sauver_pos();
    int charger_pos();
    //***************/
    int lier_puce(RAM_Puce *pu);
    int selection_probeset(char *s);
    int deselect_all();
    int prep_filtre(char *requete,int *filtre);
    /******************/
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

/************************************/
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
/************************************/
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
    REZO_LIEN *pl;
    REZO_ELEM *pa;
    int nid,fo,fac;
    
    fprintf(pf,">%d\t%d\t%6.3f\t%6.3f\t%6.3f\n",id,type,x,y,z);
    pl = lien;
    
    while(pl)
     { pa = pl->get_elem();
       pl = pl->lire(&fo,&fac); 
       nid = pa->lire_id();
       fprintf(pf,"%d\t%d\t%d\n",nid,fo,fac);
    }

    return 1;
}
int REZO_ELEM::sauver_pos(FILE *pf)
{
    REZO_LIEN *pl;
    REZO_ELEM *pa;
    int nid,fo,fac;
    
    fprintf(pf,">%d\t%d\t%6.3f\t%6.3f\t%6.3f\n",id,type,x,y,z);
    
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
    // nouveau Lien!
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
    //sprintf(Messager,"***%s",s);
    return type;
}

/************************************/
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
    
    pf = fopen(Rezoout,"w");
  
    if(!pf) return 0;
     
    for(i=0;i<n_elem;i++) 
      Elem[i].sauver(pf);
    
    fclose(pf); 
    sauver_pos();   
    return 1; 
} 
int REZO::charger()
{
    FILE *pf;
    char tamp[84];
    char tamp2[24],*pt;
    int i,j,k,k_lien;
    float ax,ay,az; 
    
   
    //pf = fopen("g:/puce50/U95nrezo.txt","r");
    //pf = fopen("g:/puce50/U95rezomodec3.txt","r");
    //pf = fopen("g:/puce50/gcomp67.txt","r");
    //pf = fopen("g:/rezonored/gminirezo.txt","r");
    //pf = fopen("g:/travaux/U133/grezo.txt","r");
    //pf = fopen("g:/puce50/gcompmodec3.txt","r");
    //pf = fopen("g:/puce/U95grezomodec3.txt","r");
    //pf = fopen("g:/bellis_short/bellis_rezo.txt","r");
    //pf = fopen("g:/bellis_short/HS54K01_S99_CR1_CL20_AR1_AL20_V02.txt","r");
    //pf = fopen("g:/bellis_short/rezomodec3.txt","r");
    pf = fopen(Rezoin,"r");
    if(!pf) return 0;
     
    // securité predefinition des planetes!!
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
          //Elem[n_elem-1].ecrire();
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
    
     pf = fopen(Rezoout_pos,"w");
    if(!pf) return 0;
     
    for(i=0;i<n_elem;i++) 
      Elem[i].sauver_pos(pf);
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
int REZO::certifier_aller_retour()
{
    REZO_ELEM *pa1,*pa2;
    REZO_LIEN *pl1,*pl2,*pl3;
    int i,j,k;
    
    k = 0;
    for(i=0;i<n_elem;i++) 
     { pa1 = &Elem[i];
       pl1 = pa1->get_lien();
       while(pl1)
        { if(pl1->is_actif())
           { pa2 = pl1->get_elem();
             pl2 = pa2->get_lien();
             if(pl2)
             { pl3 = pl2->chercher(pa1);
               if(pl3) pl3->activer();
               else
                { //pl3 = pa2->aj_lien(&Lien[n_lien],pa1,0,1);
                  //n_lien++;
                  //ATTENTION PAS ENCORE FAIT!!!!!
                }
             }
           }
          pl1 = pl1->get_suiv();
        }    
     }
    return 1;
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
int REZO::lier_puce(RAM_Puce *pu)
{
    FILE *pf;
    char tamp[84];
    PROBESET *pr;
    int i,num;
   
    
    
    //pf = fopen("U95Aprobe.txt","r");
    //pf = fopen("g:/travaux/U133/HU133listeprobe.txt","r");
    pf = fopen(Rezoprobindex,"r");
    
    if(!pf) return 0;
    i = num = 0;
    while(fgets(tamp,84,pf))
     { Ctexte(tamp);
       pr = pu->chercher_probeset(tamp);
       if(pr)
        { Elem[i].fixer_probeset(pr);
          num++;
        }
       i++;
     }
     fclose(pf);
     return num;
}
int REZO::preparer()
{
    charger();
    // Rezo.Elem[61].ecrire();
    //activer_all_links();
    // Rezo.compter_liens_actifs();
    desactiver_all_links();
    //activer_best_vois();
    activer_seuil_sup(2500);
    activer_seuil_inf(-1500);
    // activer_best_vois();
    // Rezo.compter_liens_actifs();
    // Rezo.creer_liens_sup();
    // Rezo.compter_liens_actifs();
   
    //certifier_aller_retour();
    //   act_lien = compter_liens_actifs();
    //Rezo.sortir_probeset(7808);
   // Rezo.sortir_probeset_cytoscape(7808);


      //sauver();



    return n_elem;
}

/**********************************************/
