/*
    Gestion de la fenetre opengl
    
    L'affichage!

*/
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

class WINDOWGL {
    int modeles_connus; // pointer vers le type d'affichage à realiser
    int modele_actuel;
    HDC hdc;
    int width,height;
    int n_obj,hits; // object activé
    int is_menu;
    float vx,vy,vz;
    char texte[256];
    public :
    int afficher();
    int fixer_model(int a);
    int demo1();
    int aff_menu();
    int aff_grafik();
    int tester();
    int initier(HDC h);
    int resize(int w,int h);
    int tester_mouse1();
    int tester_mouse2();
    int tester_mouse3();
    int tester_mouse4();
    int selection();
    };
/********************************************/
int WINDOWGL::initier(HDC h)
{
    hdc = h;
    modeles_connus = 4;
    n_obj = 0;
    return modeles_connus;
}
int WINDOWGL::fixer_model(int a)
{
    if(a > 0) 
     if(a<=modeles_connus) modele_actuel = a;
     
    return a;
}
int WINDOWGL::afficher()
{
    if(modele_actuel == 1) 
     { tester_mouse1();
       demo1();
     }
 
    if(modele_actuel == 2)
     { tester_mouse2();
       aff_menu();
     }
     
    if(modele_actuel == 3)
     { tester_mouse3();
       demo1();
     }
    if(modele_actuel == 4)
     { tester_mouse4();
       aff_grafik();
     }

     Sleep(20);
     return 1;
}
int WINDOWGL::resize(int w,int h)
{
    width = w;
    height = h;
    vx = 2 * 5.8 / (float) width;
    vy = 2 * 5.8 / (float) height;
    vx = vy = vz = 1; // pas encore utilisé
   
    return 1;
} 
/**********************/

