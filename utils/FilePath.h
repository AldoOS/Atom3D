

#ifndef __OpenglESLibrary__FilePath__
#define __OpenglESLibrary__FilePath__

#include <iostream>
#include <string>

using namespace std;


/* 8 bit unsigned variable.*/
typedef unsigned char		uchar8;

/* 8 bit signed variable.*/
typedef signed char		schar8;

/** 8 bit character variable. */
typedef char			char8;

/** 16 bit unsigned variable. */
typedef unsigned short		ushort16;

/** 16 bit signed variable.*/
typedef signed short		sshort16;

/** 32 bit unsigned variable.*/
typedef unsigned int		uint32;

/** 32 bit signed variable.*/
typedef signed int		sint32;

/** 32 bit signed variable.*/
typedef  int		int32;

/** 32 bit floating point variable.*/
typedef float				float32;

/** 64 bit floating point variable.*/
typedef double				float64;

/** Get the full path of a file in the filesystem.
 * @param filename the name of the file
 * @return the path to the given file
 */
const char *getPath(const char8 *filename);
string getPath(const string& filename);

/** Reads every byte from the file specified by a given path.
 * @param filepath the path obtained from getPath, check getPath
 * @return the files content, u should delete the path from outside
 */
string getContentFromPath(const char8 *filepath);
string getContentFromPath(const string& filepath);

string getContentFromFile(const char *filename);


/**
 * load a png/jpg image
 * @param filename the image file name, this function calls getPath implicitly ..
 * @param widht/height the image width and height
 * @return the data pointer to the image. dont forget to free memory by calling free.
 */
char8* LoadImage(const char8* filename, int32 *width, int32 *height);
char8* LoadImage(const string& filename, int32 *width, int32 *height);


#endif /* defined(__OpenglESLibrary__FilePath__) */
