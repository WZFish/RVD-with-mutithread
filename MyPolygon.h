/*多边形类，按一定顺序存储一些点，表示逆时针方向多边形的顶点
顶点具有x,y,z,w,s五个属性*/
#pragma once
#include"MyMesh.h"
#include"MyVertex.h"
#include<Eigen/Dense>
#include"MyVertex1.h"

using namespace Eigen;

class Pair {
public:
	int a, b;
	Pair(int aa, int bb):a(aa),b(bb) {}
};
class MyPolygon {
public:
	
	MyVertex *nearest_to_centroid;//点集中离中心最近的点。//包含k个最近的点
	CPoint nearest1;//重心
	vector<MyVertex1 > polygon;//这里面的点有x,y,z,w,s 5个属性，但没有最邻近；
	CPoint getcentroid();//得到多边形重心
	Vector4d getbisector(MyVertex *x, MyVertex *y);//得到两个点的垂直平分面方程
	bool  intersection(MyVertex *x, MyVertex *y);//两个点切割平面
	void clip();//整体切割函数
	int getverticesNum();//得到多边形顶点数
	MyPolygon(vector<MyVertex1 > a) :polygon(a) { nearest_to_centroid = new MyVertex(); }
	bool isin(vector<CPoint> &ans,CPoint &ins);//ans中是否有ins，用于去重
	void printPolygon();
	
};
double getdist(CPoint p1, CPoint p2) {//求两点距离
	double dist = sqrt((p1[0] - p2[0])*(p1[0] - p2[0]) + (p1[1] - p2[1])*(p1[1] - p2[1]) + (p1[2] - p2[2])*(p1[2] - p2[2]));
	return dist;
}
void MyPolygon::printPolygon() {
	for (int i = 0; i < polygon.size(); i++) {
		cout << polygon[i].point() << endl;
	}
}
bool MyPolygon::isin(vector<CPoint> &ans,CPoint &ins) {
	for (int i = 0; i < ans.size(); i++) {
		if (ans[i] == ins) {
			return true;
		}
	}
	return false;
}
int MyPolygon::getverticesNum() {
	return polygon.size();
}
CPoint MyPolygon::getcentroid() {
	CPoint sum(0, 0, 0);
	for (int i = 0; i < getverticesNum(); i++) {
		sum += polygon[i].point();
	}
	return sum / getverticesNum();
}
Vector4d MyPolygon::getbisector(MyVertex *x, MyVertex *y) {
	CPoint mid = (x->point() + y->point()) / 2;
	Vector4d ans = Vector4d::Zero();
	ans(0) = x->point()[0] - y->point()[0];//ans[0],1,2表示垂直平分面的法向量，从y指向x
	ans(1) = x->point()[1] - y->point()[1];
	ans(2) = x->point()[2] - y->point()[2];
	ans(3) = 0 - ans(0)*mid[0] - ans(1)*mid[1] - ans(2)*mid[2];
	return ans;
}
bool  MyPolygon::intersection(MyVertex *x, MyVertex *y) {
	bool iscut = 0;
	Vector4d v = getbisector(x, y);
	bool flag = 0;//插入点的特殊情况
	vector<Pair> index;
	CPoint tempv(v[0], v[1], v[2]);
	CPoint facePoint,directVector,linePoint;//facepoint是被中垂面上一点，directVector是多边形边的方向向量，linePoint是多边形边上一点
	if (v[0] && v[1] && v[2] || v[2])
	{
		facePoint[0] = 0;
		facePoint[1] = 0;
		facePoint[2] = -v[3] / v[2];
		
	}
	else if (v[0])
	{
		facePoint[0] = -v[3] / v[0];
		facePoint[1] = 0;
		facePoint[2] = 0;
		
	}
	else if (v[1])
	{
		facePoint[0] = 0;
		facePoint[1] = -v[3] / v[1];
		facePoint[2] = 0;
	}
	vector<CPoint> ins, ans;
	for (int i = 0; i < getverticesNum(); i++) {
		linePoint = polygon[i].point();
		directVector = polygon[i].point() - polygon[(i + 1) % getverticesNum()].point();
		double vpt = tempv*directVector;//方向向量与法向量乘积
		if (!vpt) {//积为0表示平行不切割
			continue;//平行，与该边无交点
		}
		double temp =( (facePoint - linePoint)*tempv)/vpt;
		CPoint q = linePoint + directVector*temp;//切割点
		//ins.push_back(q);
		if (min(polygon[i].point()[0], polygon[(i + 1) % getverticesNum()].point()[0]) <= q[0] && q[0]<= max(polygon[i].point()[0], polygon[(i + 1) % getverticesNum()].point()[0])
			&& min(polygon[i].point()[1], polygon[(i + 1) % getverticesNum()].point()[1]) <= q[1] && q[1] <= max(polygon[i].point()[1], polygon[(i + 1) % getverticesNum()].point()[1])
			&& min(polygon[i].point()[2], polygon[(i + 1) % getverticesNum()].point()[2]) <= q[2] && q[2] <= max(polygon[i].point()[2], polygon[(i + 1) % getverticesNum()].point()[2]))
		{//切割点在线段内
			if (!isin(ans, q)) {//未重复
				//cout << "cut success" << endl;
				iscut = 1;
				ans.push_back(q);//加入答案
				Pair p1(i, (i + 1) % getverticesNum());//记录与哪两个点形成的边相交
				if (i > (i + 1) % getverticesNum()) {//是否与最后一条边相交，相交flag=1；
					flag = 1;
				}
				index.push_back(p1);
			}
			
		}

		
	}
	if (ans.size() > 1) {//交点多余1个进行处理，否则不处理
		swap(index[1].a, index[1].b);//交换第二个交点所对应的边的点对，这样index.b之差表示垂直平分面一边的点数，总数-一边点数为另一边点数
		double a, b;//最邻近重心的点在法线方向哪一边>0表示法向一侧，=0表示在面上，<0表示在法线反向。
		int aaa, bbb;
		a = polygon[index[0].a].point()[0] * v[0] + polygon[index[0].a].point()[1] * v[1] + polygon[index[0].a].point()[2] * v[2] + v[3];
		b= polygon[index[0].b].point()[0] * v[0] + polygon[index[0].b].point()[1] * v[1] + polygon[index[0].b].point()[2] * v[2] + v[3];
		//a = getdist(polygon[index[0].a].point(),nearest_to_centroid.point());//哪边离中心更近
		//b= getdist(polygon[index[0].b].point(), nearest_to_centroid.point());
		//if (a == b) {
		//	a = getdist(polygon[index[1].a].point(), nearest_to_centroid.point());//哪边离中心更近
		//	b = getdist(polygon[index[1].b].point(), nearest_to_centroid.point());
		//}
		//aaa = abs(index[0].a - index[1].a) + 1;//两边各有几个点
		bbb = abs(index[0].b - index[1].b) + 1;//一面的点数
		aaa = polygon.size() - bbb;//另一边点数
		/*if (a < 0) {
			a = a + getverticesNum();
		}
		if (b < 0) {
			b += getverticesNum();
		}*/
		if (a <0) {//右边近
			int a1 = min(index[0].a, index[1].a);//左边的坐标
			int a2 = max(index[0].a, index[1].a);
			if (aaa > 1) {//左边点比一个多
				if (index[0].a>index[1].a) {//穿过最后一个点和第一个点之间的边
					polygon[a1].point() = ans[1];
					polygon[a2].point() = ans[0];
					polygon[a1].sets(y);
					polygon[a2].sets(y);
					if (aaa == abs(index[0].a - index[1].a) + 1) {//移除多余点
						for (int i = 0; i < aaa-2; i++) {
							polygon.erase(polygon.begin() + a1+1);
						}
					}
					else {
						for (int i = 0; i <aaa-2; i++) {
							polygon.erase(polygon.begin() + a2+1);
						}
					}
					
				}
				else {
					polygon[a1].point() = ans[0];
					polygon[a2].point() = ans[1];
					polygon[a1].sets(y);
					polygon[a2].sets(y);
					if (aaa == abs(index[0].a - index[1].a) + 1) {
						for (int i = 0; i < aaa - 2; i++) {
							polygon.erase(polygon.begin() + a1 + 1);
						}
					}
					else {
						for (int i = 0; i <aaa - 2; i++) {
							polygon.erase(polygon.begin() + ((a2 + 1)% polygon.size()));
						}
					}
				}

			}
			else {//左边就一个点
				polygon[a1].point() = ans[0];
				polygon[a1].sets(y);
				MyVertex1 aa ;
				aa.point() = ans[1];
				aa.sets(y);
				if (!flag) {
					polygon.insert(polygon.begin() + a1 + 1, aa);//插入新点
				}
				else {
					polygon.insert(polygon.end(), aa);
				}

			}

		}
		else if(b<0) {//左边近留左边
			int b1 = min(index[0].b, index[1].b);
			int b2 = max(index[0].b, index[1].b);
			if (bbb > 1) {
				if (index[0].b>index[1].b) {
					
					polygon[b1].point() = ans[1];
					polygon[b2].point() = ans[0];
					polygon[b1].sets(y);
					polygon[b2].sets(y);
					for (int i = b1 + 1; i < bbb-2; i++) {
						polygon.erase(polygon.begin() + b1+1);
					}
				}
				else {
					polygon[b1].point() = ans[0];
					polygon[b2].point() = ans[1];
					polygon[b1].sets(y);
					polygon[b2].sets(y);
					for (int i = b1 + 1; i < bbb-2; i++) {
						polygon.erase(polygon.begin() + b1+1);
					}
				}
			}
			else {
				polygon[b1].point() = ans[0];
				polygon[b1].sets(y);
				MyVertex1 aa ;
				aa.point() = ans[1];
				aa.sets(y);
				if (!flag) {
					polygon.insert(polygon.begin() + b1 + 1, aa);
				}
				else {
					polygon.insert(polygon.end(), aa);
				}
			}
		}
	}
	
	
	return iscut;
}
bool isin2(vector<int> &v, int t) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == t) {
			return true;
		}
	}
	return false;
}
void MyPolygon::clip() {
	//for (set<MyVertex *>::iterator i = nearest_to_centroid->k_nearest.begin(); i != nearest_to_centroid->k_nearest.end(); i++) {
	for (int i = 0;  i < nearest_to_centroid->k_nearest.size(); i++) {
		//if(!nearest_to_centroid.k_nearest[i]->visit)
		bool iscut;
		iscut=intersection(nearest_to_centroid, nearest_to_centroid->k_nearest[i]);//离重心最近的点依次与其最邻近的k个点切割
		//if (iscut&&!isin2(visit, nearest_to_centroid->k_nearest[i]->id())) {
		//	/*S.push(nearest_to_centroid->k_nearest[i]);
		//	visit.push_back(nearest_to_centroid->k_nearest[i]->id());*/
		//}
	}

}