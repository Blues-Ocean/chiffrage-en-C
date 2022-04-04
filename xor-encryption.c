#include <stddef.h> // pour size_t
#include <stdint.h> // pour uint64_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
const char *str = "Temporary string to be written to file!";
 
// chunks de 8
void xor_blocks( char block_1[],
                 char block_2[],
                 char result[] )
{
 // version naïve
  for (int i=0; i<8; i++)
   result[i]=block_1[i]^block_2[i];
 
  /*
  // version brutale
  *(uint64_t*)result=
  *(uint64_t*)block_1^
  *(uint64_t*)block_2;
  */
}
 
int main(int argc, char *argv[])
{
 // ici, zéro copies (et "safe" dans
 // la mesure où les buffers sont gérés
 // ailleurs, et nécessairement de bonne
 // dimension)
 //
  char * file1=argv[1];
  char * file2=argv[2];
  char * filename = "out.crypt";
 
  // argv[0]=nom du programme
  // argv[1]=fichier 1
  // argv[2]=fichier 2
  // argv[3]=nom (optionel) de sortie
  if (argc < 3)
   {
    // ici on trouve qu'on a pas assez de paramètres
    //
    fprintf(stderr,"Pas (assez) de nom(s) de fichers!\n");
    return EXIT_FAILURE; // code pour dire que ça a échoué
   }
 
  // s'il y a un 3è argument,
  // c'est le nom en sortie.
  //
  if (argc==4)
   filename = argv[3];
 
  printf("fichier 1=%s\n", file1);
  printf("fichier 2=%s\n", file2);
 
  // fopen retourne NULL si ça échoue
  //
  FILE * f1 = fopen(file1,"rb"); // read, binary
  FILE * f2 = fopen(file2,"rb"); // read, binary
  FILE * out = fopen(filename, "wb"); // write, binary
 
  if (f1 && f2 && out)
   {
    // ici f1, f2, et out sont valides
 
    while ( !feof(f1) && !feof(f2))
     {
      char bloc1[8]={0}; // automagiquement paddés avec zéro
      char bloc2[8]={0}; // (le padding n'est pas nécessaire)
      char bloc_resultat[8];
 
      // on sait pas combien il y en a
      // fread( dest, size_item, nb_items, f )
      size_t r1 = fread(bloc1,1,8,f1);
      size_t r2 = fread(bloc2,1,8,f2);
 
      // si le nombre de bytes lus du fichier à
      // chiffer est <= au nombre de bytes lu de
      // la clef, c'est qu'on a assez de bytes
      // de clef pour chiffrer
      if (r1<=r2) 
       {
        // méthode popoche à remplacer
        // dans le futur.
        //
        xor_blocks(bloc1,bloc2,bloc_resultat);
 
        // ici on écrit r1 bytes (si le bloc
        // est complet, c'est 8, si c'est moins,
        // possiblement zéro, c'est qu'on arrive
        // au bout du fichier f1, et qu'on a moins
        // de bytes à écrire)
        //
        fwrite(bloc_resultat,1,r1,out);
       }
     }
 
    // ici tout a fonctionné
    fclose(f1);
    fclose(f2);
    fclose(out);
    return EXIT_SUCCESS;
   }
  else
   {
    // f1 et/ou f2 et/ou out n'est pas bon
    if (f1) fclose(f1);
    if (f2) fclose(f2);
    if (out) fclose(out);
    fprintf(stderr,"oh noes! pas de la même longueur!\n");
    return EXIT_FAILURE;
   }
 
 
}
