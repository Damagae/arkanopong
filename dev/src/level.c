#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#define MAX_SIZE 1024

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

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Le chemin spécifié est erroné.\n");
        return NULL;
    }

    strcpy(path, cwd);
    strcat(path, "/data/level.txt");

    lvl = malloc(sizeof(int) * MAX_SIZE);
    if (lvl == NULL)
    {
        fprintf(stderr, "Echec de l'allocation du tableau niveau.\n");
        return NULL;
    }

    f = fopen(path, "r");
    if (f != NULL)
    {
        /* Dimensions */
        fgets(line1, MAX_SIZE, f); // get the first line with dimensions
        for(i = 1; i < 4; ++i)
        {
            if(i%2 == 1 && digitOrSpace(line1[i-1]) == 0) // odd : digit expected
            {
                lvl[n] = atoi(&line1[i-1]);
                ++n;
            } else if (i%2 == 0 && digitOrSpace(line1[i-1]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "Fichier niveau non conforme (dimensions).\n");
                return NULL;
            }
        }
        n = 0;

        /* Bricks' type */
        fgets(line2, MAX_SIZE, f); // get the second line with types
        printf("%s\n", line2);
        printf("%d\n", lvl[0] * lvl[1]);
        for(i = 1; i <= lvl[0] * lvl[1]; ++i)
        {
            if(i%2 == 1 && digitOrSpace(line2[i-1]) == 0) // odd : digit expected
            {
                lvl[3+n] = atoi(&line2[i-1]);
                ++n;
            } else if (i%2 == 0 && digitOrSpace(line2[i-1]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "[%d] Fichier niveau non conforme (type de brique).\n", i);
                return NULL;
            }
        }
        fclose(f);
        return lvl;        
    }

    return NULL;
}


