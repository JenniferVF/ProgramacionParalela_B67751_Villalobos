#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include <map>
#include <stdio.h>

class FileReader{

    public:
    FileReader();
    FileReader( const char * fileName, int workers = 1, int strategy = 1 );
    ~FileReader();
    bool hasNext( int worker = 0 );
    int getNext( char ** line, int worker = 0 );

    void Read(int , char **);
    int countLines(FILE*, char*);
    std::map<std::string, int> processLine( const char * , std::map<std::string, int>);

    std::map<std::string, int> OnlyOne(FILE *, char *, std::map<std::string, int>);
    std::map<std::string, int> One(FILE *, char *, std::map<std::string, int>, int);
    std::map<std::string, int> Two(FILE *, char *, std::map<std::string, int>, int, int);
    std::map<std::string, int> Estrategias(FILE *, char *, std::map<std::string, int>, int, int);

    std::map<std::string, int> comparar(char * , std::map<std::string, int>);
    void imprimir(std::map<std::string, int>);
    std::map<std::string, int> inicializar(std::map<std::string, int>);

    private:
    int workers;			// Workers that read the file
    int strategy;			// Strategy to read the file, see cases above
    int totalLines;			// Total lines in file
    FILE * fileId;			// Identifier for opened file

    int range;
    int rangeMaster;
    std::map<std::string, int> etiquetas;
};

#endif // FILEREADER_H_INCLUDED
