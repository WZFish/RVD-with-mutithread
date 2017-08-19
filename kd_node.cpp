#include"kd_node.h"
template<class V>
void kdnode<V>::setsplit(int s) {
	split = s;
}
template<class V>
int kdnode<V>::getsplit() {
	return split;
}
template<class V>
void kdnode<V>::setleftchild(kdnode * l) {
	leftchild = l;
}
template<class V>
kdnode<V>* kdnode<V>::getleftchild() {
	return leftchild;
}
template<class V>
void kdnode<V>::setrightchild(kdnode * r) {
	rightchild = r;
}
template<class V>
kdnode<V>*kdnode<V>::getrightchild() {
	return rightchild;
}
template<class V>
void kdnode<V>::setparent(kdnode *p) {
	parent = p;
}
template<class V>
kdnode<V> *kdnode<V>::getparent() {
	return parent;
}
template<class V>
void kdnode<V>::setpos(V *po) {
	pos = po;
}
template<class V>
V* kdnode<V>::getpos() {
	return pos;
}
template<class V>
bool kdnode<V>::isleaf() {
	if (!leftchild && !rightchild) {
		return true;
	}
	else {
		return false;
	}
}
template<class V>
bool kdnode<V>::haveleft() {
	if (!leftchild) {
		return false;
	}
	else {
		return true;
	}
}
template<class V>
bool kdnode<V>::haveright() {
	if (!rightchild) {
		return false;
	}
	else {
		return true;
	}
}
template<class V>
bool kdnode<V>::isright() {
	return parent->rightchild == this;
}
template<class V>
bool kdnode<V>::isleft() {
	return parent->leftchild == this;
}