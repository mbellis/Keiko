/*
    variante de fonks pour freeglut




*/

void
glutEcrire(float x,float y,float z,char *s)
{
    char *p;

    p = s;
    glRasterPos3f(x,y,z);
    while(*p) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*p++);
}

void
glutEcrireL(float x,float y,float z,char *s)
{
    char *p;

    p = s;
    glRasterPos3f(x,y,z);
    while(*p) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*p++);
}

void
glutEcrireH(float x,float y,float z,char *s)
{
    char *p;

    p = s;
    glRasterPos3f(x,y,z);
    while(*p) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p++);
}

void
glutEcrireS(float x,float y,float z,char *s)
{
    char *p;

    p = s;
    glRasterPos3f(x,y,z);
    while(*p) glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*p++);
}


