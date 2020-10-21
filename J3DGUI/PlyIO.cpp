#include "PlyIO.h"
using namespace std;
struct Point3D//点坐标
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat  r;
	GLfloat  g;
	GLfloat  b;
	GLfloat  alpha;
};
PlyIO::PlyIO(char* fileName) {
	//initializeOpenGLFunctions();
	this->vertex_N = 0;
	this->face_N = 0;
	this->fileName = fileName;
	this->type = getPlyFileType();
	this->available = false;
	this->textureFileName = "";
	std::string fn(fileName);
	fn = fn.substr(0, fn.find_last_of('/'));
	this->workDir = fn;
	//this->rgba = (GLubyte*)malloc(4);

}
PlyIO::~PlyIO() {
	if (this->vertex_N > 0) { delete[] vertex; }
	if (this->face_N > 0) { delete[] faces; }
	//free(this->rgba);
}

int PlyIO::getPlyFileType()
{
	FILE *file;
	file = fopen(this->fileName, "rb");
	if (file == NULL)
	{
		return -1;
	}
	char c;
	fread((void*)(&c), sizeof(c), 1, file);
	while (c != '\n')
		fread((void*)(&c), sizeof(c), 1, file);
	for (int i = 0; i < 8; i++)
		fread((void*)(&c), sizeof(c), 1, file);
	if (c == 'a')
		return TYPE_PLY_RT;
	else
		return TYPE_PLY_RB;
}


std::string PlyIO::vectostring(std::vector<char> vec) {
	std::string str(&vec.front(), vec.size());
	return str;
}

std::string PlyIO::readValueRB() {
	char c;
	std::vector<char> buffer;
	while (true) {
		fread((void*)(&c), sizeof(c), 1, this->file);
		if (c == ' ' || c == '\n') {
			break;
		}
		buffer.push_back(c);
	}
	std::string temp = vectostring(buffer);
	return temp;
}

int PlyIO::getTypeBytesLength(std::string type)
{
	if (type == "float32")
		return 4;
	else if (type == "uint8")
		return 1;
	else if (type == "int8")
		return 1;
	else if (type == "int16")
		return 2;
	else if (type == "int32")
		return 4;
	else if (type == "int")
		return 4;
	else if (type == "uint32")
		return 8;
	else if (type == "float")
		return 4;
	else if (type == "float64")
		return 8;
	else if (type == "uchar")
		return 1;
	else if (type == "uint")
		return 1;
	else
		return -1;
}

bool PlyIO::readHeaderRB() {
	char c;
	std::vector<char*> header;
	std::vector<char> content;

	if (readValueRB() != "ply")
		return false;
	std::string value = "";

	do {
		value = readValueRB();
		if (value == "TextureFile") {
			this->textureFileName = this->workDir + "/" + readValueRB();
			this->initPng();
		}
	} while (value != "element");

	readValueRB();
	value = readValueRB();
	this->vertex_N = atoi(value.c_str());
	while (true) {
		value = readValueRB();
		if (value != "property")
			break;
		vertex_types.push_back(getTypeBytesLength(readValueRB()));
		readValueRB();
	}
	if (value == "end_header") {
		face_N = 0;
		return true;
	}

	readValueRB();
	value = readValueRB();
	face_N = atoi(value.c_str());
	int length;
	value = readValueRB();
	if (value == "property") {
		readValueRB();
		do {
			length = getTypeBytesLength(readValueRB());
			if (length != -1)
				face_types.push_back(length);


		} while (length != -1);
		while ("end_header" != readValueRB()) {}
		return true;
	}
	return false;
}

void PlyIO::initPng() {
	CreateTextureFromPng();

}

