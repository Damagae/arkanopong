#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include "bool.h"

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
    if (nb >= 0 && nb <= 9)
        return 1;
    else
        return 0;
}

int compatibleTypeColor(int nb)
{
    if (nb >= 0 && nb <= 5)
        return 1;
    else
        return 0;
}

int randomColor()
{
    return rand() % 5 + 1;
}

void sort(char ** tab, int * numFiles)
{
    int i;
    char * temp[MAX_SIZE];
    bool order = false;
    int length = *numFiles;
    while(!order)
    {
        order = true;

        for(i = 0 ; i < length - 1 ; i++)
        {

            if(tab[i][5] > tab[i+1][5])
            {
                strcpy(temp, tab[i]);
                strcpy(tab[i], tab[i+1]);
                strcpy(tab[i+1], temp);
                order = false;
            }
        }
    }
}

int * loadLevel (const char * filepath)
{
    FILE *f = NULL;
    char cwd[MAX_SIZE];
    char path[MAX_SIZE];
    char line1[MAX_SIZE] = "";
    char line2[3*MAX_SIZE] = "";
    char line3[3*MAX_SIZE] = "";
    int * lvl;
    int n = 0;
    int i = 0;
    int parity = 0;
    int nbr;
    int fline = 0;

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // Get the program's path
        fprintf(stderr, "Le chemin est erroné.\n");
        return NULL;
    }

    strcpy(path, cwd);
    strcat(path, "/data/level/");
    strcat(path, filepath);

    lvl = malloc(sizeof(int) * 6 * MAX_SIZE);
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
        for(i = 1; i < (4 + parity); ++i)
        {
           if(i%2 == (1 - parity%2) && digitOrSpace(line1[i-1]) == 0) // odd : digit expected
            {
                if (digitOrSpace(line1[i]) == 0) // If it's a 2 digits number
                {
                    strcat(&line1[i-1], &line1[i]); // concatenate the 2 digits in line2[i-1]
                    nbr = atoi(&line1[i-1]);
                    lvl[n] = nbr; // put it into lvl array
                    ++i; // We skip a step
                    ++n;
                    ++parity; // We add a difference due to a 2 digits number - It is %2 so the result is either 0 or 1          
                } else {
                    lvl[n] = atoi(&line1[i-1]);
                    ++n;
                }
            } else if (i%2 == parity%2 && digitOrSpace(line1[i-1]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "[%d] Fichier niveau non conforme (dimension)\n", i);
                return NULL;
            }
        }
        n = 0;
        i = 0;
        parity = 0;

        /* Bricks' type */
        fgets(line2, 3 * MAX_SIZE, f); // get the second line with types
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

        fline = 3 + n;
        n = 0;
        i = 0;
        parity = 0;

        /* Bricks' color - if given */
        if (fgets(line3, 3 * MAX_SIZE, f) != NULL) // get the third line with colors if exits
        {
            for(i = 1; i <= ((lvl[0] * lvl[1])*2 - 1); ++i)
            {
                if(i%2 == 1 && digitOrSpace(line3[i-1]) == 0) // odd : digit expected
                {
                    if (compatibleTypeColor(line3[i-1]) == 0)
                    {
                        lvl[fline+n] = atoi(&line3[i-1]); // put it into lvl array
                        ++n;
                    } else {
                        fprintf(stderr, "[%d] Fichier niveau non conforme (couleur de brique)\n", i);
                        return NULL;
                    }
                } else if (i%2 == 0 && digitOrSpace(line3[i-1]) == 1) // even : space expected
                {

                } else {
                    fprintf(stderr, "[%d] Fichier niveau non conforme (couleur de brique)\n", i);
                    return NULL;
                }
            }
        } else { // random colors
            for(i = 1; i <= ((lvl[0] * lvl[1])*2 - 1); ++i)
            {
                lvl[fline+n] = randomColor(); // put it into lvl array
                ++n;
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

    /* sort the list */
    sort(list, numFiles);

    return list;
}

void createLevel(int* level, int* color)
{
    int numFiles;
    char nbr[MAX_SIZE];
    FILE* f;
    char filepath[MAX_SIZE];
    char cwd[MAX_SIZE];
    char bt[MAX_SIZE];
    int i;

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // Get the program's path
        fprintf(stderr, "Le chemin est erroné.\n");
    }

    strcpy(filepath, cwd);
    strcat(filepath, "/data/level/level");

    /* Get to know the number of existing levels and create a correct filename */
    free(levelList(&numFiles));

    numFiles += 1;
    sprintf(nbr, "%d", numFiles);
    strcat(filepath, nbr);
    strcat(filepath, ".txt");

    /* Create a file */
    f = fopen(filepath ,"a");

    /* Dimensions */
    fputs("12 10\n", f);

    /* Bricks */
    for(i = 0; i < (12 * 10) - 1; ++i)
    {
        sprintf(bt, "%d ", level[i]);
        fputs(bt, f);
    }
    sprintf(bt, "%d\n", level[12 * 10 - 1]);
    fputs(bt, f);

    /* Bricks colors */
    for(i = 0; i < (12 * 10) - 1; ++i)
    {
        sprintf(bt, "%d ", color[i]);
        fputs(bt, f);
    }
    sprintf(bt, "%d\n", color[12 * 10 - 1]);    
    fputs(bt, f);
    
    fclose(f);
}

// Parameter : the number of the level
void deleteLevelFile(int level)
{
    int numFiles;
    char oldname[MAX_SIZE];
    char newname[MAX_SIZE];
    char nbr[MAX_SIZE];
    char filepath[MAX_SIZE];
    char path[MAX_SIZE];
    char cwd[MAX_SIZE];
    int i;

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // Get the program's path
        fprintf(stderr, "Le chemin est erroné.\n");
    }

    ++level;
    strcpy(filepath, cwd);
    strcat(filepath, "/data/level/level");
    strcpy(path, filepath);
    sprintf(nbr, "%d", level);
    strcat(filepath, nbr);
    strcat(filepath, ".txt");

    /* Delete file */
    remove(filepath);

    /* Rename files */

    free(levelList(&numFiles));

        for (i = level ; i <= numFiles; ++i)
        {
            strcpy(oldname, path);
            sprintf(nbr, "%d", i+1);
            strcat(oldname, nbr);
            strcat(oldname, ".txt");

            strcpy(newname, path);
            sprintf(nbr, "%d", i);
            strcat(newname, nbr);
            strcat(newname, ".txt");

            rename(oldname, newname);
        }
    


}