/*
    Fonctions gerant le dictionnaire des regulations
    version dediee a keiko : sortie graphique
    
    a effacer !!!!!!!!!!!!!!!!!!!!!!!
*/

# define MAX_EXP 500
# define MAX_PROB 16000

class META_REGULATION{
    int l_prob[MAX_PROB+1];
    int pod[MAX_PROB+1];
    int act[MAX_PROB+1];
    int bin_act[MAX_PROB+1]; // 2 up 1 down 0?
    int l_exp[MAX_EXP+1]; // +1 pour le -1 de fin !!!!
    int n_exp,n_prob;
    public:
    int vider();
    int copier(META_REGULATION *pr);
    int initier(int *t); // premier triangle
    int completer(int *t); // triangles supp
    int fusionner(META_REGULATION *pr);
    int ecrire();
    int sauver_matlab(FILE *pf,int a);
    int lire_matlab(char *s,int a);
    /*****/
    int lire_all(int *p);
    int lire_cond_prob(int a,int b,int *p);
    int intersection(int *l1,int *l2,int *l3);
    int grouper(int *l1,int *l2,int *l3);
    }; 
class DICO_REGUL{
    META_REGULATION reg[4096];
    int masque[4096];
    int aff_reg[16000];
    int n_reg;
    int n_triangle;    
    public:
    int ref1;
    int affichage;
    int fast_aff[16000][2];
    int n_fast_aff;
    int initier();
    int ecrire();
    int ajouter(int *t);
    int fusionner();
    int sauver_matlab();
    int ouvrir_fichier();
    int afficher();
    int preparer_reg(int a); // 1 seule regulation !
    int preparer_prob(int a,int b); // 1 probeset de depart
    };
