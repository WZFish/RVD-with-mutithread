/*kd���Ľڵ���*/
#pragma once
#include<iostream>
using namespace std;
template <class V>
class kdnode {
	int split;//�ָ�ά�ȹ���ά0��ʾ��x��Ϊ��׼��1��ʾy��2��ʾz
	kdnode *leftchild;//�ڵ�����
	kdnode *rightchild;//�ڵ��Һ���
	kdnode *parent;//�ڵ㸸�ڵ�
	V *pos;//�ڵ�������
	//double dist;
public:
	void setsplit(int s);//get set����
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
	bool isleaf();//�Ƿ�ΪҶ�ӽڵ�
	bool isleft();//�Ƿ�Ϊ����
	bool isright();//�Ƿ�Ϊ�Һ���
	bool haveleft();//�Ƿ�������
	bool haveright();//�Ƿ����Һ���
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