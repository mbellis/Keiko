/*
    Gestion des elements graphiques
*/

class CADRETEXT {
    char texte[256];
    int longueur;
    int modele_cadre;
    int is_mouse;
    int is_action;
    int existe;
    int is_actif[4];
    int base;         // numero d'objet de selection
    float zicolor[3];// RGB si is_mouse (1)  
    double co[6][3];// pos : 0 le texte et 1->5 le losange!
    double col[6][3];// color : 0 le texte et 1-> le losange!
    public:
    int preparer_pos(double zx,double zy,double zz,char *s, int mo);
    int preparer_texte(char *s);
    int preparer_col(int a,int action);
    int afficher();
    //int liste_afficher();
    int tester();
    int fixer_mouse(int a,int action);
    int fixer_base(int a);
    int exister(int a);
    };

class CADRE_DISPO {
    int modele;
    int base;
    int is_mouse[2];// 0 UP 1 DOWN
    int n_deb,n_fin;
    CADRETEXT Cd[56]; // presentation en 6*7
    public :
    int aff_cadre();
    int test_cadre();
    int afficher();
    int tester();
    int prep_demo();
    int preparer(int taille);
    int fixer_mouse(int a,int action);
    int fixer_base(int a);
    int relire(GROUP_ARN *g,int pos);

};
class CADRE_GROUP{
    CADRETEXT Cexemple;
    CADRE_DISPO Cgroupe;
    GROUP_ARN *groupe;
    int base;
    int num_depart;// num de l'element affiché en pos 1.
    int num_aff;// num d'element affichés 0 à 42.
    int num_exemple; // num de l'exemple (d'apres la liste totale du groupe)
    public:
    int initier(GROUP_ARN *g);
    int afficher();
    int tester();
    int fixer_mouse(int a,int action);
    int fixer_base(int a);
    int relire();

};
/********************************************/
int CADRE_GROUP::initier(GROUP_ARN *g)
{ 
    groupe = g;
    Cexemple.preparer_pos(-1,0.6,-5,"-",0); 
    num_aff = g->get_taille();
    num_depart = num_exemple = 0;
    Cgroupe.preparer(num_aff);
    
    relire();
    return 1;
}
int CADRE_GROUP::fixer_base(int a)
{
    base = a;
    Cexemple.fixer_base(a);
    Cgroupe.fixer_base(a+1);
    
    return 1;
}
int CADRE_GROUP::afficher()
{
    Cexemple.afficher();
    Cgroupe.afficher();
    return 1;
}
int CADRE_GROUP::tester()
{
    Cexemple.tester();
    Cgroupe.tester();
    return 1;
}
int CADRE_GROUP::fixer_mouse(int a,int action)
{
    int i = 0;
    
    i = Cgroupe.fixer_mouse(a,action);
    if(action)
     { if(i == -1) 
         if((num_depart-7)>= 0) num_depart -= 7;
       if(i == -2) 
        if((num_depart+42)<num_aff) num_depart += 7;
  
       if(i>0)
        if(action = 211)
         { num_exemple = num_depart + i -1;
           groupe->fixer_exemple(num_exemple);
         }
       relire();
     }
  
    i = Cexemple.fixer_mouse(a,action);
    
    return i;
}  
int CADRE_GROUP::relire()
{
    char tamp[2][4096];
    int i,j,k,f;
    int l[2];
    
    //groupe = GroupClass.get_expo();
    f = groupe->lire_exemple(0,tamp[1]);
    Cexemple.exister(0);
    if(f)
     { Cexemple.preparer_pos(-1,0.6,-5,"-",0); 
       Cexemple.preparer_texte(tamp[1]);
       Cexemple.exister(1);
       for(i=0;i<4;i++)
        { k = Menulosange.lire_param(i,tamp[0]); // lire la selection en cours
          Cexemple.preparer_col(i,0); // par defaut inactif
          mini_text(tamp[0]);
          if(k)
           { groupe->lire_exemple(k-1,tamp[1]);
             mini_text(tamp[1]);
             j = chercher_string(tamp[0],tamp[1]);
             if(!j) j = chercher_string(tamp[1],tamp[0]); // difference de taille!
             if(j) Cexemple.preparer_col(i,1); // colorisation du sommet du losange
           } 
        }
     }
    Cgroupe.relire(groupe,num_depart);
    return 1;
}
/*********************************************/
int CADRETEXT::fixer_base(int a)
{   base = a;
    return base;
}
int CADRETEXT::fixer_mouse(int a,int action)
{
    is_mouse = is_action = 0;
    
    if(a == base) 
     { is_mouse = 1;
       is_action = action;
       // do_it();
       return 1;// menu deroulant possible
     }  
    return 0;
}
int CADRETEXT::preparer_pos(double zx,double zy,double zz,char *s,int mo)
{
    longueur = long_string(s);
    copie_string(s,texte);
    modele_cadre = mo;
    if(longueur > 255) return 0; // texte trop long!
             
       co[0][0] = zx; // le texte
       co[0][1] = zy; 
       co[0][2] = zz; 
     
    return 1;
}
int CADRETEXT::preparer_texte(char *s)
{   
    char tamp[4096];
    
    copie_string(s,tamp);
    tamp[254] = 0;
    copie_string(tamp,texte);
    return 1;
}
int CADRETEXT::preparer_col(int a,int action)
{
    is_actif[a] = 0;
    if(action) is_actif[a]++;
    return 1;
}
int CADRETEXT::exister(int a)
{
    existe = a;
    return existe;
}
int CADRETEXT::afficher()
{
    if(!existe) return 0;
    //glLoadIdentity();
    glTranslatef(co[0][0],co[0][1],co[0][2]);
    if((is_mouse)&&(modele_cadre)) glTranslatef(0,0,2);
    
    if((is_mouse)||(!modele_cadre))
     { glColor3f(0.8,0.8,0.8);
	   glRasterPos2f(0.2,0.2);
	   glPrint(texte);	
     }
	
	glColor3f(0.0,0.0,0.0);		// le losange de fond!	
	if(is_mouse) glColor3f(0.2,0.2,0.6);	
	glBegin(GL_QUADS);									
 		glVertex3f(-0.2,-0.2,-0.01);
  	    glVertex3f(1,-0.2,-0.01);
  	    glVertex3f(1,0.5,-0.01);
  	    glVertex3f(-0.02,0.5,-0.01);
	glEnd();
	
	glBegin(GL_QUADS);	// le losange de description!
        glColor3f(0.2,0.2,0.2);							
		if(is_actif[0]) glColor3f(1,0,0);				
 		glVertex3f(0.2,0,0);
 		glColor3f(0.2,0.2,0.2);
 		if(is_actif[1]) glColor3f(0,1,0);
  	    glVertex3f(0,0.2,0);
  	    glColor3f(0.2,0.2,0.2);
  	    if(is_actif[2]) glColor3f(1,1,0);
  	    glVertex3f(-0.2,0,0);
  	    glColor3f(0.2,0.2,0.2);
  	    if(is_actif[3]) glColor3f(0,0,1);
  	    glVertex3f(0,-0.2,0);
	glEnd();
	glLoadIdentity();	
	
    return 1;
}

