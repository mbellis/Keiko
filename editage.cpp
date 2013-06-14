/*

    edition de texte

*/

class EDITAGE_TEXTE{
    char text[4096];
    char ttext[4096]; // tampon pour la position du curseur!
    char *pcurseur;
    char *ptcurseur;
    char *pt;
    int longueur; // definit la taille de la fenetre : plusieurs ligne de 35 car
    /*   si longueur = -1 pas d'edition en cours !!!!!!            */
    public:
    int initier(char *s);
    int fixer_mouse(int a,int action);
    int fixer_clavier(int a,int b);
    int afficher();
    int tester();
    int aff_cadre();
    int vider();
    int is_actif();
    int get_text(char *s);
    };
int EDITAGE_TEXTE::initier(char *s)
{
    int i;
    
    for(i=0;i<4096;i++) text[i] = 0;
    longueur = long_string(s);
    copie_string(s,text);
    pcurseur = &text[0];
    
    return 1;
}
int EDITAGE_TEXTE::is_actif()
{
    return longueur+1;
}
int EDITAGE_TEXTE::vider()
{
    longueur = -1;
    return 1;
}
int EDITAGE_TEXTE::get_text(char *s)
{
    copie_string(text,s);
    return 1;
}
int EDITAGE_TEXTE::fixer_mouse(int a,int action)
{
/*
    action 1 selection de caractere
    action 2 annulation
*/

    return 1;
}
int EDITAGE_TEXTE::fixer_clavier(int a,int b)
{
    char sav[4096];
    
    if(longueur < 0) return 0;
    if(b)
     { switch(a){
        case 37: pcurseur--; // <-
                 if(pcurseur < text) pcurseur = text;
                 break;
        case 39: pcurseur++; //->
                 if(pcurseur > text+longueur) pcurseur = text + longueur;
                 break;
        case 40: pcurseur = text + longueur;// down
                 break;
        case 38: pcurseur = text;// up
                 break;
        case 32: *pcurseur++ = 32;// up
                 break;
        case 8:  copie_string(pcurseur,sav); // del
                 if(pcurseur>text) pcurseur--;
                 *pcurseur = 0;
                 aj_string(sav,text);        
                 break;  
        case 46: pcurseur++;
                 copie_string(pcurseur+1,sav); // suppr 
                 *pcurseur = 0;
                 aj_string(sav,text); 
                 pcurseur--;
                 break;   
        case 13: *pcurseur = 0;
                 Mouse.flash_button(1);
                 break;
        default: 
            if(a > 64) 
              if(a < 91)
                { copie_string(pcurseur,sav+1);
                  sav[0] = a +32;
                  *pcurseur = 0;
                  aj_string(sav,text);
                  pcurseur++;
                }
          };
      }
     
    return 1;
}
int EDITAGE_TEXTE::afficher()
{
    int i;
    
    if(longueur == -1) return 1; // rien a afficher

    pt = text;
    ptcurseur = ttext;
    while((*pt)&&(pt<pcurseur))
     { *ptcurseur++ = ' ';
        pt++;
     }
    if(*pt)*ptcurseur++ = '_';
    *ptcurseur = 0;
        
    glLoadIdentity();
    glTranslatef(-0.4,-0.4,-1);
    
    glColor3f(0.9,0.9,0.9);
    glRasterPos2f(0.02,0.02);
	glPrint(text);
    glRasterPos2f(0.02,0.02);
	glPrint(ttext);
    
    glColor3f(0.1,0.2,0.2);
    glTranslatef(0.4,0.0,0.0);
    glBegin(GL_QUADS);									
      glVertex3f(-1.0,0.0,-0.01);     
  	  glVertex3f(-1.0,0.04,-0.01);      
  	  glVertex3f(1.0,0.04,-0.01);
  	  glVertex3f(1.0,0.0,-0.01);
    glEnd();

return 1;
}
int EDITAGE_TEXTE::tester()
{

    return 1;
}
