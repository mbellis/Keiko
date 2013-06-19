/*



    Determinate the representation of the threshold



    Link Inter : d ====> force * d; : linear

*/
#include <time.h>
#include "zegeometrik.h"

#define Rayon_sphere 0.4
#define Facettes 4
#define Horizon 6
#define Dseuil 3000


char coordNulBuff[128];
int coordNul;


FORCE Soleil, Repulsion, Inter;

int FORCE::ouvrir(char *s)
{
    FILE *pf;
    char tamp[64];

    pf = fopen(s,"r");
    if(!pf) return 0;

    n_va = 0;
    while(fgets(tamp,64,pf))
     { Ctexte(tamp);
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
{   // repulsion type : a from 0 to 1!

    if(a>Horizon) return 0.0;
    p_va = (int)(a*f_fact);
    return va[p_va];
}

float FORCE::lire_sun(float a)
{ // solar type : a from 0 to 10000!
    p_va = (int)(a*10);
    if(p_va>10000) return a; // important energy !!
    return va[p_va];
}


int UNIVERS::initer(REZO *pr)  // initiate the network
{
    REZO_ELEM *po;
    int i;

    prezo = pr;
    n_planete = 0;
    po = prezo->transferer(n_planete);
    while(po)
     { planete[n_planete++] = po;
       po = prezo->transferer(n_planete);
     }


    srand((unsigned int) time(NULL));                 // calculate random coordinates (if 0 as console var.
    for(i=0;i<n_planete;i++)
     { neo[i][0] = (float)((rand() % 900) - 450)/4.0;
       neo[i][1] = (float)((rand() % 900) - 450)/4.0;
       neo[i][2] = (float)((rand() % 900) - 450)/4.0;
     }



    lien_seuil = 9000;                  // always begin with 9000 in the threshold link.
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(Dseuil-lien_seuil);

    for(i=0;i<n_planete;i++)
      force[i][0] = force[i][1] = force[i][2] = 0.0;

    copier_arrivee();

    if(!Soleil.ouvrir("soleil.txt")) return 0;
    if(!Repulsion.ouvrir("repulsion.txt")) return 0;
    Repulsion.fixer_fact(10000/(float)(Horizon));


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

    coordNul = atoi(coordNulBuff);

    for(i=0;i<n_planete;i++)
     {
        if (coordNul == 0)
        {
            planete[i]->fixer_coord(neo[i][0],neo[i][1],neo[i][2]);
        }
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
    int b;

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


int UNIVERS::marcher(int sub_etap)
{
    float ls;
    int i;

    if(!sub_etap) return 0; // pas d'action!!!!


    if(!etap) // initialisation
     { if(sub_etap == 1) // prepare parsec
        { prep_parsec();
          energie[0] = energie[1] = 0.0;
          sub_etap++;


          prezo->activer_seuil_sup(lien_seuil);
          neg_lien_seuil = Dseuil-lien_seuil;
          if(neg_lien_seuil > -1300) neg_lien_seuil = -1300;
          prezo->activer_seuil_inf(neg_lien_seuil);


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
           if(energie[1]>=energie[0])
            { pas /= 2;
              npas /= 2;
              if(pas < 0.5) //sub_etap = -1;
               { ls = 0.96*(float)(lien_seuil);
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
         }

      }


    if (lien_seuil < seuilMin)
    {
        sub_etap = 0;
        return sub_etap;
    }

     sub_etap = 1;
     return sub_etap;
}




int UNIVERS::neo_lien_seuil(int a)
{
    lien_seuil += a;
    prezo->desactiver_all_links();
    prezo->activer_seuil_sup(lien_seuil);
    prezo->activer_seuil_inf(Dseuil-lien_seuil);
    return 1;
}

