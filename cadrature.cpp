/*
 Classes pour cadre de selection et edition


*/
class CADLosange{
    char text[3][25];
    char liste_toggle[256];
    EDITAGE_TEXTE *ped_txt;
    float x,y,z;
    float box[4][3];
    float col[3][3];
    float max;
    int toggle;
    int n_col;
    int base;
    public: 
    int creer(float ax,float ay,float az,float zz);
    int fixer_col(int a,float a1,float a2,float a3);
    int fixer_text(int a,char *s);
    int fixer_liste_toggle(char *s);
    int inc_toggle(int a);
    int calc_box();
    int fixer_base(int a);
    int lier_editage_texte(EDITAGE_TEXTE *pedt);
    int get_texte(int a,char *s);
    /**********************************/
    int afficher();
    int aff_select();
    };
    
int CADLosange::creer(float ax,float ay,float az,float zz)
{
    x = ax;
    y = ay;
    //z = az;
    z = -50; // profondeur fixe -> facilite
    max = zz;
    ped_txt = NULL;
    calc_box();
    return 1;
}
int CADLosange::fixer_col(int a,float a1,float a2,float a3)
{
    col[a][0] = a1;
    col[a][1] = a2;
    col[a][2] = a3;
    return a;
}
int CADLosange::fixer_text(int a,char *s)
{
    char sav;
    
    if(long_string(s) > 24)
     { sav = s[24];
       s[24] = 0;
       copie_string(s,text[a]);
       s[24] = sav;
     }
    else copie_string(s,text[a]);
    
    return 1;
}
int CADLosange::fixer_base(int a)
{
    base = a;
    return 1; 
}
int CADLosange::fixer_liste_toggle(char *s)
{
    char sav;
    
    if(long_string(s) > 255)
     { sav = s[255];
       s[255] = 0;
       copie_string(s,liste_toggle);
       s[255] = sav;
     }
    else copie_string(s,liste_toggle);
    
    toggle = -1;
    return 1;
}
int CADLosange::inc_toggle(int a)
{
    char *pt;
    int i,j;
    
    toggle++;
    
    pt = lire_seq(liste_toggle,text[a],'\t');
    i = j = 0;
    while(i<toggle)
     { if(*pt) pt = lire_seq(pt,text[a],'\t');
       else 
        { pt = lire_seq(liste_toggle,text[a],'\t');
          j = i+1;
        }
       i++;
     }
    toggle -= j;

    return 1;
}
int CADLosange::calc_box()
{
    box[0][0] = -1.4;
    box[0][1] = 1.8;
    box[0][2] = -1;
    box[1][0] = -1.4;
    box[1][1] = -1.8;
    box[1][2] = -1;
    box[2][0] = max;
    box[2][1] = -1.8;
    box[2][2] = -1;
    box[3][0] = max;
    box[3][1] = 1.8;
    box[3][2] = -1;
    return 1;
}
int CADLosange::get_texte(int a,char *s)
{
    copie_string(text[a],s);
    return 1;
}
int CADLosange::lier_editage_texte(EDITAGE_TEXTE *pedt)
{
    ped_txt = pedt;
    return 1;
}
int CADLosange::afficher()
{ 
    glTranslatef(x,y,z);
    
    n_col = 0;
    glBegin(GL_QUADS);  
        glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);              
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;                  
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]); 
    glEnd(); 
      
    glTranslatef(-x,-y,-z);
    if(ped_txt)
     { ped_txt = ped_txt->get_text(text[1],text[2]);
       if(text[1])
       { n_col = 3;
         glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]); 
         glutEcrire(x,y-1,z,text[1]);
         glutEcrire(x,y-1,z,text[2]);
       }
     }
    else
     { if(text[0]) 
        { n_col = 2;
          glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]); 
          glutEcrire(x-0.4,y+0.5,z,text[0]);
        }
      if(text[1])
       { n_col = 1;
         glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]); 
         glutEcrire(x,y-1,z,text[1]);
       }
     }
    
    return 1;
}       
int CADLosange::aff_select()
{
    glTranslatef(x,y,z);
    glLoadName(base);  
    n_col = 0;
    glBegin(GL_QUADS);        
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;                  
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
        glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]); 
    glEnd(); 
      
    glTranslatef(-x,-y,-z);
    return 1;
}
