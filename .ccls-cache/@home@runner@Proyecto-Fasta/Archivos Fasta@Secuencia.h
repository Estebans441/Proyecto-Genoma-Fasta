#ifndef __SECUENCIA
#define __SECUENCIA
#include "ArbolBinario.h"
#include <list>
#include <stdlib.h>
#include <string>

using namespace std;

struct Histograma{
  char codigo;
  int existencias;
};

class Secuencia {
private:
  string descripción;
  list<list<char>> listaSecuencia;
  int n; // Longitud de cada línea
  list<Histograma> histograma;
  bool completa;

public:
  // Metodos Getters and Setters:
  void setDescripcion(string desc);
  void setHistograma(list<Histograma> lista);
  void setLongitud(int lon);
  void setListaSecuencia(list<list<char>> s);
  void setHuffman(ArbolBinario<Histograma> Huffman);
  void setCompleta(bool c);
  string getDescripcion();
  list<Histograma> getHistograma;
  list<list<char>> getListaSecuencia();
  ArbolBinario<Histograma> getHuffman();
  bool getCompleta();

  // Metodos Constructores
  Secuencia();
  Secuencia(string desc, char componentes[], list<list<char>> listaSecuencia, int n);
  //Constructor copia
  Secuencia(Secuencia &sec);

  // Metodo Destructor
  ~Secuencia();

  // Metodos adicionales
  void codificarSecuencia();
  void decodificarSecuencia();


  int cantidadBases();
  string listarSecuencia(); //Retorna un string con la informacion de la Secuencia, desempeña el rol de funcion intermedia de la funcion listarSecuencias() implementada al interior del TAD Genoma.
  /*
  Casos Posibles de Retorno:
  (Secuencia descripcion_secuencia contiene b bases) -> En caso tal de que la secuencia sea completa.
  (Secuencia descripcion_secuencia contiene al menos b bases) -> En caso tal de que la secuencia sea incompleta.
  */

  list<string> getHistogramaString(); //Retorna una lista de strings en la que cada linea corresponde a cada Base y su respectiva frecuencia
  
int getFilas();
};
#endif