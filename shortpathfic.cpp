/*
    Lecture et gestion des Shortest Path

    surement a effacer !!!!

*/
class SHORT_PATH {
    int marche[64];// numero du probeset
    float distance;
    int n_marche;
    int actif;
    public:
    int charger(float d,char *s);
    int activer(int a);
    int get_marche(int a);
    float get_distance();

    };
class DICO_SHORTPATH { public:
    SHORT_PATH chemin[13000]; 
    int n_chemin;
    int liste[13000];
    int n_liste;
    int origine;
    int actif;
   /****************/
    int initier();
    int charger(int a);
    int charger_liste();
    int activer();
    int is_actif();
    int fixer_limite(float a); 
    int valider_module();
    int valider_module2(int a);
    };
    
/*********************************************/
int SHORT_PATH::charger(float d,char *s)
{
    char tamp[32];
    char *pt;
    int i,a;
    
    distance = d;
    n_marche = 0;
    
    pt = s;
    while(*pt)
     { pt = lire_seq(pt,tamp,'-');
       a = atoi(tamp);
       marche[n_marche++] = a;
     }
    actif = 1;
    //sprintf(Messager,"%s - %5.1f",s,d);
    return 1;
}
int SHORT_PATH::activer(int a)
{
    if(a < 0) return actif;
    actif = a;
    return actif;
}
int SHORT_PATH::get_marche(int a)
{
    if(a >= n_marche) return -1;
    return marche[n_marche - a -1];
}
float SHORT_PATH::get_distance()
{
   return distance;
}
/*********************************************/
int DICO_SHORTPATH::initier()
{
    n_chemin = n_liste = actif = 0;
    origine = -1;
    return 1;
}
int DICO_SHORTPATH::charger(int a)
{
    FILE *pf;
    char tamp[4096];
    char tampbis[64],*pt;
    double distance;
    int i,ok;

    if(a < 0) return 1;
    if(a == origine) return 1; // deja pret!
    i = a/50;
    i = i*50;
    sprintf(tamp,"e:/Data/Puces/puceMu11K/shortpath/shortpath%d.txt",i);
    pf = fopen(tamp,"r");
    if(!pf) return 0;
    
    n_chemin = n_liste = ok = 0;
    while(fgets(tamp,4096,pf))
     { Ctexte(tamp);
       if(tamp[0] == '>')
        { i = atoi(tamp+1);
          ok = 0;
          if(i == a) ok = 1;
        }
       else
       if(ok)
        { pt = lire_seq(tamp,tampbis,'\t');
          pt = lire_seq(pt,tampbis,'\t');
          distance = atof(tampbis);
          if(*pt) chemin[n_chemin++].charger(distance,pt);
        }
     }
    fclose(pf);
    origine = a;
    sprintf(Messager,"chemin charge %d >> : %d",origine,n_chemin);
    //fixer_limite(1000000);
    //valider_module();
    // for(i=0;i<n_chemin;i++)  valider_module2(i);
    
    return 1;
}

int DICO_SHORTPATH::charger_liste()
{   // chemin entre les Elem actives!
    FILE *pf;
    char tamp[4096];
    char tampbis[64],*pt;
    double distance;
    int i,j,a,b,ok;
    
    n_liste = 0;
    for(i=0;i<Rezo.n_elem;i++)
     { liste[i] = 0;
       n_liste++;
       if(Rezo.Elem[i].lire_type()) liste[i] = 1;
     }

    n_chemin =0;
    for(i=0;i<n_liste;i++) 
    if(liste[i])
     { j = i/50;
       j = j*50;
       sprintf(tamp,"e:/Data/Puces/puceMu11K/shortpath/shortpath%d.txt",j);
       pf = fopen(tamp,"r");
       if(!pf) return 0;
       ok = 0;
       b = -1;
       while(fgets(tamp,4096,pf))
        { Ctexte(tamp);
          b++;
          if(tamp[0] == '>')
           { a = atoi(tamp+1);
             ok = 0;
             b=-1;
             if(i == a) ok = 1;
           }
          else
           if(ok)
           if(liste[b])
            { pt = lire_seq(tamp,tampbis,'\t');
              pt = lire_seq(pt,tampbis,'\t');
              distance = atof(tampbis);
              if(distance > 0)
               if(*pt) chemin[n_chemin++].charger(distance,pt);
            }
        }
     }
    fclose(pf);
    sprintf(Messager,"liste chemin >> : %d - %d[%d %d %d %d]",n_chemin,n_liste,liste[0],liste[1],liste[2],liste[3]);
    return 1; 
}
int DICO_SHORTPATH::fixer_limite(float a)
{
    int i;
    
    for(i=0;i<n_chemin;i++)
     { if(chemin[i].get_distance() < a) chemin[i].activer(1);
       else chemin[i].activer(0);
     } 
    return 1;
}
int DICO_SHORTPATH::activer()
{
    actif = actif^1;
    return actif;
}
int DICO_SHORTPATH::is_actif()
{
    return actif;
}
int DICO_SHORTPATH::valider_module()
{
    int tabo[32000];
    int i,z,e,r;
    
    /****************Remplir Tabo*******************/
    Rezo.Elem[origine].get_voisin(tabo);
    
    /***********************************************/
        
    for(i=0;i<n_chemin;i++)
     { e = 0;
       r = 1;
       z = chemin[i].get_marche(e++);
       while((z >= 0)&&(r))
        { r = r & tabo[z];
          z = chemin[i].get_marche(e++);
        }
       chemin[i].activer(r);
     }
     
    return 1; 
}
int DICO_SHORTPATH::valider_module2(int a)
{
    //FILE *pf;
    REZO_LIEN *ldep,*lmid,*ldum;
    REZO_ELEM *edep,*emid,*efin;
    int tabo[3000];
    int i,z,e,r;
    int fdep,fmid;
    
    
    /***********************************************/
        
    e = r = 0;
    z = chemin[a].get_marche(e++);
    while(z >= 0)
     { tabo[r++] = z;
       z = chemin[a].get_marche(e++);
     }
    tabo[r] = z; // le -1 final
    efin = &Rezo.Elem[tabo[r-1]];
   /**************************/
   /* pf = fopen("debugage.txt","w");
    if(!pf) return 0;
    /**********************/   
    chemin[a].activer(0);     
    ldep = Rezo.Elem[tabo[0]].get_lien();
    if(!ldep) return 0;
    
    ldep = ldep->chercher(efin);
    if(ldep)
     { chemin[a].activer(1);
       //fprintf(pf,"chemin active : %d %d\n",a,r);
       fdep = ldep->get_force();
       for(i=1;i<r-2;i++)
        { lmid = Rezo.Elem[tabo[i]].get_lien();
          lmid = lmid->chercher(efin);
          if(lmid)
           { fmid = lmid->get_force();
             if(fmid < fdep) 
              { chemin[a].activer(0);//fclose(pf);
                return 0;
              }
           }  
          if(!lmid) 
           { chemin[a].activer(0);//fclose(pf);
             return 0;
           }   
        }
       /************************/ 
       ldep = Rezo.Elem[tabo[0]].get_lien();
       for(i=1;i<r;i++)
         { lmid = ldep->chercher(&Rezo.Elem[tabo[i]]);
           if(lmid)
            { fmid = lmid->get_force();
              if(fmid < 1500) 
               { chemin[a].activer(0);
                 return 0;
               }
            }
           if(!lmid) 
            { chemin[a].activer(0);
              return 0;
            }
         }
     }   
    //fprintf(pf,"chemin reste actif\n");  
    //fclose(pf);
    return 1; 
}
