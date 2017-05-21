#ifndef __LEVEL__H
#define __LEVEL__H

int * loadLevel (char *filepath);
int digitOrSpace(char c);
int compatibleType(int nb);
char ** levelList(int* numFiles);
void createLevel(int* level);

#endif