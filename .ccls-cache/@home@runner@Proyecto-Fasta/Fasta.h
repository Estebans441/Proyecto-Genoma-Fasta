#include "Secuencia.h"

struct Componente{
  char codigo;
  char * nombre;
  int codificacion; 
};

Componente componentes[18];
char codComps[] = {'A','C','G','T','U','R','Y','K','M','S','W','B','D','H','V','N','X','-'};

class Fasta{
  list<Secuencia> secuencias;
  ArbolBinario<Histograma> Huffman;
  list<Histograma> histograma;

  //Metodos relacionadas con la carga de archivos:
  void cargarInfoArchivo (string nomArchivo);
  void guardarInfoArchivo (string nomArchivo);

  //Destructora
  ~Fasta();

  //Metodos relacionados con la codificacion
  void codificar(); //codifica los codigos de las bases de las secuencias segun el algoritmo de Huffman
  ArbolBinario<Histograma> generarArbolHuffman(); //Genera el arbol de Huffman a partir del histograma general de las secuencias
  void ordenarHistograma(); // De acuerdo con la cantidad de concurrencias

  //Metodos relacionados con la decodificacion
  void decodificar(string nomArchivo);
};