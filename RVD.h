#pragma once
#include"kd_tree.h"
#include"kd_node.h"
#include"MyVertex1.h"
#include"MyVertex.h"
#include"MyPolygon.h"
#include<stack>
#include<thread>
#include<mutex>

using namespace std;
using namespace MeshLib;
using namespace Eigen;


class RVD {
public:
	//stack<MyVertex >S;
	//vector<thread*> threadpool;
	kdtree<MyVertex> tree;//kd树
	vector<vector<MyPolygon>> allClipPolygon;//所有切割后的多边形
	void getAllFace(CMyMesh *mesh);//得到所有网格面
	void getAllVertex(CMyMesh *mesh);//得到所有的顶点
	vector<MyVertex*> allVertex;//保存所有顶点
	vector<CFace*> allface;//保存所有面
	vector<MyPolygon> allPolygon;//保存所有初始多边形
	void getAllInitPolygon();//构造初始多边形
	void getpair();//a最邻近有b，b最邻近没有a时在b中添加a
	void getallVertexK_nearest();//得到k个最邻近
	//friend void clipAllPolygon(int Begin, int End,RVD *r);
	void RVDAlgorithm(CMyMesh *mesh, CMyMesh *mesh1);//算法执行主函数
	void getallcentroid();//得到多边形重心
	void display();
};


void RVD::getAllVertex(CMyMesh *mesh) {
	for (CMyMesh::MeshVertexIterator viter(mesh); !viter.end(); viter++) {
		CVertex *pV = *viter;
		pV->point() = pV->point()*0.8;//得到原来的点的0.8倍的点
		MyVertex *mv=new MyVertex(pV);
		allVertex.push_back(mv);
		
	}
	for (CMyMesh::MeshVertexIterator viter(mesh); !viter.end(); viter++) {
		CVertex *pV = *viter;
		pV->point() = pV->point()*1.4;
		pV->id() = pV->id() + mesh->numVertices();////得到原来的点的1.4倍的点
		MyVertex *mv = new MyVertex(pV);
		allVertex.push_back(mv);

	}
} 
void  RVD::getallcentroid() {
	CPoint p;
	for (int i = 0; i < allClipPolygon.size(); i++) {
		for (int j = 0; j < allClipPolygon[i].size(); j++) {
			p= allClipPolygon[i][j].getcentroid();//得到所有切分后的多边形重心
			allClipPolygon[i][j].nearest1 = p;
			/*allClipPolygon[i][j].nearest_to_centroid->point()[0] = p[0];
			allClipPolygon[i][j].nearest_to_centroid->point()[1] = p[1];
			allClipPolygon[i][j].nearest_to_centroid->point()[2] = p[2];*/
		}
	}
}
void RVD::getAllFace(CMyMesh *mesh) {
	for (CMyMesh::MeshFaceIterator fiter(mesh); !fiter.end(); fiter++) {
		CFace *pF = *fiter;
		allface.push_back(pF);//得到所有面
	}
}
void RVD::getallVertexK_nearest() {
	tree.buildKdtree(tree.root, allVertex);//建立kd树
	vector<kdnode<MyVertex>*> a;
	vector<MyVertex*> b;
	for (int i = 0; i < allVertex.size(); i++) {
		/*if (i == 636) {
			cout << i << endl;
			getchar();
		}*/
			
		a=tree.findK_nearestNeighbor(tree.root, allVertex[i], 15);//寻找点allVertex[i]的15个最近点
		for (int j = 0; j < a.size(); j++) {
			
			b.push_back(a[j]->getpos());
		}
		/*if (i == 111) {
			getchar();
		}*/
		allVertex[i]->k_nearest = b;
		b.clear();
	}
	
}

bool isin1(vector<MyVertex*> &a, MyVertex * b) {
	for (int i = 0; i < a.size();i++) {
		if (a[i]->id() == b->id()) {
			return true;
		}
	}
	return false;
}
bool cmp(MyVertex  *a, MyVertex *b) {
	return a->dist < b->dist;
}
void RVD::getpair() {
	for (int i = 0; i < allVertex.size();i++) {
		/*for (set<MyVertex *>::iterator j = allVertex[i]->k_nearest.begin(); j != allVertex[i]->k_nearest.end(); j++) {
			(*j)->k_nearest.insert(allVertex[i]);
		}*/
		for (int j = 0; j < allVertex[i]->k_nearest.size();j++) {
			if (!isin1(allVertex[i]->k_nearest[j]->k_nearest, allVertex[i])) {
				allVertex[i]->k_nearest[j]->k_nearest.push_back(allVertex[i]);
			}
		}
	}
	/*for (int i = 0; i < allVertex.size(); i++) {
		sort(allVertex[i]->k_nearest.begin(), allVertex[i]->k_nearest.end(), cmp);
	}*/

}



void RVD::getAllInitPolygon() {
	//vector<MyPolygon> Vmpg;
	for (int i = 0; i < allface.size(); i++) {
		
		vector<MyVertex1> a;
		MyVertex1 mv1,mv2,mv3;//得到一个面的三个点，这三个点初始化多边形
			mv1.point() = allface[i]->halfedge()->target()->point();
			mv1.id() = allface[i]->halfedge()->target()->id();
			mv2.point() = allface[i]->halfedge()->source()->point();
			mv2.id() = allface[i]->halfedge()->source()->id();
			mv3.point()= allface[i]->halfedge()->he_next()->target()->point();
			mv3.id() = allface[i]->halfedge()->he_next()->target()->id();
			a.push_back(mv1);
			a.push_back(mv2);
			a.push_back(mv3);
			MyPolygon mpg(a);
			CPoint p = mpg.getcentroid();//得到初始面的重心
			
			CVertex *a1 = new CVertex();
			a1->point() = p;
			MyVertex *mv=new MyVertex(a1);
			vector<kdnode<MyVertex>*> node=tree.findK_nearestNeighbor(tree.root, mv,1);//得到距离初始重心最近的点
			mpg.nearest_to_centroid = node[0]->getpos();
			allPolygon.push_back(mpg);
	}
}

 
void RVD::RVDAlgorithm(CMyMesh *mesh, CMyMesh *mesh1) {
	getAllVertex(mesh);
	getAllFace(mesh1);
	getallVertexK_nearest();
	getpair();
	getAllInitPolygon();
	
}
void RVD::display() {
	glLineWidth(2.0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < allClipPolygon.size(); i++) {
		for (int j = 0; j < allClipPolygon[i].size(); j++) {
			for (int k = 0; k < allClipPolygon[i][j].polygon.size(); k++) {
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(allClipPolygon[i][j].polygon[k].point()[0], allClipPolygon[i][j].polygon[k].point()[1], allClipPolygon[i][j].polygon[k].point()[2]);
				glVertex3f(allClipPolygon[i][j].polygon[(k+1)% allClipPolygon[i][j].polygon.size()].point()[0], allClipPolygon[i][j].polygon[(k + 1) % allClipPolygon[i][j].polygon.size()].point()[1], allClipPolygon[i][j].polygon[(k + 1) % allClipPolygon[i][j].polygon.size()].point()[2]);
			}
		}
	}
	glEnd();
	glLineWidth(1.0);
}

