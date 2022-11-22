#ifndef __GENOMA
#define __GENOMA
#include "ArbolBinario.cpp"
#include "Secuencia.h"
#include <bitset>
#include <cstddef>
#include <fstream>
#include <sstream>

/*
  Se declara un struct con los atributos correspondientes a un Componente,
  teniendo en cuenta que para cada uno de los archivos Fasta (Es decir para cada
  Genoma), va a existir una codificación independiente para cada una de las
  bases que allí se presente, situación que en el diseño se considera importante
  para la implementación del árbol de Huffman; la razón principal radica en la
  variación que existe para la codificación de cada componente en los diferentes
  archivos correspondientes a cada genoma.
*/

struct Componente {
  char codigo;     // Letra representada
  int frecuencia;  // Cantidad de oportunidades en que se repite el componente.
  char nombre[50]; // El nombre del componente
  string codificacion; // Codificación producto de los procesos
};

/*
  -------------------------------------
  T A D - G e n o m a
  -------------------------------------
  Tendra la informacion obtenida del archivo fasta ademas de la
  necesaria para responder a los comandos del usuario,
  incluyendo las operaciones.
*/
class Genoma {
private:
  // Lista de atributos
  // Atributo que representa las lineas de secuencia que tiene todo el genoma.
  list<Secuencia> secuencias;
  // numero de secuencias del genoma
  int numSecuencias;
  // Histograma general del genoma usado para generar el arbol de codificacion
  // de Huffman.
  list<Componente> histComponentes;
  // Arbol de Huffman destinado para los procesos de codificación y
  // decodificación
  ArbolBinario<Componente> Huffman;

public:
  // Metodos Getters and Setters
  void setSecuencias(list<Secuencia> codigos) { secuencias = codigos; }
  void setNumSecuencias(int n) { numSecuencias = n; }
  void setHistComponentes(list<Componente> hist_Componentes) {
    histComponentes = hist_Componentes;
  }
  void setHuffman(ArbolBinario<Componente> arbol) { Huffman = arbol; }

  list<Secuencia> getSecuencias() { return secuencias; }
  int getNumSecuencias() { return numSecuencias; }
  list<Componente> getHistComponentes() { return histComponentes; }
  ArbolBinario<Componente> getHuffman() { return Huffman; }

  // Metodos Fundamentales para la Implementación y el cumplimiento de los
  // requisitos:

