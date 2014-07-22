
#include "tiny_obj_loader.h"
#include "Atom.h"


// Type File
typedef struct
{
    const char *name;
    std::vector<tinyobj::shape_t>shapes;
}File;

// Create a type vector of File types
typedef std::vector<File> Files;
typedef std::vector<const char *> File_Names;
Files files2;

// Load a single file
File loadFile(const char*filename)
{
    File file;
    
    file.name = filename;
    const char *fpath = GetBundlePathFromFileName(filename);
    tinyobj::LoadObj(file.shapes, fpath);
    
    return file;
}

// Load multiple files
Files loadFiles(File_Names filenames, int index)
{
    File file;
    file.name = filenames[index];
    const char *file_path = GetBundlePathFromFileName(filenames[index]);
    tinyobj::LoadObj(file.shapes, file_path);
    
    // Add to the main files array the file at index
    files2.push_back(file);
    
    return files2;
}
