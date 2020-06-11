#include "FileReader.h"
#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>

FileReader::FileReader()
{

}

FileReader::FileReader(const char * archivo, int trabajadores, int estrategia)
{
//Averiguar total de lineas de archivo (L)
//Crear los trabajadores
//posicion[trabajadores]; //La posicion de lectura de cada trabajador
//buffer [trabajadores]; //con 1024 bytes de cada trabajador
}

FileReader::~FileReader()
{
    fclose( this->fileId );
}

//bool FileReader::hasNext(int trabajador)
//{
//
//}
//
//int FileReader::getNext(char ** line, int trabajador)
//{
//
//}


/*
*Metodo encargado de inicializar los datos necesitados por el Lector.
*/
void FileReader::Read(int trabajadores, int estrategia, std::string archivo)
{
    fpos_t inicio;
    this->etiquetas = inicializar(this->etiquetas);


//    if ( argc < 4 )
//    {
//        printf( "Por favor ingrese: <el archivo a utilizar>, <cantidad de trabajadores>, <estrategia a utilizar>.\n Estrategias: \n 0 - Un solo trabajador realiza la tarea.\n 1 - Total de lineas/cantidad de trabajadores. \n 2 - Grupos no contiguos de lineas. \n 3 - Entregar las lineas por demanda. \n" );
//        exit( 1 );
//    }


    this->filename = archivo;
    const char * name = filename.c_str();
    this->fileId = fopen( name, "r" );

    if ( NULL == this->fileId )
    {
        perror( "El archivo no se pudo abrir.\n");
        exit( 2 );
    }
    //Se toma la posicion inicial del archivo.
    fgetpos(this->fileId, &inicio);


    //Actualizacion de las variables privadas.
    this->workers = trabajadores;
    this->strategy = estrategia;

    if(this->strategy == 0)
    {
        this->workers = 1;
    }
    else if(this->strategy > 4)
    {
        printf("No existe una estrategia con ese numero. Se utiliza la estrategia por default: 0.\n");
        this->strategy = 0;
        this->workers = 1;
    }

    this->totalLines = countLines(this->fileId);
    printf("Total lines: %d\n", this->totalLines);


    //Si el total de lineas es divisible entre la cantidad de trabajadores.
    if(this->totalLines%this->workers == 0)
    {
        this->range = this->totalLines/this->workers;
        this->rangeMaster = this->totalLines/this->workers;
    }
    //En caso contrario, se le asigna a algun hilo el rangeMaster, que consiste
    //en el rango mas el "sobro" de la division.
    else
    {
        this->range = this->totalLines/this->workers;
        this->rangeMaster = (this->totalLines-(range*(this->workers-1)));
    }


    //Se posiciona al inicio del archivo.
    //fsetpos(this->fileId, &this->position);
    setPos(inicio);


    fclose( this->fileId );

}


/*
*Metodo encargado de contar las lineas del archivo entrante
*/
int FileReader::countLines(FILE * archivo)
{
    int cant_lineas = 0;
    int chars;
    size_t size;
    char * line;
    size = 512;

    line = (char *) calloc( 512, 1 );

    do
    {
        chars = getline( & line, & size, archivo );
        if( chars > 0 )
        {
            cant_lineas++;
        }
    }
    while ( chars > 0 );

    free(line);
    return cant_lineas;
}

/*
*Metodo encargado de procesar una linea del archivo.
*/
std::map<std::string, int> FileReader::processLine( const char * line, std::map<std::string, int> etiquetas)
{
    char * token;
    std::regex texto( ">[^<]*<" );
    std::string ecci;

    ecci = regex_replace( line, texto, "> <" );
//   std::cout << "ecci = " << ecci << std::endl;

    token = strtok( (char * ) line, "< >\t\n" );

    while ( NULL != token )
    {
        etiquetas = comparar(token, etiquetas); //Se compara la palabra obtenida con las etiquetas guardadas
        token = strtok( NULL, "< >\t\n");
    }

    return etiquetas;
}


