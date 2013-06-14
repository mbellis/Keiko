/*
Les boutons 3D et texte


*/

class CADSphere{
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
    /**********************************/
    int afficher();
    };
    
int CADSphere::creer(float ax,float ay,float az,float zz)
{
    x = ax;
    y = ay;
    z = az;
    zoom = zz;
    nzoom = 2*zz;
    return 1;
}
int CADSphere::fixer_col(int a,float a1,float a2,float a3)
{
    col[a][0] = a1;
    col[a][1] = a2;
    col[a][2] = a3;
    return a;
}
int CADSphere::fixer_text(int a,char *s)
{
    char sav;
    
    if(long_string(s) > 62)
     { sav = s[62];
       s[62] = 0;
       copie_string(s,text[a]);
       s[62] = sav;
     }
    else copie_string(s,text[a]);
    
    return 1;
}
int CADSphere::afficher()
{
    glTranslatef(x,y,z);
    
    glPushMatrix();
     glColor3f(0.8,0.8,0);
     glutSolidSphere(zoom,12,12);
    glPopMatrix();
    
    glColor3f(0.8,0.8,0);
    if(text[1]) glutEcrire(x+nzoom,y+nzoom,z+zoom,text[1]);
    
    return 1;
}       

