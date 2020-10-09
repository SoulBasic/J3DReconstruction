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

GLuint RoamingScenceManager::read_png_file(const char* file_name)
{
	char header[8];     //8
	int k;   //用于循环
	GLuint textureID; //贴图名字
	int width, height; //记录图片到宽和高
	png_byte color_type; //图片到类型（可能会用在是否是开启来通道）
	png_byte bit_depth; //字节深度

	png_structp png_ptr; //图片
	png_infop info_ptr; //图片的信息
	int number_of_passes; //隔行扫描
	png_bytep * row_pointers;//图片的数据内容
	int row, col, pos;  //用于改变png像素排列的问题。
	GLubyte *rgba;

	FILE *fp = fopen(file_name, "rb");//以只读形式打开文件名为file_name的文件
	if (!fp)//做出相应可能的错误处理
	{
		return 0;//此处应该调用一个生成默认贴图返回ID的函数
	}
	//读取文件头判断是否所png图片.不是则做出相应处理
	fread(header, 1, 8, fp);
	if (png_sig_cmp((png_const_bytep)header, 0, 8))
	{
		fclose(fp);
		return 0; //每个错误处理都是一样的！这样报错之后锁定就要花点小时间来！
	}

	//根据libpng的libpng-manual.txt的说明使用文档 接下来必须初始化png_structp 和 png_infop
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //后三个是绑定错误以及警告的函数这里设置为空
	if (!png_ptr)//做出相应到初始化失败的处理
	{
		fclose(fp);
		return 0;
	}
	//根据初始化的png_ptr初始化png_infop
	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
	{
		//初始化失败以后销毁png_structp
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}


	//老老实实按照libpng给到的说明稳定步骤来  错误处理！
	if (setjmp(png_jmpbuf(png_ptr)))

	{
		//释放占用的内存！然后关闭文件返回一个贴图ID此处应该调用一个生成默认贴图返回ID的函数

		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

		fclose(fp);

		return 0;

	}
	//你需要确保是通过2进制打开的文件。通过i/o定制函数png_init_io
	png_init_io(png_ptr, fp);
	//似乎是说要告诉libpng文件从第几个开始missing
	png_set_sig_bytes(png_ptr, 8);
	//如果你只想简单的操作你现在可以实际读取图片信息了！
	png_read_info(png_ptr, info_ptr);
	//获得图片到信息 width height 颜色类型  字节深度
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	//如果图片带有alpha通道就需要
	//if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)

			//png_set_swap_alpha(png_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	//隔行扫描图片  这个必须要调用才能进行
	number_of_passes = png_set_interlace_handling(png_ptr);
	//将读取到的信息更新到info_ptr
	png_read_update_info(png_ptr, info_ptr);

	//读文件
	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		return 0;
	}
	rgba = (GLubyte*)malloc(width * height * 4);
	//使用动态数组  设置长度
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

	for (k = 0; k < height; k++)
		row_pointers[k] = NULL;

	//通过扫描流里面的每一行将得到的数据赋值给动态数组       
	for (k = 0; k < height; k++)
		//row_pointers[k] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
		row_pointers[k] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr,
			info_ptr));
	//由于png他的像素是由 左-右-从顶到底 而贴图需要的像素都是从左-右-底到顶的所以在这里需要把像素内容进行一个从新排列
	//读图片
	png_read_image(png_ptr, row_pointers);
	pos = (width * height * 4) - (4 * width);
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < (4 * width); col += 4)
		{
			rgba[pos++] = row_pointers[row][col];        // red
			rgba[pos++] = row_pointers[row][col + 1];    // green
			rgba[pos++] = row_pointers[row][col + 2];    // blue
			rgba[pos++] = row_pointers[row][col + 3];    // alpha
		}
		pos = (pos - (width * 4) * 2);
	}


	//开启纹理贴图特效
	glEnable(GL_TEXTURE_2D);

	//创建纹理 
	glGenTextures(1, &textureID);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, textureID); //将纹理绑定到名字

//设置贴图和纹理的混合效果这里是默认只用纹理
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//设置纹理所用到图片数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	free(row_pointers);
	fclose(fp);
	return textureID;
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

}

void RoamingScenceManager::initPng(const char* fileName) {
	//GLuint id = this->read_png_file(fileName);
	GLuint id = 0;
	if (id != 0) {
		cout << "纹理贴图" << fileName << endl;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
	}

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
