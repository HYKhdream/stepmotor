/*
 * shell.h	(by xhl 2014/11/01)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_cfg.h"
#include "shell_def.h"


void shell_push_char(char ch);
extern void Shell_PrintChar(char ch);
int Shell_AddCMD(char *cmd, char *help, char (*func)(char **, int));

