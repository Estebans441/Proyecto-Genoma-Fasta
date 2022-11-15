#include "ArbolBinario.h" 
//--------------------------------------------------------------------------- 

//
// T N o d o : : T N o d o A r b o l 
//
template <class T>
TNodoArbol<T>::TNodoArbol(const T& NodeItem, TNodoArbol<T>* L, TNodoArbol<T>* R) 
{ 
	Info = NodeItem; 
	LChildPtr = L; 
	RChildPtr = R; 
} 
//
// A r b o l B i n a r i o : : A r b o l B i n a r i o 
//

template <class T>
ArbolBinario<T>::ArbolBinario() { 
	Root = NULL; 
} 
//
// A r b o l B i n a r i o : : A r b o l B i n a r i o 
//
template <class T>
ArbolBinario<T>::ArbolBinario(const T& RootItem) 
{ 
	Root = new TNodoArbol<T>(RootItem); 
	assert(Root != NULL); 
} 
//
// A r b o l B i n a r i o : : A r b o l B i n a r i o 
//
template <class T>
ArbolBinario<T>::ArbolBinario(const T& RootItem, 
	const ArbolBinario<T>& LeftTree, 
	const ArbolBinario<T>& RightTree) 
{ 
	bool Success; 
	Root = new TNodoArbol<T>(RootItem); 
	assert(Root != NULL); 
	AttachLeftSubTree(LeftTree, Success); 
	if (Success) 
		AttachRightSubTree(RightTree, Success); 
	assert(Success); 
} 
//
// A r b o l B i n a r i o : : A r b o l B i n a r i o 
//
template <class T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& Tree) 
{ 
	CopyTree(Tree.Root, Root); 
} 
//
// A r b o l B i n a r i o : : A r b o l B i n a r i o 
//
template <class T>
ArbolBinario<T>::ArbolBinario(TNodoArbol<T> *NodePtr) 
{ 
	Root = NodePtr; 
} 
//
// A r b o l B i n a r i o : : ~ A r b o l B i n a r i o 
//
template <class T>
ArbolBinario<T>::~ArbolBinario() 
{ 
	DestroyTree(Root); 
} 
//
// b o o l
// A r b o l B i n a r i o : : B i n a r y T r e e I s E m p t y
//
template <class T>
bool ArbolBinario<T>::BinaryTreeIsEmpty() const 
{ 
	return bool(Root == NULL); 
} 
//
// i n t 
// A r b o l B i n a r i o : : R o o t D a t a
//
template <class T>
T ArbolBinario<T>::RootData() const 
{ 
	assert(Root != NULL); 
	return Root->Info; 
} 
//
// v o i d
// A r b o l B i n a r i o : : S e t R o o t D a t a
//
template <class T>
void ArbolBinario<T>::SetRootData(const T& NewItem) 
{ 
if (Root != NULL) 
	Root->Info = NewItem; 
else
	{ 
		Root = new TNodoArbol<T>(NewItem); 
		assert(Root != NULL); 
	} 
} 
//
// v o i d
// A r b o l B i n a r i o : : A t t a c h L e f t
//
template <class T>
void ArbolBinario<T>::AttachLeft(const T& NewItem, bool &Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty() && Root->LChildPtr == NULL); 
	if (Success) 
	{ // Arbol no vacio y sin hijo izquierdo 
		Root->LChildPtr = new TNodoArbol<T>(NewItem); 
		assert(Root->LChildPtr != NULL); 
	} 
} 
//
// v o i d
// A r b o l B i n a r i o : : A t t a c h R i g h t
//
template <class T>
void ArbolBinario<T>::AttachRight(const T& NewItem, bool &Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty() && Root->RChildPtr == NULL); 
	if (Success) 
	{ 
		Root->RChildPtr = new TNodoArbol<T>(NewItem); 
		assert(Root->RChildPtr != NULL); 
	} 
} 
//
// v o i d
// A r b o l B i n a r i o : : A t t a c h L e f t S u b T r e e
//
template <class T>
void ArbolBinario<T>::AttachLeftSubTree(const ArbolBinario<T>& LeftTree, 
bool &Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty() && Root->LChildPtr == NULL); 
	if (Success) // Arbol no vacio, sin hijo izquierdo 
		CopyTree(LeftTree.Root, Root->LChildPtr); 
} 
//
// v o i d
// A r b o l B i n a r i o : : A t t a c h R i g h t S u b T r e e
//
template <class T>
void ArbolBinario<T>::AttachRightSubTree(const ArbolBinario<T>& RightTree, 
bool &Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty() && Root->RChildPtr == NULL); 
	if (Success) // Arbol no vacio, sin hijo derecho 
		CopyTree(RightTree.Root, Root->RChildPtr); 
} 
//
// v o i d
// A r b o l B i n a r i o : : D e t a c h L e f t S u b t r e e
//
template <class T>
void ArbolBinario<T>::DetachLeftSubtree(ArbolBinario<T>& LeftTree, 
bool& Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty()); 
	if (Success) 
	{ 
		LeftTree = ArbolBinario(Root->LChildPtr); 
		Root->LChildPtr = NULL; 
	} 
} 
//
// v o i d
// A r b o l B i n a r i o : : D e t a c h R i g h t S u b t r e e
//
template <class T>
void ArbolBinario<T>::DetachRightSubtree(ArbolBinario<T>& RightTree, 
bool &Success) 
{ 
	Success = bool(!BinaryTreeIsEmpty()); 
	if (Success) 
	{ 
		RightTree = ArbolBinario(Root->RChildPtr); 
		Root->RChildPtr = NULL; 
	} 
} 
//
// A r b o l B i n a r i o 
// A r b o l B i n a r i o : : L e f t S u b t r e e 
//
template <class T>
ArbolBinario<T> ArbolBinario<T>::LeftSubtree() const 
{ 
	TNodoArbol<T>* SubTreePtr; 
	if (BinaryTreeIsEmpty()) 
		return ArbolBinario(); 
	else 
		{ 
			CopyTree(Root->LChildPtr, SubTreePtr); 
			return ArbolBinario(SubTreePtr); 
		} 
} 
//
// A r b o l B i n a r i o 
// A r b o l B i n a r i o : : R i g h t S u b t r e e 
//
template <class T>
ArbolBinario<T> ArbolBinario<T>::RightSubtree() const 
{ 
	TNodoArbol<T> *SubTreePtr; 
	if (BinaryTreeIsEmpty()) 
		return ArbolBinario(); 
	else 
	{ 
		CopyTree(Root->RChildPtr, SubTreePtr); 
		return ArbolBinario(SubTreePtr); 
	} 
} 
//
// v o i d 
// A r b o l B i n a r i o : : P r e o r d e r T r a v e r s e
//
template <class T>
void ArbolBinario<T>::PreorderTraverse(void (*Visit)( T& AnItem)) 
{ 
	Preorder(Root, Visit); 
} 
//
// v o i d 
// A r b o l B i n a r i o : : I n o r d e r T r a v e r s e
//
template <class T>
void ArbolBinario<T>::InorderTraverse(void (*Visit)( T& AnItem)) 
{ 
	Inorder(Root, Visit); 
} 
//
// v o i d 
// A r b o l B i n a r i o : : P o s t o r d e r T r a v e r s e
//
template <class T>
void ArbolBinario<T>::PostorderTraverse(void (*Visit)( T& AnItem)) 
{ 
	Postorder(Root, Visit); 
} 
//
// A r b o l B i n a r i o 
// A r b o l B i n a r i o : : o p e r a t o r =
//
template <class T>
ArbolBinario<T>& ArbolBinario<T>::operator=(const ArbolBinario<T>& Rhs) 
{ 
	if (this != &Rhs) 
	{ 
		DestroyTree(Root); // Desasigne memoria al lado izquierdo del op. 
		CopyTree(Rhs.Root, Root); // copy el lado derecho 
	} 
return *this; 
} 
//
// v o i d 
// A r b o l B i n a r i o : : C o p y T r e e
//
template <class T>
void ArbolBinario<T>::CopyTree(TNodoArbol<T>* TreePtr, TNodoArbol<T>*& NewTreePtr) const 
{ 
	// recorrido en preorden 
	if (TreePtr != NULL) 
		{ 
		// copiar el nodo 
			NewTreePtr = new TNodoArbol<T>(TreePtr->Info); 
			assert(NewTreePtr != NULL); 
			CopyTree(TreePtr->LChildPtr, NewTreePtr->LChildPtr); 
			CopyTree(TreePtr->RChildPtr, NewTreePtr->RChildPtr); 
		} 
	else 
		NewTreePtr = NULL; // copiar árbol vacio 
} 
//
// v o i d 
// A r b o l B i n a r i o : : D e s t r o y T r e e
//
template <class T>
void ArbolBinario<T>::DestroyTree(TNodoArbol<T>*& TreePtr) 
{ 
// postorden 
if (TreePtr != NULL) 
	{ 
		DestroyTree(TreePtr->LChildPtr); 
		DestroyTree(TreePtr->RChildPtr); 
		delete TreePtr; 
		TreePtr = NULL; 
	} 
} 
//
// T N o d o A r b o l *
// A r b o l B i n a r i o : : R o o t P t r
//
template <class T>
TNodoArbol<T>* ArbolBinario<T>::RootPtr() const 
{ 
	return Root; 
} 
//
// v o i d 
// A r b o l B i n a r i o : : S e t R o o t P t r
//
template <class T>
void ArbolBinario<T>::SetRootPtr(TNodoArbol<T>* NewRoot) 
{ 
	Root = NewRoot; 
} 
//
// v o i d 
// A r b o l B i n a r i o : : G e t C h i l d P t r s
//
template <class T>
void ArbolBinario<T>::GetChildPtrs(TNodoArbol<T>* NodePtr, TNodoArbol<T>*& LeftPtr, 
			TNodoArbol<T>*& RightPtr) const 
{ 
	LeftPtr = NodePtr->LChildPtr; 
	RightPtr = NodePtr->RChildPtr; 
} 
//
// v o i d 
// A r b o l B i n a r i o : : S e t C h i l d P t r s
//
template <class T>
void ArbolBinario<T>::SetChildPtrs(TNodoArbol<T>* NodePtr, TNodoArbol<T>* LeftPtr, 
			TNodoArbol<T>* RightPtr) 
{ 
	NodePtr->LChildPtr = LeftPtr; 
	NodePtr->RChildPtr = RightPtr; 
} 
//
// v o i d 
// A r b o l B i n a r i o : : P r e o r d e r 
//
template <class T>
void ArbolBinario<T>::Preorder(TNodoArbol<T>* TreePtr, void (*Visit)(T& AnItem)) 
{ 
	if (TreePtr != NULL) 
	{ 
		Visit(TreePtr->Info); 
		Preorder(TreePtr->LChildPtr, Visit); 
		Preorder(TreePtr->RChildPtr, Visit); 
	} 
} 
//
// v o i d 
// A r b o l B i n a r i o : : I n o r d e r 
//
template <class T>
void ArbolBinario<T>::Inorder(TNodoArbol<T>* TreePtr, void (*Visit)(T& AnItem)) 
{ 
	if (TreePtr != NULL) 
	{ 
		Inorder(TreePtr->LChildPtr, Visit); 
		Visit(TreePtr->Info); 
		Inorder(TreePtr->RChildPtr, Visit); 
	} 
} 

//
// v o i d 
// A r b o l B i n a r i o : : P o s t o r d e r
//
template <class T>
void ArbolBinario<T>::Postorder(TNodoArbol<T>* TreePtr, void (*Visit)(T& AnItem)) 
{ 
	if (TreePtr != NULL) 
		{ 
			Postorder(TreePtr->LChildPtr, Visit); 
			Postorder(TreePtr->RChildPtr, Visit); 
			Visit(TreePtr->Info); 
		} 
}