/*
*Metodo encargado de comparar la palabra entrante con cada una de las
*etiquetas guardadas en el mapa.
*/
std::map<std::string, int> FileReader::comparar(char * token, std::map<std::string, int> etiquetas)
{

    std::map<std::string, int>:: iterator it = etiquetas.begin();
    std::string key;

    while(it != etiquetas.end())
    {
        key = it->first;

        //En caso de que la palabra sea una etiqueta, se aumenta el contador de esta
        if((key.compare(token)) == 0)
        {
            etiquetas[key]++;
        }

        it++;
    }
    return etiquetas;
}


/*
*Metodo que almacena todas las estrategias de lectura.
*/
std::map<std::string, int> FileReader::Estrategias(FILE * archivo, std::map<std::string, int> etiquetas, int rango, int id, int estrategia, std::string filename, fpos_t posicion, int contador)
{
    switch(estrategia)
    {
    case 0:
        etiquetas = Zero(archivo, etiquetas, filename);
        break;
    case 1:
        etiquetas = One(archivo, etiquetas, rango, posicion, filename, contador);
        break;
    case 2:
        etiquetas = Two(archivo, etiquetas, rango, id, posicion, filename);
        break;
    case 3:
        etiquetas = Three(archivo, etiquetas, posicion, filename);
        break;
    case 4:
        etiquetas = Four(archivo, etiquetas, rango, posicion, filename);
    }

    return etiquetas;
}


/*
*Estrategia por defecto: todo el trabajo lo
*realiza un solo trabajador.
*/
std::map<std::string, int> FileReader::Zero(FILE * archivo, std::map<std::string, int> etiquetas, std::string filename)
{
    int chars;
    size_t size;
    char * line = NULL;
    size = 512;
    line = (char *) calloc( 512, 1 );

    const char * name = filename.c_str();
    FILE * fichero = fopen(name, "r");

    do
    {
        chars = getline( & line, & size, fichero );

        if( chars > 0 )
        {
            etiquetas = processLine( line, etiquetas);  //Se procesa cada linea del archivo, los contadores del mapa se actualizan
        }
    }
    while ( chars > 0 );

    setFinish();
    free(line);
    fclose(fichero);

    return etiquetas;
}


/*
*Estrategia #1: dividir el total de lineas del archivo
*entre la cantidad de trabajadores.
*/
std::map<std::string, int> FileReader::One(FILE * archivo, std::map<std::string, int> etiquetas, int rango, fpos_t posicion, std::string filename, int contador)
{
    int chars;
    size_t size;
    char * line = NULL;
    fpos_t nueva;

    size = 512;
    line = (char *) calloc( 512, 1 );

    const char * name = filename.c_str();
    FILE * fichero = fopen(name, "r");

    fsetpos(fichero, &posicion);

    if(contador == rango) //Si ya hizo su bloque correspondiente
    {
        setFinish();
    }
    else
    {
        chars = getline( & line, & size, fichero );

        if( chars > 0 )
        {
            etiquetas = processLine( line, etiquetas);  //Se procesa cada linea del archivo, los contadores del mapa se actualizan
            fgetpos(fichero, &nueva);
            setPos(nueva);
        }
        else
        {
            setFinish();
        }
    }


    free(line);
    fclose(fichero);

    return etiquetas;
}


/*
*Estrategia #2: dividir en grupos no contiguos de lineas
*linea%trabajadores == 0.
*/
std::map<std::string, int> FileReader::Two(FILE * archivo, std::map<std::string, int> etiquetas, int rango, int id, fpos_t posicion, std::string filename)
{
    int chars;
    size_t size;
    int num_linea = 0;
    int resultado;
    char * line;
    fpos_t nueva;

    //Para evitar errores al dividir por cero (id del primer trabajador)
    int identificacion = id+1;

    size = 512;
    line = (char *) calloc( 512, 1 );

    const char * name = filename.c_str();
    FILE * fichero = fopen(name, "r");

    fsetpos(fichero, &posicion);


    resultado = num_linea % identificacion;

    do
    {
        chars = getline( & line, & size, fichero );

        if( (chars > 0) && (resultado == 0) )  //Si el residuo da 0, esa linea le pertenece.
        {
            etiquetas = processLine( line, etiquetas);  //Se procesa cada linea del archivo, los contadores del mapa se actualizan
        }
        num_linea++;
        resultado = num_linea % identificacion;
    }
    while ( (chars > 0) );


    fgetpos(fichero, &nueva);
    setPos(nueva);
    setFinish();

    fclose(fichero);
    free(line);
    return etiquetas;
}


