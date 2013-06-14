/*
    Gestion de la selection par couleurs
*/

class MENULOSANGE {
    GROUP_ARN *garn;
    GROUP_CLASSEUR *gcl;
    float col[5][3]; // les couleurs utilisées
    char texte [4][256];// texte de description
    char type[4][64];// descritpion du type de texte; lié à is_actif
    int is_moused[9];
    int is_actif[9]; // -1 texte  sinon 1-x type annotation+1
    int flag_actif[9]; // 0 ou 1 : interrupteur
    int ID_base; // debut de l'ID des obket pour la selection par mouse
    int klik_defaut;
    public:
    int initier(GROUP_CLASSEUR *gcl);
    int vider();
    int afficher();
    int tester();
    int fixer_los(int num,float c[3],char *s);
    int fixer_mouse(int num, int action);
    int initier_menu(int num);
    int lire_param(int a,char *s);
    int fixer_default_param(GROUP_ARN *g);
    int fixer_param(int a,int b,char *s);
    int actualiser(GROUP_ARN *g);
    };
int MENULOSANGE::vider()
{
    int i;
    
    for(i=0;i<3;i++) 
        texte[i][0] = 0;
    for(i=0;i<9;i++)
        is_moused[i] = 0; 
    
    ID_base = 0;   
    col[4][0] = col[4][1] = col[4][2] = 0.8;
    col[4][0] = 0.1;
        
    return 1;
}   
int MENULOSANGE::fixer_mouse(int num, int action)
{
    int i,j;
    
    for(i=0;i<9;i++)
        is_moused[i] = 0; // mouse partie!
    
    if(num >= 0)
     if(num < 9) 
      { is_moused[num]++;
        if(action == 2) 
         { initier_menu(num);
           return 1; // Menu deroulant acceptable
         }
        if(action == 1) // click gauche
         { switch(num){
                  case 0 : // search -> new GROUP_ARN
                          if(klik_defaut == 1)
                           { nop();
                           }
                          break;
                  case 2 : flag_actif[num] = flag_actif[num]^1; // XOR : 0->1 1->0
                          if(!is_actif[num]) is_actif[num] = 1; // par defaut : probesetID
                          break;
                  case 4 :flag_actif[num] = flag_actif[num]^1; // XOR : 0->1 1->0
                          if(!is_actif[num]) is_actif[num] = 1;
                          break;
                  case 6 :flag_actif[num] = flag_actif[num]^1; // XOR : 0->1 1->0
                          if(!is_actif[num]) is_actif[num] = 1;
                          break;
                  case 8 :flag_actif[num] = flag_actif[num]^1; // XOR : 0->1 1->0
                          if(!is_actif[num]) is_actif[num] = 1;
                          break;
                  default :  
                         j = num >> 1;
                         Editage.initier(texte[j]);
                         return 1; // editage
                  };
           actualiser(garn);
           return 0; // Menu deroulant inacceptable
         }
        
      }
     
    return 0;
}
int MENULOSANGE::fixer_los(int num,float c[3],char *s)
{
    int i;
    char *ps,*pt,*pto;
    
    if((num>3)||(num < 0)) return 0;
    
    for(i = 0;i<3;i++) 
        col[num][i] = c[i];
    
    ps = s;
    pt = pto = texte[num];   
    while((*ps)&&(pt-pto< 25)) *pt++ = *ps++;
    *pt = 0;
    
    return 1;
}
int MENULOSANGE::afficher()
{
    int i,j,n_obj;
    
    glTranslatef(-2,1,-4);
    
   	glBegin(GL_QUADS);	// fond de decor
        glColor3f(0.1,0.1,0.1);								
        glVertex3f(-1.5,-4.0,-0.5);
        glVertex3f(0.6,-4.0,-0.5);
        glVertex3f(0.6,0.5,-0.5);
        glVertex3f(-1.5,0.5,-0.5);	
	glEnd();	
    
    j=0;
    n_obj = 0;
	glBegin(GL_QUADS);	
        glColor3f(col[j][0],col[j][1],col[j][2]);								
        glVertex3f(0.2,0,0);
        j++;
        glColor3f(col[j][0],col[j][1],col[j][2]);			
        glVertex3f(0,0.2,0);
        j++;	
        glColor3f(col[j][0],col[j][1],col[j][2]);				
        glVertex3f(-0.2,0,0);
        j++;
        glColor3f(col[j][0],col[j][1],col[j][2]);				
        glVertex3f(0,-0.2,0);
        j++;
        glColor3f(0.1,0.1,0.1);	        
        if(is_moused[n_obj]) glColor3f(col[j][0],col[j][1],col[j][2]);
        glVertex3f(0.23,0,-0.02);
        glVertex3f(0,0.23,-0.02);	
        glVertex3f(-0.23,0,-0.02);
        glVertex3f(0,-0.23,-0.02);      			
	glEnd();	

    for(i=0;i<4;i++)
     { glTranslatef(0,-0.5,0);
      if(is_actif[++n_obj])
      if(flag_actif[n_obj+1])
      if(texte[i][0])
   	   { /*glColor3f(0.1,0.4,0.4);	
	     glBegin(GL_QUADS);				 // le losange de propriété						
 		  glVertex3f(0.09,0.09,-0.1);
  	      glVertex3f(0.7,0.09,-0.1);
  	      glVertex3f(0.7,0.2,-0.1);
  	      glVertex3f(0.07,0.2,-0.1);
         glEnd();*/
         glColor3f(0.8,0.8,0.8);
   	     if(is_moused[n_obj]) glColor3f(0.6,0.6,1.0);
	     glRasterPos2f(0.1,0.1);
	     glPrint(texte[i]);
	    /* KillFont();
	     BuildFont2();
	     glPrint(texte[i]);
	     KillFont();
	     BuildFont();
        */
	   }
	   glColor3f(col[i][0],col[i][1],col[i][2]);
	   
       //if(!is_actif[++n_obj])
       if(!flag_actif[++n_obj])	glColor3f(0.2,0.2,0.2);
       glBegin(GL_QUADS);				 // le losange de propriété						
 		  glVertex3f(0.1,0,0);
  	      glVertex3f(0,0.1,0);
  	      glVertex3f(-0.1,0,0);
  	      glVertex3f(0,-0.1,0);
          glColor3f(0.1,0.1,0.1);
  	      if(is_moused[n_obj]) glColor3f(col[j][0],col[j][1],col[j][2]);	
          glVertex3f(0.13,0,-0.02);
          glVertex3f(0,0.13,-0.02);	
          glVertex3f(-0.13,0,-0.02);
          glVertex3f(0,-0.13,-0.02);    
	   glEnd();
     }
 					
	glLoadIdentity();
	return 1;
}
int MENULOSANGE::tester()
{
    int i,j,n_obj;
    
    glTranslatef(-2,1,-4);
    
    n_obj = ID_base;
    glLoadName(n_obj++);
    j=0;
	glBegin(GL_QUADS);	
        glVertex3f(0.2,0,0);
        j++;
        glVertex3f(0,0.2,0);
        j++;	  			
        glVertex3f(-0.2,0,0);
        j++;  		
        glVertex3f(0,-0.2,0);	
	glEnd();	
	
    for(i=0;i<4;i++)
     {  glTranslatef(0,-0.5,0);
        glLoadName(n_obj++); // 1 le texte
        if(flag_actif[n_obj])
        if(is_actif[n_obj - ID_base])
         { glBegin(GL_QUADS);				 // Pour la selection du texte						
 		     glVertex3f(0.09,0.09,-0.1);     // toujours selectionnable
  	         glVertex3f(0.7,0.09,-0.1);      // A traiter ulterieurement
  	         glVertex3f(0.7,0.25,-0.1);
  	         glVertex3f(0.09,0.25,-0.1);
           glEnd();
        }
	        
	    glLoadName(n_obj++); // 2 le losange
        glBegin(GL_QUADS);				 // le losange de propriété						
 		  glVertex3f(0.1,0,0);
  	      glVertex3f(0,0.1,0);
  	      glVertex3f(-0.1,0,0);
  	      glVertex3f(0,-0.1,0);
	    glEnd();
     }    
	glLoadIdentity();
}
int MENULOSANGE::initier_menu(int num)
{
    char tamp[4096];
    
    switch(num){
        case 0 : Menud.initier("Chercher\tActualiser\0");
                 break;
        case 1 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);
                 break;
        case 2 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);break;
        case 3 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);
                 break;
        case 4 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);break;
        case 5 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);
                 break;
        case 6 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);break;
        case 7 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);break;
        case 8 : Puce.get_definitions(tamp);
                 Menud.initier(tamp);break;
        default : nop();
        };
    return 1;
}
int MENULOSANGE::fixer_default_param(GROUP_ARN *g)
{
    int i;
    
    klik_defaut = 1;
    
    is_actif[1] = is_actif[2] = 2; // attention au +1!
    is_actif[3] = is_actif[4] = 3; // attention au +1!
    is_actif[5] = is_actif[6] = 0; // attention au +1!
    is_actif[7] = is_actif[8] = 0; // attention au +1!
    
    for(i=0;i<5;i++)  flag_actif[i] = 1;
    for(i=5;i<9;i++)  flag_actif[i] = 0;
    
    garn = g;
    actualiser(garn);    

    return 1;
}
int MENULOSANGE::actualiser(GROUP_ARN *g)
{
    char tamp[4096];
    int i;
    
    if(g) garn = g;

    for(i=0;i<4;i++)
     { is_actif[i*2+1] = is_actif[i*2+2];
       copie_string("texte",type[i]);
       if(is_actif[2+2*i]> 0)
        { garn->lire_exemple(is_actif[2+2*i]-1,tamp);
          tamp[25] = 0;
          copie_string(tamp,texte[i]);
        } 
     }
     return 1; 
}
int MENULOSANGE::lire_param(int a,char *s)
{
    if(a<4)
     { copie_string(texte[a],s);
       if(flag_actif[a*2+2]) return is_actif[a*2+2]; 
       // +2 point sur le losange et non sur le texte!
     }
    return 0;
}
int MENULOSANGE::fixer_param(int a,int b,char *s)
{
    int k;
    
    if(!a) // losange de commande
     { klik_defaut = b;
       return 1;       
     }
    if(a>8) return 0;
    if(a & 1) k = a+1;
    if(b>=0)
     { is_actif[a] = b+1;
       is_actif[k] = b+1;
     }
    if(b<0)
     { k = a >> 1;
       copie_string(s,texte[k]);
       is_actif[k*2+1] = is_actif[k*2+2] = b; // texte 
     }
    actualiser(garn); 
    return 1;
}
