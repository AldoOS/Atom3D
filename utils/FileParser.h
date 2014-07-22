//
//  File.h
//  OpenglEngine
//
//  Created by harald bregu on 19/04/14.
//
//

#ifndef __OpenglEngine__File__
#define __OpenglEngine__File__

#ifdef __APPLE__

#endif

#include <CoreFoundation/CoreFoundation.h>

#include <iostream>



namespace FileParser {
    
    
    const char *filePath(const char *filename);
    
    const char *fileGetPath(const char *fileName);

    //const char *fileGetSourceFromPath(const char *filePath);
    
    std::string fileGetSourceFromPath(const char *filePath);

}

#endif 