/*
*Estrategia #3: entregar las lineas por demanda.
*/
std::map<std::string, int> FileReader::Three(FILE * archivo, std::map<std::string, int> etiquetas, fpos_t posicion, std::string filename)
{
    int chars;
    size_t size;
    char * line;
    fpos_t nueva;

    size = 512;
    line = (char *) calloc( 512, 1 );

    const char * name = filename.c_str();
    FILE * fichero = fopen(name, "r");

    fsetpos(fichero, &posicion);

    chars = getline( & line, & size, fichero );

    //Se procesa solo una linea del archivo y los contadores del mapa se actualizan.
    if( chars > 0 )
    {
        etiquetas = processLine( line, etiquetas);
    }
    else
    {
        setFinish();
    }

    fgetpos(fichero, &nueva);
    setPos(nueva);

    free(line);
    fclose(fichero);
    return etiquetas;
}


/*
*Estrategia #4: inventada.
*
*/
std::map<std::string, int> FileReader::Four(FILE * archivo, std::map<std::string, int> etiquetas, int rango, fpos_t posicion, std::string filename)
{
    int chars;
    size_t size;
    char * line = NULL;
    int contador = 0; //Almacena las lineas que lleva leidas hasta el momento.
    fpos_t nueva;

    size = 512;
    line = (char *) calloc( 512, 1 );

    const char * name = filename.c_str();
    FILE * fichero = fopen(name, "r");

    fsetpos(fichero, &posicion);


    do
    {
        chars = getline( & line, & size, fichero );

        if( chars > 0 )
        {
            etiquetas = processLine( line, etiquetas);  //Se procesa cada linea del archivo, los contadores del mapa se actualizan
        }
        contador++;
    }
    while ( contador < rango );  //Si todavia no llega al rango correspondiente, continua. (chars != -1) &&


    fgetpos(fichero, &nueva);
    setPos(nueva);
    setFinish();

    free(line);
    fclose(fichero);

    return etiquetas;
}



/*
*Metodo encargado de imprimir en consola las etiquetas
*encontradas en el archivo. Asi como sus contadores.
*/
void FileReader::imprimir(std::map<std::string, int> etiquetas)
{
    std::map<std::string, int>:: iterator it = etiquetas.begin();
    char key[it->first.size() + 1];

    printf( "Lectura del archivo completada.\n" );
    while(it != etiquetas.end())
    {

        //Si el contador de la etiqueta es mayor a cero, es decir, aparecio al menos una vez, se imprime
        if(etiquetas[it->first] > 0)
        {
            strcpy(key, it->first.c_str());
            printf("Etiqueta: %s. Cantidad de veces encontrada: %d.\n", key, etiquetas[key]);
        }
        it++;
    }
}