int WINDOWGL::demo1()
{
      
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

/*	glTranslatef(0.f,0.0f,-1.0f);
	glColor3f(0.8,0.8,0.8);
	glRasterPos2f(0.0,0.0);
	glPrint("x= %d || y= %d|| b= %d >> %d",Mouse.act_x,Mouse.act_y,n_obj,Mouse.action[1]);	
	glTranslatef(0.f,0.0f,1.0f); */
	
	Menulosange.afficher();
	GroupClass.afficher();
 
    //glTranslatef(0.f,0.0f,-1.0f);		
    Cg.afficher();
    Editage.afficher();	

	
	//Sleep(8);
    return 1;
}
int WINDOWGL::aff_menu()
{
      
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	//Menulosange.afficher();
 	//Cadre[0].afficher();
	//CadreD.afficher();
	
	Menud.afficher();
	
	
	//Sleep(8);
    return 1;
}
int WINDOWGL::aff_grafik()
{
      
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix


	VisualD.afficher();
	

    return 1;
}
int WINDOWGL::tester()
{
      
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	if(modele_actuel == 1)	
	 { Menulosange.tester();
	   Cg.tester();
	   GroupClass.tester();
     }
    if(modele_actuel == 2)	// MENU_Deroulant
	 { Menud.tester();
     }   
    if(modele_actuel == 4)	// grafik
	 { nop();
     }   
    return 1;
}
/******************************************/
int WINDOWGL::tester_mouse1() // gestion du premier niveau graphique
{
    //Mouse.action[0] = 1;
    if(Mouse.action[0] == 1) // deplacement mouse : reperage de l'objet pointé
     { n_obj = selection();
       Menulosange.fixer_mouse(n_obj,Mouse.action[1]);
       //Cadre[0].fixer_mouse(n_obj,Mouse.action[1]);
       //CadreD.fixer_mouse(n_obj,Mouse.action[1]);
       Cg.fixer_mouse(n_obj,Mouse.action[1]);
       Mouse.archive(n_obj); 
     }
    if(Mouse.action[0] == 2) // dragging en cours
     { n_obj = Mouse.archive(-1);
      /* Menulosange.fixer_mouse(n_obj,Mouse.action[1]);
       Cadre[0].fixer_mouse(n_obj,Mouse.action[1]);
       CadreD.fixer_mouse(n_obj,Mouse.action[1]);*/
     }
     if(Mouse.action[0] == 3) // action enclenchée par click
     { n_obj = selection();
       is_menu = 0;
       is_menu |= Menulosange.fixer_mouse(n_obj,Mouse.action[1]);
       //is_menu |= Cadre[0].fixer_mouse(n_obj,Mouse.action[1]);
       //is_menu |= CadreD.fixer_mouse(n_obj,Mouse.action[1]);
       is_menu |= GroupClass.fixer_mouse(n_obj,Mouse.action[1]);
       is_menu |= Cg.fixer_mouse(n_obj,Mouse.action[1]);
       //if(Mouse.action[1] == 1) Cg.relire();
       if(Mouse.action[1] == 2) 
        if(is_menu) modele_actuel = 2;
       if(Mouse.action[1] == 1) 
        if(is_menu) modele_actuel = 3;/////////////////*** 3
       Mouse.vider();
     }
    return n_obj;
}
int WINDOWGL::tester_mouse2() // gestion du MENU Deroulant!
{
    int num;

    if(Mouse.action[0] == 1) // deplacement mouse : reperage de l'objet pointé
     { n_obj = selection();
       Menud.fixer_mouse(n_obj,0);
       //Mouse.archive(n_obj); 
     }
 
    if(Mouse.action[0] == 3) // action enclenchée par click
     { n_obj = selection();
       num = Menud.fixer_mouse(n_obj,Mouse.action[1]);
       if(num >= 0)  // vers l'action!!!!
        { n_obj = Mouse.archive(-1);// l'objet ayant appeler le menu!
          Menulosange.fixer_param(n_obj,num,"-"); // repercute l'information
        }
       modele_actuel = 1; // quitter le mode menu
       Cg.relire();
       Mouse.vider();
     }
    return n_obj;
}
int WINDOWGL::tester_mouse3() // gestion de l'editage!
{
    int num;
    char ta[256];

    if(Mouse.action[0] == 3) // action enclenchée par click
     { n_obj = Mouse.archive(-1);// l'objet ayant appeler le menu!
       num = Menud.fixer_mouse(n_obj,Mouse.action[1]);
       if(Mouse.action[1] == 1)
        { Editage.get_text(texte);
          Editage.vider();
          Menulosange.fixer_param(n_obj,-1,texte); // repercute l'information          
        }
       modele_actuel = 1; // quitter le mode menu
       Cg.relire();
       Mouse.vider();
     }
    return n_obj;
}
int WINDOWGL::tester_mouse4() // gestion du Mgrafik 3D
{
    int num;

    if(Mouse.action[0] == 2) // dragging
     { VisualD.fixer_mouse(Mouse.act_x - Mouse.prev_x,Mouse.act_y - Mouse.prev_y,2);   
     }
    if(Mouse.action[0] == 4) // dragging 2
     { VisualD.fixer_mouse(Mouse.act_x - Mouse.prev_x,Mouse.act_y - Mouse.prev_y,1);   
     }
    if(Mouse.action[0] == 3) // reset
     { if(Mouse.button[0] == 3) VisualD.fixer_mouse(0,0,3);   
     }
    return 1;
}
int WINDOWGL::selection()
{
    GLint	viewport[4]; 
    GLuint	buffer[512];
    int	choose,depth;

    // This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);								// Tell OpenGL To Use Our Array For Selection

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	(void) glRenderMode(GL_SELECT);

	glInitNames();												// Initializes The Name Stack
	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
	glPushMatrix();												// Push The Projection Matrix
	glLoadIdentity();											// Resets The Matrix

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix((GLdouble) Mouse.act_x, (GLdouble) (viewport[3]-Mouse.act_y), 5.0f, 5.0f, viewport);
	
	// Apply The Perspective Matrix
	gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	
    tester();         
                       												// Render The Targets To The Selection Buffer
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glPopMatrix();												// Pop The Projection Matrix
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	hits=glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
	choose = -1;														
	if (hits > 0)												// If There Were More Than 0 Hits
	{
		choose = buffer[3];									// Make Our Selection The First Object
		depth = buffer[1];									// Store How Far Away It Is 
	
		for (int loop = 1; loop < hits; loop++)					// Loop Through All The Detected Hits
		{
			// If This Object Is Closer To Us Than The One We Have Selected
			if (buffer[loop*4+1] < GLuint(depth))
			{
				choose = buffer[loop*4+3];						// Select The Closer Object
				depth = buffer[loop*4+1];						// Store How Far Away It Is
			}       
		}
    }
    return choose;
}

