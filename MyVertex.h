#pragma once
#include<iostream>
#include<set>
#include"MyMesh.h"
using namespace std;
using namespace MeshLib;

class MyVertex :public CVertex {
public:
	//set<MyVertex *> 
	vector<MyVertex*>k_nearest;//�õ����ڽ���k����
	bool visit;
	double dist;//����
	MyVertex(CVertex * a) :CVertex(*a) { visit = false; dist = 0; }
	MyVertex() { visit = false; dist = 0; }
	
};