#ifndef __SECUENCIA
#define __SECUENCIA
#include "Grafo.h"
#include <algorithm>
#include <list>
#include <set>
#include <stdlib.h>
#include <string>
#include <utility>

using namespace std;

/*
  Se declara un struct con los atributos correspondientes al Histograma asignado
  para cada secuencia de manera individual, dicho histograma se declara en el
  interior del TAD Secuencia cómo una lista, a raíz de sus dos atributos
  (Código y Existencias), estructura de datos que permite almacenar dicha
  información para cada una de las bases presentes en la secuencia.
*/
struct Histograma {
  char codigo;     // Codigo de la base representada.
  int existencias; // Cantidad de repeticiones de la base
};

/*
  -------------------------------------
  T A D - S e c u e n c i a
  -------------------------------------
  Abstraccion del mundo real, que permite ilustrar las
  secuencias genéticas en el problema planteado, en conjunto
  con sus operaciones y relaciones con otros TADS.
*/
class Secuencia {
private:
  // Lista de atributos:
  // Descripción de la secuencia
  string descripcion;
  // Multi-Lista que contiene todas las líneas genéticas pertenecientes a la
  // Secuencia.
  /*
    Especificación Multi - Lista
    list<list<char>>
      1. list<char>: Correspondiente a la secuencia de cada línea del
    código de la secuencia- EJ: AGCTUWAGCTUW

      2. list<list<char>>: Correspondiente a todas las lineas de
    código de la secuencia. Ejemplo: AGCTUWAGCTUW AGCTUWAGCTUW AGC
  */
  list<list<char>> listaSecuencia;
  // Cantidad de lineas de la secuencia
  int n;
  // Longitud de la secuencia
  int longitud;
  // Identacion de la secuencia
  int identacion;
  // Histograma correspondiente a la lista del Struct mencionado anteriormente
  // para el control de las repeticiones de cada una de las bases
  list<Histograma> histograma;
  // Variable booleana que permite saber si una secuencia es completa
  bool completa;
  // Grafo
  Grafo<pair<int, int>, float> grafo;

public:
  // Metodos Getters and Setters:
  // Metodos Setters:
  void setDescripcion(string desc) { descripcion = desc; }
  void setHistograma(list<Histograma> lista) { histograma = lista; }
  void setN(int lon) { n = lon; }
  void setListaSecuencia(list<list<char>> s) { listaSecuencia = s; }
  void setCompleta(bool c) { completa = c; }
  void setLongitud(int longitud) { this->longitud = longitud; }
  void setIdentacion(int identacion) { this->identacion = identacion; }

  // Metodos Getters:
  string getDescripcion() { return descripcion; }
  list<Histograma> getHistograma() {
    if (histograma.empty())
      generarHistograma();
    return histograma;
  }
  list<list<char>> getListaSecuencia() { return listaSecuencia; }
  bool getCompleta() { return completa; }
  int getLongitud() { return this->longitud; }
  int getIdentacion() { return this->identacion; }

  // Metodo Constructor
  Secuencia(string desc, list<list<char>> l, bool completa, int n, int longitud,
            int identacion) {
    descripcion = desc;
    listaSecuencia = l;
    this->completa = completa;
    this->n = n;
    this->longitud = longitud;
    this->identacion = identacion;
  }
  // Metodo Destructor
  ~Secuencia() {
    descripcion.erase();
    histograma.clear();
    listaSecuencia.clear();
    n = 0;
  }

  // Metodos para cumplir con los requisitos
  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : c a n t i d a d B a s e s -> i n t
    -------------------------------------------------------------------
    Método que permite contar la cantidad de bases diferentes presentadas en una
    secuencia (Cómo apoyo del método de apoyo "listarSecuencia()", para
    mostrar la información presente), información que será enviada entre la
    jerarquía de clases por medio del método mencionado lineas atrás.
  */
  int cantidadBases() {
    list<list<char>> listAux = this->getListaSecuencia();
    set<char> setAux;
    list<list<char>>::iterator it = listAux.begin();
    for (; it != listAux.end(); it++) {
      list<char> subLista = *it;
      list<char>::iterator it1 = subLista.begin();
      for (; it1 != subLista.end(); it1++) {
        setAux.insert(*it1);
      }
    }
    return setAux.size();
  }

  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : l i s t a r S e c u e n c i a -> s t r i n g
    -------------------------------------------------------------------
    Retorna un string con la informacion de la
    Secuencia, desempeña el rol de funcion intermedia
    de la funcion listarSecuencias() implementada al
    interior del TAD Genoma.

