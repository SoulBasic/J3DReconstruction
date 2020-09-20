#pragma once
#include "RoamingScenceManager.h"

RoamingScenceManager::RoamingScenceManager()
{
	OldMouse = new Position3D();
	Mouse = new Position3D();

	NewEye = new Vector3D(1, 0, 0);
	NewUp = new Vector3D(0, 0, 1);
	NewView = new Vector3D(0, 0, 0);

	AuxY = new Vector3D(0, 1, 0);
	AuxZ = new Vector3D();
	*AuxZ = (Vector3D)*NewEye - (Vector3D)*NewView;
	AuxX = new Vector3D();
	*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
	AuxX->normalize();

	TempTranslateVec = new Vector3D(0, 0, 0);
	TempscaleFactor = 10;
}

RoamingScenceManager::~RoamingScenceManager()
{
	delete OldMouse;
	delete Mouse;
	delete NewEye;
	delete NewUp;
	delete NewView;
	delete AuxX;
	delete AuxY;
	delete AuxZ;
	delete TempTranslateVec;
}

void RoamingScenceManager::init()
{
	
	init_CoordinaryDisplayList();
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.70588f, 1.0f, 0.78431f, 0);
	//glClearDepth(1.0f);
	glPointSize(1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//材质反光性设置
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射参数
	GLfloat mat_shininess[] = { 50.0 };               //高光指数
	GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };   //灯位置(1,1,1), 最后1-开关
	GLfloat Light_Model_Ambient[] = { 0.2, 0.2, 0.2, 1.0 }; //环境光参数

	//灯光设置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);   //散射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);  //镜面反射光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //环境光参数


	glEnable(GL_LIGHT0);     //打开0#灯
	glEnable(GL_LIGHT1);

	GLfloat light_position1[4] = { 0.0f, 0.0f, 5.0f, 0 };
	GLfloat direction1[3] = { 0.0f,0.0f,-5.0f };
	GLfloat color1[4];
	color1[0] = 0.0f; color1[1] = 0.0f; color1[2] = 0.0f; color1[3] = 1.0f;
	GLfloat ambient[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat material_color[4] = { 1,1,1,0.5f };
	GLfloat material_specular[4] = { 0.5f,0.5f,0.5f,0.5f };
	GLfloat material_ambient[4] = { 0.0,0.0,0.0,0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);


	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glViewport(0, 0, 1800, 900);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	glOrtho(-20, 20, -10, 10, -10.0, 10.0);
	//glOrtho(-100,100, -100, 100, -100, 100);

}

void RoamingScenceManager::render()
{
	glClearColor(0.9019f, 0.9019f, 0.9821f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*NewView = *NewEye*-1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(TempTranslateVec->X()/40, TempTranslateVec->Y()/40, TempTranslateVec->Z()/40);
	glScalef(TempscaleFactor,  TempscaleFactor,1 );
	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());
	glColor3f(1, 0, 0);
	glCallList(ID_COORDINATY);
}

void RoamingScenceManager::executeRotateOperation(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);

	Vector3D MouseTrace = *AuxY*(OldMouse->Y() - Mouse->Y()) + *AuxX*(Mouse->X() - OldMouse->X());
	Vector3D RotateAsix = Vector3D::crossProduct(MouseTrace, *AuxZ);
	RotateAsix.normalize();

	float angle = MouseTrace.length();
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(angle, RotateAsix);

	*NewEye = rotatMatrix * (*NewEye);
	*NewUp = rotatMatrix * (*NewUp);

	NewUp->normalize();

	*AuxY = *NewUp;
	*AuxZ = (Vector3D)*NewEye - (Vector3D)*NewView;
	*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
	AuxX->normalize();
	/*更新鼠标 */
	*OldMouse = *Mouse;
}

void RoamingScenceManager::executeScaleOperation(float factor)
{
	if (TempscaleFactor <= 0)
	{
		TempscaleFactor = 0.1;
	}
	else
	{
		TempscaleFactor += factor;
	}
}

void RoamingScenceManager::executeTranslateOperation(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);
	Vector3D vec(-OldMouse->X() + Mouse->X(), -Mouse->Y() + OldMouse->Y(), 0);
	*TempTranslateVec = *TempTranslateVec + vec;
	*OldMouse = *Mouse;
}

void RoamingScenceManager::getInitPos(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);
	*OldMouse = *Mouse;
}

void RoamingScenceManager::init_CoordinaryDisplayList()
{
	ID_COORDINATY = glGenLists(1);
	glNewList(ID_COORDINATY, GL_COMPILE);
	/*红色轴是X轴，绿色是Y轴，蓝色是Z轴 */
	int length = 2;
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	glEnd();

	glLineWidth(0.5);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, length);
	glEnd();

	glLineWidth(1);
	glEnd();
	glEndList();
}
