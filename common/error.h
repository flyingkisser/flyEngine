//
//  error.h
//  lines
//
//  Created by Joe on 2017/4/26.
//  Copyright © 2017年 joe. All rights reserved.
//

#ifndef error_h
#define error_h

#include <stdio.h>



int checkGLError (const char* str=NULL);
int checkGLErrorWithFileLine (const char* filename,int line);


#define checkGLErrorWithFileLines() checkGLErrorWithFileLine(__FILE__, __LINE__)


#endif /* error_h */

