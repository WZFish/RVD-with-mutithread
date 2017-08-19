#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL\glut.h>
#include "viewer\Arcball.h"                           /*  Arc Ball  Interface         */
#include<iostream>
#include"kd_node.h"
#include"kd_tree.h"
#include"MyPolygon.h"
#include"RVD.h";
#include<vector>
#include "MyMesh.h"

#define PI 3.141592653
mutex Mutex;
using namespace MeshLib;
using namespace std;
/* window width and height */
int win_width, win_height;
int gButton;
int startx, starty;
int shadeFlag = 0;


/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);

/* global mesh */
CMyMesh mesh;
RVD r;
/* arcball object */
CArcball arcball;

int textureFlag = 2;
/* texture id and image */
GLuint texName;
//RgbImage image;


/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void)
{
	double rot[16];

	glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
	ObjRot.convert(rot);
	glMultMatrixd((GLdouble *)rot);
}

/*! the eye is always fixed at world z = +5 */
void setupEye(void) {
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

/*! setup light */
void setupLight()
{
	GLfloat lightOnePosition[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! draw axis */
void draw_axis()
{
	glLineWidth(2.0);
	//x axis
	glColor3f(1.0, 0.0, 0.0);	//red
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();

	//y axis
	glColor3f(0.0, 1.0, 0);		//green
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	//z axis
	glColor3f(0.0, 0.0, 1.0);	//blue
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();

	glLineWidth(1.0);
}

/*! draw mesh */
void draw_mesh()
{
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_TRIANGLES);
	for (CMyMesh::MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		CMyFace * pf = *fiter;
		for (CMyMesh::FaceVertexIterator fviter(pf); !fviter.end(); ++fviter)
		{
			CMyVertex * v = *fviter;
			CPoint & pt = v->point();
			CPoint2 & uv = v->uv();
			CPoint & rgb = v->rgb();
			CPoint n;
			switch (shadeFlag)
			{
			case 0:
				n = pf->normal();
				break;
			case 1:
				n = v->normal();
				break;
			}
			glNormal3d(n[0], n[1], n[2]);
			glTexCoord2d(uv[0], uv[1]);
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex3d(pt[0], pt[1], pt[2]);
		}
	}
	glEnd();
}
void draw() {
	glLineWidth(2.0);
	glColor3f(1, 0, 0);
	glPointSize(4.0f);
	glBegin(GL_LINES);
	/*double a[4][3] = { {-0.3993,0.49575,-0.76952 },{-0.3982,0.55292,-0.72654},{-0.5089,0.52196,-0.68246},{-0.51531,0.517,-0.68238} };
	for (int i = 0; i < 4; i++) {
		glVertex3f(a[i][0],a[i][1],a[i][2]);
		glVertex3f(a[(i+1)%4][0], a[(i + 1) % 4][1], a[(i + 1) % 4][2]);
	}*/
	//glBegin(GL_POINTS);
	/*glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 0, 0);*/
	//for (int k = 0; k < r.allClipPolygon[0][1].polygon.size(); k++) {
	//	//glColor3f(1.0f, 0.0f, 0.0f);
	//	cout << r.allClipPolygon[0][2].polygon[k].point()<<endl;
	//	//cout << r.allClipPolygon[0][0].polygon[k].point() << endl;
	//	//glVertex3f(r.allClipPolygon[0][2].polygon[k].point()[0], r.allClipPolygon[0][2].polygon[k].point()[1], r.allClipPolygon[0][2].polygon[k].point()[2]);
	//	//glVertex3f(r.allClipPolygon[0][2].polygon[(k + 1) % r.allClipPolygon[0][2].polygon.size()].point()[0], r.allClipPolygon[0][2].polygon[(k + 1) % r.allClipPolygon[0][2].polygon.size()].point()[1], r.allClipPolygon[0][2].polygon[(k + 1) % r.allClipPolygon[0][2].polygon.size()].point()[2]);
	//	//必须是加上s，要不然显示不了
	//	glVertex3f(r.allClipPolygon[0][3].polygon[k].point()[0], r.allClipPolygon[0][3].polygon[k].point()[1], r.allClipPolygon[0][3].polygon[k].point()[2]);
	//	glVertex3f(r.allClipPolygon[0][3].polygon[(k + 1) % r.allClipPolygon[0][3].polygon.size()].point()[0], r.allClipPolygon[0][3].polygon[(k + 1) % r.allClipPolygon[0][3].polygon.size()].point()[1], r.allClipPolygon[0][3].polygon[(k + 1) % r.allClipPolygon[0][3].polygon.size()].point()[2]);

	//}
	for (int i = 0; i < r.allClipPolygon.size(); i++) {
		for (int j =0; j <r.allClipPolygon[i].size(); j++) {
			for (int k = 0; k < r.allClipPolygon[i][j].polygon.size(); k++) {
				glVertex3f(r.allClipPolygon[i][j].polygon[k].point()[0], r.allClipPolygon[i][j].polygon[k].point()[1], r.allClipPolygon[i][j].polygon[k].point()[2]);
				glVertex3f(r.allClipPolygon[i][j].polygon[(k + 1) % r.allClipPolygon[i][j].polygon.size()].point()[0], r.allClipPolygon[i][j].polygon[(k + 1) % r.allClipPolygon[i][j].polygon.size()].point()[1], r.allClipPolygon[i][j].polygon[(k + 1) % r.allClipPolygon[i][j].polygon.size()].point()[2]);
			}
		}
	}
	/*for (int i = 0; i < r.allClipPolygon.size(); i++) {
	for (int j = 0; j < r.allClipPolygon[i].size(); j++) {
	for (int k = 0; k < r.allClipPolygon[i][j].polygon.size(); k++) {
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(r.allClipPolygon[i][j].polygon[k].point()[0], r.allClipPolygon[i][j].polygon[k].point()[1], r.allClipPolygon[i][j].polygon[k].point()[2]);
	glVertex3f(r.allClipPolygon[i][j].polygon[(k + 1) % r.allClipPolygon[i][j].polygon.size()].point()[0], r.allClipPolygon[i][j].polygon[(k + 1) %r.allClipPolygon[i][j].polygon.size()].point()[1], r.allClipPolygon[i][j].polygon[(k + 1) % r.allClipPolygon[i][j].polygon.size()].point()[2]);
	}
	}
	}*/
	//r.allClipPolygon[i].size()
	glEnd();
	glBegin(GL_POINTS);
	for (int i = 0; i < r.allClipPolygon.size(); i++) {
		for (int j = 0; j <r.allClipPolygon[i].size(); j++) {
			for (int k = 0; k < r.allClipPolygon[i][j].polygon.size(); k++) {
				glVertex3f(r.allClipPolygon[i][j].nearest1[0], r.allClipPolygon[i][j].nearest1[1], r.allClipPolygon[i][j].nearest1[2]);
			}
		}
	}
	
	glEnd();
	glFlush();



}
void draw_sharp_edges()
{
	glLineWidth(2.0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	for (CMyMesh::MeshEdgeIterator eiter(&mesh); !eiter.end(); ++eiter)
	{
		CMyEdge * pE = *eiter;
		if (pE->sharp() == true)
		{
			CMyVertex * p0 = mesh.edgeVertex1(pE);
			CMyVertex * p1 = mesh.edgeVertex2(pE);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(p0->point()[0], p0->point()[1], p0->point()[2]);
			glVertex3f(p1->point()[0], p1->point()[1], p1->point()[2]);
		}
	}
	glEnd();
	glLineWidth(1.0);
}

/*! display call back function
*/
void display()
{
	/* clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLight();
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();
	draw();
	/* draw sharp edges */
	//draw_sharp_edges();
	///* draw the mesh */
	//draw_mesh();
	///* draw the axis */
	//draw_axis();*/

	glPopMatrix();
	glutSwapBuffers();
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h)
{
	float ar;
	//std::cout << "w:" << w << "\th:" << h << std::endl;
	win_width = w;
	win_height = h;

	ar = (float)(w) / h;
	glViewport(0, 0, w, h);               /* Set Viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// magic imageing commands
	gluPerspective(40.0, /* field of view in degrees */
		ar, /* aspect ratio */
		1.0, /* Z near */
		100.0 /* Z far */);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/*! helper function to remind the user about commands, hot keys */
void help()
{
	printf("w  -  Wireframe Display\n");
	printf("f  -  Flat Shading \n");
	printf("s  -  Smooth Shading\n");
	printf("?  -  Help Information\n");
	printf("esc - quit\n");
}

/*! Keyboard call back function */
void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 0;
		break;
	case 's':
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 1;
		break;
	case 'w':
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 't':
		textureFlag = (textureFlag + 1) % 3;
		switch (textureFlag)
		{
		case 0:
			glDisable(GL_TEXTURE_2D);
			break;
		case 1:
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			break;
		case 2:
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		}
		break;
	case '?':
		help();
		break;
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

/*! setup GL states */
void setupGLstate() {
	GLfloat lightOneColor[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat globalAmb[] = { .1, .1, .1, 1 };
	GLfloat lightOnePosition[] = { .0, 0.0, 1.0, 1.0 };

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.35, 0.53, 0.70, 0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightOneColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! mouse click call back function */
void  mouseClick(int button, int state, int x, int y) {
	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gButton = GLUT_LEFT_BUTTON;
		arcball = CArcball(win_width, win_height, x - win_width / 2, win_height - y - win_height / 2);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_MIDDLE_BUTTON;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_RIGHT_BUTTON;
	}
	return;
}

/*! mouse motion call back function */
void mouseMove(int x, int y)
{
	CPoint trans;
	CQrot  rot;

	/* rotation, call arcball */
	if (gButton == GLUT_LEFT_BUTTON)
	{
		rot = arcball.update(x - win_width / 2, win_height - y - win_height / 2);
		ObjRot = rot * ObjRot;
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON)
	{
		double scale = 10. / win_height;
		trans = CPoint(scale*(x - startx), scale*(starty - y), 0);
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton == GLUT_RIGHT_BUTTON) {
		double scale = 10. / win_height;
		trans = CPoint(0, 0, scale*(starty - y));
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

}


/*! Normalize mesh
* \param pMesh the input mesh
*/
void normalize_mesh(CMyMesh * pMesh)
{
	CPoint s(0, 0, 0);
	for (CMyMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		s = s + v->point();
	}
	s = s / pMesh->numVertices();

	for (CMyMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		p = p - s;
		v->point() = p;
	}

	double d = 0;
	for (CMyMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		for (int k = 0; k < 3; k++)
		{
			d = (d > fabs(p[k])) ? d : fabs(p[k]);
		}
	}

	for (CMyMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		p = p / d;
		v->point() = p;
	}
};

/*! Compute the face normal and vertex normal
* \param pMesh the input mesh
*/
void compute_normal(CMyMesh * pMesh)
{
	for (CMyMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint n(0, 0, 0);
		for (CMyMesh::VertexFaceIterator vfiter(v); !vfiter.end(); ++vfiter)
		{
			CMyFace * pF = *vfiter;

			CPoint p[3];
			CHalfEdge * he = pF->halfedge();
			for (int k = 0; k < 3; k++)
			{
				p[k] = he->target()->point();
				he = he->he_next();
			}

			CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
			pF->normal() = fn / fn.norm();
			n += fn;
		}

		n = n / n.norm();
		v->normal() = n;
	}
};


void initGL(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
	glutCreateWindow("A_Line");
	glutInitWindowSize(1800, 1600);
	//glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 1800, 1600);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	getchar();
}
void init_openGL(int argc, char * argv[])
{
	/* glut stuff */
	glutInit(&argc, argv);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 800, 600);

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	setupGLstate();

	glutMainLoop();                       /* Start GLUT event-processing loop */
}

/*! main function for viewer
*/

double CalculateAngle(CPoint a, CPoint b) {
	double ab = a*b; //计算点积
	double a2 = a.norm();//计算范数
	double b2 = b.norm();
	double Cosine = ab / (a2*b2);//余弦公式
	double arccosine = acos(Cosine);//计算角度
	return arccosine;
}
void clipAllPolygon(int Begin, int End, RVD *r,int j) {//切分从begin到End的初始面
	cout << j << "is running" << endl;
	vector<MyPolygon> afterClip;//保存切过的多边形
	stack<MyVertex*>S;
	vector<int> visit;
	//vector<MyVertex*> newnear;
	/*for (int i = 0; i < r->allPolygon.size(); i++) {
		r->allPolygon[i].clip();
		r->allPolygon[i].nearest_to_centroid->point() = r->allPolygon[i].getcentroid();
		afterClip.push_back(r->allPolygon[i]);
	}
	r->allClipPolygon.push_back(afterClip);*/
	for (int i = Begin; i < End; i++) {
		visit.clear();
		//newnear.clear();
		MyPolygon mpg = r->allPolygon[i];
		
		/*if (i == 844) {
			getchar();
		}
		cout << i << endl;*/
		/*cout << mpg.polygon[0].id() << endl;
		cout << mpg.polygon[1].id() << endl;
		cout << mpg.polygon[2].id() << endl;
		cout << mpg.nearest_to_centroid.id() << endl;*/
		/*if (mpg.nearest_to_centroid.id() == 394) {
			getchar();
		}*/
		S.push(mpg.nearest_to_centroid);
		visit.push_back(mpg.nearest_to_centroid->id());
		//getchar();
		while (!S.empty()) {
			MyVertex *a = S.top();  
			S.pop();
			//visit.push_back(a->id());
			mpg.nearest_to_centroid = a;
			//sort(mpg.nearest_to_centroid->k_nearest.begin(), mpg.nearest_to_centroid->k_nearest.end(), cmp);
			//cout <<afterClip.size()<<":"<< "centroid is " << mpg.nearest_to_centroid->id() << "  ";
			//mpg.nearest_to_centroid->visit = true;
			/*if (afterClip.size() > 55) {
				cout << afterClip[55].polygon[2].point() << "   " << afterClip[55].polygon[3].point() << endl;
				getchar();
			}*/
			/*if (afterClip.size() == 55) {
				getchar();
			}*/
			/*if (afterClip.size() == 4) {
				getchar();
			}*/
			
			mpg.clip();//对当前面切分
			/*for (int i = 0; i < mpg.polygon.size(); i++) {
				if (mpg.polygon[i].gets() != NULL) {
					cout << "cliped with:" << mpg.polygon[i].gets()->id() << " ";
				}
			}*/
			/*cout << endl;*/
			//getchar();
			afterClip.push_back(mpg);
			for (int i = 0; i < mpg.polygon.size(); i++) {	
				if (mpg.polygon[i].gets() != NULL && /*!mpg.polygon[i].gets()->visit*/!isin2(visit, mpg.polygon[i].gets()->id())) {//遍历切分过的多边形所有顶点，把切除的点的s属性去重压栈
					//mpg.polygon[i].gets()->k_nearest.insert(a);
					S.push(mpg.polygon[i].gets());
					visit.push_back(mpg.polygon[i].gets()->id());
					//newnear.push_back(mpg.polygon[i].gets());
					//mpg.polygon[i].gets().visit = true;
				}
			}
			/*for (int i = 0; i < newnear.size(); i++) {
				for (int j = 0; j < newnear.size(); j++) {
					if (i != j) {
						newnear[i]->k_nearest.insert(newnear[j]);
					}
				}
			}*/
			mpg = r->allPolygon[i];//下一个点仍对原三角面片切分
		}
		/*for (int i = 0; i < afterClip.size(); i++) {
			afterClip[i].nearest_to_centroid->point() = afterClip[i].getcentroid();
		}*/

	}
	unique_lock<mutex> lock(Mutex);
	r->allClipPolygon.push_back(afterClip);//保存本线程处理的多边形
	lock.unlock();
}
int main(int argc, char * argv[])
{
	/*if (argc != 2)
	{
		std::cout << "Usage: input.m" << std::endl;
		return -1;
	}*/

	std::string mesh_name("C:\\Users\\84443\\Documents\\Visual Studio 2015\\Projects\\kd-tree\\Data\\sphere.m");
	if (strutil::endsWith(mesh_name, ".obj"))
	{
		mesh.read_obj(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".m"))
	{
		mesh.read_m(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".off"))
	{
		mesh.read_off(mesh_name.c_str());
	}
	CMyMesh mesh1;
	if (strutil::endsWith(mesh_name, ".obj"))
	{
		mesh1.read_obj(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".m"))
	{
		mesh1.read_m(mesh_name.c_str());
	}
	if (strutil::endsWith(mesh_name, ".off"))
	{
		mesh1.read_off(mesh_name.c_str());
	}
	
	vector<thread*> threadpool;
	thread t[1000];
	
	r.RVDAlgorithm(&mesh,&mesh1);
	//clipAllPolygon(0, r.allPolygon.size(), &r,1);
	

	int i,j;
	for (i = 0,j=0; i < r.allPolygon.size()-100; i = i + 100,j++) {//每个线程计算100个面
		t[j] = thread(clipAllPolygon, i, i + 100,&r,j);
		//threadpool.push_back(&t[]);
	}
	
	for (int i = 0; i < j; i++) {//执行线程
		t[i].join();
	}
	
	//normalize_mesh(&mesh);
	//compute_normal(&mesh);
	
	/*mesh.output_mesh_info();
	mesh.test_iterator();*/
	//initGL(argc, argv);
	r.getallcentroid();//得到新的重心
	init_openGL(argc, argv);
	
	return 0;
}