GLuint PlyIO::CreateTextureFromPng()
{
	unsigned char header[8];     //8
	int k;   //用于循环

	png_byte bit_depth; //字节深度
	png_structp png_ptr; //图片
	png_infop info_ptr; //图片的信息
	int number_of_passes; //隔行扫描
	png_bytep * row_pointers;//图片的数据内容
	int row, col, pos;  //用于改变png像素排列的问题。
	FILE *fp = fopen(this->textureFileName.c_str(), "rb");//以只读形式打开文件名为file_name的文件
	if (!fp)//做出相应可能的错误处理
	{
		fclose(fp);//关闭打开的文件！给出默认贴图
		return 0;//此处应该调用一个生成默认贴图返回ID的函数
	}
	//读取文件头判断是否所png图片.不是则做出相应处理
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
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
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		cout << "colortype=rgba" << endl;
	}
	else {
		cout << "colortype=rgb" << endl;
	}

	// png_set_swap_alpha(png_ptr);
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

	//png_set_rows(png_ptr,info_ptr,row_pointers);
	png_read_image(png_ptr, row_pointers);
	pos = (width * height * 4) - (4 * width);
	cout << "png_read_image after line done" << endl;
	if (row_pointers == nullptr) {
		cout << "error nullptr" << endl;
		return 0;
	}
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		for (row = 0; row < height; row++)
		{
			for (col = 0; col < (4 * width); col += 4)
			{

				rgba[pos++] = row_pointers[row][col];        // red
				rgba[pos++] = row_pointers[row][col + 1];    // green
				rgba[pos++] = row_pointers[row][col + 2];    // blue
				rgba[pos++] = row_pointers[row][col + 3];    // alpha
				//cout << "red=" << (int)rgba[pos-4] << " green=" << (int)rgba[pos-3] << " blue=" << (int)rgba[pos-2] << " alpha=" << (int)rgba[pos-1] << endl;
			}
			pos = (pos - (width * 4) * 2);
		}
	}
	else
	{
		for (row = 0; row < height; row++)
		{
			for (col = 0; col < (4 * width); col += 3)
			{

				rgba[pos++] = row_pointers[row][col];        // red
				rgba[pos++] = row_pointers[row][col + 1];    // green
				rgba[pos++] = row_pointers[row][col + 2];    // blue
				//cout << "red=" << (int)rgba[pos-3] << " green=" << (int)rgba[pos-2] << " blue=" << (int)rgba[pos-1]<< endl;

			}
			pos = (pos - (width * 4) * 2);
		}
	}

	cout << "rgba read done" << endl;
	//开启纹理贴图特效
	glEnable(GL_TEXTURE_2D);

	//创建纹理 
	glGenTextures(1, &textureID);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, textureID); //将纹理绑定到名字

	//设置贴图和纹理的混合效果这里是默认只用纹理
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//设置纹理所用到图片数据
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgba);
	}

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	cout << "kkkkkkkkkkk" << endl;
	free(row_pointers);

	fclose(fp);
	return textureID;

}

void PlyIO::calculateNormal(PlyIO::Face3D &face) {
	Point3D FirstPoint = vertex[face.v1];
	//qDebug("第一点：%f %f %f", FirstPoint.x, FirstPoint.y, FirstPoint.z);

	Point3D SecondPoint = vertex[face.v2];
	//qDebug("第二点：%f %f %f", SecondPoint.x, SecondPoint.y, SecondPoint.z);
	Point3D ThirdPoint = vertex[face.v3];
	//qDebug("第三点：%f %f %f", ThirdPoint.x, ThirdPoint.y, ThirdPoint.z);
	float dVector1[3], dVector2[3], norm[3];
	dVector1[0] = SecondPoint.x - FirstPoint.x;
	dVector1[1] = SecondPoint.y - FirstPoint.y;
	dVector1[2] = SecondPoint.z - FirstPoint.z;

	dVector2[0] = ThirdPoint.x - FirstPoint.x;
	dVector2[1] = ThirdPoint.y - FirstPoint.y;
	dVector2[2] = ThirdPoint.z - FirstPoint.z;

	norm[0] = dVector1[1] * dVector2[2] - dVector1[2] * dVector2[1];
	norm[1] = dVector1[2] * dVector2[0] - dVector1[0] * dVector2[2];
	norm[2] = dVector1[0] * dVector2[1] - dVector1[1] * dVector2[0];

	float dNormalLength = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);

	face.normal[0] = norm[0] / dNormalLength;
	face.normal[1] = norm[1] / dNormalLength;
	face.normal[2] = norm[2] / dNormalLength;
	//qDebug("法向量：%f %f %f", face.normal[0], face.normal[1], face.normal[2]);
}


