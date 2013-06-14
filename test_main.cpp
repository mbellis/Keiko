#include <iostream>
#include <stdlib.h>

# include "fonks.cpp"

# include "dico_regul.cpp"

using namespace std;
 
DICO_REGUL Dregul;  
/************************************************************/
int do_it()
{ 
    int i,j;  
       
    Dregul.initier();
    Dregul.ouvrir_fichier();
    Dregul.preparer_reg(10);
    
    j = 1;
    j |= 2;
    printf("%d\n",j);

} 

  




/***********************************************************/
int main(int argc, char *argv[])
{
  do_it();
  system("PAUSE");	
  return 0;
}
