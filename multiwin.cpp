/*
    Gestion de plusieurs fenetres



*/
#include <GL/glut.h>


class HAPPY_WIN{
    int win[32];
    int n_win;
    int act_win,pw;
    public:
    int sel_fen(int a);
    int create_princeps();
    int aj_win();
    //int supprimer();
    int affichage();
    int mis_a_jour();
    int resize(int width, int height);
    int resize2(int width, int height);
    int key_press(int a,unsigned char key);
    int key_princeps(unsigned char key);
    int key_sub1(unsigned char key);
    int key_sub2(unsigned char key);
    int select_princeps(int x,int y);
    int select_sub1(int x,int y);
    int select_sub2(int x,int y);
    /***********/
    int aff_sup1();
    /*********/
    int demo1();
    int demo2();
    };
/******************************************/    
int HAPPY_WIN::create_princeps()
{
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("princeps");
   
    //glClearColor(0.9,0.9,0.9,1);
    glClearColor(0,0.6,0.8,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);


    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
  
    win[0] = glutGetWindow();
    act_win = win[0];
    n_win = 1;
    return n_win;
}    
int HAPPY_WIN::sel_fen(int a)
{
    glutSetWindow(win[a]);
    return 1;
}
int HAPPY_WIN::aj_win()
{
    glutInitWindowSize(540,600);
    glutInitWindowPosition(550+(50*n_win),360);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("sub");
   
    if(n_win == 2) //glClearColor(0.9,0.9,0.9,1);
      glClearColor(0,0.6,0.8,1);
    else glClearColor(0,0,0,1);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);


    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    win[n_win] = glutGetWindow();
    act_win = n_win;
    return n_win++;
}
int HAPPY_WIN::resize(int width, int height) 
{
    //const float ar = (float) width / (float) height;
    AR = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-AR, AR, -1.0, 1.0, 2.0, 800.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    return 1;
}  
int HAPPY_WIN::resize2(int width, int height) 
{
    //const float ar = (float) width / (float) height;
    //AR = (float) width / (float) height;
    glViewport(0, 0, 540,600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    return 1;
}  
int HAPPY_WIN::mis_a_jour()
{
    for(pw = 0;pw<n_win;pw++) 
     { 
       glutPostRedisplay();  
     } 
    return 1;  
}
int HAPPY_WIN::affichage()
{
    sel_fen(0);   
     Univers.affichage();

    sel_fen(1);
     aff_sup1(); 
    
    sel_fen(2);
       Univ2.affichage();   
     
    return 1;
}
int HAPPY_WIN::key_press(int a,unsigned char key)
{
    if(a == win[0]) key_princeps(key); 
    if(a == win[1]) key_sub1(key); 
    if(a == win[2]) key_sub2(key); 
  
    return a;
}
int HAPPY_WIN::key_princeps(unsigned char key)
{
    switch (key)   
    { 
        case 27 : 
        //case 'q':
            exit(0); 
            break;
    
        case '8': angle[0]++;break;
        case '2': angle[0]--;break;
        case '6': angle[1]++;break;
        case '4': angle[1]--;break;
        case '9': angle[2]++;break;
        case '1': angle[2]--;break;
        case 's': Rezo.sauver();
                  sprintf(Messager,"reseau sauvegarde\0");
                  break;
        case 'h': Helios = Helios^1;break;
        case 'H': HIDE = HIDE^1;break;
        case 'A': Univers.act_marcher(-1);break;
        case 'a': Univers.act_marcher(0);break;
        case ' ': Univers.act_marcher(1);break;
        case '+': Univers.neo_lien_seuil(200);
                  break;
        case '-': Univers.neo_lien_seuil(-200);
                  break;
        case 'r': profondeur[2] = -40.0;
        case 'c': profondeur[0] = profondeur[1] = 0.0;break;
        case 'I': Univers.initer(&Rezo);break;
        case 'j': Univers.act_selection();
        case 'l': Univers.activer_lien(1);break;
        case 'L': Univers.activer_lien(2);break;
        case 'm': voir_force = voir_force^1;break;
        case 'k': Univers.fixe_aff_pos(-1,0,0,0);break;
        case 'z':Univers.geler_pos(1);break;
        case 'Z':Univers.geler_pos(0);break;
        //case 'P': CChemin.charger(Univers.idcamera);break;
        //case 'O': CChemin.charger_liste();break;
        //case 'p': CChemin.activer();break;
        //case 'n': Dregul.afficher();break;
        //case 'N': Dregul.preparer_reg(53);break;
    }
    return 1;
}
int HAPPY_WIN::key_sub1(unsigned char key)
{
   /* switch (key)   
    { 
        case 27 : 
            exit(0); 
            break;
    }*/
    Ed_texte.fixer_clavier(key,1);
    return 1;
}
int HAPPY_WIN::key_sub2(unsigned char key)
{
    switch (key)   
    {   case 27 : 
        //case 'q':
            exit(0); 
            break;
    
        case '8': angle2[0]++;break;
        case '2': angle2[0]--;break;
        case '6': angle2[1]++;break;
        case '4': angle2[1]--;break;
        case '9': angle2[2]++;break;
        case '1': angle2[2]--;break;
        case 's': Rezo2.sauver();
                  sprintf(Messager2,"reseau sauvegarde\0");
                  break; 
        case 'h': Helios = Helios^1;break;
        case 'A': Univ2.act_marcher(-1);break;
        case 'a': Univ2.act_marcher(0);break;
        case ' ': Univ2.act_marcher(1);break; 
        case '+': Univ2.neo_lien_seuil(200);
                  break;
        case '-': Univ2.neo_lien_seuil(-200);
                  break;
        case 'r': profondeur2[2] = -40.0;
        case 'c': profondeur2[0] = profondeur2[1] = 0.0;break;
        case 'I': Univ2.initier(&Rezo2);break;
        case 'j': Univ2.act_selection();
        case 'l': Univ2.activer_lien(1);break;
        case 'L': Univ2.activer_lien(2);break;
        case 'm': voir_force = voir_force^1;break;
        case 'k': Univ2.fixe_aff_pos(-1,0,0,0);break;
        case 't': Univ2.flag_txt ^= 1;
      //  case 'P': CChemin.charger(Univers.idcamera);break;
      //  case 'O': CChemin.charger_liste();break;
      //  case 'p': CChemin.activer();break;
      //  case 'n': Dregul.afficher();break;
      //  case 'N': Dregul.preparer_reg(53);break;
    }
    return 1;
}
int HAPPY_WIN::select_princeps(int x,int y)
{
    GLint choose,depth,hits;
    GLint viewport[4];
    int i;


    glGetIntegerv(GL_VIEWPORT, viewport);
    //AR = (float) viewport[2]/(float) viewport[3];
    glSelectBuffer(1024, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
     glLoadIdentity();
     gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y), 1.0f, 1.0f, viewport);
     glFrustum(-AR, AR, -1.0, 1.0, 2.0, 200.0);
     glMatrixMode(GL_MODELVIEW);

     Univers.aff_select();
     
     glMatrixMode(GL_PROJECTION);   
    glPopMatrix();   
    glMatrixMode(GL_MODELVIEW);     
    
    choose = -1;
    hits = glRenderMode(GL_RENDER);
    if (hits > 0)                                                               
     { choose = selectBuf[3];
       depth = selectBuf[1];    
       for (i = 1; i < hits; i++)                           
        { if (selectBuf[i*4+1] < GLuint(depth))
          // If This Object Is Closer To Us Than The One We Have Selected
             if (selectBuf[i*4+1] < GLuint(depth))
              { choose = selectBuf[i*4+3];                      // Select The Closer Object
                depth = selectBuf[i*4+1];                       // Store How Far Away It Is
              }       
        }
        /**************************ACTION*********************************/
        if(choose)Univers.fixe_aff_pos(choose,0,0,0);
        sprintf(Messager,"selection : %d\0",choose+1);
     }
    return choose;
}
int HAPPY_WIN::select_sub2(int x,int y)
{
    GLint choose,depth,hits;
    GLint viewport[4];
    int i;


    glGetIntegerv(GL_VIEWPORT, viewport);
    //AR = (float) viewport[2]/(float) viewport[3];
    glSelectBuffer(1024, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
     glLoadIdentity();
     gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y), 1.0f, 1.0f, viewport);
     glFrustum(-AR, AR, -1.0, 1.0, 2.0, 200.0);
     glMatrixMode(GL_MODELVIEW);

     Univ2.aff_select();
     
     glMatrixMode(GL_PROJECTION);   
    glPopMatrix();   
    glMatrixMode(GL_MODELVIEW);     
    
    choose = -1;
    hits = glRenderMode(GL_RENDER);
    if (hits > 0)                                                               
     { choose = selectBuf[3];
       depth = selectBuf[1];    
       for (i = 1; i < hits; i++)                           
        { if (selectBuf[i*4+1] < GLuint(depth))
          // If This Object Is Closer To Us Than The One We Have Selected
             if (selectBuf[i*4+1] < GLuint(depth))
              { choose = selectBuf[i*4+3];                      // Select The Closer Object
                depth = selectBuf[i*4+1];                       // Store How Far Away It Is
              }       
        }
        /**************************ACTION*********************************/
        if(choose)Univ2.fixe_aff_pos(choose,0,0,0);
        sprintf(Messager2,"selection : %d\0",choose+1);
     }
    return choose;
}
int HAPPY_WIN::select_sub1(int x,int y)
{
    GLint choose,depth,hits;
    GLint viewport[4];
    int i;


    glGetIntegerv(GL_VIEWPORT, viewport);
    //AR = (float) viewport[2]/(float) viewport[3];
    glSelectBuffer(1024, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
     glLoadIdentity();
     gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y), 1.0f, 1.0f, viewport);
     glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 100.0);
     glMatrixMode(GL_MODELVIEW);

     Fen1.aff_select();
     
     glMatrixMode(GL_PROJECTION);   
    glPopMatrix();   
    glMatrixMode(GL_MODELVIEW);     
    
    choose = -1;
    hits = glRenderMode(GL_RENDER);
    if (hits > 0)                                                               
     { choose = selectBuf[3];
       depth = selectBuf[1];    
       for (i = 1; i < hits; i++)                           
        { if (selectBuf[i*4+1] < GLuint(depth))
          // If This Object Is Closer To Us Than The One We Have Selected
             if (selectBuf[i*4+1] < GLuint(depth))
              { choose = selectBuf[i*4+3];                      // Select The Closer Object
                depth = selectBuf[i*4+1];                       // Store How Far Away It Is
              }       
        }
        /**************************ACTION*********************************/
        //if(choose)
         Fen1.fixe_clic(choose);
     }
    //Fen1.fixe_clic(hits);
    return choose;
}
int HAPPY_WIN::aff_sup1()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    Fen1.afficher();  
    
    glutSwapBuffers();
    return 1;
}

/********************************************************************/ 
int HAPPY_WIN::demo1()
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
    glPushMatrix();
     glColor3d(1,0,0);
     glTranslated(-2.4,1.2,-20);
     glutSolidSphere(1,20,20);
    glPopMatrix();
    
    glutSwapBuffers();
    return 1;
}
int HAPPY_WIN::demo2()
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
    glPushMatrix();
     glColor3d(0,1,0);
     glTranslated(0,0,-10);
     Sphere.afficher();
    glPopMatrix();
    
  /*  glPushMatrix();
     glColor3d(1,0,0);
     glTranslated(-2.4,1.2,-20);
     glutSolidSphere(1,20,20);
    glPopMatrix(); */
    
    glutSwapBuffers();
    return 1;
}

