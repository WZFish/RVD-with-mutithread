/*kd树类*/
#pragma once
#include"kd_node.h"
#include<vector>
#include<algorithm>
#include<queue>
#include<functional>
#include<stack>
using namespace std;
template<class V>
class kdtree {
public:
	kdnode<V> *root;
	bool isempty();//树是否为空
	void clear();//清空树
	void buildKdtree(kdnode<V>* root1, vector<V*> data);//构建kd树
	void insertKdtree();//插入节点（未完成）
	V* findmidValue(vector<V*> &a,int split);//寻找中间值
	double mean(vector<V*> a,int split);//求均值
	double std(vector<V*>a, int split);//求方差
	kdnode<V> * findnearestNeighbor(kdnode<V>* root1, V*v);//寻找最邻近节点
	vector<kdnode<V>*>  findK_nearestNeighbor(kdnode<V>* root1, V*v,int k);//寻找最邻近k个节点
	kdtree(kdnode<V> *r) :root(r) {}
	/*bool cmp1(V*a, V*b);
	bool cmp2(V*a, V*b);
	bool cmp3(V*a, V*b);*/
	kdtree() { root = new kdnode<V>; }
	void printtree(kdnode<V> *r);
};

template<class V>
class op {//优先队列比较类
public:
	bool operator ()(kdnode<V> *a, kdnode<V> *b) {
		return a->getpos()->dist > b->getpos()->dist;
	}
};
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
void kdtree<V>::buildKdtree(kdnode<V>* root1, vector<V*> data) {
	int samplenum = data.size();
	int split1;
	double Std[3];
	if (samplenum == 0) {
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
		double maxStd = Std[0];
		for (int i = 1; i < 3; i++) {//寻找方差最大的维度分割
			if (Std[i] > maxStd) {
				maxStd = Std[i];
				split1 = i;
			}
		}
		V *pV = findmidValue(data, split1);
		root1->setpos(pV);
		root1->setsplit(split1);
		vector<V *>subset1;//储存分割后左边的值
		vector<V *>subset2;//储存分割后右边的值
		for (int i = 0; i < data.size() / 2; i++) {
			subset1.push_back(data[i]);
		}
		for (int i = data.size() / 2 + 1; i < data.size(); i++) {
			subset2.push_back(data[i]);
		}
		
		
		if (subset1.size() > 0) {
			kdnode<V> *l = new kdnode<V>();
			root1->setleftchild(l);
			root1->getleftchild()->setparent(root1);
			buildKdtree(root1->getleftchild(), subset1);//递归建立左子树
		}
		if (subset2.size() > 0) {
			kdnode<V> *r = new kdnode<V>();
			root1->setrightchild(r);
			root1->getrightchild()->setparent(root1);
			buildKdtree(root1->getrightchild(), subset2);//递归建立右子树
		}
		
	}
}
template<class V>
void kdtree<V>::insertKdtree() {

}
template<class V>
bool cmp1(V *a, V *b) {
	return a->point()[0] < b->point()[0];
}
template<class V>
bool cmp2(V *a, V *b) {
	return a->point()[1] < b->point()[1];
}
template<class V>
bool cmp3(V *a, V *b) {
	return a->point()[2] < b->point()[2];
}


