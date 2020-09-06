#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

#ifndef USER_TABLE
#define USER_TABLE
int Match(char *username, char *password);
int Register(char *username, char *password);
int Display();
int deleteMd5Info(char* filename);
int isIntact(char* filename);
int setIntact(char* filename, char* intact);
int insertMd5Info(char* filename, char* md5value, char* username);
int isVaildUser(char* filename, char* username);
int Match(char* username, char* password);
int matchMd5(char* filename, char* md5value);
int initStart();
#endif
