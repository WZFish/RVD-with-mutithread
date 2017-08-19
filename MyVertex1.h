#pragma once
#include<iostream>
#include"MyMesh.h"
using namespace MeshLib;
class MyVertex1 {
	CPoint p;//x,y,z Ù–‘
	double w;//w Ù–‘
	MyVertex *s;//s Ù–‘
	int Id;//id
public:
	double getw() { return w; }
	int &id() { return Id; }
	
	void setw(double w1) { w = w1; }
	MyVertex *gets() { return s; }
	void sets(MyVertex *s1) { s = s1; }
	CPoint &point() { return p; }
	MyVertex1() { s=NULL; w = 1; }
};