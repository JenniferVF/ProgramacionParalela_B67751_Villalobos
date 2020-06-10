#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include <map>
#include <string>
#include <stdio.h>

class FileReader{

    public:
    FileReader();
    FileReader( const char * fileName, int workers = 1, int strategy = 1 );
    ~FileReader();
    bool hasNext( int worker = 0 );
    int getNext( char ** line, int worker = 0 );

    void Read(int , char **);
    int countLines(FILE*);
    std::map<std::string, int> processLine( const char * , std::map<std::string, int>);

    std::map<std::string, int> Zero(FILE *, std::map<std::string, int>, std::string);
    std::map<std::string, int> One(FILE *, std::map<std::string, int>, int, fpos_t, std::string);
    std::map<std::string, int> Two(FILE *, std::map<std::string, int>, int, int);
    std::map<std::string, int> Three(FILE *, std::map<std::string, int>);
    std::map<std::string, int> Estrategias(FILE *, std::map<std::string, int>, int, int, int, std::string, fpos_t);

    std::map<std::string, int> comparar(char * , std::map<std::string, int>);
    void imprimir(std::map<std::string, int>);
    std::map<std::string, int> inicializar(std::map<std::string, int>);

    int getStrategy();
    int getRange();
    int getRangeMaster();
    int getWorkers();
    fpos_t getPos();
    void setPos(fpos_t);
    std::string getFilename();
    FILE* getFile();
    bool getFinish();
    void setFinish();
    std::map<std::string, int> getMap();
    void setMap(std::map<std::string, int>);

    private:
    int workers;			// Workers that read the file
    int strategy;			// Strategy to read the file, see cases above
    int totalLines;			// Total lines in file
    FILE * fileId;			// Identifier for opened file

    int range;
    int rangeMaster;
    bool finish;
    fpos_t position;  //posicion de inicio del archivo
    std::string filename;
    std::map<std::string, int> etiquetas;
};

#endif // FILEREADER_H_INCLUDED
