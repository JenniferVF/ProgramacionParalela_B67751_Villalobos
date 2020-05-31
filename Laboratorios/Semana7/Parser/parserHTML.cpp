#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <regex>
#include <iostream>
#include <map>


std::map<std::string, int> processLine(const char*, std::map<std::string, int>);
std::map<std::string, int> inicializar();
std::map<std::string, int> comparar(char *, std::map<std::string, int>);
void imprimir(std::map<std::string, int>);


int main( int argc, char ** argv )
{
    FILE * fileId;
    int chars;
    size_t size;
    char * line;
    //char * saveptr;

    //std::map<std::string, int> etiquetas = {{"BODY", 0}, {"/BODY", 0}, {"TITLE", 0}, {"/TITLE", 0}, {"HEAD", 0}, {"/HEAD", 0}, {"DIV", 0}, {"/DIV", 0}, {"A", 0}, {"/A", 0}, {"STRONG", 0}, {"/STRONG", 0}, {"BR", 0}, {"H1", 0}, {"H2", 0}, {"H3", 0}, {"H4", 0}, {"H5", 0}, {"H6", 0}, {"/H1", 0}, {"/H2", 0}, {"/H3", 0}, {"/H4", 0}, {"/H5", 0}, {"/H6", 0}, {"IMG", 0}, {"/IMG", 0}, {"OL", 0}, {"UL", 0}, {"LI", 0}, {"/LI", 0}, {"P", 0}, {"/P", 0}, {"SPAN", 0}};
    std::map<std::string, int> etiquetas = inicializar();


    if ( argc < 2 )
    {
        printf( "Por favor ingrese el archivo a utilizar.\n" );
        exit( 1 );
    }

    size = 512;
    line = (char *) calloc( 512, 1 );

    fileId = fopen( argv[ 1 ], "r" );
    if ( NULL == fileId )
    {
        perror( "El archivo no se pudo abrir.");
        exit( 2 );
    }

    do
    {
        chars = getline( & line, & size, fileId );
        etiquetas = processLine( line, etiquetas);  //Se procesa cada linea del archivo, los contadores del mapa se actualizan
    }
    while ( chars > 0 );

    free( line );

    fclose( fileId );

    imprimir(etiquetas); //Se imprimen las etiquetas encontradas, con sus respectivos contadores

}

/*
*Metodo encargado de procesar una linea del archivo.
*/
std::map<std::string, int> processLine( const char * line, std::map<std::string, int> etiquetas)
{
    char * token;
    std::regex texto( ">[^<]*<" );
    std::string ecci;

    ecci = regex_replace( line, texto, "> <" );
//   std::cout << "ecci = " << ecci << std::endl;

    token = strtok( (char * ) line, "< >\t\n" );
    //printf( "Lista de tokens: " );
    while ( NULL != token )
    {
        etiquetas = comparar(token, etiquetas); //Se compara la palabra obtenida con las etiquetas guardadas
        //printf( "%s, ", token );
        token = strtok( NULL, "< >\t\n");
    }
    //printf( "\n" );
    return etiquetas;
}

/*
*Metodo encargado de comparar la palabra entrante con cada una de las
*etiquetas guardadas en el mapa.
*/
std::map<std::string, int> comparar(char * token, std::map<std::string, int> etiquetas)
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
*Metodo encargado de imprimir en consola las etiquetas
*encontradas en el archivo. Asi como sus contadores.
*/
void imprimir(std::map<std::string, int> etiquetas)
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
std::map<std::string, int> inicializar()
{

    std::map<std::string, int> etiquetas;

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
