#include "Genoma.h"


// Metodos Fundamentales para la Implementación y el cumplimiento de los
// requisitos:
int Genoma::cargarInfoArchivo(string nomArchivo) {
  ifstream in_01("tablaComponentes.txt");
  ifstream in_02(nomArchivo);
  if (in_01.fail() || in_02.fail())
    return -1;
  string buffer;
  while (!in_01.eof()) {
    Componente c;
    getline(in_01, buffer);
    istringstream i(buffer);
    i >> c.codigo;
    i >> c.nombre;
    c.frecuencia = 0;
    histComponentes.push_back(c);
  }
  in_01.close();

  list<list<char>> cods;
  list<char> line;
  string descripcion;
  bool completa;
  char c;
  
  while(!in_02.eof()) 
  {
    getline(in_02, buffer);
    istringstream i(buffer);
    i>>c;
    if(c =='>'){
      if(!cods.empty())
      {
        Secuencia s = *new Secuencia(descripcion, cods, completa);
        secuencias.push_back(s);
        cods.clear();
      }
      i >> descripcion;
      completa = true;
      }
    else
    {
      line.push_back(c);
      while(!i.eof()){
        i >> c;
        if(c == '-') completa = false;
        line.push_back(c);
      }
      cods.push_back(line);
      line.clear();
    }
  }
  if(secuencias.size() == 0) return 0;
  else return 1;
}



