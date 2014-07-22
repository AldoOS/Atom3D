//
//  File.cpp
//  OpenglEngine
//
//  Created by harald bregu on 19/04/14.
//
//

#include "FileParser.h"
#include <fstream>

namespace FileParser {
    

    const char *filePath(const char *filename)
    {
        // Get a reference to the main bundle
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        // Get a reference to the file's URL
        CFURLRef imageURL = CFBundleCopyResourceURL(mainBundle, CFSTR("dice_texture"), CFSTR("jpg"), NULL);
        // Convert the URL reference into a string reference
        CFStringRef imagePath = CFURLCopyFileSystemPath(imageURL, kCFURLPOSIXPathStyle);
        // Get the system encoding method
        CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
        // Convert the string reference into a C string
        const char *path = CFStringGetCStringPtr(imagePath, encodingMethod);
        //fprintf(stderr, "File is located at %s\n", path);
        
        return path;
    }
    
    const char *fileGetPath(const char *fileName)
    {
        char *ptr;
        std::string fnm(fileName);
        if(fnm.find("/") == fnm.npos)
        {
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
            CFStringRef str = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
            CFRelease(resourcesURL);
            ptr = new char[CFStringGetLength(str)+1];
            CFStringGetCString(str, ptr, FILENAME_MAX, kCFStringEncodingASCII);
            CFRelease(str);
            
            std::string res(ptr);
            res += std::string("/");
            res += std::string(fileName);
            
            delete[] ptr;
            ptr = new char[res.length()+1];
            strcpy(ptr, res.c_str());
        }
        
        else
        {
            ptr = new char[fnm.length()+1];
            strcpy(ptr, fnm.c_str());
        }
        
        std::string s(ptr);
        delete[] ptr;
        return s.c_str();
    }
    
    std::string fileGetSourceFromPath(const char *filePath)
    {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);
        
        if(!fileStream.is_open()) {
            std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
            return "";
        }
        
        std::string line = "";
        while(!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }
        
        fileStream.close();
        return content;
    }  
}