/***********************************************************************/
int META_REGULATION::vider()
{
    n_exp = n_prob = 0;
    return 1;
}
int META_REGULATION::copier(META_REGULATION *pr)
{
    int i;
    
    n_prob = pr->n_prob;
    for(i=0;i<n_prob;i++)
     { l_prob[i] = pr->l_prob[i];
       pod[i] = pr->pod[i];
       act[i] = pr->act[i];
     }
    n_exp = pr->n_exp;
    for(i=0;i<n_exp;i++) l_exp[i] = pr->l_exp[i];
     
    return 1;
}
int META_REGULATION::initier(int *t)
{
    int i;
    
    i = 0;
    l_prob[i] = t[0];
    pod[i] = 1;
    act[i++] = t[7];
    
    l_prob[i] = t[1];
    pod[i] = 1;
    act[i++] = t[8];
    
    l_prob[i] = t[2];
    pod[i] = 1;
    act[i++] = t[9];
    
    n_prob = i;
    l_prob[n_prob] = -1;
    
    for(i=0;i<MAX_EXP;i++)
     { l_exp[i] = t[10+i];
     }
    l_exp[i] = -1; // assure la fin de liste!

    while((n_exp<=MAX_EXP)&(l_exp[n_exp] != -1)) n_exp++;

    return 1;
}
int META_REGULATION::completer(int *t)
{
    double a;
    int temp[MAX_EXP+1];
    int i,j;
    
    if(t[0] != l_prob[0]) return 0;
    if(t[1] != l_prob[1]) return 0;
    // test l'intersection
    if(!intersection(t+10,l_exp,temp)) return 0;
    
    l_prob[n_prob] = t[2];
    pod[n_prob] = 1;
    act[n_prob++] = t[9];
    
    l_prob[n_prob] = -1;
    
    // actualisation
    a = (double) act[0] * (double) pod[0];
    a += (double) t[7];
    pod[0]++;
    a /= (double) pod[0];
    act[0] = (int) a;
    
    a = (double) act[1] * (double) pod[1];
    a += (double) t[8];
    pod[1]++;
    a /= (double) pod[1];
    act[1] = (int) a;    
   
    // union des listes exp
    j = grouper(t+10,l_exp,temp);
    for(i=0;i<j;i++) l_exp[i] = temp[i];
    n_exp = i;
    l_exp[n_exp] = -1;

    return 1;
}
int META_REGULATION::fusionner(META_REGULATION *pr)
{
    int temp[MAX_PROB+1]; // MAX_PROB >> MAX_EXP
    int i,j,k;

     printf("\ntest....\n");
    // tests de validite 
    if(l_prob[0] != pr->l_prob[0]) return 0;
    k = intersection(l_prob,pr->l_prob,temp);
    if(k < 1) return 0;
    printf("> %d  ",k);
    
    k = intersection(l_exp,pr->l_exp,temp);
    if(!k) return 0;
    printf(">> %d  :",k);
    i=0;
    while(l_exp[i] != -1) i++;
    j=0;
    while(pr->l_exp[j] != -1) j++;
    printf("(%d %d)  :",i,j);
    if((j != k) &&(i != k)) return 0;
    
    printf("bingo\n");

    return 1;
}
int META_REGULATION::ecrire()
{
    int i;
    
    for(i=0;i<n_prob;i++)
     printf(" %6d > %6d   %5d   %5d\n",i,l_prob[i],pod[i],act[i]);
     
    for(i=0;i<n_exp-1;i++) printf("%4d ",l_exp[i]);
    printf("%4d\t(%d)\n",l_exp[i],n_exp);
    
    return 1;
}
int META_REGULATION::sauver_matlab(FILE *pf,int a)
{
    int i;
    
    if(a == -1)
     { for(i=0;i<n_exp;i++) fprintf(pf,"%d ",l_exp[i]+1);
       while(i++<MAX_EXP+1) fprintf(pf,"%d ",0);
       fprintf(pf,"\n");
       return 1;
     }

     for(i=0;i<n_prob;i++)
      fprintf(pf,"%d %d %d %d\n",a+1,l_prob[i]+1,pod[i],act[i]);
      
     return 1;
}
int META_REGULATION::lire_matlab(char *s,int a)
{
    char tamp[256],*pt;
    int i;
    
    if(a == -1)
     { n_exp = 0;
       pt = lire_seq(s,tamp,' ');
       l_exp[n_exp++] = atoi(tamp);
       while(*pt)
        { pt = lire_seq(pt,tamp,' ');
          l_exp[n_exp++] = atoi(tamp);
        }
     
       return 1;
     }
    
    pt = lire_seq(s,tamp,' ');
    l_prob[n_prob] = atoi(tamp)-1; // because matlab  : 1->0 !!!
    pt = lire_seq(pt,tamp,' ');
    pod[n_prob] = atoi(tamp);
    pt = lire_seq(pt,tamp,' ');
    act[n_prob++] = atoi(tamp);
    
    // calcul du bin_act
    bin_act[n_prob-1] = 0; 
    if(act[n_prob-1] > 150) bin_act[n_prob-1] = 2;    
    if(act[n_prob-1] < 67) bin_act[n_prob-1] = 1;
     
    return n_prob;
}
int META_REGULATION::intersection(int *l1,int *l2,int *l3)
{ // l3 = intersection de l1 et l2
    int i,j,k;
    int ok;
    
    i = k = 0;
    while(l1[i] != -1)
     { j = 0;
       ok = 0;
       while((l2[j] != -1)&&(!ok))
        { if(l2[j] == l1[i])
           { l3[k++] = l1[i];
             ok = 1;
           }
          j++;
        }
       i++; 
     }
    l3[k] = -1;
    return k; 
}
int META_REGULATION::grouper(int *l1,int *l2,int *l3)
{ // l3 = union de l1 et l2 // disparition de l'ordre des exp !!!
    int i,j,k;
    int ok;
    
    i = k = 0;
    while(l1[i] != -1) l3[k++] = l1[i++];
    
    j=0;
    while(l2[j] != -1)
     { i=0;
       ok = 0;
       while((l1[i] != -1)&&(!ok))
        if(l1[i++] == l2[j]) ok = 1;
        
       if(!ok) l3[k++] = l2[j];
       j++;       
     } 
     
    l3[k] = -1;
    return k; 
}
int META_REGULATION::lire_all(int *p)
{
    int i;
    
    for(i=0;i<n_prob;i++) p[l_prob[i]] |= bin_act[i];
  
    return 1;
}
/***********************************************************************/
/***********************************************************************/
int DICO_REGUL::initier()
{
    n_reg = 0;
    n_fast_aff = 0;
    affichage = 0;
    ref1 = 5133;
    return 1; 
}
int DICO_REGUL::ecrire()
{
    printf("DICO : %d regs\t %d triangles\n",n_reg,n_triangle);
    return 1;
}
int DICO_REGUL::ajouter(int *t)
{
    int i,ok;
    //if(n_triangle >= 2) return 0;
    
    ok = 0;
    for(i=0;i<n_reg;i++)
     { ok = reg[i].completer(t);
      // reg[i].ecrire();
       if(ok) return ++n_triangle;
     }
        
    reg[n_reg++].initier(t);
    //reg[n_reg-1].ecrire();
    //printf("(%4d)!-> %5d\n",n_reg,n_triangle+1);
    if(n_reg > 4095)
     {  printf("deborde !!!!!!!!!!!\n");
        n_reg = 4095;
     }
    return ++n_triangle;
}
int DICO_REGUL::fusionner()
{
    int i,j;
    
    for(i=1;i<n_reg;i++)
     for(j=0;j<i;j++)
      reg[i].fusionner(&reg[j]);

     return 1;
}
int DICO_REGUL::sauver_matlab()
{
    FILE *pf;
    int i,j;
     
    //pf = fopen("g:/travaux/RdicoP.txt","w");
    pf = fopen("g:/travaux/dicoP.txt","w");
    if(!pf) return 0;

    for(i=0;i<n_reg;i++)
     reg[i].sauver_matlab(pf,i);    

    fclose(pf);
    
    //pf = fopen("g:/travaux/RdicoE.txt","w");
    pf = fopen("g:/travaux/dicoE.txt","w");
    if(!pf) return 0;

    for(i=0;i<n_reg;i++)
     reg[i].sauver_matlab(pf,-1);    

    fclose(pf);
    return 1;
}
int DICO_REGUL::ouvrir_fichier()
{
    FILE *pf;
    char tamp[40960],*pt;
    char tampbis[256];
    int i,j;
     
    for(i=0;i<4096;i++) reg[i].vider();
    //pf = fopen("g:/travaux/RdicoP.txt","w");
    //pf = fopen("g:/travaux/Mu11/dicoP.txt","r");
    pf = fopen("g:/travaux/U95/dicoP.txt","r");
    if(!pf) return 0;

    n_reg = 0;
    while(fgets(tamp,40960,pf))
     { Ctexte(tamp);
       pt = lire_seq(tamp,tampbis,' ');
       n_reg = atoi(tampbis); 
       reg[n_reg-1].lire_matlab(pt,1);
     }
    fclose(pf);
    
    //pf = fopen("g:/travaux/RdicoE.txt","w");
    //pf = fopen("g:/travaux/Mu11/dicoE.txt","r");
    pf = fopen("g:/travaux/U95/dicoE.txt","r");
    if(!pf) return 0;

    n_reg = 0;
    while(fgets(tamp,40960,pf))
     { Ctexte(tamp);
       reg[n_reg++].lire_matlab(tamp,-1);
     }
    fclose(pf);
    return 1;

}   
int DICO_REGUL::afficher()
{
    affichage ^= 1;
    return affichage;
}  
int DICO_REGUL::preparer_reg(int a)
{
    int i,j;
    
    for(i=0;i<MAX_PROB;i++) aff_reg[i] = 0;
    
    //for(i=0;i<n_reg;i++) reg[i].lire_all(aff_reg);
    reg[a].lire_all(aff_reg);
    
    n_fast_aff=0;
    for(i=0;i<MaxProbe;i++)
      if(aff_reg[i]) 
        { fast_aff[n_fast_aff][0] = i;
          fast_aff[n_fast_aff++][1] = aff_reg[i];
        }
    affichage = 1;    
    return 1;
}
