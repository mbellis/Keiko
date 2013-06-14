/*


Fonctions de base de l'application freeGlut

 */
#include <GL/glut.h>



#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>


#include "fonks.cpp"
#include "freeglutfonks.cpp"

#define MaxProbe 46000
#define MaxLink 38000000



int win1;
int win2;
GLuint selectBuf[1024];
char Messager[64];
char Messager2[64];
double angle[3];
double profondeur[3];
double angle2[3];
double profondeur2[3];
int Liste_sel[32000];
int Helios;
int voir_force = 1;
int HIDE = 0;
float AR;


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };



#include "Ram_puce.cpp"
RAM_Puce Puce;

#include "zerezo.cpp"
REZO Rezo;

#include "shortpathfic.cpp"
DICO_SHORTPATH CChemin;

#include "dico_regul.cpp"
DICO_REGUL Dregul;

#include "zegeometrik.cpp"
UNIVERS Univers;

#include "bigeometrik.cpp"
REZO_2 Rezo2;
UNIV_2 Univ2;

#include "zemouse.cpp"
MOUSE Mouse;

#include "editage_text.cpp"
EDITAGE_TEXTE Ed_texte;

#include "cadrature.cpp"
CADLosange Losa;

#include "spheritude.cpp"
CADSphere Sphere;

#include "sel_fen.cpp"
FEN_DESC Fen1;

#include "multiwin.cpp"
HAPPY_WIN Fen;




// GLUT callback Handlers

static void
resize(int width, int height)
{
    Fen.resize(width,height);
}
static void
resize_fix(int width, int height)
{
    Fen.resize2(width,height);
}

static void
display(void)
{
    Fen.affichage();
}

static void
key(unsigned char key, int x, int y)
{
    Fen.key_press(glutGetWindow(),key);
    glutPostRedisplay();
}

static void
idle(void)
{
   Univers.marcher();
   Univ2.marcher();
   Fen.mis_a_jour();
}

static void
souris_button1(int button, int state, int x, int y)
{
    int ax,ay;
    Mouse.move(x,y);

    if(state==GLUT_DOWN)
     { if(button==GLUT_LEFT_BUTTON) Mouse.push(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.push(2,x,y);
     }
    if(state==GLUT_UP)
     { if(button==GLUT_LEFT_BUTTON) Mouse.release(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.release(2,x,y);
       if(Mouse.get_dragging(&ax,&ay) == 1) // selection!
        { Fen.select_princeps(x,y);
          Mouse.vider();
        }
     }
}
static void
souris_button2(int button, int state, int x, int y)
{
    int ax,ay;
    Mouse.move(x,y);

    if(state==GLUT_DOWN)
     { if(button==GLUT_LEFT_BUTTON) Mouse.push(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.push(2,x,y);
     }
    if(state==GLUT_UP)
     { if(button==GLUT_LEFT_BUTTON) Mouse.release(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.release(2,x,y);
       if(Mouse.get_dragging(&ax,&ay) == 1) // selection!
        { Fen.select_sub1(x,y);
          Mouse.vider();
        }
     }
    if(Mouse.get_dragging(&ax,&ay) == 3) // selection!
        { nop();
        }
}
static void
souris_button3(int button, int state, int x, int y)
{
    int ax,ay;
    Mouse.move(x,y);

    if(state==GLUT_DOWN)
     { if(button==GLUT_LEFT_BUTTON) Mouse.push(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.push(2,x,y);
     }
    if(state==GLUT_UP)
     { if(button==GLUT_LEFT_BUTTON) Mouse.release(1,x,y);
       if(button==GLUT_RIGHT_BUTTON) Mouse.release(2,x,y);
       if(Mouse.get_dragging(&ax,&ay) == 1) // selection!
        { Fen.select_sub2(x,y);
          Mouse.vider();
        }
     }
}
static void
souris_position1(int x,int y)
{
    int ax,ay;

    Mouse.move(x,y);
    //action
    if(Mouse.get_dragging(&ax,&ay) == 2) // dragging gauche
     { angle[0] -= ((float)ay)/50;
       angle[1] -= ((float)ax)/50;
     }
    if(Mouse.get_dragging(&ax,&ay) == 4) // dragging droit
     { angle[2] += ((float)ax)/50;
       profondeur[2] += ((float)ay)/50;
     }
}
static void
souris_position2(int x,int y)
{
    int ax,ay;

    Mouse.move(x,y);
}
static void
souris_position3(int x,int y)
{
    int ax,ay;

    Mouse.move(x,y);
    //action
    if(Mouse.get_dragging(&ax,&ay) == 2) // dragging gauche
     { angle2[0] -= ((float)ay)/50;
       angle2[1] -= ((float)ax)/50;
     }
    if(Mouse.get_dragging(&ax,&ay) == 4) // dragging droit
     { angle2[2] += ((float)ax)/50;
       profondeur2[2] += ((float)ay)/50;
     }
}

int init()
{
    Sphere.creer(-2,-1,-10,1);
    Sphere.fixer_text(0,"tfs1");
    Sphere.fixer_text(1,"tfs2");

    return 1;
}

int initialisation()
{
    int i,num_prob;

    sprintf(Messager,"***\0");
    profondeur[2] = -40.0;
    profondeur2[2] = -30.0;
    Helios = 1;
    Ed_texte.vider();

    Puce.initier();

    //Dregul.initier();
    //Dregul.ouvrir_fichier();
    i = Rezo.preparer();
    if(i) sprintf(Messager,"reseau charge : %d noeuds",i);

    Rezo.deselect_all();
    num_prob = Rezo.lier_puce(&Puce);
    //num_prob = Rezo.selection_probeset("proteasom");



    if(!Univers.initer2(&Rezo)) return 0;
    sprintf(Messager,"Univers initie (%d) (%d)",i,Rezo.n_lien);
    Univers.activer_lien(0);
    Univers.fixe_aff_pos(-1,0,0,0);

    Fen1.initier_dep();
    Rezo2.initier();
    Univ2.initier(&Rezo2);
    Univers.activer_lien(0);
    Univers.fixe_aff_pos(-1,0,0,0);
    return 1;
}


int initialisation_var(int a,char *b[])
{
    if(a != 4) exit(0);
    sprintf(Rezoin,"%s.txt",b[1]);
    sprintf(Rezoout,"%s_sav.txt",b[1]);
    sprintf(Rezoout_pos,"%s_pos.txt",b[1]);
    sprintf(Rezoannot,"%s.csv",b[2]);
    sprintf(Rezoprobindex,"%s.txt",b[3]);

    return 1;
}




int
main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    initialisation_var(argc,argv);
    initialisation();

    Fen.create_princeps();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(souris_button1);
    glutMotionFunc(souris_position1);


    Fen.aj_win();
    glutReshapeFunc(resize_fix);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(souris_button2);
    glutMotionFunc(souris_position2);

    Fen.aj_win();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(souris_button3);
    glutMotionFunc(souris_position3);
    glutMainLoop();

    return EXIT_SUCCESS;

}

