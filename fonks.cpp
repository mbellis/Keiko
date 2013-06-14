/*
Some usefull functions
*/

#include <string.h>
#include <stdio.h>

int kezako_ascii(char c) //  ASCII
{
    if(c == ' ') return 5; // space
    if(c == '.') return 5;
    if(c == ',') return 5;
    if(c<0) return 4;
    if(c<32) return -1; // unprintable
    if(c<48) return 0; // !"'&#.....
    if(c<58) return 1; // numeral
    if(c<65) return 4;
    if(c<91) return 2;// capital letter
    if(c<97) return 4;
    if(c<123) return 3; // small letter
    return 4;
}
int mini_text(char *s) // capital letter to small letter
{
    char *pt;

    pt = s;
    while(*pt)
     { if(*pt > 64)
        if(*pt < 91) *pt += 32;
       pt++;
     }
    return 1;
}

void Ctexte (char *tamp)  // removed residual \n and put the \0 to the end of the string
{
        char *ptr;
        ptr=tamp;
        while( *ptr )
          { if ( *ptr =='\n') *ptr=0;
            ptr++;
          }
}
void Ctexte2(char *s) // same function for more complexe case. Removed unprintable letter.
{
    char *ps;
    int encore = 1;
    int a,b;

    ps = s;
    while(*ps)
     { a = kezako_ascii(*ps);
       if(a>0) ps++;
       else if(!a) *ps = '_';
       else
        { b = *ps;
          *ps = 0;
        }
     }
    *ps = 0;
}


int copie_string(char *source, char *dest)  // compare source and dest (if source is <= than dest)
{
        char *src,*dst;
        src=source;
        dst=dest;
        while (*src) *dst++ = *src++;
        *dst=0;
        return (dst - dest);
}
int long_string(char *ligne)
{
        char *ptr;
        ptr = ligne;
        while((*ptr)&&(*ptr!='\n')) ptr++;
        return ptr - ligne;
}

char* lire_seq(char *entree,char *sortie, char sep) // read a sequence without the separator
{
    char *pe,*ps;

    pe = entree;
    ps = sortie;

    while((*pe)&&(*pe != sep)) *ps++ = *pe++;
    *ps = 0;
    if(*pe) pe++;
    return pe;
}
int nop()
{ return 1;
}
int chercher_string(char *s, char *phrase)
{
    char *ps,*ph;
    int LS,ls,lh;

    ps = s;
    LS = long_string(ps);
    ph = phrase;
    lh = long_string(ph);
    //printf("%d>%s\n%d>%s\n",LS,ps,lh,ph);
    if(LS < 1) return 0; // Paglop si 0 ou negatif.
    if(LS > lh) return 0;

    //printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
    ls = LS;
    while(*ph)
    { while((*ph)&&( *ph != *ps)) ph++;
      //printf(">%s\n>%s\n",ps,ph);
      while((*ph)&&(*ph == *ps)&&(--ls))
       { ph++;
         ps++;
       }
      if(!ls) return 1; // MATCH !!!!!!!!!!!!!!!!!
      ps = s;
      ls = LS; // sinon encore un tour de plus...
    }

    return 0;
}
int get_email(char *s,char *em)
{
    char *ps,*pem;
    char sav;


    pem = s;
    while( (*pem)&&(*pem != '@') ) pem++;
    if(!*pem) return 0; // pas d'adresse Email.

    ps = pem;
    while( (ps>s)&&(*ps >32) ) ps--;
    if(ps>s) ps++;

    while(*pem>32) pem++;
    sav = *pem;
    *pem = 0;
    copie_string(ps,em);
    *pem = sav;


    return 1;
}
int aj_string(char *s1,char *s2)
{ // ajoute s1 � s2
    char *ps;

    ps = s2;
    while(*ps) ps++;
    copie_string(s1,ps);
    return 1;
}
int aj_string2(char *s1,char *s2)
{ // ajoute s1 � s2
    char *ps;

    printf(">%s\n%s\n",s1,s2);
    ps = s2;
    while(*ps)
     { ps++;
       //printf(">%s\t%d\n",ps,*ps);
       //system("PAUSE");
     }

    //copie_string(s1,ps);
    printf(">%s\n",s2);
       system("PAUSE");

    return 1;
}
int fixe_majuscule(char *s)
{
    char *p;

    p=s;
    while(*p)
     { if(kezako_ascii(*p) == 3) // minuscules
         *p -= 32; // passe en majuscules
       p++;
     }
     return 1;
}
int fec_fasta(FILE *pf,char *s,int a)
{
        char *pt;
        int j;

    pt = s;
        j=0;
        while(*pt)
         { fprintf(pf,"%c",*pt++);
           if(++j > a)
            { fprintf(pf,"\n");
              j=0;
            }
         }
        if(j) fprintf(pf,"\n");

        return 1;
}
