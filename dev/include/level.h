#ifndef __LEVEL__H
#define __LEVEL__H

/* FUNCTIONS */

int * loadLevel (char *filepath);
int digitOrSpace(char c);
int compatibleType(int nb);
int compatibleTypeColor(int nb);
int randomColor();
char ** levelList(int* numFiles);
void createLevel(int* level, int* color);

#endif