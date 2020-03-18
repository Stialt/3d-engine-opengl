#pragma once
#include <vector>
#include <fstream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265
#define TEX_OLD_1 6
#define TEX_OLD_2 7
#define TEX_OLD_4 8
#define TEX_OLD_5 9
#define TEX_OLD_MANY1 10
#define TEX_OLD_MANY2 16
#define TEX_OLD_MANY3 17
#define TEX_NEW_1 11
#define TEX_NEW_2 12
#define TEX_NEW_4 13
#define TEX_NEW_5 14
#define TEX_NEW_MANY 15
#define TEX_NEW_MANY1 18
#define TEX_NEW_MANY2 19

int myEarthID, mySunID, myMoonId, mySkyId, myDiscoId;
unsigned int MyTextureObject[30];
void loadSphere();
void drawBuilding(int size, bool old, float x, float y, float z);
int numofBuildings = 1000;





typedef union PixelInfo
{
	std::uint32_t Colour;
	struct
	{
		std::uint8_t B, G, R, A;
	};
} *PPixelInfo;


class BMP
{
private:
	std::uint32_t width, height;
	std::uint16_t BitsPerPixel;
	std::vector<std::uint8_t> Pixels;

public:
	BMP(const char* FilePath);
	BMP();
	std::vector<std::uint8_t> GetPixels() const { return this->Pixels; }
	std::uint32_t GetWidth() const { return this->width; }
	std::uint32_t GetHeight() const { return this->height; }
	bool HasAlphaChannel() { return BitsPerPixel == 32; }
};

BMP::BMP(const char* FilePath)
{
	std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
	if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

	hFile.seekg(0, std::ios::end);
	std::size_t Length = hFile.tellg();
	hFile.seekg(0, std::ios::beg);
	std::vector<std::uint8_t> FileInfo(Length);
	hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

	if (FileInfo[0] != 'B' && FileInfo[1] != 'M')
	{
		hFile.close();
		throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
	}

	if (FileInfo[28] != 24 && FileInfo[28] != 32)
	{
		hFile.close();
		throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
	}

	BitsPerPixel = FileInfo[28];
	width = FileInfo[18] + (FileInfo[19] << 8);
	height = FileInfo[22] + (FileInfo[23] << 8);
	std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
	std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
	Pixels.resize(size);

	hFile.seekg(PixelsOffset, std::ios::beg);
	hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
	hFile.close();
}
BMP::BMP() {

}

class MyPoint {
public:
	float x, z;
	MyPoint() {
		x = 0.0;
		z = 0.0;
	}
	MyPoint(float nx, float nz) {
		x = nx;
		z = nz;
	}
	MyPoint addDistanceVector(float Distance, float angle) {
		float radAngle = angle / 360 * 2 * PI;
		/*
		float nx = x + cos(radAngle) * Distance;
		float nz = z + sin(radAngle) * Distance;
		*/
		float nx = x - sin(radAngle) * Distance;
		float nz = z + cos(radAngle) * Distance;
		return MyPoint(nx, nz);
	}
	float findDistanceBetweenPoints(MyPoint a, MyPoint b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z));
	}
};

class mySpace {
public:
	bool isRec; //if true - rectangle space, else - circle space
	float x, z;
	float width;
	mySpace() {};
	mySpace(bool Rec, float nx, float nz, float nwidth) {
		isRec = Rec;
		x = nx;
		z = nz;
		width = nwidth;
	}
	int checkSpace(float nx, float nz) {
		if (isRec) {
			if (nx < x + width && nx > x - width && nz < z + width && nz > z - width) {
				return false;
			}
		}
		else {
			MyPoint a(x, z);
			MyPoint b(nx, nz);
			if (a.findDistanceBetweenPoints(a, b) < width) {
				return false;
			}
		}
		return true;
	}
};

std::vector<mySpace> objectsSpace;

struct building {
	float x, z;
	int size;
	bool isRec;
};

//building city[numofBuildings];
building* city = new building[numofBuildings];

void loadSun() {
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	mySunID = glGenLists(1);
	glNewList(mySunID, GL_COMPILE);
	gluSphere(sphere, 5.0, 20, 20);
	glEndList();
	gluDeleteQuadric(sphere);
}

void loadMoon() {
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	myMoonId = glGenLists(1);
	glNewList(myMoonId, GL_COMPILE);
	gluSphere(sphere, 3.0, 20, 20);
	glEndList();
	gluDeleteQuadric(sphere);
}

void loadDiscoBall() {
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	myDiscoId = glGenLists(1);
	glNewList(myDiscoId, GL_COMPILE);
	gluSphere(sphere, 2.0, 20, 20);
	glEndList();
	gluDeleteQuadric(sphere);
}

void loadSky() {
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);

	mySkyId = glGenLists(1);
	glNewList(mySkyId, GL_COMPILE);
	gluSphere(sphere, 200.0, 200, 200);
	glEndList();
	gluDeleteQuadric(sphere);
}