/*
*Metodo encargado de inicializar y retornar el mapa con las etiquetas
*HTML mas probables.
*/
std::map<std::string, int> FileReader::inicializar(std::map<std::string, int> etiquetas)
{

    //std::map<std::string, int> etiquetas;

    etiquetas["!–…–"]=0;
    etiquetas["!DOCTYPE"]=0;
    etiquetas["a"]=0;
    etiquetas["abbr"]=0;
    etiquetas["address"]=0;
    etiquetas["area"]=0;
    etiquetas["article"]=0;
    etiquetas["aside"]=0;
    etiquetas["audio"]=0;
    etiquetas["b"]=0;
    etiquetas["base"]=0;
    etiquetas["bdi"]=0;
    etiquetas["bdo"]=0;
    etiquetas["blockquote"]=0;
    etiquetas["body"]=0;
    etiquetas["br"]=0;
    etiquetas["button"]=0;
    etiquetas["canvas"]=0;
    etiquetas["caption"]=0;
    etiquetas["cite"]=0;
    etiquetas["code"]=0;
    etiquetas["col"]=0;
    etiquetas["colgroup"]=0;
    etiquetas["command"]=0;
    etiquetas["datalist"]=0;
    etiquetas["dd"]=0;
    etiquetas["del"]=0;
    etiquetas["details"]=0;
    etiquetas["dfn"]=0;
    etiquetas["dialog"]=0;
    etiquetas["div"]=0;
    etiquetas["dl"]=0;
    etiquetas["dt"]=0;
    etiquetas["em"]=0;
    etiquetas["embed"]=0;
    etiquetas["fieldset"]=0;
    etiquetas["figcaption"]=0;
    etiquetas["figure"]=0;
    etiquetas["footer"]=0;
    etiquetas["form>"]=0;
    etiquetas["h1"]=0;
    etiquetas["h6"]=0;
    etiquetas["head"]=0;
    etiquetas["header"]=0;
    etiquetas["hgroup"]=0;
    etiquetas["hr"]=0;
    etiquetas["html"]=0;
    etiquetas["i"]=0;
    etiquetas["iframe"]=0;
    etiquetas["img"]=0;
    etiquetas["input"]=0;
    etiquetas["ins"]=0;
    etiquetas["kbd"]=0;
    etiquetas["keygen"]=0;
    etiquetas["label"]=0;
    etiquetas["legend"]=0;
    etiquetas["li"]=0;
    etiquetas["link"]=0;
    etiquetas["map"]=0;
    etiquetas["mark"]=0;
    etiquetas["menu"]=0;
    etiquetas["meta"]=0;
    etiquetas["meter"]=0;
    etiquetas["nav"]=0;
    etiquetas["noscript"]=0;
    etiquetas["objet"]=0;
    etiquetas["ol"]=0;
    etiquetas["optgroup"]=0;
    etiquetas["option"]=0;
    etiquetas["output"]=0;
    etiquetas["p"]=0;
    etiquetas["param"]=0;
    etiquetas["pre"]=0;
    etiquetas["progress"]=0;
    etiquetas["q"]=0;
    etiquetas["rp"]=0;
    etiquetas["rt"]=0;
    etiquetas["ruby"]=0;
    etiquetas["s"]=0;
    etiquetas["samp"]=0;
    etiquetas["script"]=0;
    etiquetas["section"]=0;
    etiquetas["select"]=0;
    etiquetas["small"]=0;
    etiquetas["source"]=0;
    etiquetas["span"]=0;
    etiquetas["strong"]=0;
    etiquetas["style"]=0;
    etiquetas["sub"]=0;
    etiquetas["summary"]=0;
    etiquetas["sup"]=0;
    etiquetas["table"]=0;
    etiquetas["tbody"]=0;
    etiquetas["td"]=0;
    etiquetas["textarea"]=0;
    etiquetas["tfoot"]=0;
    etiquetas["th"]=0;
    etiquetas["thead"]=0;
    etiquetas["time"]=0;
    etiquetas["title"]=0;
    etiquetas["tr"]=0;
    etiquetas["track"]=0;
    etiquetas["ul"]=0;
    etiquetas["var"]=0;
    etiquetas["video"]=0;
    etiquetas["wbr"]=0;
    etiquetas["/!–…–"]=0;
    etiquetas["/!DOCTYPE"]=0;
    etiquetas["/a"]=0;
    etiquetas["/abbr"]=0;
    etiquetas["/address"]=0;
    etiquetas["/area"]=0;
    etiquetas["/article"]=0;
    etiquetas["/aside"]=0;
    etiquetas["/audio"]=0;
    etiquetas["/b"]=0;
    etiquetas["/base"]=0;
    etiquetas["/bdi"]=0;
    etiquetas["/bdo"]=0;
    etiquetas["/blockquote"]=0;
    etiquetas["/body"]=0;
    etiquetas["/br"]=0;
    etiquetas["/button"]=0;
    etiquetas["/canvas"]=0;
    etiquetas["/caption"]=0;
    etiquetas["/cite"]=0;
    etiquetas["/code"]=0;
    etiquetas["/col"]=0;
    etiquetas["/colgroup"]=0;
    etiquetas["/command"]=0;
    etiquetas["/datalist"]=0;
    etiquetas["/dd"]=0;
    etiquetas["/del"]=0;
    etiquetas["/details"]=0;
    etiquetas["/dfn"]=0;
    etiquetas["/dialog"]=0;
    etiquetas["/div"]=0;
    etiquetas["/dl"]=0;
    etiquetas["/dt"]=0;
    etiquetas["/em"]=0;
    etiquetas["/embed"]=0;
    etiquetas["/fieldset"]=0;
    etiquetas["/figcaption"]=0;
    etiquetas["/figure"]=0;
    etiquetas["/footer"]=0;
    etiquetas["/form>"]=0;
    etiquetas["/h1"]=0;
    etiquetas["/h6"]=0;
    etiquetas["/head"]=0;
    etiquetas["/header"]=0;
    etiquetas["/hgroup"]=0;
    etiquetas["/hr"]=0;
    etiquetas["/html"]=0;
    etiquetas["/i"]=0;
    etiquetas["/iframe"]=0;
    etiquetas["/img"]=0;
    etiquetas["/input"]=0;
    etiquetas["/ins"]=0;
    etiquetas["/kbd"]=0;
    etiquetas["/keygen"]=0;
    etiquetas["/label"]=0;
    etiquetas["/legend"]=0;
    etiquetas["/li"]=0;
    etiquetas["/link"]=0;
    etiquetas["/map"]=0;
    etiquetas["/mark"]=0;
    etiquetas["/menu"]=0;
    etiquetas["/meta"]=0;
    etiquetas["/meter"]=0;
    etiquetas["/nav"]=0;
    etiquetas["/noscript"]=0;
    etiquetas["/objet"]=0;
    etiquetas["/ol"]=0;
    etiquetas["/optgroup"]=0;
    etiquetas["/option"]=0;
    etiquetas["/output"]=0;
    etiquetas["/p"]=0;
    etiquetas["/param"]=0;
    etiquetas["/pre"]=0;
    etiquetas["/progress"]=0;
    etiquetas["/q"]=0;
    etiquetas["/rp"]=0;
    etiquetas["/rt"]=0;
    etiquetas["/ruby"]=0;
    etiquetas["/s"]=0;
    etiquetas["/samp"]=0;
    etiquetas["/script"]=0;
    etiquetas["/section"]=0;
    etiquetas["/select"]=0;
    etiquetas["/small"]=0;
    etiquetas["/source"]=0;
    etiquetas["/span"]=0;
    etiquetas["/strong"]=0;
    etiquetas["/style"]=0;
    etiquetas["/sub"]=0;
    etiquetas["/summary"]=0;
    etiquetas["/sup"]=0;
    etiquetas["/table"]=0;
    etiquetas["/tbody"]=0;
    etiquetas["/td"]=0;
    etiquetas["/textarea"]=0;
    etiquetas["/tfoot"]=0;
    etiquetas["/th"]=0;
    etiquetas["/thead"]=0;
    etiquetas["/time"]=0;
    etiquetas["/title"]=0;
    etiquetas["/tr"]=0;
    etiquetas["/track"]=0;
    etiquetas["/ul"]=0;
    etiquetas["/var"]=0;
    etiquetas["/video"]=0;
    etiquetas["/wbr"]=0;

    return etiquetas;
}

