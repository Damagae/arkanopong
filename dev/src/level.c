#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

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

int * loadLevel (char *filepath)
{
    FILE *f = NULL;
    char line1[MAX_SIZE] = "";
    char line2[MAX_SIZE] = "";
    int * lvl;
    int n = 0;
    int i = 0;

    lvl = malloc(sizeof(int) * MAX_SIZE);
    if (lvl == NULL)
    {
        fprintf(stderr, "Echec de l'allocation du tableau niveau.\n");
        return NULL;
    }

    f = fopen(filepath, "r");
    if (f != NULL)
    {
        /* Dimensions */
        fgets(line1, MAX_SIZE, f); // get the first line with dimensions
        for(i = 0; i < 4; ++i)
        {
            if(i%2 == 1 && digitOrSpace(line1[i])) // odd : digit expected
            {
                lvl[n] = line2[i];
                ++n;
            } else if (i%2 == 0 && digitOrSpace(line1[i]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "Fichier niveau non conforme.\n");
                return NULL;
            }
        }
        i = 0;

        /* Bricks' type */
        fgets(line2, MAX_SIZE, f); // get the second line with types
        for(i = 0; i < lvl[0] * lvl[1]; ++i)
        {
            if(i%2 == 1 && digitOrSpace(line2[i])) // odd : digit expected
            {
                lvl[3+n] = line2[i];
                ++n;
            } else if (i%2 == 0 && digitOrSpace(line2[i]) == 1) // even : space expected
            {

            } else {
                fprintf(stderr, "Fichier niveau non conforme.\n");
                return NULL;
            }
        }

        return lvl;

        fclose(f);
    }

    return NULL;
}


