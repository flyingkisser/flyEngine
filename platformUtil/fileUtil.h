//
//  fileUtil.hpp
//  lines
//
//  Created by joe on 19/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef fileUtil_h
#define fileUtil_h



unsigned char* readFile(const char* szFileName);
int writeFile(const char*szFileName,unsigned char* buf,int size);

#endif /* fileUtil_h */
