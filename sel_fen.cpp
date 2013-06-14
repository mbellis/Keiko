/* Affichage de la reference du probeset selectionne

    1ere fenetre de description!
    
*/
class FEN_COMMAND {
    CADLosange los[9];
    int base;
    int etat;
    int filtre[2][MaxProbe];
    int n_filtre[2];
    public:
    int initier();
    int afficher();
    int aff_tester();    
    int fixe_clic(int a);   
    int clusteriser();
    };
class FEN_SEARCH {
    EDITAGE_TEXTE ed;
    CADLosange los[12];
    char s_texte[8][26];
    int filtre[8][MaxProbe];
    int n_filtre[5];
    int operation[8]; // 0 nop 1 AND , 2 OR, 3 XOR
    int n_op;
    int base;
    int max;
    public:
    int initier();
    int afficher();
    int aff_tester();    
    int fixe_clic(int a);
    int recherche();
    int valider();
    int charger_sel();
    };    
class FEN_DESC {
    
    char desc[60][64];
    char texte[60][256];
    int probeid;
    int selection[60];
    int n_sel;
    int n_color;
    int flag_aff; // numeroter le type d'affichage
    int base;
    public:
    FEN_SEARCH Fsearch;
    FEN_COMMAND Fcomm;
    int initier();
    int initier_dep();
    int afficher();
    int aff_select();
    int fixe_clic(int a);
    int selection_default();
    };

