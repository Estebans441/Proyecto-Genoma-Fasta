// Arbol Binario
/*
Se decide dar uso del TAD Arbol Binario especificada en clase a raíz de que de manera colectiva se conoce su funcionamiento y permite realizar la implementación de Arboles de Huffman a lo largo del proyecto.
*/
#ifndef __ARBOL_BINARIO
#define __ARBOL_BINARIO
#include <assert.h> // Para Assert
#include <iostream>
#include <stddef.h> // Para NULL

template <class T> class ArbolBinario; // declaración adelantada

template <class T> class TNodoArbol {
private:
  T Info;
  TNodoArbol<T> *LChildPtr;
  TNodoArbol<T> *RChildPtr;
  TNodoArbol(const T &NodeItem, TNodoArbol<T> *L = NULL,
             TNodoArbol<T> *R = NULL);
  friend class ArbolBinario<T>;
};

template <class T> class ArbolBinario {

private:
  TNodoArbol<T> *Root; // apuntador a la raíz del árbol

public:
  // constructores y destructores

  ArbolBinario(); // constructor predefinido
  ArbolBinario(const T &RootItem);
  ArbolBinario(const T &RootItem, const ArbolBinario<T> &LeftTree,
               const ArbolBinario<T> &RightTree);
  ArbolBinario(const ArbolBinario<T> &Tree);

  ~ArbolBinario();

  // operaciones Analizadoras

  bool BinaryTreeIsEmpty() const;
  T RootData() const;
  ArbolBinario<T> LeftSubtree() const;
  ArbolBinario<T> RightSubtree() const;
  void PreorderTraverse(void (*Visit)(T &AnItem));
  void InorderTraverse(void (*Visit)(T &AnItem));
  void PostorderTraverse(void (*Visit)(T &AnItem));

  // modificadoras

  void SetRootData(const T &NewItem);
  void AttachLeft(const T &NewItem, bool &Success);
  void AttachRight(const T &NewItem, bool &Success);
  void AttachLeftSubTree(const ArbolBinario<T> &LeftTree, bool &Success);
  void AttachRightSubTree(const ArbolBinario<T> &RightTree, bool &Success);
  void DetachLeftSubtree(ArbolBinario<T> &LeftTree, bool &Success);
  void DetachRightSubtree(ArbolBinario<T> &RightTree, bool &Success);

  // sobrecarga del operador de asignacion
  ArbolBinario<T> &operator=(const ArbolBinario<T> &Rhs);

protected:
  ArbolBinario(TNodoArbol<T> *NodePtr); // constructor

  void CopyTree(TNodoArbol<T> *TreePtr, TNodoArbol<T> *&NewTreePtr) const;

  // Copia el árbol de raíz en TreePtr dentro del árbol
  // de raíz NewTreePtr

  void DestroyTree(TNodoArbol<T> *&TreePtr);
  // Desasigna la memoria para un árbol
  // las siguientes dos funciones/metodos reciben y establecen
  // el valor de la variable miembro privada Root

  TNodoArbol<T> *RootPtr() const;
  void SetRootPtr(TNodoArbol<T> *NewRoot);

  // Las siguientes dos funciones reciben y establecen los
  // valores de los apuntadores hijos de un nodo de un árbol

  void GetChildPtrs(TNodoArbol<T> *NodePtr, TNodoArbol<T> *&LChildPtr,
                    TNodoArbol<T> *&RChildPtr) const;
  void SetChildPtrs(TNodoArbol<T> *NodePtr, TNodoArbol<T> *LChildPtr,
                    TNodoArbol<T> *RChildPtr);
  void Preorder(TNodoArbol<T> *TreePtr, void (*Visit)(T &AnItem));
  void Inorder(TNodoArbol<T> *TreePtr, void (*Visit)(T &AnItem));
  void Postorder(TNodoArbol<T> *TreePtr, void (*Visit)(T &AnItem));
};
#endif