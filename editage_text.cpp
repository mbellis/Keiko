/*

    edition de texte

*/

class EDITAGE_TEXTE{
    char text[96];
    char ttext[96]; // tampon pour la position du curseur!
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
    EDITAGE_TEXTE* get_text(char *s1,char *s2);
    };

int EDITAGE_TEXTE::initier(char *s)
{
    int i;
    
    for(i=0;i<96;i++) text[i] = 0;
    longueur = long_string(s);
    copie_string(s,text);
    pcurseur = &text[longueur];
    
    pt = text;
    ptcurseur = ttext;
    while((*pt)&&(pt<pcurseur))
     { *ptcurseur++ = ' ';
        pt++;
     }
    //if(*pt)
    *ptcurseur++ = '_';
    *ptcurseur = 0;
    
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
EDITAGE_TEXTE* EDITAGE_TEXTE::get_text(char *s1,char *s2)
{
    copie_string(text,s1);
    copie_string(ttext,s2);
    if(Mouse.action[0] == 3) 
     { longueur = -1;
       return NULL;
     }
    return this;
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
    char sav[96];
    
    if(longueur < 0) return 0;
    if(b)
     { switch(a){
    /*    case GLUT_KEY_LEFT: pcurseur--; // <-
                 if(pcurseur < text) pcurseur = text;
                 break;
        case GLUT_KEY_RIGHT: pcurseur++; //->
                 if(pcurseur > text+longueur) pcurseur = text + longueur;
                 break;
        case GLUT_KEY_DOWN: pcurseur = text + longueur;// down
                 break;
        case GLUT_KEY_UP: pcurseur = text;// up
                 break; */
        case 32: *pcurseur++ = 32;// espace
                 break;
        case 8:  copie_string(pcurseur,sav); // del
                 if(pcurseur>text) pcurseur--;
                 *pcurseur = 0;
                 aj_string(sav,text);        
                 break;  
  /*      case 46: pcurseur++;
                 copie_string(pcurseur+1,sav); // suppr 
                 *pcurseur = 0;
                 aj_string(sav,text); 
                 pcurseur--;
                 break;   */
        case 13: *pcurseur = 0; // terminaison
                 Mouse.flash_button(1);
                 break;
        default:  
            if(a >= 'a') 
              if(a <= 'z')
                { copie_string(pcurseur,sav+1);
                  sav[0] = a;
                  *pcurseur = 0;
                  aj_string(sav,text);
                  pcurseur++;
                }
             if(a >= 'A') 
              if(a <= 'Z')
                { copie_string(pcurseur,sav+1);
                  sav[0] = a;
                  *pcurseur = 0;
                  aj_string(sav,text);
                  pcurseur++;
                } 
             if(a >= '0') 
              if(a <= '9')
                { copie_string(pcurseur,sav+1);
                  sav[0] = a;
                  *pcurseur = 0;
                  aj_string(sav,text);
                  pcurseur++;
                } 
          };
      }
    pt = text;
    ptcurseur = ttext;
    while((*pt)&&(pt<pcurseur))
     { *ptcurseur++ = ' ';
        pt++;
     }
    //if(*pt)
    *ptcurseur++ = '_';
    *ptcurseur = 0;
    
    return 1;
}
int EDITAGE_TEXTE::afficher()
{
    int i;
  /*  
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
 */
return 1;
}
int EDITAGE_TEXTE::tester()
{

    return 1;
}
