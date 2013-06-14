/*

    Les menus deroulants maisons!


*/

class MENU_Deroulant {
    char liste_sel[4096]; // format ***\t****\t***...\0
    char texte[128][36];
    int max_liste;
    int is_moused[128];
    int modele;
    public:
    int initier(char *s);
    int afficher();
    int tester();
    int fixer_mouse(int num,int action);
    };
    
int MENU_Deroulant::initier(char *s)
{
    char tamp[4096],*pt;
    int l;
    
    l = long_string(s);
    if(l > 4094) return 0;
    copie_string(s,liste_sel);
    
    max_liste = 0;
    pt = liste_sel;
    while(*pt)
     { pt = lire_seq(pt,tamp,'\t');
       tamp[35] = 0;
       copie_string(tamp,texte[max_liste]);
       is_moused[max_liste++] = 0;
     }
    glLoadIdentity();
    return 1;
}

int MENU_Deroulant::afficher()
{
    int i,j;
    
    glTranslatef(0,0,-10);
    
	glTranslatef(-4,3,0);
	i = j = 0;
    while(i<max_liste)
     { 
         glColor3f(0.8,0.8,0.8);
   	     if(is_moused[i]) glColor3f(0.6,0.6,1.0);
	     glRasterPos2f(0.1,0.1);
	     glPrint(texte[i]);
	     
	     glColor3f(0.1,0.1,0.3);	
         glBegin(GL_QUADS);				 // le losange de propriété						
 		  glVertex3f(0,0,0);
  	      glVertex3f(0,0.2,0);
  	    
          glColor3f(0.1,0.1,0.3);
  	      if(is_moused[i++]) glColor3f(0.3,0.1,0.2);	
        
          glVertex3f(-0.6,0.2,0);
          glVertex3f(-0.6,0,0);    
	     glEnd();
         glTranslatef(0,-0.3,0);
         if(j++ >18) 
          { glTranslatef(4,6,0);
            j = 0;
          }
     }	
     glLoadIdentity();
  	 return 1;
}
int MENU_Deroulant::tester()
{
    int i,j;
 
    glTranslatef(0,0,-10);
    
	glTranslatef(-4,3,0);
	i = j = 0;
    while(i<max_liste)
     {   glLoadName(i++); // 1 le texte      
         glBegin(GL_QUADS);				 				
 		  glVertex3f(0,0,0);
  	      glVertex3f(0,0.2,0); 	    
          glVertex3f(-0.6,0.2,0);
          glVertex3f(-0.6,0,0);    
	     glEnd();
         glTranslatef(0,-0.3,0);
         if(j++ >18) 
          { glTranslatef(4,6,0);
            j = 0;
          }
     }	
    glLoadIdentity();
  	return 1;
}
int MENU_Deroulant::fixer_mouse(int num, int action)
{
    int i;
    
    for(i=0;i<max_liste;i++)
        is_moused[i] = 0; // mouse partie!
    
    if(num >= 0)
     if(num < max_liste) 
      { is_moused[num]++;
        if(action == 2) return -1; // selection effectuée
        if(action == 1) return num; // selection effectuée
      }
     
    return -1; // 
}
