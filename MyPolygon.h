/*������࣬��һ��˳��洢һЩ�㣬��ʾ��ʱ�뷽�����εĶ���
�������x,y,z,w,s�������*/
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
	
	MyVertex *nearest_to_centroid;//�㼯������������ĵ㡣//����k������ĵ�
	CPoint nearest1;//����
	vector<MyVertex1 > polygon;//������ĵ���x,y,z,w,s 5�����ԣ���û�����ڽ���
	CPoint getcentroid();//�õ����������
	Vector4d getbisector(MyVertex *x, MyVertex *y);//�õ�������Ĵ�ֱƽ���淽��
	bool  intersection(MyVertex *x, MyVertex *y);//�������и�ƽ��
	void clip();//�����и��
	int getverticesNum();//�õ�����ζ�����
	MyPolygon(vector<MyVertex1 > a) :polygon(a) { nearest_to_centroid = new MyVertex(); }
	bool isin(vector<CPoint> &ans,CPoint &ins);//ans���Ƿ���ins������ȥ��
	void printPolygon();
	
};
double getdist(CPoint p1, CPoint p2) {//���������
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
	ans(0) = x->point()[0] - y->point()[0];//ans[0],1,2��ʾ��ֱƽ����ķ���������yָ��x
	ans(1) = x->point()[1] - y->point()[1];
	ans(2) = x->point()[2] - y->point()[2];
	ans(3) = 0 - ans(0)*mid[0] - ans(1)*mid[1] - ans(2)*mid[2];
	return ans;
}
bool  MyPolygon::intersection(MyVertex *x, MyVertex *y) {
	bool iscut = 0;
	Vector4d v = getbisector(x, y);
	bool flag = 0;//�������������
	vector<Pair> index;
	CPoint tempv(v[0], v[1], v[2]);
	CPoint facePoint,directVector,linePoint;//facepoint�Ǳ��д�����һ�㣬directVector�Ƕ���αߵķ���������linePoint�Ƕ���α���һ��
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
		double vpt = tempv*directVector;//���������뷨�����˻�
		if (!vpt) {//��Ϊ0��ʾƽ�в��и�
			continue;//ƽ�У���ñ��޽���
		}
		double temp =( (facePoint - linePoint)*tempv)/vpt;
		CPoint q = linePoint + directVector*temp;//�и��
		//ins.push_back(q);
		if (min(polygon[i].point()[0], polygon[(i + 1) % getverticesNum()].point()[0]) <= q[0] && q[0]<= max(polygon[i].point()[0], polygon[(i + 1) % getverticesNum()].point()[0])
			&& min(polygon[i].point()[1], polygon[(i + 1) % getverticesNum()].point()[1]) <= q[1] && q[1] <= max(polygon[i].point()[1], polygon[(i + 1) % getverticesNum()].point()[1])
			&& min(polygon[i].point()[2], polygon[(i + 1) % getverticesNum()].point()[2]) <= q[2] && q[2] <= max(polygon[i].point()[2], polygon[(i + 1) % getverticesNum()].point()[2]))
		{//�и�����߶���
			if (!isin(ans, q)) {//δ�ظ�
				//cout << "cut success" << endl;
				iscut = 1;
				ans.push_back(q);//�����
				Pair p1(i, (i + 1) % getverticesNum());//��¼�����������γɵı��ཻ
				if (i > (i + 1) % getverticesNum()) {//�Ƿ������һ�����ཻ���ཻflag=1��
					flag = 1;
				}
				index.push_back(p1);
			}
			
		}

		
	}
	if (ans.size() > 1) {//�������1�����д������򲻴���
		swap(index[1].a, index[1].b);//�����ڶ�����������Ӧ�ıߵĵ�ԣ�����index.b֮���ʾ��ֱƽ����һ�ߵĵ���������-һ�ߵ���Ϊ��һ�ߵ���
		double a, b;//���ڽ����ĵĵ��ڷ��߷�����һ��>0��ʾ����һ�࣬=0��ʾ�����ϣ�<0��ʾ�ڷ��߷���
		int aaa, bbb;
		a = polygon[index[0].a].point()[0] * v[0] + polygon[index[0].a].point()[1] * v[1] + polygon[index[0].a].point()[2] * v[2] + v[3];
		b= polygon[index[0].b].point()[0] * v[0] + polygon[index[0].b].point()[1] * v[1] + polygon[index[0].b].point()[2] * v[2] + v[3];
		//a = getdist(polygon[index[0].a].point(),nearest_to_centroid.point());//�ı������ĸ���
		//b= getdist(polygon[index[0].b].point(), nearest_to_centroid.point());
		//if (a == b) {
		//	a = getdist(polygon[index[1].a].point(), nearest_to_centroid.point());//�ı������ĸ���
		//	b = getdist(polygon[index[1].b].point(), nearest_to_centroid.point());
		//}
		//aaa = abs(index[0].a - index[1].a) + 1;//���߸��м�����
		bbb = abs(index[0].b - index[1].b) + 1;//һ��ĵ���
		aaa = polygon.size() - bbb;//��һ�ߵ���
		/*if (a < 0) {
			a = a + getverticesNum();
		}
		if (b < 0) {
			b += getverticesNum();
		}*/
		if (a <0) {//�ұ߽�
			int a1 = min(index[0].a, index[1].a);//��ߵ�����
			int a2 = max(index[0].a, index[1].a);
			if (aaa > 1) {//��ߵ��һ����
				if (index[0].a>index[1].a) {//�������һ����͵�һ����֮��ı�
					polygon[a1].point() = ans[1];
					polygon[a2].point() = ans[0];
					polygon[a1].sets(y);
					polygon[a2].sets(y);
					if (aaa == abs(index[0].a - index[1].a) + 1) {//�Ƴ������
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
			else {//��߾�һ����
				polygon[a1].point() = ans[0];
				polygon[a1].sets(y);
				MyVertex1 aa ;
				aa.point() = ans[1];
				aa.sets(y);
				if (!flag) {
					polygon.insert(polygon.begin() + a1 + 1, aa);//�����µ�
				}
				else {
					polygon.insert(polygon.end(), aa);
				}

			}

		}
		else if(b<0) {//��߽������
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
		iscut=intersection(nearest_to_centroid, nearest_to_centroid->k_nearest[i]);//����������ĵ������������ڽ���k�����и�
		//if (iscut&&!isin2(visit, nearest_to_centroid->k_nearest[i]->id())) {
		//	/*S.push(nearest_to_centroid->k_nearest[i]);
		//	visit.push_back(nearest_to_centroid->k_nearest[i]->id());*/
		//}
	}

}