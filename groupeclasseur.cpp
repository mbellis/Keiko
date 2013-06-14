/*

    Gestion du classeur de groupe pointant sur GROUP_ARN
    Gestion purement graphique : simple selection par souris!

*/

class GROUP_CLASSEUR {
    RAM_ARN *RAMarn;
    GROUP_ARN* Ga[4096];
    GROUP_ARN SGa[4096];
    int n_Ga; // nombre de groupe 
    int p_SGa,p_max; // position pointant la lecture des Ga 
    int num_expo; // numero du groupe actif
    int base; // reference pour la seelction des objects
    int borne[2][2];// actif/ inactif et position
    int is_moused[161];
    double rquad;
    public:
    GROUP_ARN* Ga_expo;
    int initier(RAM_ARN *ra);
    int afficher();
    int tester();
    int ajouter();
    int retirer(int a);
    GROUP_ARN* get_expo();
    int fixer_mouse(int a,int action);
    
    };
    
int GROUP_CLASSEUR::initier(RAM_ARN *ra)
{
    int i;
    RAMarn = ra;
    p_max = 4096;
    num_expo = n_Ga = p_SGa = 0;
    borne[0][0] = borne[0][1] = 0;
    borne[1][0] = borne[1][1] = num_expo;
    base = 70;
    
    for(i=0;i<p_max;i++) SGa[i].initier(RAMarn);
    
    ajouter();
    Ga_expo = Ga[num_expo];
    
    return 1;
}
GROUP_ARN* GROUP_CLASSEUR::get_expo()
{
    return Ga[num_expo];
}

int GROUP_CLASSEUR::ajouter()
{
    int i;
    ARN *ps;
    
    if(n_Ga > 4095) 
     { n_Ga--; // evite l'overflow
       SGa[p_SGa].initier(RAMarn);
       num_expo = n_Ga;
       return ++n_Ga; // signale pas le depassement
     }
    
    i = 0;
    ps = SGa[p_SGa].go_prime(); // verifie si deja utilisé 
    while(ps && (i<p_max))
     { p_SGa++;
       i++;
       if(p_SGa >= p_max) p_SGa = 0; // retour au debut du tableau
       ps = SGa[p_SGa].go_prime();
     }
    if(i >= p_max) // Ca ne doit jamais arriver! (en theorie....)
     { p_SGa = p_max-1;
     } 
   
    Ga[n_Ga] = &SGa[p_SGa];
    SGa[p_SGa].initier(RAMarn);
    num_expo = n_Ga; // le dernier entré est actif!
    return ++n_Ga;
}

int GROUP_CLASSEUR::retirer(int a)
{
    int i;
    
    if(a>n_Ga) return 0;
    
    if(Ga[a]->go_prime()) // encore utilisé
     { Ga[a]->effacer();
     }
 
    n_Ga--;
    for(i=a;i<n_Ga;i++)
     Ga[i] = Ga[i+1];
    
    return n_Ga;
}

