#include "PlyIO.h"

PlyIO::PlyIO(char* fileName) {
	this->fileName = fileName;
	this->type = getPlyFileType();
	this->available = false;
}
PlyIO::~PlyIO() {
	delete fileName;

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

int PlyIO::getTypeBytesLength(std::string type) {
	std::cout << type;
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
		value = readValueRB();
		if (value == "end_header")
			return true;
	}
	return false;
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
	std::cout << "开始加载点云数据" << std::endl;

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
			std::cout << "文件解析失败\n" << std::endl;
			return false;
		}
		std::cout << "文件解析完成\n" << "Vertexs:" << vertex_N << " \n" << "Faces:" << face_N << " \n";
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
		std::cout << vertex_types.size() << std::endl;
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
		int dummy;
		int indexs[3];
		indexs[0] = 0;
		indexs[1] = 0;
		indexs[2] = 0;
		for (int i = 0; i < face_N; i++) {
			fread((void*)(&dummy), 1, 1, file);

			for (int j = 0; j < 3; j++) {
				fread((void*)(&indexs[j]), 4, 1, file);
			}
			faces[i].v1 = indexs[0];
			faces[i].v2 = indexs[1];
			faces[i].v3 = indexs[2];
			calculateNormal(faces[i]);
			//qDebug("法向量：%f %f %f", faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
		}

	}
	else
	{
		MessageBox(NULL, L"格式错误", L"ERROR", MB_OK);
		return FALSE;
	}
	std::cout << "加载完成 \n" << std::endl;
	this->available = true;
	return true;



}

GLvoid PlyIO::render() {

	if (face_N > 0) {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < face_N; i++)
		{
			glNormal3f(faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
			glVertex3f(vertex[faces[i].v1].x, vertex[faces[i].v1].y, vertex[faces[i].v1].z);
			glVertex3f(vertex[faces[i].v2].x, vertex[faces[i].v2].y, vertex[faces[i].v2].z);
			glVertex3f(vertex[faces[i].v3].x, vertex[faces[i].v3].y, vertex[faces[i].v3].z);
		}

		glEnd();
		glEnable(GL_LIGHTING);   //开关:使用光

	}
	else {
		glDisable(GL_LIGHTING);
		glBegin(GL_POINTS);
		//glColor3f(0.0f, 0.0f, 0.0f);
		
		for (int i = 0; i < vertex_N; i++)
		{
			glColor3f(vertex[i].r, vertex[i].g, vertex[i].b);
			glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
		}
		glEnd();
	}


	glFlush();

}