void loadSphere() {

	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	//Making a display list
	myEarthID = glGenLists(1);
	glNewList(myEarthID, GL_COMPILE);
	gluSphere(sphere, 1.0, 200, 200);
	glEndList();
	gluDeleteQuadric(sphere);
	//
	loadSun();
	loadMoon();
	loadSky();
	loadDiscoBall();
}

void LoadGLTextures(void) {
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glGenTextures(30, &MyTextureObject[0]);

	BMP bmps[30];

	bmps[0] = BMP("photos/earth.bmp");
	bmps[1] = BMP("photos/6.bmp");
	bmps[2] = BMP("photos/normal_ground1.bmp");
	bmps[3] = BMP("photos/sun.bmp");
	bmps[4] = BMP("photos/moon.bmp");
	bmps[5] = BMP("photos/clear_sky.bmp");
	bmps[6] = BMP("photos/buildings/old_1.bmp");
	bmps[7] = BMP("photos/buildings/old_2.bmp");
	bmps[8] = BMP("photos/buildings/old_4.bmp");
	bmps[9] = BMP("photos/buildings/old_5.bmp");
	bmps[10] = BMP("photos/buildings/old_many2.bmp");
	bmps[11] = BMP("photos/buildings/new_1.bmp");
	bmps[12] = BMP("photos/buildings/new_2.bmp");
	bmps[13] = BMP("photos/buildings/new_4.bmp");
	bmps[14] = BMP("photos/buildings/new_5.bmp");
	bmps[15] = BMP("photos/buildings/new_many.bmp");
	bmps[16] = BMP("photos/buildings/old_many1.bmp");
	bmps[17] = BMP("photos/buildings/old_many.bmp");
	bmps[18] = BMP("photos/buildings/new_many1.bmp");
	bmps[19] = BMP("photos/buildings/new_many2.bmp");
	bmps[20] = BMP("photos/hitman_body.bmp");
	bmps[21] = BMP("photos/face1.bmp");
	bmps[22] = BMP("photos/hitman_hand.bmp");
	bmps[23] = BMP("photos/discoball.bmp");
	bmps[24] = BMP("photos/bricks.bmp");
	bmps[25] = BMP("photos/bricks1.bmp");
	bmps[26] = BMP("photos/dance.bmp");

	for (int i = 0; i < 27; i++) {
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmps[i].GetWidth(), bmps[i].GetHeight(), 0,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, bmps[i].GetPixels().data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	glEnable(GL_TEXTURE_2D);
	loadSphere();
}

void drawBuilding(int size, bool old, float x, float y, float z) {
	int myTexId;
	int floors = size;
	if (old) {
		switch (size) {
		case 0:
			return;
		case 1:
			myTexId = TEX_OLD_1;
			break;
		case 2:
			myTexId = TEX_OLD_2;
			break;
		case 3:
			myTexId = TEX_OLD_4;
			floors = 4;
			break;
		case 4:
			myTexId = TEX_OLD_5;
			floors = 5;
			break;
		case 5:
			myTexId = TEX_OLD_MANY1;
			floors = 10;
			break;
		case 6:
			myTexId = TEX_OLD_MANY2;
			floors = 10;
			break;
		case 7:
			myTexId = TEX_OLD_MANY3;
			floors = 10;
			break;

		default:
			myTexId = TEX_OLD_1;
			floors = 1;
		}
	}
	else {
		switch (size) {
		case 0:
			return;
		case 1:
			myTexId = TEX_NEW_1;
			break;
		case 2:
			myTexId = TEX_NEW_2;
			break;
		case 3:
			myTexId = TEX_NEW_4;
			floors = 4;
			break;
		case 4:
			myTexId = TEX_NEW_5;
			floors = 5;
			break;
		case 5:
			myTexId = TEX_NEW_MANY1;
			floors = 10;
			break;
		case 6:
			myTexId = TEX_NEW_MANY2;
			floors = 10;
			break;
		case 7:
			myTexId = TEX_NEW_MANY;
			floors = 10;
			break;
		default:
			myTexId = TEX_NEW_1;
			floors = 1;
		}
	}
	glTranslatef(x, y, z);


	glBindTexture(GL_TEXTURE_2D, MyTextureObject[myTexId]);
	glEnable(GL_TEXTURE_2D);
	GLfloat width = 1.0 * 2 * floors;
	if (floors >= 4) {
		width /= 1.5;
	}
	else if (floors >= 10) {
		width /= 2.0;
	}
	mySpace instance(true, -x, -z, width + 0.25);
	objectsSpace.push_back(instance);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -1.0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -1.0, width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -1.0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -1.0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, -width);
	glEnd();


	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -1.0, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -1.0, width);
	glEnd();

	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -1.0, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -1.0, width);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
	glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 3.0f * floors - 1.0, width);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, 3.0f * floors - 1.0, +width);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, 3.0f * floors - 1.0, -width);



	glEnd();

	glDisable(GL_TEXTURE_2D);
	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0);
	glEnd();
	*/
}