int CADRETEXT::tester()
{
    if(!existe) return 0;
    //glLoadIdentity();
    glTranslatef(co[0][0],co[0][1],co[0][2]);
    glLoadName(base);
	
	glBegin(GL_QUADS);								
		glColor3f(1,0,0);		// le losange de demo!		
 		glVertex3f(0.2,0,0);
 		glColor3f(0,1,0);
  	    glVertex3f(0,0.2,0);
  	    glColor3f(1,1,0);
  	    glVertex3f(-0.2,0,0);
  	    glColor3f(0,0,1);
  	    glVertex3f(0,-0.2,0);
	glEnd();
	glLoadIdentity();		
	return 1;
}
/*********************************************/
int CADRE_DISPO::prep_demo()
{
    float dx,dy,i,j;
    int k;
    
    base = 10;
    dx = 0.5;
    dy = 0.5;
    i = j = 0.0;
    for(k = 0; k<42; k++)
     {  if(i > 6) 
         { j++;
           i = 0;
         }
        Cd[k].preparer_pos(i*dx,-j*dy,-10,"dummy",1);
        Cd[k].fixer_base(base+2+k);
        i++;
     }
    return 1;
}
int CADRE_DISPO::preparer(int taille)
{
    float dx,dy,i,j;
    int k;
    
    n_fin = taille;
    base = 10;
    dx = 0.5;
    dy = 0.5;
    i = j = 0.0;
    for(k = 0; k<42; k++)
     {  if(i > 6) 
         { j++;
           i = 0;
         }
        Cd[k].preparer_pos(i*dx,-j*dy,-10,"-",1);
        i++;
     }
    return 1;
}
int CADRE_DISPO::aff_cadre()
{
    // afficher les fleches UP et DOWN
    
    //glLoadIdentity();
    glTranslatef(3,0.1,-10);
    
    glColor3f(0.1,0.6,0.6);
    if(is_mouse[0]) glColor3f(0.8,0.3,0.3);
    if(n_deb < 7) glColor3f(0.8,0.3,0.3);
    
   	glBegin(GL_TRIANGLES);								
		glVertex3f(0,0.6,0);
 		glColor3f(0,1,0);
  	    glVertex3f(0.1,0.2,0.2);
  	    glVertex3f(-0.6,0.3,0);
    glEnd();  
    
    glTranslatef(0,-2.7,0);
    glColor3f(0.1,0.6,0.6);
    if(is_mouse[1]) glColor3f(0.8,0.3,0.3);
    if(n_fin - n_deb < 42) glColor3f(0.8,0.3,0.3);
    glBegin(GL_TRIANGLES);		
 		glVertex3f(0,-0.6,0);
 		glColor3f(0,1,0);
  	    glVertex3f(0.1,-0.1,0.2);
  	    glVertex3f(-0.6,-0.3,0);
	glEnd();
    glLoadIdentity();
    return 1;
}
int CADRE_DISPO::test_cadre()
{
    //glLoadIdentity();
    glTranslatef(3,0.1,-10);
    
    glLoadName(base);
   	glBegin(GL_TRIANGLES);								
		glVertex3f(0,0.6,0);
 		glColor3f(0,1,0);
  	    glVertex3f(0.1,0.2,0.2);
  	    glVertex3f(-0.6,0.3,0);
    glEnd();  
    
    glTranslatef(0,-2.7,0);
    glLoadName(base+1);
    glBegin(GL_TRIANGLES);		
 		glVertex3f(0,-0.6,0);
 		glColor3f(0,1,0);
  	    glVertex3f(0.1,-0.1,0.2);
  	    glVertex3f(-0.6,-0.3,0);
	glEnd();
	
	glLoadIdentity();

    return 1;
}
int CADRE_DISPO::afficher()
{
    int k;
    
    for(k = 0; k<42; k++) Cd[k].afficher();

    aff_cadre();

    return 1;
}
int CADRE_DISPO::tester()
{
    int k;
    
    test_cadre();
    for(k = 0; k<42; k++) Cd[k].tester();


    return 1;
}
int CADRE_DISPO::fixer_mouse(int a, int action)
{
    int k,r;
    
    r = 0;
    is_mouse[0] = is_mouse[1] = 0;
    if(a == base) 
     { is_mouse[0]++;
       r = -1;
     }
    if(a == base+1)
     { is_mouse[1]++;
       r = -2;
     } 
    if(r)
     { for(k = 0; k<42; k++) Cd[k].fixer_mouse(a,action);
     }
    else for(k = 0; k<42; k++) if (Cd[k].fixer_mouse(a,action)) r = k+1;

    return r;
}
int CADRE_DISPO::fixer_base(int a)
{
    int k;
    
    base = a;
    for(k = 0; k<42; k++)
      Cd[k].fixer_base(base+2+k);
      
    return base;
}
int CADRE_DISPO::relire(GROUP_ARN *g,int pos)
{
    char tamp[2][4096];
    int i,j,k,o,f;
    int l[2];
    
    g->go_prime();
    n_deb = pos;
    for(i=0;i<pos;i++) g->suivant();
    for(i=0;i<42;i++)
     { f = g->lire_idem(0,tamp[1]);
       Cd[i].exister(0); // absent par defaut
       if(f)
        { Cd[i].exister(1);
          Cd[i].preparer_texte(tamp[1]);           
          for(j=0;j<4;j++)
           { k = Menulosange.lire_param(j,tamp[0]); // lire la selection en cours
             Cd[i].preparer_col(j,0); // par defaut inactif
             if(k)
              { f = g->lire_idem(k-1,tamp[1]);
                mini_text(tamp[0]);
                mini_text(tamp[1]);  
                o = chercher_string(tamp[0],tamp[1]);
                if(!o) o = chercher_string(tamp[1],tamp[0]); // difference de taille!
                if(o) Cd[i].preparer_col(j,1); // colorisation du sommet du losange          
              }
           }              
        } 
       g->suivant(); 
     }
    return 1;
}