int GROUP_CLASSEUR::afficher()
{
    double a,dx,dy,dz;
    int i,j,k;
       
    glLoadIdentity(); 
    if(!n_Ga) return 1;
    // segment inferieur
    k = num_expo;
    if(k > 80) k = 80;
    a = (double)k;
    if(a < 0.01) a = 0.01;
    a = 5 / a;
    if(a < 0.01) a = 0.01;
    
    glTranslatef(2,10,-30);
    dx = dz = dy = 0.0;
    
   rquad = 45; 
   glRotatef(rquad,0.0f,1.0f,0.0f); 
   for(i=0;i<k;i++) // num_expo
     { dx -= a;
       dz -= a;
       dy += a/10;
       glBegin(GL_QUADS);	// losange des groupes
        glColor3f(0.2,0.9,0.2);								
        glVertex3f(-1+dx,0+dy,0+dz);
        glVertex3f(0+dx,-1+dy,0+dz);
        glColor3f(0.2,0.1,0.9);	
        glVertex3f(1+dx,0+dy,0+dz);
        glVertex3f(0+dx,1+dy,0+dz);	
	   glEnd();	
     }  
    rquad = -45; 
    glRotatef(rquad,0.0f,1.0f,0.0f);  
    
   // segment superieur
    //k = n_Ga;
    k = n_Ga - num_expo -1;
    if(k > 80) k = 80;
    a = (double)k;
    if(a < 0.01) a = 0.01;
    a = 5 / a;
    if(a < 0.01) a = 0.01;
    
    glTranslatef(3,0,0);
    dx = dz = dy = 0.0;
    
   rquad = -45; 
   glRotatef(rquad,0.0f,1.0f,0.0f); 
   for(i=0;i<k;i++) // num_expo
     { dx += a;
       dz -= a;
       dy += a/10;
       glBegin(GL_QUADS);	// losange des groupes
        glColor3f(0.2,0.1,0.9);								
        glVertex3f(-1+dx,0+dy,0+dz);
        glColor3f(0.2,0.9,0.2);	
        glVertex3f(0+dx,-1+dy,0+dz);
        glVertex3f(1+dx,0+dy,0+dz);
        glColor3f(0.2,0.1,0.9);	
        glVertex3f(0+dx,1+dy,0+dz);	
	   glEnd();	
     }   
    rquad = 45; 
    glRotatef(rquad,0.0f,1.0f,0.0f); 
        
    // le groupe actif : num_expo
    glTranslatef(-1.5,1,0);
    glColor3f(0.2,0.9,0.2);	
    glBegin(GL_QUADS);	// losange des groupes							
        glVertex3f(-1,0,0);
        glVertex3f(0,-1,0);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);	
	   glEnd();	
    
    glLoadIdentity();  
    
    return 1;
}
int GROUP_CLASSEUR::tester()
{
    double a,dx,dy,dz;
    int i,j,k;
       
    glLoadIdentity(); 
    // segment inferieur
    k = num_expo;
    if(k > 80) k = 80;
    a = (double)k;
    a = 5 / a;
    if(a < 0.01) a = 0.01;
    
    glTranslatef(2,10,-30);
    dx = dz = dy = 0.0;
    
   rquad = 45; 
   glRotatef(rquad,0.0f,1.0f,0.0f); 
   for(i=0;i<k;i++) // num_expo
     { dx -= a;
       dz -= a;
       dy += a/10;
       glLoadName(70 + i);
       glBegin(GL_QUADS);	// losange des groupes
        glVertex3f(-1+dx,0+dy,0+dz);
        glVertex3f(0+dx,-1+dy,0+dz);
        glVertex3f(1+dx,0+dy,0+dz);
        glVertex3f(0+dx,1+dy,0+dz);	
	   glEnd();	
     }  
    rquad = -45; 
    glRotatef(rquad,0.0f,1.0f,0.0f);  
    
   // segment superieur
    k = n_Ga;
    k = n_Ga - num_expo;
    if(k > 80) k = 80;
    a = (double)k;
    a = 5 / a;
    if(a < 0.01) a = 0.01;
    
    glTranslatef(3,0,0);
    dx = dz = dy = 0.0;
    
   rquad = -45; 
   glRotatef(rquad,0.0f,1.0f,0.0f); 
   for(i=0;i<k;i++) // num_expo
     { dx += a;
       dz -= a;
       dy += a/10;
       glLoadName(70 + 80 +i);
       glBegin(GL_QUADS);	// losange des groupes
        glVertex3f(-1+dx,0+dy,0+dz);
        glVertex3f(0+dx,-1+dy,0+dz);
        glVertex3f(1+dx,0+dy,0+dz);
        glVertex3f(0+dx,1+dy,0+dz);	
	   glEnd();	
     }   
    rquad = 45; 
    glRotatef(rquad,0.0f,1.0f,0.0f); 
        
    // le groupe actif : num_expo
    glTranslatef(-1.5,1,0);
    glLoadName(70 + 160);
    glBegin(GL_QUADS);	// losange des groupes							
        glVertex3f(-1,0,0);
        glVertex3f(0,-1,0);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);	
	   glEnd();	
    
    glLoadIdentity();  
    
    return 1;
}
int GROUP_CLASSEUR::fixer_mouse(int a, int action)
{
    int i,k;
    
    for(i=0;i<161;i++)
        is_moused[i] = 0; // mouse partie!
        
    if(a < 70) return 0;
    if(a > 70 +161) return 0; // exit rapide
    
    k = a - 70; 
    is_moused[k]++;
    i = -1;
    if(action == 2)  // definir une limite?
     { if(k == 160) i = num_expo;
       if(k < 60) i = num_expo - (k+1);
       if((k < 140)&&(k >= 80)) i = num_expo + (k-79);
       if(i > -1)
        { if(borne[0][0])
           { borne[0][0] = borne[0][1] = 1; // ensemble activé
             if(i< num_expo)
              { borne[1][0] = i;
                borne[1][1] = num_expo;
              }
             else
              { borne[1][1] = i;
                borne[1][0] = num_expo;
              }
           }
          else // mise à jour
           { if(i< borne[1][0]) borne[1][0] = i;
             else borne[1][1] = i;
           }
        }      
     }
    if(action == 1) // simple selection -> vers num_expo
     { if(k == 160) // num_expo
         { borne[0][0] = borne[0][1] = 0; // pas de selection
           borne[1][0] = borne[1][1] = num_expo; // par defaut
           return 0;
         }
       if(k < 60) // selection direct
         { num_expo -= (k+1); 
           return 0;
         }
       if(k<80) // selection proportionnelle
         { num_expo = 10;
           return 0;
         }
       if(k < 140) // selection direct
         { num_expo += (k-79); 
           return 0;
         }
       if(k < 160) // selection proportionnelle
         { num_expo = n_Ga - 10;
           return 0;
         }
     }
    return 0;
}