template<class V>
V* kdtree<V>::findmidValue(vector<V*> &a, int split) {
	if (split == 0) {
		sort(a.begin(), a.end(), cmp1<V>);
		return a[a.size() / 2];
	}
	else if (split == 1) {
		sort(a.begin(), a.end(), cmp2<V>);
		return a[a.size() / 2];
	}
	else {
		sort(a.begin(), a.end(), cmp3<V>);
		return a[a.size() / 2];
	}
}
template<class V>
double kdtree<V>::mean(vector<V*> a, int split) {
	double sum = 0;
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
		sum = sum + (a[i]->point()[split] - Mean)*(a[i]->point()[split] - Mean);
	}
	return sum / (a.size() - 1);
}
template<class V>
double getdist(V* a, V* b) {
	return sqrt((a->point()[0] - b->point()[0])*(a->point()[0] - b->point()[0]) + (a->point()[1] - b->point()[1])*(a->point()[1] - b->point()[1]) + (a->point()[2] - b->point()[2])*(a->point()[2] - b->point()[2]));
}
template<class V>
kdnode<V> * kdtree<V>::findnearestNeighbor(kdnode<V>* root1, V*v) {
	kdnode<V> *r = root1, *nearest;
	stack<kdnode<V>*> st;
	bool flag = 0;
	while (!r->isleaf()&&flag==0) {//先找到叶子节点
		st.push(r);
		if (r->getleftchild()!=NULL&&(r->getsplit() == 0 && v->point()[0] <= r->getpos()->point()[0] || r->getsplit() == 1 && v->point()[1] <= r->getpos()->point()[1] || r->getsplit() == 2 && v->point()[2] <= r->getpos()->point()[2])) {
			r = r->getleftchild();
		}
		else {
			if ((r->getrightchild() != NULL)) {
				r = r->getrightchild();
			}
			else {
				flag = 1;
			}
			
		}
	}
	double minDist = getdist(v, r->getpos());
	nearest = r;
	double dist = 0;
	while (!st.empty()) {//从叶子结点回溯
		kdnode<V> *parent = st.top();
		st.pop();
		dist = getdist(v, parent->getpos());
		if (dist < minDist) {
			minDist = dist;
			nearest = parent;
		}
		if (!parent->isleaf()) {
			double d_line = abs(v->point()[parent->getsplit()] - parent->getpos()->point()[parent->getsplit()]);
			if (d_line < minDist) {
				if (v->point()[parent->getsplit()] > parent->getpos()->point()[parent->getsplit()]) {
					if (parent->getleftchild() != NULL)
						parent = parent->getleftchild();
					else {
						parent = NULL;
					}
				}
				else {
					if (parent->getrightchild()!= NULL)
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

/*template<class V>
struct cmp4 {
	bool cmp4(kdnode<V> *a, kdnode<V> *b) {
		return a->getDistance() < b->getDistance();
	}
};*/

template<class V>
vector<kdnode<V>*> kdtree<V>::findK_nearestNeighbor(kdnode<V>* root1,V*v,int k) {
	/*if (v->id() ==37) {
		cout << v->id() << endl;
		getchar();
	}*/
	vector<V*> k_nearest;
	
	vector<kdnode<V>*>  knn;
	priority_queue<kdnode<V>*,vector<kdnode<V>*>,op<V>>q;//优先队列以距离为基准选取搜索过程中k个最近的节点
	kdnode<V> *r = root1, *nearest;
	stack<kdnode<V>*> st;
	//bool flag = 0;
	while (r != NULL/*&&r->getsplit()!=-1*/ ) {
		st.push(r);
		if (r->getleftchild() != NULL && (r->getsplit() == 0 && v->point()[0] <= r->getpos()->point()[0] || r->getsplit() == 1 && v->point()[1] <= r->getpos()->point()[1] || r->getsplit() == 2 && v->point()[2] <= r->getpos()->point()[2])) {
			r = r->getleftchild();
		}
		else {
			r = r->getrightchild();
			/*if ((r->getrightchild()->getpos() != NULL)) {
				
			}
			else {
				flag = 1;
			}*/

		}
	}
	double minDist;
	if (st.top()->getpos()->id() != v->id()) {
		
		minDist = getdist(v, st.top()->getpos());
	}
	else {
		minDist = 10000;
		st.pop();
	}

	
	//r->setDistance(minDist);
	//if (q.size()<k)
	//q.push(r);
	//nearest = r;
	double dist = 0;
	while (!st.empty()) {
		if (st.top()->getpos()->id() == v->id()) {
			st.pop();
			continue;
		}
		kdnode<V> *parent = st.top();
		st.pop();
		dist = getdist(v, parent->getpos());
		parent->getpos()->dist = dist;
		//parent->setDistance(dist);
		
		q.push(parent);
		if (dist < minDist) {
			minDist = dist;
			//nearest = parent;
		}
		if (!parent->isleaf()) {//非叶子结点则走到没走过的叶子
			double d_line = abs(v->point()[parent->getsplit()] - parent->getpos()->point()[parent->getsplit()]);
			if (d_line < minDist) {
				if (v->point()[parent->getsplit()] > parent->getpos()->point()[parent->getsplit()]) {
					if (parent->getleftchild() != NULL)
						parent = parent->getleftchild();
					else {
						parent = NULL;
					}
				}
				else {
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

	}
	//q.pop();
	for (int i = 0; i < k; i++) {
		if (!q.empty()) {
			knn.push_back(q.top());
			q.pop();
		}
	}
	return knn;

}

template<class V> 
void kdtree<V>::printtree(kdnode<V> *r) {
	if (r!= NULL&&r->getpos()!=NULL) {
		cout << r->getpos()->point() << endl;
		printtree(r->getleftchild());
		printtree(r->getrightchild());
	}
}