/***************************************************/
int FEN_COMMAND::initier()
{
    int i;
    
    n_filtre[0] = n_filtre[1] = 0;
    base = 3;
    i = 0;
    los[i].creer(-5,20,-50,9);
    los[i].fixer_col(0,0.0,0.2,0.3);
    los[i].fixer_col(1,1,0.6,0.6);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"Cluster\0");//8
    los[i].fixer_base(base + i); 
    
    return 1;
}
int FEN_COMMAND::fixe_clic(int a)
{
    if(a == base) 
     { clusteriser();
     }
    
    return 1;
}
int FEN_COMMAND::afficher()
{
    char tamp[32];

    sprintf(tamp,">>>%d\0",n_filtre[0]);
    glutEcrireS(10,10,-50,tamp);
    sprintf(tamp,">>>%d\0",n_filtre[1]);
    glutEcrireS(0,10,-50,tamp);
    sprintf(tamp,">>>%d\0",Univers.idcamera);
    glutEcrireS(-10,10,-50,tamp);

    los[0].afficher();
    return 1;
}
int FEN_COMMAND::aff_tester()
{
    los[0].aff_select();
    return 1;
}
/***/
int FEN_COMMAND::clusteriser()
{
    REZO_ELEM *pe,*pe2;
    REZO_LIEN *pl;
    int i,j,k;
    
    n_filtre[1] = 5;
    if(Univers.idcamera < 0) return 0;
    n_filtre[1] = 1;
    
    for(i=0;i<MaxProbe;i++)
     { filtre[0][i] = filtre[1][i] = -1;
       Rezo.Elem[i].fixer_lu(0);
     }
    filtre[0][0] = Univers.idcamera;  
    i = k = 0;
    while(filtre[0][i] > -1)
     { pe = &Rezo.Elem[filtre[0][i]];
       pe->fixer_lu(1);
       pl = pe->get_lien();
       while(pl)
        { if(pl->is_actif())
           { j = pl->get_force();
             if(j>0)
              { pe2 = pl->get_elem();
                if(pe2->fixer_lu(-1) == 0)
                 { filtre[1][k++] = pe2->lire_id();
                   pe2->fixer_lu(1);
                 }
              }
           }
          pl = pl->get_suiv();
        }
       i++;
       if(filtre[0][i] == -1)
        { for(j=0;j<k;j++)
           { filtre[0][j] = filtre[1][j];
             filtre[1][j] = -1;
           }
          filtre[0][k] = -1;
          i = k = 0; // on recommence une nouvelle serie
        }
     } 
    n_filtre[0] = 0;
    for(i=0;i<MaxProbe;i++)
     { Liste_sel[i] = Rezo.Elem[i].fixer_lu(-1); // SELECTION
       if(Liste_sel[i]) n_filtre[0]++;
     }
    n_filtre[1] = 2;
     /*****/ 
    Rezo2.preparer_interne();
    //Rezo2.preparer_voisin();
     /****/
    Univ2.initier(&Rezo2);

    return 0;

}
/************************************************/
int FEN_SEARCH::initier()
{
    int i;
    
    max = Rezo.n_elem;
    i = 0;
    base = 3;
    los[i].creer(-5,20,-50,9);
    los[i].fixer_col(0,0.0,0.2,0.3);
    los[i].fixer_col(1,1,0.6,0.6);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    //los[i].fixer_text(1,"Total\0");
    los[i].fixer_liste_toggle("TOTAL\tSelection\tFichier\0");
    los[i].inc_toggle(1);
    los[i].fixer_base(base + i); //0
    
    i++;
    los[i].creer(-14,14,-50,5);
    los[i].fixer_col(0,0.0,0.4,0.1);
    los[i].fixer_col(1,0.6,0.8,0.8);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    //los[i].fixer_text(1,">>>\0");
    los[i].fixer_liste_toggle("...\tAND\tOR\tXOR\0");
    los[i].inc_toggle(1);
    los[i].fixer_base(base + i);//1
    
    i++;
    los[i].creer(-5,14,-50,9);
    los[i].fixer_col(0,0.0,0.1,0.1);
    los[i].fixer_col(1,1,0.4,0.4);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"\0");
    los[i].fixer_base(base + i);//2
    
    i++;
    los[i].creer(-14,10,-50,5);
    los[i].fixer_col(0,0.0,0.4,0.1);
    los[i].fixer_col(1,0.6,0.8,0.8);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    //los[i].fixer_text(1,">>>\0");
    los[i].fixer_liste_toggle("...\tAND\tOR\tXOR\0");
    los[i].inc_toggle(1);
    los[i].fixer_base(base + i);//3
    
    i++;
    los[i].creer(-5,10,-50,9);
    los[i].fixer_col(0,0.0,0.1,0.1);
    los[i].fixer_col(1,1,0.4,0.4);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"\0");
    los[i].fixer_base(base + i);//4
   
    i++;
    los[i].creer(-14,6,-50,5);
    los[i].fixer_col(0,0.0,0.4,0.1);
    los[i].fixer_col(1,0.6,0.8,0.8);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    //los[i].fixer_text(1,">>>\0");
    los[i].fixer_liste_toggle("...\tAND\tOR\tXOR\0");
    los[i].inc_toggle(1);
    los[i].fixer_base(base + i);//5
    
    i++;
    los[i].creer(-5,6,-50,9);
    los[i].fixer_col(0,0.0,0.1,0.1);
    los[i].fixer_col(1,1,0.4,0.4);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"\0");
    los[i].fixer_base(base + i);//6
   
    i++;
    los[i].creer(-5,0,-50,9);
    los[i].fixer_col(0,0.0,0.2,0.3);
    los[i].fixer_col(1,1,0.6,0.6);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"Neo Select\0");
    los[i].fixer_base(base + i); //7
    
    i++;
    los[i].creer(-5,-6,-50,9);
    los[i].fixer_col(0,0.0,0.2,0.3);
    los[i].fixer_col(1,1,0.6,0.6);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"Valider\0");//8
    los[i].fixer_base(base + i); 
    
    /*i++; // coloriage de la selection en cours
    los[i].creer(-5,-16,-50,9);
    los[i].fixer_col(0,0.0,0.2,0.3);
    los[i].fixer_col(1,1,0.6,0.6);
    los[i].fixer_col(2,1,1,1);
    los[i].fixer_col(3,0,1,0.8);
    los[i].fixer_text(0,"\0");
    los[i].fixer_text(1,"Liste\0");
    los[i].fixer_base(base + i); */
  
    for(i=0;i<8;i++) copie_string("0\0",s_texte[i]);
    for(i=0;i<4;i++) operation[i] = 0;
    n_op = 0;
    return 1;
}  
int FEN_SEARCH::fixe_clic(int a)
{
    if(a == base) 
     { los[0].inc_toggle(1);
       operation[0]++;
       if(operation[0]>3) operation[0] = 0;
     }
    
    if(a == base+1) 
     { los[1].inc_toggle(1);
       operation[1]++;
       if(operation[1]>3) operation[1] = 0;
     }
    if(a == base+2) 
     { los[2].get_texte(1,s_texte[0]);
       Ed_texte.initier(s_texte[0]);
       los[2].lier_editage_texte(&Ed_texte);
     }
     
    if(a == base+3) 
     { los[3].inc_toggle(1);
       operation[2]++;
       if(operation[2]>3) operation[2] = 0;
     }
    if(a == base+4) 
     { los[4].get_texte(1,s_texte[0]);
       Ed_texte.initier(s_texte[0]);
       los[4].lier_editage_texte(&Ed_texte);
     }
     
    if(a == base+5) 
     { los[5].inc_toggle(1);
       operation[3]++;
       if(operation[3]>3) operation[3] = 0;
     }
    if(a == base+6) 
     { los[6].get_texte(1,s_texte[0]);
       Ed_texte.initier(s_texte[0]);
       los[6].lier_editage_texte(&Ed_texte);
     }
    if(a == base+7) // recherche !
     { recherche();
     
     }
    if(a == base+8) // valider !
     { valider();
     }
    return 1;
}
int FEN_SEARCH::valider()
{
    int i;
    
    //if(max > 4000) return 0; // modif!!!!
    
    for(i=0;i<max;i++)
     Liste_sel[i] = filtre[7][i] = filtre[6][i]; // SELECTION
    
     /*****/ 
    Rezo2.preparer_interne();
    //Rezo2.preparer_voisin();
     /****/
    Univ2.initier(&Rezo2);
    n_op = 0;
    return 1; 
}
int FEN_SEARCH::charger_sel()
{
    FILE *pf;
    char tamp[256];
    int i,j;
    
    pf = fopen("rezosel.txt","r");
    if(!pf) return 0;

    for(i=0;i<max;i++) filtre[0][i] = 0; 
    n_filtre[0] = 0;
    while(fgets(tamp,256,pf))
     { Ctexte(tamp);
       n_filtre[0]++; 
       Rezo.prep_filtre(tamp,filtre[1]);
       for(i=0;i<max;i++) filtre[0][i] = filtre[0][i] | filtre[1][i];
     }
    fclose(pf);
    n_filtre[4] = n_filtre[3] = n_filtre[2] =n_filtre[1] = 0; 
    for(i=0;i<max;i++)
     { filtre[6][i] = filtre[0][i];
       if(filtre[6][i]) n_filtre[4]++;
     }
    for(i=0;i<5;i++) sprintf(s_texte[i+1],"%d\0",n_filtre[i]);
    //for(i=0;i<5;i++) sprintf(s_texte[i+1],"%d\0",-1);
    n_op = 1;
    
    return 1;
}
int FEN_SEARCH::recherche()
{
    int i,j;
    char tamp[4096];
     
    n_filtre[0] = 0;
    /***************/
    if(operation[0] == 2) 
     { charger_sel();
       return 1;
     }
     
    /***************/
    if(operation[0] == 1)
      for(i=0;i<max;i++) 
       { filtre[0][i] = filtre[7][i]; // SELECTION
         if(filtre[0][i]) n_filtre[0]++;
       }
    else
      for(i=0;i<max;i++) 
       { filtre[0][i] = 1; // TOTAL
         n_filtre[0]++;
       }
   
    //**************************
    if(operation[1])
     { los[2].get_texte(1,tamp);
       n_filtre[1] = Rezo.prep_filtre(tamp,filtre[1]);
       if(operation[1] == 1) // AND
         for(i=0;i<max;i++) filtre[2][i] = filtre[0][i] & filtre[1][i];
       if(operation[1] == 2) // OR
         for(i=0;i<max;i++) filtre[2][i] = filtre[0][i] | filtre[1][i];
       if(operation[1] == 3) // XOR
         for(i=0;i<max;i++) filtre[2][i] = filtre[0][i] ^ filtre[1][i];
     }
    else 
      for(i=0;i<max;i++) filtre[2][i] = filtre[0][i]; 
    
    //***************************
    if(operation[2])
     { los[4].get_texte(1,tamp);
       n_filtre[2] = Rezo.prep_filtre(tamp,filtre[3]);
       if(operation[2] == 1) // AND
         for(i=0;i<max;i++) filtre[4][i] = filtre[2][i] & filtre[3][i];
       if(operation[2] == 2) // OR
         for(i=0;i<max;i++) filtre[4][i] = filtre[2][i] | filtre[3][i];
       if(operation[2] == 3) // OR
         for(i=0;i<max;i++) filtre[4][i] = filtre[2][i] ^ filtre[3][i];
     }
    else 
      for(i=0;i<max;i++) filtre[4][i] = filtre[2][i]; 
      
    //***************************
    if(operation[3])
     { los[6].get_texte(1,tamp);
       n_filtre[3] = Rezo.prep_filtre(tamp,filtre[5]);
       if(operation[3] == 1) // AND
         for(i=0;i<max;i++) filtre[6][i] = filtre[4][i] & filtre[5][i];
       if(operation[3] == 2) // OR
         for(i=0;i<max;i++) filtre[6][i] = filtre[4][i] | filtre[5][i];
       if(operation[3] == 3) // OR
         for(i=0;i<max;i++) filtre[6][i] = filtre[4][i] ^ filtre[5][i];
     }
    else 
      for(i=0;i<max;i++) filtre[6][i] = filtre[4][i]; 
      
    //*************************
    n_filtre[4] = 0;
    for(i=0;i<max;i++)
      if(filtre[6][i]) n_filtre[4]++;
    
    for(i=0;i<5;i++) sprintf(s_texte[i+1],"%d\0",n_filtre[i]);
    //for(i=0;i<4;i++) sprintf(s_texte[i+1],"%d\0",operation[i]);
      
    //if(n_filtre[4]) 
    n_op = 1;    
    
    return 1;
}
int FEN_SEARCH::afficher()
{
    los[0].afficher();
    los[1].afficher();
    los[2].afficher();
    los[3].afficher();
    los[4].afficher();
    los[5].afficher();
    los[6].afficher();
    los[7].afficher();
    
    if(n_op) los[8].afficher();
    
  /********statitstics***********/
   glColor3f(0.4,0.8,0.4);      
   glutEcrire(12,20,-50,s_texte[1]);
   glutEcrire(12,14,-50,s_texte[2]);
   glutEcrire(12,10,-50,s_texte[3]);
   glutEcrire(12,6,-50,s_texte[4]);
   glutEcrire(12,0,-50,s_texte[5]);
   
       

  /********le cadre gauche*******/
    glTranslatef(-24,-24,-50);
     glBegin(GL_QUADS);  
        glColor3f(0,0.8,0.4);                   
        glVertex3f(5,0,0);                                     
        glVertex3f(5,12,0);               
        glVertex3f(0,12,0); 
        glVertex3f(0,0,0);  
        
        glColor3f(1,0,0.4);                   
        glVertex3f(5,13,0);                                     
        glVertex3f(5,25,0);               
        glVertex3f(0,25,0); 
        glVertex3f(0,13,0); 
        
        glColor3f(0,0.4,0.8);                   
        glVertex3f(5,26,0);                                     
        glVertex3f(5,38,0);               
        glVertex3f(0,38,0); 
        glVertex3f(0,26,0);
        
     glEnd();
    glTranslatef(24,24,50);
    return 1;
}
int FEN_SEARCH::aff_tester()
{

    int i;
    
    los[0].aff_select();
    los[1].aff_select();
    los[2].aff_select();
    los[3].aff_select();
    los[4].aff_select();
    los[5].aff_select();
    los[6].aff_select();
    los[7].aff_select();
    
    if(n_op) los[8].aff_select();
    
    return 1;
}
/***************************************************/    
int FEN_DESC::initier_dep()
{
    Fsearch.initier();
    initier();
    return 1;
}
int FEN_DESC::initier()
{
    char tamp[4096],*pt;
    char tampbis[4096];
    PROBESET *pr;
    int i;
    
    n_sel = 0;
    n_color = 0;
    for(i=0;i<20;i++) copie_string(".\0",texte[i]);
    if(Univers.idcamera < 0) return 0;
    
    probeid = Univers.idcamera;
    pr = Rezo.Elem[probeid].get_probeset();
    if(!pr) return 0;
    
    pr->lire(tamp);
    pt = tamp;
    while(*pt) pt = lire_seq(pt,texte[n_sel++],'\t');
    
    n_sel = 0;
    Puce.get_definitions(tamp);
    pt = tamp;
    while(*pt) pt = lire_seq(pt,desc[n_sel++],'\t');
    
    flag_aff = 0;
    base = 0;
    Fcomm.initier();
    
    selection_default();
    return 1;
}
int FEN_DESC::selection_default()
{
    int i;
    
    for(i=0;i<20;i++) selection[i] = 1;
   
    return 1;
}
int FEN_DESC::fixe_clic(int a)
{   
    if(a > 2)
    if(flag_aff == 1)
     { Fsearch.fixe_clic(a);
       return 1; 
     }
    if(a > 2)
    if(flag_aff == 2)
     { Fcomm.fixe_clic(a);
       return 1; 
     }
    flag_aff = a;
    return 1;
}
int FEN_DESC::afficher()
{
    int i,j,k;
    char tamp[256];
       
    if(Univers.idcamera != probeid) initier();
    /********redirection************/
    
     sprintf(tamp,"%d\0",flag_aff);
     glutEcrireS(20,20,-50,tamp);
     if(flag_aff == 1)
     { Fsearch.afficher();
       return 1;
     }
     if(flag_aff == 2)
     { Fcomm.afficher();
     }
    /*******************************/
    
    glColor3f(0.6,0.6,1.0);
    i = -24;
    j = -18;
    for(k=0;k<n_sel;k++)
    if(selection[k])
    { i += 2;
      glColor3f(0.6,0.6,1.0);
      glutEcrireS(j,i,-50,desc[k]);
      glColor3f(0.5,0.9,0.6);
      glutEcrireS(j+5,i-1,-50,texte[k]);
    }
    /********le cadre gauche*******/
    glTranslatef(-24,-24,-50);
     glBegin(GL_QUADS);  
        glColor3f(1,0,0.4);                   
        glVertex3f(5,0,0);                                     
        glVertex3f(5,12,0);               
        glVertex3f(0,12,0); 
        glVertex3f(0,0,0);  
        
        glColor3f(0,1,0.4);                   
        glVertex3f(5,13,0);                                     
        glVertex3f(5,25,0);               
        glVertex3f(0,25,0); 
        glVertex3f(0,13,0); 
        
        glColor3f(0,0.4,0.8);                   
        glVertex3f(5,26,0);                                     
        glVertex3f(5,38,0);               
        glVertex3f(0,38,0); 
        glVertex3f(0,26,0);
        
     glEnd();
    glTranslatef(24,24,50);
    return 1;
}
int FEN_DESC::aff_select()
{
    GLint i;
    char tamp[256];
       
    /********redirection************/
    if(flag_aff == 1)
     { Fsearch.aff_tester();
       //return 1;
     }
    if(flag_aff == 2)
     { Fcomm.aff_tester();
     }
    /*******************************/
    
    /********le cadre gauche*******/
    i = base;
    glTranslatef(-24,-24,-50);
    
        glLoadName(i);  
        glBegin(GL_QUADS);                 
        glVertex3f(5,0,0);                                     
        glVertex3f(5,12,0);               
        glVertex3f(0,12,0); 
        glVertex3f(0,0,0);  
        glEnd();
        i++;
    
        glLoadName(i);     
        glBegin(GL_QUADS);            
        glVertex3f(5,13,0);                                     
        glVertex3f(5,25,0);               
        glVertex3f(0,25,0); 
        glVertex3f(0,13,0); 
        glEnd();
        i++;

        glLoadName(i);   
        glBegin(GL_QUADS);                 
        glVertex3f(5,26,0);                                     
        glVertex3f(5,38,0);               
        glVertex3f(0,38,0); 
        glVertex3f(0,26,0);
        glEnd();
  
    glTranslatef(24,24,50);
    return 1;
}
