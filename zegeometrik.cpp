/*



    Determination de la representation du reseau



    Lien Inter : d ====> force * d; : lineaire




*/
#include <time.h>


#define Rayon_sphere 0.4
#define Facettes 4
#define Horizon 6
#define Dseuil 3000
 

class FORCE{
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
class UNIVERS {
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
    float camera[3];
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
    int ancre[MaxProbe];
    float couleur[100][3];
    public:
    int idcamera;
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
    int default_couleur();
    int load_couleur();
    int geler_pos(int a);
};
/*********************************/
FORCE Soleil;
FORCE Repulsion;
FORCE Inter; 
/*********************************/
int FORCE::ouvrir(char *s)
{
    FILE *pf;
    char tamp[64];
    
    pf = fopen(s,"r");
    if(!pf) return 0;
    
    n_va = 0;
    while(fgets(tamp,64,pf)) 
     { Ctexte(tamp);
       //printf("%s %f\n",tamp,atof(tamp));
       va[n_va] = atof(tamp);
       n_va++;
     }
    fclose(pf);
     
    return n_va;
}
int FORCE::fixer_fact(float a)
{
    f_fact = a;
    return 1;
}
float FORCE::lire(int a) 
{   return va[a];
}
float FORCE::lire_rep(float a)
{   // type repulsion : a de 0 à 1!

    if(a>Horizon) return 0.0;
    p_va = (int)(a*f_fact);
    return va[p_va];
}
float FORCE::lire_sun(float a)
{ // type solaire : a de 0 à 10000!
    p_va = (int)(a*10);
    if(p_va>10000) return a; // energie enorme!!!!
    return va[p_va];
}
/*********************************/
int UNIVERS::initer(REZO *pr)
{
    REZO_ELEM *po;
    float ix,iy,iz;
    int i;
    
    prezo = pr;
    n_planete = 0;    
    po = prezo->transferer(n_planete);
    while(po)
     { planete[n_planete++] = po;
       po = prezo->transferer(n_planete);
     }  
      
    srand((unsigned int) time(NULL));
    for(i=0;i<n_planete;i++)
     { neo[i][0] = (float)((rand() % 900) - 450)/4.0;
       neo[i][1] = (float)((rand() % 900) - 450)/4.0;
       neo[i][2] = (float)((rand() % 900) - 450)/4.0;
     }
    /********************************/ 
    lien_seuil = 9000; 
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(Dseuil-lien_seuil);
  
    for(i=0;i<n_planete;i++)
      force[i][0] = force[i][1] = force[i][2] = 0.0;      
    copier_arrivee();
          
    if(!Soleil.ouvrir("soleil.txt")) return 0;
    if(!Repulsion.ouvrir("repulsion.txt")) return 0;
    Repulsion.fixer_fact(10000/(float)(Horizon));
    //if(Inter.ouvrir("inter.txt")) nop();
    /**************/
    act_sel = 0;
    prep_selection();
    load_couleur();
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
int UNIVERS::initer2(REZO *pr)
{
    REZO_ELEM *po;
    float ix,iy,iz;
    int i;
    
    prezo = pr;
    n_planete = 0;
      
    po = prezo->transferer(n_planete);
    while(po)
     { planete[n_planete++] = po;
       po = prezo->transferer(n_planete);
     }   
   
    for(i=0;i<n_planete;i++)
      force[i][0] = force[i][1] = force[i][2] = 0.0;      
          
    if(!Soleil.ouvrir("soleil.txt")) return 0;
    if(!Repulsion.ouvrir("repulsion.txt")) return 0;
    //if(Inter.ouvrir("inter.txt")) nop();
    
     /**************/
    act_sel = 0; 
    prep_selection();
    //default_couleur();
    load_couleur();
    /**************/
    fixe_aff_pos(-1,0,0,0);
    lien_seuil = 9000;
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(Dseuil-lien_seuil);
    
    copier_depart();
    copier_arrivee();
    msoleil = 8;
    mrepulsion = 0.01;
    minter = 1;
    etap = sub_etap = 0;
    pas = 40.0;
    npas = -40.0;
    return n_planete;
}
int UNIVERS::copier_depart()
{
    int i;
    
    for(i=0;i<n_planete;i++)
     { planete[i]->lire_coord(&neo[i][0],&neo[i][1],&neo[i][2]);  
       caractere[i] =  planete[i]->lire_type();    
     }

    return 1;
}
int UNIVERS::copier_arrivee()
{
    int i; 
    
    for(i=0;i<n_planete;i++)
     { planete[i]->fixer_coord(neo[i][0],neo[i][1],neo[i][2]);  
       depart[i][0] = neo[i][0];
       depart[i][1] = neo[i][1];
       depart[i][2] = neo[i][2];
       mouv[i][0] = force[i][0] + forceI[i][0];
       mouv[i][1] = force[i][1] + forceI[i][1];
       mouv[i][2] = force[i][2] + forceI[i][2];
     }   

    return 1;
}
int UNIVERS::prep_parsec()
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
double UNIVERS::calc_parsec(int a)
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
       if(zx <= Horizon)
        if(zy <= Horizon)
         if(zz <= Horizon)
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
double UNIVERS::calc_inter(int a)
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
             if(d > 80) d *= 10;
             e = 10*e/d;
             //e /= 10; // limite la repulsion!!!!
             if(e<-500) e = -500;             
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
double UNIVERS::calc_force()
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
       if(Esun < 0) Esun = Esun/4; // Attention !!!!!!!!!!!!!!!!!!!!!!!!!!
       force[i][0] = (-1)*Esun * (neo[i][0]/d);
       force[i][1] = (-1)*Esun * (neo[i][1]/d);
       force[i][2] = (-1)*Esun * (neo[i][2]/d);
       En += Esun;
     }
    //energie[1] = En;
    return En;
}
int UNIVERS::bouger()
{
    float dx,dy,dz;
    int i;
    
    for(i=0;i<n_planete;i++)
    if(!ancre[i])
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
float UNIVERS::modifier_pas(float a)
{
    pas *= a;
    npas *= a;
    return pas;
}
int UNIVERS::do_cycle(int a)
{
    int encore;
    // position de depart!
    pas = 10.0;
    npas = -10.0;
    copier_depart(); // deja fait en initier()
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
double UNIVERS::calc_energie()
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
double UNIVERS::calc_energie2()
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
int UNIVERS::act_selection()
{
    act_sel = act_sel^1;
    return act_sel;
}
int UNIVERS::prep_selection()
{
    int i;
    
    nsel = 0;
    for(i=0;i<n_planete;i++)
     { sel[i] = planete[i]->lire_type();
       if(sel[i]) nsel++;
     }
    return 1; 
}
int UNIVERS::activer_lien(int a)
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
int UNIVERS::act_marcher(int a)
{ // activer la fonk marcher!
    if(!a) 
     {  sub_etap = 0;
        return 0;
     } 
    if(a == -1) 
     {  etap = sub_etap = 0;
        return 0;
     } 
    return ++sub_etap;
}
int UNIVERS::marcher()
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
          neg_lien_seuil = Dseuil-lien_seuil;
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
              if(pas < 0.5) //sub_etap = -1;
               { ls = 0.96*(float)(lien_seuil);
               if(ls < 100)
               //if(ls < 1400)
               //  if(ls < 5000)
                  { sub_etap = 0;
                    return sub_etap;
                  }
                 lien_seuil = (int) ls;
                 prezo->activer_seuil_sup(lien_seuil);
                 neg_lien_seuil = Dseuil-lien_seuil;
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
/******************************************************************************/
int UNIVERS::fixe_aff_pos(int a,float ax,float ay,float az)
{
    double zx,zy,zz,lim;
    int i;
    
    if(a > 0)
     if(a < n_planete)
      { camera[0] = depart[a][0];
        camera[1] = depart[a][1];
        camera[2] = depart[a][2];
        idcamera = a;
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
int UNIVERS::aff_grafik()
{ // simple affichage de depart[]!
    int i,a,b;
    if(etap)
     { if(energie[0] < 10000)
        sprintf(Messager,"E(%d-%d)(%d / %4.2f) = %8.1lf [%d]\0",etap,sub_etap,lien_seuil,pas,energie[0],idcamera);
       else
        sprintf(Messager,"E(%d-%d)(%d / %4.2f) = %8.1lfK [%d]\0",etap,sub_etap,lien_seuil,pas,energie[0]/1000,idcamera);
     }
    if(act_sel) sprintf(Messager,"sel : %d (%d)",idcamera+1,nsel);
        
    for(i=0;i<n_planete;i++)
     { //if(!brume[i])
       if(repulsif[i])
       if((sel[i])||(!act_sel))
       { glTranslated(aff_pos[i][0],aff_pos[i][1],aff_pos[i][2]);
         //if(caractere[i]) glColor3d(0.6,0.2,0.2); // choix de la couleur
         //if(!caractere[i]) glColor3d(0.4,0.7,1); 
         glColor3d(couleur[caractere[i]][0],couleur[caractere[i]][1],couleur[caractere[i]][2]); 
         glutSolidSphere(Rayon_sphere,Facettes,Facettes);        
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
                  glVertex3f(aff_pos[dum][0],aff_pos[dum][1],aff_pos[dum][2]);*/
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
int UNIVERS::neo_lien_seuil(int a)
{
    lien_seuil += a;
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(Dseuil-lien_seuil);
    return 1;
} 
int UNIVERS::affichage()
{
    if(HIDE) return 0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*************************************/
    glColor3d(1,1,1);
    if(Messager[0]) glutEcrire(-30,20,-50,Messager);
  
    /*************************************/
        
    glPushMatrix();   
     glTranslated(profondeur[0],profondeur[1],profondeur[2]);
     glRotated(angle[0],1,0,0);
     glRotated(angle[1],0,1,0);
     glRotated(angle[2],0,0,1);  
     if(Helios)
      { glColor3d(1,0,0);  
        glutSolidSphere(1,20,20);
      }
     // glColor3d(0.1,0.1,1);
     glColor3d(0.4,0.7,1); 
     aff_grafik();
    glPopMatrix();


    glutSwapBuffers();
}
int UNIVERS::aff_select()
{
    GLint i;
    if(HIDE) return 0;
    glPushMatrix();   
     glTranslated(profondeur[0],profondeur[1],profondeur[2]);
     glRotated(angle[0],1,0,0);
     glRotated(angle[1],0,1,0);
     glRotated(angle[2],0,0,1);  
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
int UNIVERS::default_couleur()
{
    int i,j;
    
    i=j=0;
    couleur[i][j++] = 0.4;
    couleur[i][j++] = 0.7;
    couleur[i++][j++] = 1.0;
    j=0;
    couleur[i][j++] = 0.6;
    couleur[i][j++] = 0.2;
    couleur[i++][j++] = 0.2;
    couleur[i][j++] = 0.4;
    couleur[i][j++] = 0.7;
    couleur[i++][j++] = 1.0;
    j=0;
    couleur[i][j++] = 0.4;
    couleur[i][j++] = 0.4;
    couleur[i++][j++] = 1.0;
    j=0;
    couleur[i][j++] = 0.4;
    couleur[i][j++] = 0.8;
    couleur[i++][j++] = 0.2;
         
    return 1;
}
int UNIVERS::load_couleur()
{
    FILE *pf;
    char tamp[256];   
    char tampbis[256];
    char *pt;
    int i,j;
    
    for(i=0;i<100;i++) couleur[i][0] = couleur[i][1] = couleur[i][2] = 0.0;
    
    pf = fopen("couleurs.txt","r");
    if(!pf) 
     { //default_couleur();
       return 0;
     }
         
    i = j = 0;
    while(fgets(tamp,256,pf))
     { Ctexte(tamp);
       pt = lire_seq(tamp,tampbis,'\t');
       couleur[i][j++] = atof(tampbis);
       pt = lire_seq(pt,tampbis,'\t');
       couleur[i][j++] = atof(tampbis);
       pt = lire_seq(pt,tampbis,'\t');
       couleur[i][j++] = atof(tampbis);
       j = 0; 
       i++;
       if(i>99) i = 99; // limite sup
     }
    fclose(pf);
    return i;
}
int UNIVERS::geler_pos(int a)
{
    int i;
    
    if(!a)
     { for(i=0;i<n_planete;i++) ancre[i] = 0;
       return 0;
     }
    
    for(i=0;i<n_planete;i++) ancre[i] = repulsif[i];
    act_marcher(0);
    
    return 1;
}
