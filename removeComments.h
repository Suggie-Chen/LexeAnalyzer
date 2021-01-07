#pragma once
#include <iostream>
#include <fstream>
using namespace std;
void in_comment(FILE* fpin, FILE* fpout);
void deleteTail(FILE* fpin, FILE* fpout);
void rcomment(char  c, FILE* fpin, FILE* fpout);
int removeComments(FILE* fpin, FILE* fpout);
