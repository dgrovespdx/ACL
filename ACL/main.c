
//=== ACLmain.c 2022/05/13 =================================================
//
//  ACL main program
//
//  Usage: main()           - Interactive mode.
//         main(filename)   - Compile and execute file.
//                            (not yet implemented)
/*
 * Copyright (c) 2022 Don Groves
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include "ACL.h"
#include "ACLvector.h"
#include "ACLterm.h"
#include "ACLstack.h"
#include "ACLsymbols.h"
#include "ACLcompile.h"
#include "ACLprim.h"
#include "ACLexecute.h"

int StrCat(char* str,char* str1,char* str2) {   // cat strings properly!
    unsigned short len0 = strlen(str);
    unsigned short len1 = strlen(str1);
    unsigned short len2 = strlen(str2);

    if (len0 < (len1 + len2)) {
        error("StrCat","dest string too short","");
        return FAIL;
    }
    int i = 0; int j = 0;
    while (str1[i] != '\0') {                   // insert str1
        str[j] = str1[i];
        i++;
        j++;
    }
    i = 0;
    while (str2[i] != '\0') {                   // insert str2
        str[j] = str2[i];
        i++;
        j++;
       }
    str[j] = '\0';
    return OK;
}

int GetLibrary() {
	FILE *fp;
    char str[120];
    //char* path;
    char path[120] = "                                                ";

    StrCat(path,ACLpath,"ACLlib.txt");
    fp = fopen(path,"r");	            // open library file
    if (fp == NULL) {
        error("GetLibrary","ACLlib.txt",strerror(errno));
        return FAIL;
    }
    printf("\n");
	while (1) {			                // compile lines until EOF
    	if (fgets(str,LINESIZE,fp) != NULL)
            compile(str);
        else break;
    }
    fclose(fp);
    return OK;
}

int main(int argc, const char * argv[]) { // ACL main program
    char *line = NULL;
    size_t len = 0;
    size_t size;
    int ret;
    
    if (argc==1 || argc==2) {
        InitSymbols();
        InitStack();
        ACLstate = COMPILING;
        GetLibrary();
    }
    else {
        error("main","invalid command line args","");
        return FAIL;
    }
    if (argc == 1) {
        printf("\n\nACL (v %.2f) \n============\n",VERSION);
        ACLstate = INTERACTING;
        while (1) {                           // loop until quit
            PrintStack();
            size = getline(&line, &len, stdin);
            ret = compile(line);
            if (ret == QUIT) {
                printf("\nACL terminated\n");
                return OK;
            }
        }
    }
    else {
        FILE *fp;
        char str[120];
        char path[120] = "                                                ";
        
        ACLstate = RUNNING;
        StrCat(path,ACLprog,argv[1]);
        fp = fopen(path,"r");                   // open program file
        if (fp == NULL) {
            error("main","program file",strerror(errno));
            return FAIL;
        }
        printf("\nexecuting %s\n",argv[1]);
        while (1) {                             // compile until EOF
            if (fgets(str,LINESIZE,fp) != NULL) {              
            	compile(str);
            }
            else break;
        }
        fclose(fp);
        return OK;
    }
}

// === end ACLmain.c ===============================================