bool PlyIO::open()
{
	std::cout << "Start loading point cloud data" << std::endl;

	if (this->type == TYPE_PLY_RT)
	{
		FILE *file;		// 工作文件
		file = fopen(this->fileName, "rt");					// 打开文件
		if (file == NULL)
		{
			MessageBox(NULL, L"Can not open file", L"Error", MB_OK);
			return false;
		}
		this->file = file;
		int counter = 0;
		while (counter < 14)
		{
			if (counter == 3)
			{
				fscanf(file, "%d", &vertex_N);
				vertex = new Point3D[vertex_N];
			}
			char c;
			fread((void*)(&c), sizeof(c), 1, file);
			if (c == '\n') {
				counter++;
			}
		}
		std::cout << "文件解析完成\n" << "Vertexs:" << vertex_N << " \n" << "Faces:" << face_N << " \n";
		float p[3];
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		int k[4];
		k[0] = 0;
		k[1] = 0;
		k[2] = 0;
		k[3] = 0;
		float dummy;
		//C_Progress_display progressbar(vertex_N, std::cout, "正在加载\n");
		for (int i = 0; i < vertex_N; i++) {
			fscanf(file, "%f", &p[0]);//x
			fscanf(file, "%f", &p[1]);//y
			fscanf(file, "%f", &p[2]);//z
			fscanf(file, "%d", &k[0]);//r
			fscanf(file, "%d", &k[1]);//g
			fscanf(file, "%d", &k[2]);//b
			fscanf(file, "%d", &k[3]);//alpha
			//MessageBox(NULL, std::to_string(p[5]).c_str(), "Error", MB_OK);

			///int colors[3] = { 0,0,0 };
			if (x_min > p[0])
			{
				x_min = p[0];
			}
			if (x_max < p[0])
			{
				x_max = p[0];
			}
			if (y_max < p[1])
			{
				y_max = p[1];
			}
			if (y_min > p[1])
			{
				y_min = p[1];
			}
			if (z_max < p[1])
			{
				z_max = p[1];
			}
			if (z_min > p[1])
			{
				z_min = p[1];
			}
			//setVertex(i, p, k);
		}
		int f[3];
		for (int i = 0; i < face_N; i++) {
			fscanf(file, "%f", &dummy);
			fscanf(file, "%d", &f[0]);
			fscanf(file, "%d", &f[1]);
			fscanf(file, "%d", &f[2]);
			//setFace(i, f);
		}

	}
	else if (type == TYPE_PLY_RB)
	{
		FILE *file;											// 工作文件
		file = fopen(this->fileName, "rb");					// 打开文件
		if (file == NULL)
		{
			MessageBox(NULL, L"Can not open file", L"Error", MB_OK);
			return false;
		}
		this->file = file;
		if (!readHeaderRB()) {
			std::cout << "File parsing failed\n" << std::endl;
			return false;
		}
		std::cout << "File parsing completed \n" << "Vertexs:" << vertex_N << " \n" << "Faces:" << face_N << " \n";
		vertex = new Point3D[vertex_N];
		faces = new Face3D[face_N];
		float* p;
		int k[4];
		k[0] = 0;
		k[1] = 0;
		k[2] = 0;
		k[3] = 0;
		bool haveColors = (vertex_types.size() > 3);
		bool haveAlphas = (vertex_types.size() > 6);
		switch (vertex_types.size())
		{
		case 3: {
			p = new float[3];
		}

		case 6: {
			p = new float[3];
		}
		case 7: {
			p = new float[3];

		}
		default: {
			p = new float[99];
		}
		}


		//C_Progress_display progressbar(vertex_N, std::cout, "正在加载");


		for (int i = 0; i < vertex_N; i++) {
			for (int j = 0; j < vertex_types.size(); j++) {

				if (vertex_types[j] == 4) {
					fread((void*)(&p[j]), vertex_types[j], 1, file);
				}
				else {
					fread((void*)&k[j - 3], vertex_types[j], 1, file);
				}
			}
			//if (x_min > p[0])
			//{
			//	x_min = p[0];
			//}
			//if (x_max < p[0])
			//{
			//	x_max = p[0];
			//}
			//if (y_max < p[1])
			//{
			//	y_max = p[1];
			//}
			//if (y_min > p[1])
			//{
			//	y_min = p[1];
			//}
			//if (z_max < p[1])
			//{
			//	z_max = p[1];
			//}
			//if (z_min > p[1])
			//{
			//	z_min = p[1];
			//}

			vertex[i].x = p[0];
			vertex[i].y = p[1];
			vertex[i].z = p[2];

			if (haveColors) {
				vertex[i].r = ((float)k[0]) / 255;
				vertex[i].g = ((float)k[1]) / 255;
				vertex[i].b = ((float)k[2]) / 255;
				vertex[i].alpha = haveAlphas ? (((float)k[3]) / 255) : 1.0f;
			}

		}
		delete[] p;
		int dummy;
		int indexs[3];
		float uvs[6] = { 0,0,0,0,0,0 };
		indexs[0] = 0;
		indexs[1] = 0;
		indexs[2] = 0;
		//if (this->textureFileName != "") {
		//	std::string t = this->workDir + "/" + this->textureFileName;
		//	CreateTextureFromPng(t.c_str());
		//}
		for (int i = 0; i < face_N; i++)
		{
			fread((void*)(&dummy), 1, 1, file);

			for (int j = 0; j < 3; j++) {
				fread((void*)(&indexs[j]), 4, 1, file);
			}

			faces[i].v1 = indexs[0];
			faces[i].v2 = indexs[1];
			faces[i].v3 = indexs[2];
			//std::cout << "face " << i << " v1=" << indexs[0] << " v2=" << indexs[1] << " v3=" << indexs[2] << std::endl;

			if (this->textureFileName != "")
			{
				fread((void*)(&dummy), 1, 1, file);
				for (int t = 0; t < 6; t++) {
					fread((void*)(&uvs[t]), 4, 1, file);
				}
				faces[i].u[0] = uvs[0];
				faces[i].v[0] = uvs[1];
				faces[i].u[1] = uvs[2];
				faces[i].v[1] = uvs[3];
				faces[i].u[2] = uvs[4];
				faces[i].v[2] = uvs[5];

				//std::cout << " v1.u=" << vertex[indexs[0]].u << " v1.v=" << vertex[indexs[0]].v << " v2.u=" << vertex[indexs[1]].u << " v2.v=" << vertex[indexs[1]].v
				//	<< " v3.u=" << vertex[indexs[2]].u << " v3.v=" << vertex[indexs[2]].v << std::endl; 
			}
			calculateNormal(faces[i]);
			//qDebug("法向量：%f %f %f", faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
		}
		fclose(file);
	}
	else
	{
		MessageBox(NULL, L"Format error", L"ERROR", MB_OK);
		return FALSE;
	}
	std::cout << "loading accomplished \n" << std::endl;
	this->available = true;

	return true;



}

