#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

#define MAX_SIZE 200

int digitOrSpace (char c)
{
    if( c >= '0' && c <= '9' )
    {
        return 0;
    } else if (c == ' ')
    {
        return 1;
    } else
    {
        return -1;
    }
    return -1;
}

int compatibleType(int nb)
{
    if (nb >= 0 && nb <= 11)
        return 1;
    else
        return 0;
}

int * loadLevel (const char * filepath)
{
    FILE *f = NULL;
    char cwd[MAX_SIZE];
    char path[MAX_SIZE];
    char line1[MAX_SIZE] = "";
    char line2[MAX_SIZE] = "";
    int * lvl;
    int n = 0;
    int i = 0;
    int parity = 0;
    int nbr;

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // Get the program's path
        fprintf(stderr, "Le chemin est erroné.\n");
        return NULL;
    }

    strcpy(path, cwd);
    strcat(path, "/data/level/");
    strcat(path, filepath);

    lvl = malloc(sizeof(int) * MAX_SIZE);
    if (lvl == NULL)
    {
        fprintf(stderr, "Echec de l'allocation du tableau niveau\n");
        return NULL;
    }

    f = fopen(path, "r");
    if (f != NULL)
    {
        /* Dimensions */
        fgets(line1, MAX_SIZE, f); // get the first line with dimensions
        for(i = 1; i < 4; ++i)
        {
            if(i%2 == 1 && digitOrSpace(line1[i-1])%2 == 0) // odd : digit expected
            {
                lvl[n] = atoi(&line1[i-1]);
                ++n;
            } else if (i%2 == 0 && digitOrSpace(line1[i-1]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "Fichier niveau non conforme (dimensions)\n");
                return NULL;
            }
        }
        n = 0;

        /* Bricks' type */
        fgets(line2, MAX_SIZE, f); // get the second line with types
        for(i = 1; i <= ((lvl[0] * lvl[1])*2 - 1 + parity); ++i)
        {
            if(i%2 == (1 - parity%2) && digitOrSpace(line2[i-1]) == 0) // odd : digit expected
            {
                if (digitOrSpace(line2[i]) == 0) // If it's a 2 digits number
                {
                    strcat(&line2[i-1], &line2[i]); // concatenate the 2 digits in line2[i-1]
                    nbr = atoi(&line2[i-1]);
                    if (compatibleType(nbr) == 0)
                    {
                        lvl[3+n] = nbr; // put it into lvl array
                        ++i; // We skip a step
                        ++n;
                        ++parity; // We add a difference due to a 2 digits number - It is %2 so the result is either 0 or 1
                    } else {
                        fprintf(stderr, "[%d] Fichier niveau non conforme (type de brique)\n", i);
                        return NULL;
                    }             
                } else {
                    lvl[3+n] = atoi(&line2[i-1]);
                    ++n;
                }
            } else if (i%2 == parity%2 && digitOrSpace(line2[i-1]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "[%d] Fichier niveau non conforme (type de brique)\n", i);
                return NULL;
            }
        }
        fclose(f);
        return lvl;        
    }

    return NULL;
}

/* NumFiles is here to get the length of levelList*/
char ** levelList(int* numFiles)
{
    int i = 0;
    DIR *dir;
    struct dirent *ent;
    char path[MAX_SIZE];
    char cwd[MAX_SIZE];
    char ** list;

    list = malloc(10 * MAX_SIZE * sizeof(char));
    if (list == NULL)
    {
        fprintf(stderr, "Erreur de l'allocation.\n");
        return NULL;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // Get the program's path
        fprintf(stderr, "Le chemin est erroné.\n");
        return NULL;
    }

    strcpy(path, cwd);
    strcat(path, "/data/level/");

    if ((dir = opendir (path)) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
            list[i] = ent->d_name;
            if (list[i][0] != '.')
            {
                ++i;
            }
    }
        closedir (dir);
    } else {
        /* could not open directory */
        fprintf(stderr, "Ouverture du dossier impossible\n");
        return NULL;
    }

    *numFiles = i;
    return list;
}

