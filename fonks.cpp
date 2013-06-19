/*
Some usefull functions
*/

#include <string.h>
#include <stdio.h>


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
int chercher_string(char *s, char *phrase)  // search in a string
{
    char *ps,*ph;
    int LS,ls,lh;

    ps = s;
    LS = long_string(ps);
    ph = phrase;
    lh = long_string(ph);

    if(LS < 1) return 0;
    if(LS > lh) return 0;

    ls = LS;
    while(*ph)
    { while((*ph)&&( *ph != *ps)) ph++;
      while((*ph)&&(*ph == *ps)&&(--ls))
       { ph++;
         ps++;
       }
      if(!ls) return 1; // MATCH !
      ps = s;
      ls = LS; // if not, do another loop
    }

    return 0;
}

int aj_string(char *s1,char *s2) // concatenate s1 with s2
{
    char *ps;

    ps = s2;
    while(*ps) ps++;
    copie_string(s1,ps);
    return 1;
}

