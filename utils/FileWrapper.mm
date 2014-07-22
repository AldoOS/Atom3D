//
//  FileWrapper.m
//  OpenglEngine
//
//  Created by harald bregu on 01/05/14.
//
//

#import "FileWrapper.h"
#import <Foundation/Foundation.h>


const char *GetPathFromBundle(const char *fileName)
{
    NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:fileName] ofType:nil];
    
    return [path UTF8String];
}

const char *GetBundlePathFromFileName (const char *fileName)
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    fileName = [path cStringUsingEncoding:1];
    
    return fileName;
}

const char *GetSourceFromPath (const char *filePath)
{
    NSString *file = [NSString stringWithUTF8String:filePath];
    NSError *error;
    NSString *source = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:&error];
    const char *sourceFile = [source UTF8String];
    return sourceFile;
}

const char *GetSourceFromFilename (const char *fileName)
{
    const char *path = GetBundlePathFromFileName(fileName);
    const char *source = GetSourceFromPath(path);
    
    return source;
}