/*
*Metodo que retorna el atributo privado
*/
int FileReader::getStrategy()
{
    return this->strategy;
}

/*
*Metodo que retorna el atributo privado
*/
int FileReader::getRange()
{
    return this->range;
}

/*
*Metodo que retorna el atributo privado
*/
int FileReader::getRangeMaster()
{
    return this->rangeMaster;
}

/*
*Metodo que retorna el atributo privado
*/
int FileReader::getWorkers()
{
    return this->workers;
}

/*
*Metodo que retorna el atributo privado
*/
fpos_t FileReader::getPos()
{
    return this->position;
}


/*
*Metodo que retorna el atributo privado
*/
FILE * FileReader::getFile()
{
    return this->fileId;
}

/*
*Metodo que retorna el atributo privado
*/
std::map<std::string, int> FileReader::getMap()
{
    return this->etiquetas;
}

/*
*Metodo que retorna el atributo privado
*/
std::string FileReader::getFilename()
{
    return this->filename;
}

/*
*Metodo que retorna el atributo privado
*/
bool FileReader::getFinish()
{
    return this->finish;
}

/*
*Metodo que implanta el atributo privado
*/
void FileReader::setFinish()
{
    this->finish = true;
}

/*
*Metodo que implanta el atributo privado
*/
void FileReader::setMap(std::map<std::string, int> actualizado)
{
    this->etiquetas = actualizado;
}

/*
*Metodo que implanta el atributo privado
*/
void FileReader::setPos(fpos_t posicion)
{
    this->position = posicion;
}
