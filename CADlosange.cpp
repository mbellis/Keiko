/*
 Classes pour cadre de selection et edition


*/
class CADLosange{
    char text[2][64];
    float x,y,z;
    float box[4][3];
    float col[6][3];
    float zoom,nzoom;
    int n_col;
    int base;
    public:
    int creer(float ax,float ay,float az,float zz);
    int fixer_col(int a,float a1,float a2,float a3);
    int fixer_text(int a,char *s);
    int calc_box();
    /**********************************/
    int afficher();
    };
    
int CADLosange::creer(float ax,float ay,float az,float zz)
{
    x = ax;
    y = ay;
    z = az;
    zoom = zz;
    nzoom = -1*zz;
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
    
    sav = s[62];
    s[62] = 0;
    copie_string(s,text[a]);
    s[62] = sav;
    
    return 1;
}
int CADLosange::calc_box()
{
    box[0][0] = -3;
    box[0][1] = 3;
    box[0][2] = -0.5;
    box[1][0] = 20;
    box[1][1] = 3;
    box[1][2] = -0.5;
    box[2][0] = 20;
    box[2][1] = -6;
    box[2][2] = -0.5;
    box[3][0] = -3;
    box[3][1] = -6;
    box[3][2] = -0.5;
    return 1;
}
int CADLosange::afficher()
{
    glTranslatef(x,y,z);
    
    n_col = 2;
    glBegin(GL_QUADS);  
        glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);   
        n_col = 0;              
        glVertex3f(-2,2,-1);
        glVertex3f(-2,-4,-1);
        glVertex3f(20,-4,-1);
        glVertex3f(20,2,-1);
        
        
        
                glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;                  
                glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
                glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);n_col++;
                glVertex3f(box[n_col][0],box[n_col][1],box[n_col][2]);
        glEnd();
   
   
   
    n_col = 1;
    glBegin(GL_QUADS);  
        glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);                   
                glVertex3f(zoom,0,0);   
                n_col++;
                glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);                   
                glVertex3f(0,zoom,0);
                n_col++;
                glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);
                glVertex3f(nzoom,0,0);  
                n_col++;
                glColor3f(col[n_col][0],col[n_col][1],col[n_col][2]);
                glVertex3f(0,nzoom,0);  
        glEnd();
    
    return 1;
}       