    Casos Posibles de Retorno:
    (Secuencia descripcion_secuencia contiene b bases) -> En caso tal de que la
    secuencia sea completa. (Secuencia descripcion_secuencia contiene al menos b
    bases) -> En caso tal de que la secuencia sea incompleta.
  */
  string listarSecuencia() {
    if (this->getCompleta()) {
      return "Secuencia " + getDescripcion() + " contiene " +
             to_string(cantidadBases()) + " bases.";
    } else {
      return "Secuencia " + getDescripcion() + " contiene al menos " +
             to_string(cantidadBases()) + " bases.";
    }
  }

  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : g e t H i s t o g r a m a S t r i n g -> l i s t < s t
    r i n g >
    -------------------------------------------------------------------
    Retorna una lista de strings en la que
    cada linea corresponde a cada Base y su respectiva
    frecuencia, del histograma de cada secuencia respectivamente.
  */
  list<string> getHistogramaString() {
    list<string> valorRetorno;
    if (histograma.empty())
      generarHistograma();
    list<Histograma>::iterator it = histograma.begin();
    string hstring;
    for (; it != histograma.end(); it++) {
      hstring.clear();
      hstring.push_back((*it).codigo);
      hstring += ": " + to_string((*it).existencias);
      valorRetorno.push_back(hstring);
    }
    return valorRetorno;
  }

  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : g e n e r a r H i s t o g r a m a -> v o i d
    -------------------------------------------------------------------
    Genera el respectivo Histograma de la secuencia que
    corresponde a la frecuencia con la que aparece cada base en la
    secuencia y en el TAD es representado por una lista de la estructura
    Histograma definida al inicio: list<Histograma>.
  */
  void generarHistograma() {
    list<list<char>>::iterator it = listaSecuencia.begin();
    bool encontrado;
    for (; it != listaSecuencia.end(); it++) {
      list<char> listaInterna = *it;
      list<char>::iterator it1 = listaInterna.begin();
      for (; it1 != listaInterna.end(); it1++) {
        encontrado = false;
        list<Histograma>::iterator it2 = histograma.begin();
        for (; it2 != histograma.end(); it2++) {
          if ((*it2).codigo == *it1) {
            (*it2).existencias = (*it2).existencias + 1;
            encontrado = true;
            break;
          }
        }
        if (!encontrado) {
          Histograma histogramaIndividual;
          histogramaIndividual.codigo = *it1;
          histogramaIndividual.existencias = 1;
          histograma.push_back(histogramaIndividual);
        }
      }
    }
  }

  float rutaMasCorta(int ix, int jx, int x, int y, vector<char> &camino) {
    pair<int, int> origen(ix, jx);
    pair<int, int> destino(x, y);
    list<list<char>>::iterator it;
    list<char>::iterator jt;
    if (grafo.esVacio()) generarGrafo();
    int indDestino = grafo.verticeToIndice(destino);
    vector<vector<pair<int, int>>> caminos;
    float *distanciasMinimas = grafo.Dijkstra(grafo.verticeToIndice(origen), caminos);
    for (int i = 0; i < caminos[indDestino].size(); i++) {
      it = listaSecuencia.begin();
      advance(it, caminos[indDestino][i].first);
      jt = it->begin();
      advance(jt, caminos[indDestino][i].second);
      camino.push_back(*jt);
    }
    return distanciasMinimas[indDestino];
  }