GLvoid PlyIO::render() {
	if (face_N > 0) {


		if (this->textureFileName != "")
		{
			glBegin(GL_TRIANGLES);
			glColor3f(0.5, 0.5, 0.5);
			for (int i = 0; i < face_N; i++)
			{
				glNormal3f(faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
				//std::cout << "face " << i << " " << faces[i].v1 << " " << faces[i].v2 << " " << faces[i].v3 << std::endl;
				glTexCoord2f(faces[i].u[0], faces[i].v[0]);
				glVertex3f(vertex[faces[i].v1].x, vertex[faces[i].v1].y, vertex[faces[i].v1].z);
				//std::cout << "vertex.v1.u=" << faces[i].u[0] << "vertex.v1.v=" << faces[i].v[0] << std::endl;

				glTexCoord2f(faces[i].u[1], faces[i].v[1]);
				glVertex3f(vertex[faces[i].v2].x, vertex[faces[i].v2].y, vertex[faces[i].v2].z);
				//std::cout << "vertex.v2.u=" << faces[i].u[1] << "vertex.v2.v=" << faces[i].v[1] << std::endl;

				glTexCoord2f(faces[i].u[2], faces[i].v[2]);
				glVertex3f(vertex[faces[i].v3].x, vertex[faces[i].v3].y, vertex[faces[i].v3].z);
				//std::cout << "vertex.v3.u=" << faces[i].u[2] << "vertex.v3.v=" << faces[i].v[2] << std::endl;
			}
			glEnd();
			glColor3f(0.0, 0.0, 0.0);
			glPointSize(1);
			glBegin(GL_POINTS);

			for (int i = 0; i < vertex_N; i++)
			{
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
			}
			glEnd();

		}
		else
		{
			glBegin(GL_TRIANGLES);
			glColor3f(0.0, 1.0, 1.0);
			for (int i = 0; i < face_N; i++)
			{
				glNormal3f(faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
				glVertex3f(vertex[faces[i].v1].x, vertex[faces[i].v1].y, vertex[faces[i].v1].z);
				glVertex3f(vertex[faces[i].v2].x, vertex[faces[i].v2].y, vertex[faces[i].v2].z);
				glVertex3f(vertex[faces[i].v3].x, vertex[faces[i].v3].y, vertex[faces[i].v3].z);
			}
			glEnd();
			//glEnable(GL_LIGHTING);
		}


		if (this->textureFileName != "")
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

	}
	else {
		glDisable(GL_LIGHTING);
		glPointSize(1);
		glBegin(GL_POINTS);

		for (int i = 0; i < vertex_N; i++)
		{
			glColor3f(vertex[i].r, vertex[i].g, vertex[i].b);
			glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
		}
		glEnd();
	}



	glFlush();

}

