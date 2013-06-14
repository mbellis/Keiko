/*
    Le sous reseau
    
    demande une liste de points a gerer!
    

*/

#define B_Rayon_sphere 0.4
#define B_Facettes 4
#define B_Horizon 30
#define B_Dseuil 2000


class REZO_2{ public:
    REZO_LIEN Lien[6000000];
    REZO_ELEM Elem[13000];
    REZO_ELEM *plecture;    
    int index[32000];
    int index_inv[32000];
    int act_lien;   
    int n_elem;
    int n_lien;
    int initier();
    REZO_ELEM* transferer(int a);
    int preparer();
    int sauver();
    //***************/
    int lier_puce(RAM_Puce *pu);
    int selection_probeset(char *s);
    int deselect_all();
    int preparer_interne();
    int preparer_voisin();
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
    
class UNIV_2 {
    REZO_2 *prezo;
    REZO_ELEM *pla;
    REZO_LIEN *pli;
    REZO_ELEM *planete[16000];
    float depart[16000][3]; // copie de REZO :: acces rapide!
    float neo[16000][3];
    float aff_pos[16000][3];
    double force[16000][3];
    double forceI[16000][3];
    float mouv[16000][3];
    float camera[3];
    float pas,npas;
    float msoleil,mrepulsion,minter;
    char texte[16000][24];
    int n_planete,etap,sub_etap,memo;
    int act_lien,dum;
    int lien_seuil,neg_lien_seuil;
    int parsec[16000][3];// division de l'espace => voisins!
    int brume[16000];
    int sel[16000],act_sel,nsel;
    int repulsif[16000];
    int caractere[16000];
    public:
    int flag_txt;
    int idcamera;
    double energie[2]; // 0 depart  // 1 arrivee
    /*******/
    int initier(REZO_2 *pr);
    int prep_texte();
    int sauver_gml();
    /********/
    int copier_arrivee();
    int prep_parsec();
    int do_cycle(int a);
    double calc_parsec(int a);
    double calc_inter(int a);
    double calc_force();
    int bouger();
    int affichage();
    int aff_select();
    int aff_grafik();
    int act_marcher(int a);
    int activer_lien(int a);
    int marcher();
    float modifier_pas(float a);
    double calc_energie();
    double calc_energie2();
    int fixe_aff_pos(int a,float ax,float ay,float az);
    int prep_selection();
    int act_selection(); 
    int neo_lien_seuil(int a);
    };
  
    
/***********************************************************************/
int REZO_2::initier()
{
    plecture = NULL;
    n_elem = n_lien = 0;
    return n_elem;
}
REZO_ELEM* REZO_2::transferer(int a)
{
    if(a<n_elem) return &Elem[a];
    return NULL;    
}
int REZO_2::sauver()
{
    FILE *pf;
    int i;
    
    //pf = fopen("e:/Data/Puces/puceMu11K/cpp/rezomodec3p.txt","w");
    //pf = fopen("e:/Data/Puces/puceMu11K/cpp/rezomod01p.txt","w");
    //pf = fopen("e:/Data/Puces/puceU95A/cpp/rezoall.txt","w");
    pf = fopen("rezopart.txt","w");
    if(!pf) return 0;
    
    for(i=0;i<n_elem;i++) 
      Elem[i].sauver(pf);
    
    fclose(pf);    
    return 1;
} 
int REZO_2::sortir_probeset(int a)
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
int REZO_2::sortir_probeset_cytoscape(int a)
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
int REZO_2::activer_all_links()
{
    int i;
    
    for(i=0;i<n_lien;i++) Lien[i].activer();
    return 1;
}
int REZO_2::desactiver_all_links()
{ 
    int i;
    
    for(i=0;i<n_lien;i++) Lien[i].desactiver();
    return 1;
}
int REZO_2::activer_seuil_sup(int a)
{
    int i;
    
    for(i=0;i<n_lien;i++) 
     if(Lien[i].get_force() > a)  Lien[i].activer();
     
    return 1;   
}
int REZO_2::activer_seuil_inf(int a)
{
    int i;
    
    for(i=0;i<n_lien;i++) 
     if(Lien[i].get_force() < a)  Lien[i].activer();
     
    return 1;   
}
int REZO_2::activer_best_vois()
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
int REZO_2::creer_liens_sup()
{
    int i;
    
    for(i=0;i<n_lien;i++) Lien[i].activer_seuil_force(4000);
    return 1;
}
int REZO_2::compter_liens_actifs()
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
int REZO_2::certifier_aller_retour()
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
int REZO_2::selection_probeset(char *s)
{
    int i,k;
    
    k = 0;
    for(i=0;i<n_elem;i++) k += Elem[i].tester(s);
    
    return k;
}
int REZO_2::deselect_all()
{
    int i,k; 
    
    k = 0;
    for(i=0;i<n_elem;i++) k += Elem[i].fixer_type(0);
    
    return k;
}
int REZO_2::lier_puce(RAM_Puce *pu)
{
    FILE *pf;
    char tamp[84];
    PROBESET *pr;
    int i,num;
   
    
    //pf = fopen("Mus11probe.txt","r");
    pf = fopen("U95Aprobe.txt","r");
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
int REZO_2::preparer_voisin()
{ // cherche les liens sur 1 etap et les ajoute a la liste!
    int liste_sav[32000];
    REZO_LIEN *pl1,*pl2;
    REZO_ELEM *pe1,*pe2;
    int i,j,k,z,k_lien;
    
    for(i=0;i<Rezo.n_elem;i++) liste_sav[i] = Liste_sel[i];
    
    for(i=0;i<Rezo.n_elem;i++)
     { index[i] = -1;
       if(Liste_sel[i] == 1)
        { pl1 = Rezo.Elem[i].get_lien();
          while(pl1)
           { pl1->lire(&k,&z);
             if(k)
              { pe1 = pl1->get_elem();
                j = pe1->lire_id();
                if(!Liste_sel[j]) Liste_sel[j] = 2;
              }
             pl1 = pl1->get_suiv(); 
           }
        }
     }     
    preparer_interne();
    
    for(i=0;i<Rezo.n_elem;i++) Liste_sel[i] = liste_sav[i];
    return 1;
}
int REZO_2::preparer_interne()
{ // seul les probeset de la liste !
    REZO_LIEN *pl1,*pl2;
    REZO_ELEM *pe1,*pe2;
    int i,j,k,z,k_lien;
    
    n_elem = n_lien = 0;
    
    for(i=0;i<Rezo.n_elem;i++)
     { index[i] = -1;
       if(Liste_sel[i])
        { index_inv[n_elem] = i; 
          index[i] = n_elem++;
        }
     }
    for(i=0;i<Rezo.n_elem;i++)
     { if(Liste_sel[i])
        { Elem[index[i]].copier(&Rezo.Elem[i]);
          Elem[index[i]].fixer_id(index[i]); // id relative !!
          // mis a jour des liens
          pl1 = Rezo.Elem[i].get_lien();
          k_lien = 0;
          while(pl1)
           { pe1 = pl1->get_elem();
             j = pe1->lire_id();
             if(Liste_sel[j])
              { pl1->lire(&k,&z);
               if(k_lien) Lien[n_lien-1].lier(&Lien[n_lien]);
               else Elem[index[i]].fixer_lien(&Lien[n_lien]);
               Lien[n_lien++].charger(&Elem[index[j]],k,1);
               k_lien++; 
              }
             pl1 = pl1->get_suiv(); 
           }
        }
     }
    sprintf(Messager2," %d ->  %d liens\0",n_elem,n_lien);
    return n_elem;
}      
        
                    
                
/***********************************************************************/
int UNIV_2::initier(REZO_2 *pr)
{
    REZO_ELEM *po;
    float ix,iy,iz;
    int i;
    
    prezo = pr;
    i = n_planete = 0;    
    po = prezo->transferer(n_planete);
    while(po)
     { //if(liste[i++]) 
       planete[n_planete++] = po;
       po = prezo->transferer(n_planete);
     }  
    prep_texte(); 
      
    srand((unsigned int) time(NULL));
    for(i=0;i<n_planete;i++)
     { neo[i][0] = (float)((rand() % 600) - 300)/4.0;
       neo[i][1] = (float)((rand() % 600) - 300)/4.0;
       //neo[i][2] = (float)((rand() % 600) - 300)/4.0;
       neo[i][2] = 0;
       caractere[i] =  planete[i]->lire_type();   
     }
    /********************************/ 
    lien_seuil = 3200; 
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(B_Dseuil-lien_seuil);
  
    for(i=0;i<n_planete;i++)
      force[i][0] = force[i][1] = force[i][2] = 0.0;      
    copier_arrivee();
          
    /**************/
    act_sel = 0;
    flag_txt = 1;
    prep_selection();
    /**************/
    fixe_aff_pos(-1,0,0,0);
    
    msoleil = 10;
    mrepulsion = 0.01; 
    minter = 1;
    etap = sub_etap = 0;
    pas = 40.0;
    npas = -40.0;
    return n_planete;
}
int UNIV_2::prep_texte()
{
    PROBESET *pr;
    char tamp[4096];
    int i;
    
    for(i=0;i<n_planete;i++)
     { pr = planete[i]->get_probeset();
       pr->lire_d(5,tamp);
       tamp[25] = 0;
       copie_string ("-\0",texte[i]);
       copie_string(tamp,texte[i]+1);
     }
    return 1;
}
/************/
int UNIV_2::copier_arrivee()
{
    int i; 
    
    for(i=0;i<n_planete;i++)
     { //planete[i]->fixer_coord(neo[i][0],neo[i][1],neo[i][2]);  
       depart[i][0] = neo[i][0];
       depart[i][1] = neo[i][1];
       depart[i][2] = neo[i][2];
       mouv[i][0] = force[i][0] + forceI[i][0];
       mouv[i][1] = force[i][1] + forceI[i][1];
       mouv[i][2] = force[i][2] + forceI[i][2];
     }   

    return 1;
}    
int UNIV_2::prep_parsec()
{
     int i;
    
     for(i=0;i<n_planete;i++)
      { parsec[i][0] = (int) neo[i][0];
        if(neo[i][0]<0) parsec[i][0] --;
        parsec[i][1] = (int) neo[i][1];
        if(neo[i][1]<0) parsec[i][1] --;
        parsec[i][2] = (int) neo[i][2];
        if(neo[i][2]<0) parsec[i][2] --;
      }
     return 1;
}
double UNIV_2::calc_parsec(int a)
{ // cacul sur les Horizon parsecs autour de a!
    double d,dx,dy,dz,e;
    int i,px,py,pz;
    int zx,zy,zz;
                
    px = parsec[a][0];
    py = parsec[a][1];
    pz = parsec[a][2];
    for(i=0;i<n_planete;i++)
    if(repulsif[i])
     { zx = abs(parsec[i][0] - px);
       zy = abs(parsec[i][1] - py);
       zz = abs(parsec[i][2] - pz);
       if(zx <= B_Horizon)
        if(zy <= B_Horizon)
         if(zz <= B_Horizon)
          if(i != a)
           { dx = neo[a][0]-neo[i][0];
             dy = neo[a][1]-neo[i][1];
             dz = neo[a][2]-neo[i][2];
             d = (dx * dx)+(dy * dy)+(dz * dz);
             d = sqrt(d);
             e = Repulsion.lire_rep(d);
             if(e)
              { e *= mrepulsion;
                force[a][0] += e * (dx/d);
                force[a][1] += e * (dy/d);
                force[a][2] += e * (dz/d);
              }
           }   
     }
   return e; 
}
double UNIV_2::calc_inter(int a)
{
    double d,dx,dy,dz,e,sume;
    int i,b; 
   
    forceI[a][0]=forceI[a][1]=forceI[a][2]=0.0;
    repulsif[a] = 0;
     
    sume = e = 0.0;
    pli = planete[a]->get_lien();
    while(pli)
     { pla = pli->get_elem();
       if(pli->is_actif())
       if(pla)
        { b = pla->lire_id(); // le couple a et b!
          dx = neo[b][0]-neo[a][0];
          dy = neo[b][1]-neo[a][1];
          dz = neo[b][2]-neo[a][2];
          d = (dx * dx)+(dy * dy)+(dz * dz);
          d = sqrt(d); 
          e = (float) pli->get_force();
          e /= 1000;  
          //e = 0; 
          if(e>0)
           { if(e>6) e = 6;
             //e = Inter.lire(d+e);
             e = (d - (6-e)) * (e*e);
             if(e>500) e = 500;
             if(e<0) e = 0;
           }
          if(e<0)
           { if(e<-8) e = -8;
             if(d < 0.1) d = 0.1;
             e = 800*e/d;
             //e /= 1; // limite la repulsion!!!!
             if(e<-800) e = -800;             
           }
          repulsif[a] = 1;
          if(e)
            { e *= minter;
              forceI[a][0] += e * (dx/d);
              forceI[a][1] += e * (dy/d);
              forceI[a][2] += e * (dz/d);
              sume += (double)e;
            }
        }  
       pli = pli->get_suiv();
     }
    d = (forceI[a][0]*forceI[a][0]);
    d += (forceI[a][1]*forceI[a][1]);
    d += (forceI[a][2]*forceI[a][2]);
    if(d)
     { e = 0.2*sqrt(d);
       forceI[a][0] /= e;
       forceI[a][1] /= e;
       forceI[a][2] /= e;
     }
    return sume;
}
double UNIV_2::calc_force()
{
    double En,Esun,Erep,Einter;
    float d;
    int i;
    
    En = 0.0;
    for(i=0;i<n_planete;i++)
     { Esun = Erep = Einter = 0.0;     
       // lecture de soleil
       d = neo[i][0]*neo[i][0];
       d += neo[i][1]*neo[i][1];
       d += neo[i][2]*neo[i][2];
       d = sqrt(d);    
       Esun = msoleil*Soleil.lire_sun(d);
       if(Esun < 0) Esun = 0;
       force[i][0] = (-1)*Esun * (neo[i][0]/d);
       force[i][1] = (-1)*Esun * (neo[i][1]/d);
       force[i][2] = (-1)*Esun * (neo[i][2]/d);
       En += Esun;
     }
    //energie[1] = En;
    return En;
}
int UNIV_2::bouger()
{
    float dx,dy,dz;
    int i;
    
    for(i=0;i<n_planete;i++)
     { dx = (mouv[i][0]);
       dy = (mouv[i][1]);
       dz = (mouv[i][2]);
       if(dx>pas) dx = pas;
       if(dx<npas) dx = npas;
       if(dy>pas) dy = pas;
       if(dx<npas) dy = npas;
       if(dz>pas) dz = pas;
       if(dx<npas) dz = npas;
       neo[i][0] = depart[i][0] + dx;
       neo[i][1] = depart[i][1] + dy;
       neo[i][2] = depart[i][2] + dz;
     }
    return n_planete; 
}
float UNIV_2::modifier_pas(float a)
{
    pas *= a;
    npas *= a;
    return pas;
}
int UNIV_2::do_cycle(int a)
{
    int encore;
    // position de depart!
    pas = 10.0;
    npas = -10.0;
    //copier_depart(); // deja fait en initier()
    prep_parsec();
    energie[1] = calc_force();
    copier_arrivee();
   
    /*****************************/    
   // aff_grafik();
    /*****************************/
     energie[0] = energie[1];
    
    // mouvement !
    etap = 0;
    encore = 1;
    while(encore)
     { encore = 0;
       Sleep(500);
       etap++;
       bouger();
       prep_parsec();
       energie[1] = calc_force();
       //printf("> %d (%4.1f):Nouvelle Energie = %8.3f \t depuis %8.3f\n",etap,pas,energie[1]/1000,energie[0]/1000); 
       if(energie[1] < energie[0]) 
        { copier_arrivee();
          /*****************************/    
          // aff_grafik();
          /*****************************/
          energie[0] = energie[1];
          encore = 1;
        }
       else
        { pas = pas/2.0;
          npas = npas/2.0; 
          encore = 1;
        } 
       if(etap>550) encore = 0; 
       if(pas<0.01) encore = 0;
     }
    return 1;
} 
/*********************************************/
double UNIV_2::calc_energie()
{
    double a,b;
    int i;
    
    b = 0.0;
    for(i=0;i<n_planete;i++)
     { a = (force[i][0]+forceI[i][0])*(force[i][0]+forceI[i][0]);
       a += (force[i][1]+forceI[i][1])*(force[i][1]+forceI[i][1]);
       a += (force[i][2]+forceI[i][2])*(force[i][2]+forceI[i][2]);
       b += sqrt(a); 
     }
     return b;
}
double UNIV_2::calc_energie2()
{
    double a,b;
    int i;
    
    b = 0.0;
    for(i=0;i<n_planete;i++)
     { a = forceI[i][0]*forceI[i][0];
       a += forceI[i][1]* forceI[i][1];
       a += forceI[i][2]*forceI[i][2];
       b += sqrt(a); 
     }
     return b;
}
int UNIV_2::act_selection()
{
    act_sel = act_sel^1;
    return act_sel;
}
int UNIV_2::prep_selection()
{
    int i;
    
    nsel = 0;
    for(i=0;i<n_planete;i++)
     { sel[i] = planete[i]->lire_type();
       if(sel[i]) nsel++;
     }
    return 1; 
}
int UNIV_2::activer_lien(int a)
{ // activer l'affichage des liens!
    if(!a) 
     { act_lien = a;
       return act_lien;
     }
    if(a == 2)
     { if(act_lien != 2) act_lien = 2;
       else act_lien = 0;
       return act_lien;     
     }
    act_lien = act_lien^1;
    return act_lien; 
}
int UNIV_2::act_marcher(int a)
{ // activer la fonk marcher!
    if(!a) 
     {  sub_etap = 0;
        return 0;
     } 
    if(a == -1) 
     {  etap = sub_etap = 0;
        return 0;
     } 
    sprintf(Messager2,"etap = %d",sub_etap + a);
    return ++sub_etap;
}
int UNIV_2::marcher()
{
    float ls;
    int i;
    
    if(!sub_etap) return 0; // pas d'action!!!!
    
    if(!etap) // initialisation 
     { if(sub_etap == 1) // prepare parsec
        { prep_parsec();
          energie[0] = energie[1] = 0.0;
          sub_etap++;
          /************/
          prezo->activer_seuil_sup(lien_seuil);
          neg_lien_seuil = B_Dseuil-lien_seuil;
          if(neg_lien_seuil > -1300) neg_lien_seuil = -1300;
          prezo->activer_seuil_inf(neg_lien_seuil);
          /*************/
          return sub_etap;
        }
       if(sub_etap == 2) // calcule depuis soleil
        { energie[0] += calc_force();
          sub_etap++;
          memo = 0;
          return sub_etap;
        }
       if(sub_etap == 3) // calcule inter
        { for(i=memo;i<n_planete;i++)
           { energie[0] += calc_inter(i);
             if(i>memo+600)
              { memo = i+1;
                return sub_etap;
              }
           }      
          sub_etap ++;
          memo = 0;
          return sub_etap;
        } 
       if(sub_etap == 4) // calcule depuis parsec
         { for(i=memo;i<n_planete;i++)
           { energie[0] += calc_parsec(i);
             if(i>memo+600)
              { memo = i+1;
                return sub_etap;
              }
           }
           sub_etap++;
           memo = 0;
           return sub_etap;
         }
       if(sub_etap == 5) // energie?
         { energie[0] = calc_energie();
           energie[1] = 0.0;
           sub_etap = 1;
           etap++;
           return sub_etap;
         }
      } // fin de if(!etap) // initialisation 
     if(etap) // force deja determinee!
      { if(sub_etap == 1) // prepare parsec
         { bouger();
           sub_etap++;
           return sub_etap;
         }
        if(sub_etap == 2) // prepare parsec
         { prep_parsec();
           sub_etap++;
           return sub_etap;
         }
        if(sub_etap == 3) // prepare parsec
         { energie[1] += calc_force();
           memo = 0;
           sub_etap++;
           return sub_etap;
         }
        if(sub_etap == 4) // prepare inter
         { for(i=memo;i<n_planete;i++)
           { energie[1] += calc_inter(i);
             if(i>memo+600)
              { memo = i+1;
                return sub_etap;
              }
           }
           sub_etap++;
           memo = 0;
           return sub_etap;
         }
        if(sub_etap == 5) // prepare parsec
         { for(i=memo;i<n_planete;i++)
           { energie[1] += calc_parsec(i);
             if(i>memo+600)
              { memo = i+1;
                return sub_etap;
              }
           }
           sub_etap++;
           memo = 0;
           return sub_etap;
         }
        if(sub_etap == 6) // energie?
         { if(lien_seuil>6) 
             energie[1] = calc_energie();
           else
             energie[1] = calc_energie();
           copier_arrivee();
           fixe_aff_pos(idcamera,0,0,0);
           if(energie[1]>=energie[0])
            { pas /= 2;
              npas /= 2;
              if(pas < 0.1) //sub_etap = -1;
               { ls = 0.90*(float)(lien_seuil);
               if(ls < 100)
                 //if(ls < 1400)
                 //if(ls < 5000)
                  { sub_etap = 0;
                    return sub_etap;
                  }
                 lien_seuil = (int) ls;
                 prezo->activer_seuil_sup(lien_seuil);
                 neg_lien_seuil = B_Dseuil-lien_seuil;
                 if(neg_lien_seuil > -1300) neg_lien_seuil = -1300;
                 prezo->activer_seuil_inf(neg_lien_seuil);
                 pas = 20;
                 npas = -20;
               } 
            }
           energie[0] = energie[1];
           energie[1] = 0.0;
           etap++;
           //return sub_etap;
         }
      
      }
      
     sub_etap = 1;
     //if(etap > 200) sub_etap = 0;
     return sub_etap;
}    
int UNIV_2::sauver_gml()
{
    FILE *pf;
    char tamp[512];
    REZO_LIEN *pli;
    REZO_ELEM *pla;
    int i,j,k;
    
    pf = fopen("sortie.gml","w");
    if(!pf) return 0;  
    
    fprintf(pf,"Version\t1.0\n");
    fprintf(pf,"graph\t[\n");
    for(i=0;i<n_planete;i++) 
     { fprintf(pf,"\tnode\t[\n");
       //fprintf(pf,"\t\troot_index\t-%d\n",i+1);
       fprintf(pf,"\t\tid\t-%d\n",i+1);      
       fprintf(pf,"\t\tgraphics\t[\n");
       fprintf(pf,"\t\t\tx\t%f\n",neo[i][0]);
       fprintf(pf,"\t\t\ty\t%f\n",neo[i][1]);
       fprintf(pf,"\t\t\tw\t%4.1f\n",6.0);
       fprintf(pf,"\t\t\th\t%4.1f\n",6.0);
       fprintf(pf,"\t\t\tfill\t\"#ff9999\"\n");
       fprintf(pf,"\t\t\ttype\t\"ellipse\"\n");
       //fprintf(pf,"\t\t\toutline\t\"#000000\"\n");
       //fprintf(pf,"\t\t\toutline_width\t1.0\n");
       fprintf(pf,"\t\t]\n");
       fprintf(pf,"\t\tlabel\t\"%s_%d\"\n",texte[i]+1,i+1);
       fprintf(pf,"\t]\n");
              
     }  
    k = 0;
    for(i=0;i<n_planete;i++) 
     { pli = planete[i]->get_lien(); 
       while(pli)
         { pla = pli->get_elem();
           j = pla->lire_id();
           if(pli->is_actif())
           if(pli->get_force() > 0) // lien positifs!!
           if(j > i)
            { fprintf(pf,"\tedge\t[\n");
             // fprintf(pf,"\t\troot_index\t-%d\n",k++);
              fprintf(pf,"\t\tsource\t-%d\n",i+1);
              fprintf(pf,"\t\ttarget\t-%d\n",j+1);
              fprintf(pf,"\t\tlabel\t\"pp\"\n");  
              fprintf(pf,"\t]\n");
            }
            pli = pli->get_suiv();
         }  
     }  
    fprintf(pf,"]\n");      
    fclose(pf);
    
    return 1;
}
 /******************************************************************************/
int UNIV_2::neo_lien_seuil(int a)
{
    lien_seuil += a;
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(B_Dseuil-lien_seuil);
    return 1;
} 
int UNIV_2::fixe_aff_pos(int a,float ax,float ay,float az)
{
    double zx,zy,zz,lim;
    int i;
    
    if(a > 0)
     if(a < n_planete)
      { camera[0] = depart[a][0];
        camera[1] = depart[a][1];
        camera[2] = depart[a][2];
        idcamera = a;
        Univers.fixe_aff_pos(prezo->index_inv[a],0.0,0.0,0.0);
      } 
    if(a == -1)
     { camera[0] = ax;
       camera[1] = ay;
       camera[2] = az;
       idcamera = -1;
       for(i=0;i<n_planete;i++) 
        { brume[i] = 0;
          aff_pos[i][0] = depart[i][0]-camera[0];
          aff_pos[i][1] = depart[i][1]-camera[1];
          aff_pos[i][2] = depart[i][2]-camera[2];
        }  
       return a;
     }
    if(idcamera == -1) return a;
    
    lim = (double) Horizon; 
    lim = lim*lim * 2;
    for(i=0;i<n_planete;i++)
     { aff_pos[i][0] = depart[i][0]-camera[0];
       aff_pos[i][1] = depart[i][1]-camera[1];
       aff_pos[i][2] = depart[i][2]-camera[2];
       zx = aff_pos[i][0] * aff_pos[i][0];
       zy = aff_pos[i][1] * aff_pos[i][1];
       zz = aff_pos[i][2] * aff_pos[i][2];
       zx = zx + zy + zz;
       brume[i] = 1;
       if(zx < lim) brume[i] = 0;
     }
    return a;
}
int UNIV_2::aff_grafik()
{ // simple affichage de depart[]!
    int i,a,b;
    if(etap)
     { if(energie[0] < 10000)
        sprintf(Messager2,"E(%d-%d)(%d / %4.2f) = %8.1lf [%d]\0",etap,sub_etap,lien_seuil,pas,energie[0],idcamera);
       else
        sprintf(Messager2,"E(%d-%d)(%d / %4.2f) = %8.1lfK [%d]\0",etap,sub_etap,lien_seuil,pas,energie[0]/1000,idcamera);
     }
    if(act_sel) sprintf(Messager2,"sel : %d (%d)",idcamera+1,nsel);
        
    for(i=0;i<n_planete;i++)
     { //if(!brume[i])
       if((sel[i])||(!act_sel))
       { glTranslated(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
         if(caractere[i]) glColor3d(0.6,0.2,0.2);
         if(!caractere[i]) glColor3d(0.4,0.7,1); 
         glutSolidSphere(Rayon_sphere,Facettes,Facettes); 
         if(flag_txt)
         if(texte[i]) glutEcrire(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2],texte[i]);       
         glTranslated(-1*aff_pos[i][0],-1*aff_pos[i][1],-1*aff_pos[i][2]);
       }  
     }
    if(etap)
     if(voir_force)
     { glColor3d(0.6,0.2,0.2);
       glBegin(GL_LINES);
       for(i=0;i<n_planete;i++)
        { glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
          glVertex3f(aff_pos[i][0]+force[i][0],aff_pos[i][1]+force[i][1],aff_pos[i][2]+force[i][2]);       
        }
       glEnd (); 
       glColor3d(0,1,1);
       glBegin(GL_LINES);
       for(i=0;i<n_planete;i++)
        { glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
          glVertex3f(aff_pos[i][0]+forceI[i][0],aff_pos[i][1]+forceI[i][1],aff_pos[i][2]+forceI[i][2]);       
        }
       glEnd ();
     }  
          
    if(act_lien == 1) // tous les liens activés de idcamera
    if(idcamera >= 0)
     { glColor3d(0,1,1);
       glBegin(GL_LINES);
       i = idcamera;
       pli = planete[i]->get_lien();
          while(pli)
          { if(pli->is_actif()) 
            //if(i)
             { pla = pli->get_elem();
               dum = pla->lire_id();
               if(pli->get_force()> 0) 
                { glColor3d(0,1,0);
                  glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
                  glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
                }
               else 
                { //nop();
                  glColor3d(1,0,0);
                  glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
                  glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
                }
             }
            pli = pli->get_suiv(); 
          }
       glEnd ();
     }
    if(act_lien == 2) // tous les liens activés 
     { glColor3d(0,1,1);
       glBegin(GL_LINES);
       for(i=0;i<n_planete;i++)
       //if(act_sel)
       //if(sel[i])
       if(1)
        { pli = planete[i]->get_lien();
          while(pli)
          { if(pli->is_actif()) 
             { pla = pli->get_elem();
               dum = pla->lire_id();
               if(pli->get_force()> 0) 
                { glColor3d(0,1,0);
                  glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
                  glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
                }
               else 
                { nop();
                  /*glColor3d(1,0,0);
                  glVertex3f(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
                  glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]); */
                }
             }
            pli = pli->get_suiv(); 
          }
        }
       glEnd ();
     }
    /*************************************************/
    if(CChemin.is_actif()) // tous les chemins activés 
     { glColor3d(1,0,0);
       glBegin(GL_LINES);
       for(i=0;i<CChemin.n_chemin;i++)
       if(CChemin.chemin[i].activer(-1))
        { dum = 0;
          a = CChemin.chemin[i].get_marche(dum++);
          b = CChemin.chemin[i].get_marche(dum++);
          while(b >= 0)
            { glVertex3f(aff_pos[a][0],aff_pos[a][1],aff_pos[a][2]);
              glVertex3f(aff_pos[b][0],aff_pos[b][1],aff_pos[b][2]);
              a = b;
              b = CChemin.chemin[i].get_marche(dum++);
            }
        }
       glEnd();
     }  
    /*************************************************/
    if(Dregul.affichage)
     { 
       sprintf(Messager,"n_fast_aff : %d",Dregul.n_fast_aff);
       glBegin(GL_LINES);
       for(i=0;i<Dregul.n_fast_aff;i++)
        { dum = Dregul.fast_aff[i][0];
          if(Dregul.fast_aff[i][1] == 1) 
           { glColor3d(0,0,1);
             glVertex3f(-camera[0],-camera[1],-camera[2]);
             glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
           }
          if(Dregul.fast_aff[i][1] == 2) 
           { glColor3d(1,0,0);
             glVertex3f(-camera[0],-camera[1],-camera[2]);
             glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
           }
         /* if(Dregul.fast_aff[i][1] == 3) 
           { glColor3d(0,1,0);
             glVertex3f(-camera[0],-camera[1],-camera[2]);
             glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);
           } */
        }
       glEnd();
     }
    /*************************************************/
    return 1;
}
int UNIV_2::affichage()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*************************************/
    glColor3d(1,1,1);
    if(Messager[0]) glutEcrire(-20,20,-50,Messager2);
  
    /*************************************/
        
    glPushMatrix();   
     glTranslated(profondeur2[0],profondeur2[1],profondeur2[2]);
     glRotated(angle2[0],1,0,0);
     glRotated(angle2[1],0,1,0);
     glRotated(angle2[2],0,0,1);  
     if(Helios)
      { glColor3d(1,0,0);  
        glutSolidSphere(1,20,20);
      }
     glColor3d(0.4,0.7,1); 
     aff_grafik();
    glPopMatrix();


    glutSwapBuffers();
}
int UNIV_2::aff_select()
{
    GLint i;

    glPushMatrix();   
     glTranslated(profondeur2[0],profondeur2[1],profondeur2[2]);
     glRotated(angle2[0],1,0,0);
     glRotated(angle2[1],0,1,0);
     glRotated(angle2[2],0,0,1);  
     for(i=0;i<n_planete;i++)
     if((sel[i])||(!act_sel))
      { glLoadName(i);
        glTranslated(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
        glutSolidSphere(Rayon_sphere,Facettes,Facettes);        
        glTranslated(-1*aff_pos[i][0],-1*aff_pos[i][1],-1*aff_pos[i][2]);  
      }
    glPopMatrix();
    
    return 1;
}
   