  float rutaMasLargaBaseRemota(int i, int j, int &x, int &y, vector<char> &camino) {
    pair<int, int> origen(i, j);
    list<list<char>>::iterator it, it1;
    list<char>::iterator jt, jt1;

    if (grafo.esVacio()) generarGrafo();
    vector<vector<pair<int, int>>> caminos;
    float *distanciasMaximas = grafo.DijkstraInv(grafo.verticeToIndice(origen), caminos);
    int ix, jx;
    
    // Define el caracter de la base a la que se le buscara su remota.
    it = listaSecuencia.begin();
    advance(it, i);
    jt = it->begin();
    advance(jt, j);
    char base = *jt;

    //Costo mayor
    float mayor = -1;
    //Coordenadas en las que estara cada iteracion del for
    pair<int, int> cActuales;
    //El indice del vertice dentro del vector del grafo
    int indiceBase;
    // Recorrer la secuencia.
    for (ix = 0, it = listaSecuencia.begin(); it != listaSecuencia.end(); it++, ix++) {
      for(jx = 0, jt = it->begin() ; jt != it->end() ; jt++, jx++)      
      if(*jt == base){
        cActuales.first = ix;
        cActuales.second = jx;
        
        //Asignación para guardar las coordenadas de donde se presenta la coordenada remota.
        x = ix;
        y = jx;
        
        indiceBase = grafo.verticeToIndice(cActuales);
        if(distanciasMaximas[indiceBase] > mayor){
          mayor = distanciasMaximas[indiceBase];
          camino.clear();
          for (int iy = 0; iy < caminos[indiceBase].size(); iy++) {
            it1 = listaSecuencia.begin();
            advance(it1, caminos[indiceBase][iy].first);
            jt1 = it1->begin();
            advance(jt1, caminos[indiceBase][iy].second);
            camino.push_back(*jt1);
          }
        }
      }
    }
    return mayor;
  }

protected:
  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : g e t F i l a s -> i n t
    -------------------------------------------------------------------
    Retorna la cantidad de filas (Pensadas cómo matriz para la
    elaboración de los puntos relacionados con la
    implementación de Grafos)
  */
  int getFilas() {
    int valorRetorno = 0;
    list<list<char>> multiListaAux = this->getListaSecuencia();
    list<list<char>>::iterator it = multiListaAux.begin();
    for (; it != multiListaAux.end(); it++) {
      list<char> listaInterna = *it;
      valorRetorno += listaInterna.size();
    }
    return valorRetorno;
  }

  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : i n s e r t a r L i n e a G e n e t i c a -> v o i d
    -------------------------------------------------------------------
    Inserta una línea genética a la multilista superior
    de acuerdo con la especificación realizada. Nota: No
    se implementa la función de insertar base
    individualmente a raíz de que ello se realiza
    directamente con la inserción en la lista a través
    del metodo de STL conocido cómo push_back();
  */
  void insertarLineaGenetica(list<char> linea) {
    list<list<char>> multiListaAux = this->getListaSecuencia();
    multiListaAux.push_back(linea);
    this->setListaSecuencia(multiListaAux);
  }

  /*
    -------------------------------------------------------------------
    S e c u e n c i a : : g e n e r a r G r a f o -> v o i d
    -------------------------------------------------------------------
    Genera el grafo para la representacion de la secuencia.
    Corresponde a un grafo dirigido y ponderado cuyos vertices corresponden
    a las coordenadas que ocupa ij base en la multilista que se maneja
    para almacenar las bases de secuencia.
    El peso de cada arco se calcula de la siguiente forma
    * PESO = 1 / (1 + (ASCIIij - ASCIIxy))
    - Siendo ij la posicion del vertice de origen (base) y xy las posicion
    del vertice destino (base);
  */
  void generarGrafo() {
    list<list<char>>::iterator it, it2;
    list<char>::iterator jt, jt2;
    pair<int, int> v, next;
    int i, j;
    float peso;

    grafo = new Grafo<pair<int, int>, float>(true, true);

    // se agregan los vertices
    for (i = 0, it = listaSecuencia.begin(); it != listaSecuencia.end();
         it++, i++) {
      for (j = 0, jt = it->begin(); jt != it->end(); jt++, j++) {
        v.first = i;
        v.second = j;
        grafo.agregarVertice(v);
      }
    }

    // Se agregan los arcos (aristas)
    for (i = 0, it = listaSecuencia.begin(); it != listaSecuencia.end();
         it++, i++) {
      for (j = 0, jt = it->begin(); jt != it->end(); jt++, j++) {
        v.first = i;
        v.second = j;
        // vecino i+1, j (abajo)
        if (i < listaSecuencia.size() - 1) {
          next.first = i + 1;
          next.second = j;
          it2 = it;
          it2++;
          jt2 = it2->begin();
          advance(jt2, j);
          peso =
              1 / (1 + abs(static_cast<float>(*jt) - static_cast<float>(*jt2)));
          grafo.agregarArista(v, next, peso);
        }
        // vecino i-1, j (arriba)
        if (i > 0) {
          next.first = i - 1;
          next.second = j;
          it2 = it;
          it2--;
          jt2 = it2->begin();
          advance(jt2, j);
          peso =
              1 / (1 + abs(static_cast<float>(*jt) - static_cast<float>(*jt2)));
          grafo.agregarArista(v, next, peso);
        }
        // vecino i, j+1 (derecha)
        if (j < it->size() - 1) {
          next.first = i;
          next.second = j + 1;
          jt2 = it->begin();
          advance(jt2, j + 1);
          peso =
              1 / (1 + abs(static_cast<float>(*jt) - static_cast<float>(*jt2)));
          grafo.agregarArista(v, next, peso);
        }
        // vecino i, j-1 (izquierda)
        if (j > 0) {
          next.first = i;
          next.second = j - 1;
          jt2 = it->begin();
          advance(jt2, j - 1);
          peso =
              1 / (1 + abs(static_cast<float>(*jt) - static_cast<float>(*jt2)));
          grafo.agregarArista(v, next, peso);
        }
      }
    }
  }
};
#endif