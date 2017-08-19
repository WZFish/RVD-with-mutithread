/*kd树的节点类*/
#pragma once
#include<iostream>
using namespace std;
template <class V>
class kdnode {
	int split;//分割维度共三维0表示以x轴为基准，1表示y，2表示z
	kdnode *leftchild;//节点左孩子
	kdnode *rightchild;//节点右孩子
	kdnode *parent;//节点父节点
	V *pos;//节点中数据
	//double dist;
public:
	void setsplit(int s);//get set函数
	int getsplit();
	void setleftchild(kdnode * l);
	kdnode *getleftchild();
	void setrightchild(kdnode * r);
	kdnode *getrightchild();
	void setparent(kdnode *p);
	kdnode *getparent();
	void setpos(V *po);
	V* getpos();
	//double getDistance();
	//void setDistance(double dist);
	kdnode(int s,kdnode*l,kdnode*r,kdnode*p,V*po):split(s),leftchild(l),rightchild(r),parent(p),pos(po){}
	kdnode() { leftchild = rightchild = parent = NULL; split = -1; pos = NULL; }
	bool isleaf();//是否为叶子节点
	bool isleft();//是否为左孩子
	bool isright();//是否为右孩子
	bool haveleft();//是否有左孩子
	bool haveright();//是否有右孩子
	//friend bool operator >(const kdnode*a,const kdnode*b) { dist > b->getDistance(); }
	
};

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
//template<class V>
//double kdnode<V>::getDistance() {
//	return dist;
//}
//template<class V>
//void kdnode<V>::setDistance(double d) {
//	dist = d;
//}