  /*
    -------------------------------------------------------------------
    G e n o m a : : c a r g a r I n f o A r c h i v o -> i n t
    -------------------------------------------------------------------
    Carga la informacion de un archivo Fasta y retorna el
    numero de secuencias cargadas (En este caso se puede
    tomar esta función también como método constructor)

    Casos:
      * retorna 0 si el archivo se encuentra vacio
      * retorna -1 si el archivo no se encuentra o no puede leerse
      * retorna 1 si solo se carga correctamente una secuencia desde el archivo
      * retorna un valor diferente, es decir n, en caso tal de que logre leer n
        cantidad de secuencias a partir del archivo FASTA
  */
  int cargarInfoArchivo(string nomArchivo) {
    ifstream in_01("tablaComponentes.txt");
    ifstream in_02(nomArchivo);
    string buffer;

    // Variables para lectura del archivo fasta
    list<list<char>> cods;
    list<char> line;
    string descripcion;
    bool completa = true;
    char c;
    int n = 0, longitud = 0, identacion = 0;

    // Verificacion de apertura de archivos
    if (in_01.fail() || in_02.fail())
      return -1;

    // Vacia las listas para sobreescribir datos existentes en memoria
    secuencias.clear();
    histComponentes.clear();

    // Lectura del archivo correspondiente a la tabla de componentes
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

    // Lectura del archivo fasta
    while (!in_02.eof()) {
      getline(in_02, buffer);
      istringstream i(buffer);
      if (buffer[0] == '>') {
        if (!cods.empty()) {
          insertarSecuencia(*new Secuencia(descripcion, cods, completa, n,
                                           longitud, identacion));
          n = 0;
          longitud = 0;
          identacion = 0;
        }
        cods.clear();
        i >> c;
        i >> descripcion;
        completa = true;
      } else {
        if (buffer.length() > identacion)
          identacion = buffer.length();
        for (int i = 0; i < buffer.length(); i++) {
          c = buffer[i];
          if (c == 65 || c == 67 || c == 71 || c == 84 || c == 85 || c == 45)
            ;
          else
            completa = false;
          longitud++;
          line.push_back(c);
        }
        n++;
        cods.push_back(line);
        line.clear();
      }
    }
    insertarSecuencia(
        *new Secuencia(descripcion, cods, completa, n, longitud, identacion));
    in_02.close();

    return secuencias.size();
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : g u a r d a r I n f o A r c h i v o -> i n t
    -------------------------------------------------------------------
    Guarda en el archivo pasado como parametro las
    secuencias cargadas en memoria, se debe tener en cuenta
    la justificación de cada linea correspondiente al largo
    de cada una de las secuencias.

      Posibles Valores de Retorno:
        * 0: No hay secuencias cargadas en memoria.
        * -1: Error guardando en el archivo que se nombra cómo se pasa en el
    string de referencia.
        * 1: Escritura exitosa en el archivo de las secuencias guardadas
        en memoria.
  */
  int guardarInfoArchivo(string nomArchivo);

  /*
    -------------------------------------------------------------------
    G e n o m a : : l i s t a r S e c u e n c i a s -> l i s t < s t r i n g >
    -------------------------------------------------------------------
    Retorna una lista de strings en la que cada linea corresponde a la
    informacion de cada secuencia, proceso que se hace por medio del llamado del
    metodo listarSecuencia(), específicado como intermediario en el proceso. Si
    no hay ninguna secuencia la primera linea corresponde al mensaje de No hay
    secuencias cargadas en memoria.
  */
  list<string> listarSecuencias() {
    list<string> listaRetorno;
    if (secuencias.empty()) {
      listaRetorno.push_back("No hay secuencias cargadas en memoria");
    } else {
      list<Secuencia>::iterator it = secuencias.begin();
      for (; it != secuencias.end(); it++) {
        Secuencia secuenciaAux = *it;
        listaRetorno.push_back(secuenciaAux.listarSecuencia());
      }
    }
    return listaRetorno;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : l i s t a r h i s t o g r a m a -> l i s t < s t r i n g >
    -------------------------------------------------------------------
    Retorna una lista de Strings, correspondiente al histograma de la secuencia
    cuya descripción coincida con el nombre pasado cómo parámetro. En caso de
    éxito, devuelve en cada cadena de la lista, la información de una de las
    bases de dicho histograma.

    * En caso de fallo, es decir que la descripción de la secuencia pasada cómo
    parametro no coincida (No exista): Se retorna en la lista un único mensaje
    de error como éxito en el procedimiento.
  */
  list<string> listarhistograma(string descripcionSecuencia) {
    bool encontrada = false;
    list<string> retorno;
    list<Secuencia>::iterator it1 = secuencias.begin();
    for (; it1 != secuencias.end(); it1++) {
      if (descripcionSecuencia == (*it1).getDescripcion()) {
        retorno = (*it1).getHistogramaString();
        encontrada = true;
      }
    }
    if (!encontrada) {
      retorno.push_back("Secuencia invalida.");
    }
    return retorno;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : c o n t e o -> s t r i n g
    -------------------------------------------------------------------
    Retorna el mensaje correspondiente de acuerdo al numero de secuencias
    cargadas en memoria.
  */
  string conteo() {
    string retorno;
    if (secuencias.empty())
      return "No hay secuencias cargadas en memoria.";
    else if (secuencias.size() == 1)
      return "1 secuencia en memoria.";
    else
      return to_string(secuencias.size()) + " secuencias en memoria.";
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : c o d i f i c a r -> v o i d
    -------------------------------------------------------------------
    Genera el arbol de Huffman (en el metodo de
    generarArbolHuffman()) y de acuerdo a este le asigna los
    codigos de la codificacion a cada componente en el
    histograma. Posteriormente se usara este histograma para
    generar el archivo binario, ya que contiene la
    informacion necesaria para esto.
  */
  bool codificar(string nomArchivo) {
    // ofstream fichero(nomArchivo, ofstream::binary);
    list<Componente>::iterator it;
    list<Secuencia>::iterator it2;
    string codificacion = "";
    filebuf fichero;
    bitset<8> byte;

    GenerarHistograma();
    ordenarHistograma();
    Huffman = generarArbolHuffman();
    generarCodificacion(Huffman, "");

    // cantidad de bases en el genoma
    string n = toBinary(histComponentes.size());

    //Rellenar de ceros hasta cumplir los dos bytes.
    for (int i = n.size(); i < 16; i++)
      n = "0" + n;
    codificacion = n;

    // codigo de la base y su frecuencia
    string ci = "";
    string fi = "";
    string codi = "";
    for (it = histComponentes.begin(); it != histComponentes.end(); it++) {
      ci = toBinary(it->codigo);
      for (int i = ci.size(); i < 8; i++)
        ci = "0" + ci;

      fi = toBinary(it->frecuencia);
      for (int i = fi.size(); i < 8 * 8; i++)
        fi = "0" + fi;

      codi = it->codificacion;
      for (int i = codi.size(); i < 8; i++)
        codi = codi + "0";

      codificacion = codificacion + ci + codi + fi;
    }

    // cantidad de secuencias en el archivo
    string ns = toBinary(secuencias.size());
    for (int i = ns.size(); i < 8 * 4; i++)
      ns = "0" + ns;
    codificacion = codificacion + ns;

    // Secuencias
    string descripcion, li, sij, wi, xi, bc;
    list<list<char>> seq;
    list<list<char>>::iterator it3;
    list<char>::iterator it4;
    for (it2 = secuencias.begin(); it2 != secuencias.end(); it2++) {
      //En terminos de enunciado: it = i; it2 = j.
      
      descripcion = it2->getDescripcion();
      seq = it2->getListaSecuencia();
      it3 = seq.begin();

      // Tamanio descripcion
      li = toBinary(descripcion.size());
      for (int i = li.size(); i < 8 * 2; i++)
        li = "0" + li;
      codificacion += li;

      // Descripcion
      // Lo pasa a binario de acuerdo con el codigo ASCII de descripcion sub [i]
      for (int i = 0; i < descripcion.size(); i++) {
        sij = toBinary(descripcion[i]);
        for (int i = sij.size(); i < 8; i++)
          sij = "0" + sij;
        codificacion += sij;
      }

      // Longitud
      wi = toBinary(it2->getLongitud());
      for (int i = wi.size(); i < 8 * 8; i++)
        wi = "0" + wi;
      codificacion += wi;

      // Indentacion
      xi = toBinary(it2->getIdentacion());
      for (int i = xi.size(); i < 8 * 2; i++)
        xi = "0" + xi;
      codificacion += xi;

      // Secuencia en binario -> Codificacion
      for (; it3 != seq.end(); it3++) {
        for (it4 = it3->begin(); it4 != it3->end(); it4++) {
          bc = codificacionComponente(*it4);
          for (int i = bc.size(); i < 8; i++)
            bc = bc + "0";
          codificacion += bc;
        }
      }
    }

    // Apertura del archivo
    if (!fichero.open(nomArchivo, ios::out | ios::binary | ios::trunc))
      return 0;

    // Escribe en el archivo
    string s1;
    for (int i = 0; i < codificacion.size(); i += 8) {
      s1 = codificacion.substr(i, i + 7);
      byte = *new bitset<8>(s1);
      char a = (char)byte.to_ulong();
      fichero.sputn(&a, sizeof(char));
    }
    fichero.close();
    return 1;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : d e c o d i f i c a r -> v o i d
    -------------------------------------------------------------------
    Lee el archivo de formato .fabin y genera el genoma a partir de este.
  */
  bool decodificar(string nomArchivo) {
    filebuf fb;
    bitset<8> bytes;
    bitset<16> bytes2;
    bitset<32> bytes4;
    bitset<64> bytes8;
    char byte, ci, sij, base;
    int n, fi, li, wi, xi;
    string codi, buffer, descripcion;
    Componente componente;
    list<Componente>::iterator it;
    list<list<char>> cods;
    list<char> line;
    bool completa;

    // Elimina datos cargados anteriormente
    secuencias.clear();
    numSecuencias = 0;
    histComponentes.clear();

    if (!fb.open(nomArchivo, std::ios::binary | std::ios::in))
      return 0;

    // cantidad de bases en el genoma
    fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
    buffer = bytes.to_string();
    fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
    buffer += bytes.to_string();
    bytes2 = *new bitset<16>(buffer);
    n = bytes2.to_ulong();

    // Histograma general
    for (int i = 0; i < n; i++) {
      
      // codigo
      fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
      ci = (char)bytes.to_ulong();

      // codificacion
      fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
      codi = bytes.to_string();

      // frecuencia
      buffer = "";
      for (int j = 0; j < 8; j++) {
        fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
        buffer += bytes.to_string();
      }
      bytes8 = *new bitset<64>(buffer);
      fi = bytes8.to_ulong();

      // Agrega el componente a la lista correspondiente al histograma
      componente.codigo = ci;
      componente.codificacion = codi;
      componente.frecuencia = fi;
      histComponentes.push_back(componente);
    }

    /*cout << "------------------------" << endl;
    for (it = histComponentes.begin(); it != histComponentes.end(); it++)
      cout << it->codigo << ": " << it->frecuencia << ": " << it->codificacion
    << endl; cout << "------------------------" << endl;*/

    // Secuencias en el archivo
    buffer = "";
    for (int j = 0; j < 4; j++) {
      fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
      buffer += bytes.to_string();
    }
    bytes4 = *new bitset<32>(buffer);
    numSecuencias = bytes4.to_ulong();

    // Lectura de secuencias
    for (int i = 0; i < numSecuencias; i++) {
      cods.clear();
      line.clear();
      completa = true;
      // Tamanio del nombre de la secuencia
      buffer = "";
      for (int j = 0; j < 2; j++) {
        fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
        buffer += bytes.to_string();
      }
      bytes2 = *new bitset<16>(buffer);
      li = bytes2.to_ulong();

      // Descripcion
      descripcion = "";
      for (int j = 0; j < li; j++) {
        fb.sgetn(reinterpret_cast<char *>(&sij), sizeof(char));
        descripcion += sij;
      }

      // Longitud de la secuencia
      buffer = "";
      for (int j = 0; j < 8; j++) {
        fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
        buffer += bytes.to_string();
      }
      bytes8 = *new bitset<64>(buffer);
      wi = bytes8.to_ulong();

      // Identacion de la secuencia
      buffer = "";
      for (int j = 0; j < 2; j++) {
        fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
        buffer += bytes.to_string();
      }
      bytes2 = *new bitset<16>(buffer);
      xi = bytes2.to_ulong();

      // binary code
      int j;
      for (int i = 0; i < wi; i++) {
        fb.sgetn(reinterpret_cast<char *>(&bytes), sizeof(char));
        base = codigoCodificacion(bytes.to_string());
        if (base == 65 || base == 67 || base == 71 || base == 84 ||
            base == 85 || base == 45)
          ;
        else
          completa = false;
        line.push_back(base);
        j++;
        if (j == xi) {
          cods.push_back(line);
          line.clear();
          j = 0;
        }
      }
      cods.push_back(line);
      insertarSecuencia(*new Secuencia(descripcion, cods, completa, n, wi, xi));
    }
    return 1;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : r u t a M a s C o r t a -> l i s t < s t r i n g >
    -------------------------------------------------------------------
    Retorna una lista con la informacion que requiere el metodo ruta
    mas corta, donde la primera posicion de la lista corresponde al
    costo de la ruta y de ahi en adelante la ruta en si.
  */
  list<string> rutaMasCorta(string descripcion, int i , int j, int x, int y){
    list<string> retorno;
    vector<char> ruta;
    float costo;
    string aux;
    for(list<Secuencia>::iterator it = secuencias.begin() ; it != secuencias.end() ; it++){
      if(it->getDescripcion() == descripcion){
        costo = it->rutaMasCorta(i,j,x,y,ruta);
      }
    }
    retorno.push_back(to_string(costo));
    for(int i = 0 ; i < ruta.size() ; i++){
      aux = ruta[i];
      retorno.push_back(aux);
    }
    return retorno;
  }

/*
    -------------------------------------------------------------------
    G e n o m a : : r u t a B a s e R e m o t a -> l i s t < s t r i n g >
    -------------------------------------------------------------------
    Retorna una lista con la informacion que requiere el metodo ruta
    mas corta, donde la primera posicion de la lista corresponde al
    costo de la ruta y de ahi en adelante la ruta en si.
  */
  list<string> rutaBaseRemota (string descripcion, int i , int j){
    list<string> retorno;
    vector<char> ruta;
    
    float costo;
    string aux;
    
    int coordXRemota;
    int coordYRemota;
    
    for(list<Secuencia>::iterator it = secuencias.begin() ; it != secuencias.end() ; it++){
      if(it->getDescripcion() == descripcion){
        costo = it->rutaMasLargaBaseRemota(i,j,coordXRemota,coordYRemota,ruta);
      }
    }
    retorno.push_back(to_string(costo));

    string coordenadas;
    stringstream ss;
    ss << coordXRemota << "," << coordYRemota;
    coordenadas = ss.str();
    retorno.push_back(coordenadas);
    
    for(int i = 0 ; i < ruta.size() ; i++){
      aux = ruta[i];
      retorno.push_back(aux);
    }
    
    return retorno;
  }



protected:
  /*
    -------------------------------------------------------------------
    G e n o m a : : i n s e r t a r S e c u e n c i a -> v o i d
    -------------------------------------------------------------------
    Inserta una secuencia en la
    lista de secuencias del genoma.
    Ordena la lista (correspondiente al histograma
    general del genoma) ascendentemente de acuerdo a
    la frecuencia de los componentes. Esto para tener
    una lista base para generar el arbol de huffman.
  */
  void insertarSecuencia(Secuencia secuencia) {
    list<Secuencia> listAux = getSecuencias();
    listAux.push_back(secuencia);
    setSecuencias(listAux);
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : G e n e r a r H i s t o g r a m a -> v o i d
    -------------------------------------------------------------------
    Genera el histograma general del genoma teniendo en cuenta
    todas las secuencias. Esto como primer paso para le generacion
    del arbol de Huffman
  */
  void GenerarHistograma() {
    list<Histograma> aux;
    list<Secuencia>::iterator it;
    list<Histograma>::iterator it2;
    list<Componente>::iterator it3, it4;

    for (it3 = histComponentes.begin(); it3 != histComponentes.end(); it3++) {
      it3->frecuencia = 0;
      for (it = secuencias.begin(); it != secuencias.end(); it++) {
        aux = it->getHistograma();
        for (it2 = aux.begin(); it2 != aux.end(); it2++) {
          if (it2->codigo == it3->codigo) {
            it3->frecuencia += it2->existencias;
          }
        }
      }
    }
    it4 = histComponentes.end();

    // Elimina los componentes que no se encuentran dentro del genoma
    for (it3 = histComponentes.begin(); it3 != it4; it3++) {
      if (it3->frecuencia == 0) {
        histComponentes.erase(it3);

        // Iteradores que cambian si se elimina algun elemento, por consiguiente
        // es necesario inicializarlos con los valores correspondientes.
        it3 = histComponentes.begin();
        it4 = histComponentes.end();
      }
    }
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : o r d e n a r H i s t o g r a m a -> v o i d
    -------------------------------------------------------------------
    Ordena el histograma general del genoma a partir de menor a mayor
    de acuerdo a su frecuencia, esto es usado como paso anterior a la
    generacion del arbol de Huffman para la codificacion de las secuencias.
  */
  void ordenarHistograma() {
    list<Componente>::iterator it, it2, fin;
    Componente aux, aux2;
    fin = histComponentes.end();
    for (it = histComponentes.begin(); it != fin; it++) {
      for (it2 = histComponentes.begin(); it2 != fin; it2++) {
        if (it->frecuencia < it2->frecuencia) {
          // Asignacion de variables auxilaires de tipo componente para realizar
          // los cambios de orden
          aux = *it;
          aux2 = *it2;

          // Efectuar los cambios de acuerdo con lo establecido anteriormente.
          *it = aux2;
          *it2 = aux;
        }
      }
    }
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : g e n e r a r C o d i f i c a c i o n -> v o i d
    -------------------------------------------------------------------
    Recorre el arbol de Huffman asignando las codificaciones de las bases
    del genoma en la lista del Histograma.
  */
  void generarCodificacion(ArbolBinario<Componente> &a, string codificacion) {
    ArbolBinario<Componente> aux;
    string codIzq = codificacion, codDer = codificacion;
    list<Componente>::iterator it;
    codIzq.push_back('0');
    codDer.push_back('1');
    bool ok;
    if (!a.BinaryTreeIsEmpty()) {
      a.RootData().codificacion = codificacion;
      if (a.RootData().codigo != '*') // Asigna la codificacion a los
                                      // componentes en la lista del histograma
        for (it = histComponentes.begin(); it != histComponentes.end(); it++) {
          if (it->codigo == a.RootData().codigo) {
            it->codificacion = codificacion;
          }
        }
      
      a.DetachLeftSubtree(aux, ok);
      generarCodificacion(aux, codIzq);
      a.AttachLeftSubTree(aux, ok);
      a.DetachRightSubtree(aux, ok);
      generarCodificacion(aux, codDer);
      a.AttachLeftSubTree(aux, ok);
    }
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : g e n e r a r A r b o l H u f f m a n -> A r b o l B i n a r
    i o < C o m p o n e n t e >
    -------------------------------------------------------------------
    Genera el arbol de Huffman a partir de una list<Arbol<Componente>>
    por los siguientes pasos:

      1. Crear un arbol para cada componente del histograma en el que la raiz
      seria este componente y no tendria hijos.
      2. Toma los primeros dos elementos de la lista y los asigna al arbol
    derecho e izquierdo de un arbol nuevo cuya raiz corresponda a un componente
    con codigo * y frecuencia correspondiente a la suma de las frecuencias de
      sus hijos.
      3. Inserta este nuevo arbol al final de la lista
      4. Repite este proceso hasta que solo quede un arbol que corresponderia al
      arbol de huffman para la codificacion del genoma
      5. Retorna el arbol de huffman generado
  */
  ArbolBinario<Componente> generarArbolHuffman() {
    list<Componente>::iterator it;
    list<ArbolBinario<Componente>> listHuffman;
    list<ArbolBinario<Componente>>::iterator it2, it3;

    // Arma la lista de Huffman de acuerdo con todos los elementos del
    // Histograma de Componentes.
    for (it = histComponentes.begin(); it != histComponentes.end(); it++) {
      listHuffman.push_back(*new ArbolBinario<Componente>(*it));
    }

    while (listHuffman.size() != 1) {
      it2 = it3 = listHuffman.begin();
      it3++;

      // Genera el componente de raiz intermedia del arbol
      Componente c;
      c.codigo = '*';
      c.frecuencia = it2->RootData().frecuencia + it3->RootData().frecuencia;
      ArbolBinario<Componente> a(c, *it2, *it3);

      // Eliminacion de los primeros dos elementos de la lista (ahora
      // pertenecientes al arbol a)
      listHuffman.pop_front();
      listHuffman.pop_front();
      it2 = listHuffman.end();
      it2--;

      if (listHuffman.size() == 0 ||
          a.RootData().frecuencia >= it2->RootData().frecuencia) {
        listHuffman.push_back(a);
      } else {
        it2 = listHuffman.begin();
        while (it2 != listHuffman.end()) {
          if (a.RootData().frecuencia < it2->RootData().frecuencia) {
            break;
          }
          it2++;
        }
        if (it2 == listHuffman.end())
          listHuffman.push_back(a);
        else
          listHuffman.insert(it2, a); // Insertar 'a' antes de it2.
      }
    }

    it2 = listHuffman.begin();
    return *it2;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : t o B i n a r y -> s t r i n g
    -------------------------------------------------------------------
    Retorna un string con la forma binaria de un entero recibido como parametro.
  */
  string toBinary(int n) {
    int residuo;
    string binario = "";
    while (n != 0) {
      residuo = n % 2;
      binario = to_string(residuo) + binario;
      n /= 2;
    }
    return binario;
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : c o d i f i c a c i o n C o m p o n e n t e -> s t r i n g
    -------------------------------------------------------------------
    Retorna un string con la codificacion alamacenada en la lista que
    corresponde al histograma de los componentes a partir de un char recibido
    como parametro que es el codigo del componente.
  */
  string codificacionComponente(char c) {
    list<Componente>::iterator it;
    for (it = histComponentes.begin(); it != histComponentes.end(); it++)
      if (c == it->codigo)
        return it->codificacion;
    return "";
  }

  /*
    -------------------------------------------------------------------
    G e n o m a : : c o d i g o C o d i f i c a c i o n -> c h a r
    -------------------------------------------------------------------
    Retorna un char correspondiente al codigo del componente al que
    le corresponde la codificacion recibida como parametro.
  */
  char codigoCodificacion(string codificacion) {
    list<Componente>::iterator it;
    for (it = histComponentes.begin(); it != histComponentes.end(); it++)
      if (codificacion == it->codificacion)
        return it->codigo;
    return '?';
  }
};
#endif