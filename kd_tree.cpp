#include"kd_tree.h"
#include"kd_node.h"
#include<stack>
using namespace std;
template<class V>
bool kdtree<V>::isempty() {
	if (!root) {
		return true;
	}
	else {
		return false;
	}
}
template<class V>
void kdtree<V>::clear() {
	//会不会内存泄漏？
	root = NULL;
}
template<class V>
void kdtree<V>::buildKdtree(kdnode<V>* root1,vector<V*> data){
	int samplenum = data.size();
	int split1;
	double Std[3];
	if (samplenum ==0) {
		return;
	}
	else if (samplenum == 1) {
		root1->setpos(data[0]);
	}
	else {
		for (int i = 0; i < 3; i++) {
			Std[i] = std(data, i);
		}
		split1 = 0;
		double maxStd = std[0];
		for (int i = 1; i < 3; i++) {
			if (std[i] > maxStd) {
				maxStd = std[i];
				split1 = i;
			}
		}
		V *pV=findmidValue(data, split1);
		root1->setpos(pV);
		root1->setsplit(split1);
		vector<V *>subset1;
		vector<V *>subset2;
		for (int i = 0; i < data.size() / 2; i++) {
			subset1.push_back(data[i]);
		}
		for (int i = data.size() / 2+1; i < data.size() ; i++) {
			subset2.push_back(data[i]);
		}
		root1->getleftchild() = new kdnode();
		root1->getrightchild() = new kdnode();
		root1->getleftchild()->setparent(root1);
		root1->getrightchild()->setparent(root1);
		buildKdtree(root->getleftChild(), subset1);
		buildKdtree(root->getrightChild(), subset2);
	}
}
template<class V>
void kdtree<V>::insertKdtree() {

}
template<class V>
bool kdtree<V>::cmp1(V *a, V *b) {
	return a->point[0] < b->point()[0];
}
template<class V>
bool kdtree<V>::cmp2(V *a, V *b) {
	return a->point[1] < b->point()[1];
}
template<class V>
bool kdtree<V>::cmp3(V *a, V *b) {
	return a->point[2] < b->point()[2];
}
template<class V>
V* kdtree<V>::findmidValue(vector<V*> &a,int split) {
	if (split == 0) {
		sort(a.begin(), a.end(), cmp1);
		return a[a.size() / 2];
	}
}
template<class V>
double kdtree<V>::mean(vector<V*> a, int split) {
	double sum=0;
	for (int i = 0; i < a.size(); i++) {
		sum = sum + a[i]->point()[split];
	}
	return sum / a.size();
}
template<class V>
double kdtree<V>::std(vector<V*>a, int split) {
	double Mean = mean(a, split);
	double sum = 0;
	for (int i = 0; i < a.size(); i++) {
		sum = sum + (a[i]->point()[split] - Mean)(a[i]->point()[split] - Mean);
	}
	return sum / a.size()-1;
}
template<class V>
double getdist(V* a, V* b) {
	return sqrt((a->point()[0] - b->point()[0])*(a->point()[0] - b->point()[0]) + (a->point()[1] - b->point()[1])*(a->point()[1] - b->point()[1]) + (a->point()[2] - b->point()[2])*(a->point()[2] - b->point()[2]));
}
template<class V>
kdnode<V> * kdtree<V>::findnearestNeighbor(kdnode<V>* root1,V*v) {
	kdnode<V> *r = root1,*nearest;
	stack<kdnode<V>*> st;
	
	while (!r->isleaf()) {
		st.push(r);
		if (root1->getsplit() == 0 && v->point()[0] <= root1->getpos()->point()[0] || root1->getsplit() == 1 && v->point()[1] <= root1->getpos()->point()[1] || root1->getsplit() == 2 && v->point()[2] <= root1->getpos()->point()[2]) {
			r = r->getleftchild();
		}
		else {
			r = r->getrightchild();
		}
	}
	double minDist = getdist(v, r->getpos());
	nearest = r;
	double dist = 0;
	while (!st.empty()) {
		kdnode<V> *parent = st.top();
		st.pop();
		dist = getdist(v, parent->getpos());
		if (dist < minDist) {
			minDist = dist;
			nearest = parent;
		}
		double d_line = abs(v->point()[parent->getsplit()] - parent->getpos()->point()[parent->getsplit()]);
		if (d_line < mindist) {
			if (v->point()[parent->getsplit()] > parent->getpos()->point()[parent->getsplit()]) {
				if(parent->getleftchild() != NULL)
					parent = parent->getleftchild();
				else {
					parent = NULL;
				}
			}
			else if(v->point()[parent->getsplit()] < parent->getpos()->point()[parent->getsplit()] ){
				if (parent->getrightchild() != NULL)
					parent = parent->getrightchild();
				else {
					parent = NULL;
				}
			}
			if (parent != NULL) {
				while (parent->haveleft() || parent->haveright()) {
					st.push(parent);
					if (v->point()[parent->getsplit()] <= parent->getpos()->point()[parent->getsplit()]) {
						if (parent->getleftchild() != NULL) {
							parent = parent->getleftchild();
						}
						else {
							break;
						}
					}
					else {
						if (parent->getrightchild() != NULL) {
							parent = parent->getrightchild();
						}
						else {
							break;
						}
					}
				}
				if (parent->getleftchild() == NULL&&parent->getleftchild() == NULL) {
					st.push(parent);
				}
			}
		}
	}
	return nearest;
	/*if (root1 == NULL) {
		return;
	}
	double curdist = getdist(V, root1->getpos());
	if (curdist < mindist) {
		mindist = curdist;
		res = root1->getpos();
	}
	if (root1->getsplit() == 0 && v->point()[0] <= root1->getpos()->point()[0] || root1->getsplit() == 1 && v->point()[1] <= root1->getpos()->point()[1] || root1->getsplit() == 2 && v->point()[2] <= root1->getpos()->point()[2]) {
		findnearestNeighbor(root1->getleftchild(), v, res, mindist);
	}
	else {
		findnearestNeighbor(root1->getrightchild(), v, res, mindist);
	}
	if (root1->getsplit() == 0&&v->point()[0]+) {

	}*/
}
template<class V>
vector<V> kdtree<V>::findK_nearestNeighbor(V*v) {

}
