#pragma once
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <MMSystem.h>

#define LEFT_LEG 45465
#define RIGHT_LEG 23765

int numofdogs = 10;



float timeofDog = 0.0;
int legFlag = LEFT_LEG;

void drawBody(float size);
void drawDog(float size);
void drawLeg(float size);
void drawHead(float size);
void drawBack(float size);
void initializeDogs();
void drawDogs();
void MoveDogs();
void DogsChangeDir();

void drawRoom(float size);
void drawWall(float width, float height, float length);


class Dog {
public:
	float size;
	float x, y, z;
	float direction;
	Dog() {}
	Dog(float nsize, float nx, float ny, float nz) {
		size = nsize;
		x = nx; y = ny; z = nz;
	}
	void Draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		drawDog(size);
		glPopMatrix();
	}
	void RandomMove() {
		float myAngle = rand() % 360;
		x += size * 0.5 * cos((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
		z += size * 0.5 * sin((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
	}
	void Move() {
		float myAngle = direction;
		x += size * 0.5 * cos((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
		z += size * 0.5 * sin((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
		if (x > 212 || z > 223 || x < 187 || z < 202) {
			ChangeDirection();
		}
	}
	void ChangeDirection() {
		direction = rand() / 100.0;
	}
};

Dog* dogs = new Dog[numofdogs];
void drawBody(float size) {
	GLUquadricObj *cyl = gluNewQuadric();

	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricTexture(cyl, TRUE);
	gluQuadricNormals(cyl, GLU_SMOOTH);
	gluCylinder(cyl, 5.0 * size, 5.0 * size, 10.0 * size, 50, 50);

}

void drawDog(float size) {
	glPushMatrix();
	glTranslatef(0.0, -1.0 + size * 9.0, 0.0);
	glColor3f(165 / 255.0, 42 / 255.0, 42 / 255.0);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[20]);
	glEnable(GL_TEXTURE_2D);
	drawBody(size);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[22]);

	legFlag = LEFT_LEG;
	drawLeg(size);

	glPushMatrix();
	glTranslatef(0.0, 0.0, 6.0 * size);
	drawLeg(size);
	glPopMatrix();



	legFlag = RIGHT_LEG;

	glPushMatrix();
	glTranslatef(-6.0 * size, 0.0, 0.0);
	drawLeg(size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-6.0 * size, 0.0, 6.0 * size);
	drawLeg(size);
	glPopMatrix();
	drawBack(size);

	drawHead(size);


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawLeg(float size) {
	glPushMatrix();
	GLUquadricObj *cyl = gluNewQuadric();

	float arcus;
	if (legFlag == LEFT_LEG)
		arcus = sin(timeofDog);
	else
		arcus = -sin(timeofDog);

	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricTexture(cyl, TRUE);
	gluQuadricNormals(cyl, GLU_SMOOTH);

	glTranslatef(3.0* size, -4.0* size, 2.0* size);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(arcus * 45, 1.0, 0.0, 0.0);
	gluCylinder(cyl, 1.0* size, 1.0* size, 3.5* size, 50, 50);

	glPushMatrix();

	glTranslatef(0.0, 0, 3.5* size);
	glRotatef(-abs(arcus) * 45, 1.0, 0.0, 0.0);
	gluCylinder(cyl, 1.0* size, 1.0* size, 3.5* size, 50, 50);
	glPopMatrix();

	glPopMatrix();
}

void drawHead(float size) {
	glPushMatrix();
	GLUquadricObj *cyl = gluNewQuadric();

	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricTexture(cyl, TRUE);
	gluQuadricNormals(cyl, GLU_SMOOTH);

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[21]);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(270, 1.0, 0.0, 0.0);

	//glRotatef(180, 0.0, 0.0, 1.0);
	gluSphere(cyl, 5.0 * size, 50, 50);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[20]);
	glPopMatrix();
}

void drawBack(float size) {
	glPushMatrix();
	GLUquadricObj *cyl = gluNewQuadric();

	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricTexture(cyl, TRUE);
	gluQuadricNormals(cyl, GLU_SMOOTH);
	glTranslatef(0, 0, 10.0 * size);
	gluSphere(cyl, 5.0 * size, 20, 20);
	glPopMatrix();
}

void initializeDogs() {
	for (int i = 0; i < numofdogs; i++) {
		dogs[i].size = rand() % 100 / 1000.0;
		dogs[i].x = rand() % 10 + 186;
		dogs[i].y = 0;
		dogs[i].z = rand() % 10 + 201;
	}
}

void drawDogs() {
	for (int i = 0; i < numofdogs; i++) {
		dogs[i].Draw();
	}
}

void MoveDogs() {
	for (int i = 0; i < numofdogs; i++) {
		dogs[i].Move();
	}
}

void DogsChangeDir() {
	for (int i = 0; i < numofdogs; i++) {
		dogs[i].ChangeDirection();
	}
}

void drawWall(float width, float height, float length) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[24]);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, width / 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length / 2, -1.0, width / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, height - 1.0, width / 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-length / 2, height - 1.0, width / 2);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, -width / 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length / 2, -1.0, -width / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glEnd();


	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, -width / 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-length / 2, height - 1.0, width / 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-length / 2, -1.0, width / 2);
	glEnd();

	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(length / 2, -1.0, -width / 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, height - 1.0, width / 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length / 2, -1.0, width / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
	glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length / 2, height - 1.0, width / 2);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-length / 2, height - 1.0, +width / 2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glEnd();

}

void drawTransWall(float width, float height, float length) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[24]);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5, 0.8, 1.0, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(-length / 2, -1.0, width / 2);
	glVertex3f(length / 2, -1.0, width / 2);
	glVertex3f(length / 2, height - 1.0, width / 2);
	glVertex3f(-length / 2, height - 1.0, width / 2);
	//glEnd();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	///glColor4f(0.5, 0.8, 1.0, 0.3);
	//glBegin(GL_QUADS);
	glVertex3f(-length / 2, -1.0, -width / 2);
	glVertex3f(length / 2, -1.0, -width / 2);
	glVertex3f(length / 2, height - 1.0, -width / 2);
	glVertex3f(-length / 2, height - 1.0, -width / 2);
	//glEnd();


	//
	//glBegin(GL_QUADS);
	glVertex3f(-length / 2, -1.0, -width / 2);
	glVertex3f(-length / 2, height - 1.0, -width / 2);
	glVertex3f(-length / 2, height - 1.0, width / 2);
	glVertex3f(-length / 2, -1.0, width / 2);
	//glEnd();

	//
	//glBegin(GL_QUADS);
	glVertex3f(length / 2, -1.0, -width / 2);
	glVertex3f(length / 2, height - 1.0, -width / 2);
	glVertex3f(length / 2, height - 1.0, width / 2);
	glVertex3f(length / 2, -1.0, width / 2);
	//glEnd();

	//glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glVertex3f(length / 2, height - 1.0, -width / 2);
	glVertex3f(length / 2, height - 1.0, width / 2);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glVertex3f(-length / 2, height - 1.0, +width / 2);
	glVertex3f(-length / 2, height - 1.0, -width / 2);
	glDisable(GL_BLEND);
	glEnd();

}

void drawsmallWall(float width, float height, float length) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[24]);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, width / 2);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(length / 2, -1.0, width / 2);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(length / 2, height - 1.0, width / 2);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-length / 2, height - 1.0, width / 2);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, -width / 2);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(length / 2, -1.0, -width / 2);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glEnd();


	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-length / 2, -1.0, -width / 2);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(-length / 2, height - 1.0, width / 2);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-length / 2, -1.0, width / 2);
	glEnd();

	//
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(length / 2, -1.0, -width / 2);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(length / 2, height - 1.0, width / 2);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(length / 2, -1.0, width / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
	glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, 3.0f * floors - 1.0, width);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(length / 2, height - 1.0, -width / 2);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(length / 2, height - 1.0, width / 2);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 3.0f * floors - 1.0, -width);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-length / 2, height - 1.0, +width / 2);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-length / 2, height - 1.0, -width / 2);
	glEnd();

}

void drawRoom(float size) {
	glPushMatrix();
	//drawWall(1* size, 5 * size, 10 * size);

	GLfloat lightColor2[] = { 1.0, 0.0, 0.0, 1 };
	GLfloat lightPos2[] = { -1, -1, -1, 0 };

	GLfloat lightColor3[] = { 0.0, 1.0, 0.0, 1 };
	GLfloat lightPos3[] = { -1, -1, 1, 0 };

	GLfloat lightColor4[] = { 1.0, 1.0, 0.0, 1 };
	GLfloat lightPos4[] = { -1, 1, -1, 0 };

	GLfloat lightColor5[] = { 1.0, 0.0, 1.0, 1 };
	GLfloat lightPos5[] = { 1, -1, -1, 0 };

	GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shin[] = { 128 };

	glPushMatrix();
	glTranslatef(5.0 * size, 0.0, (5.0 - 0.5)* size);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawWall(1 * size, 5 * size, 10 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0* size, 0.0, (5.0 - 0.5)* size);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawWall(1 * size, 5 * size, 10 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, (10.0 - 1.0)* size);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawWall(1 * size, 5 * size, 10 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, (5 - 1.0)* size, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawWall(1 * size, 10 * size, 10 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5 * size, 2.5 * size, -0.25 * size);
	drawsmallWall(1, 2.5 * size, 5 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+2.5 * size, 2.5 * size, -0.25 * size);
	drawsmallWall(1, 2.5 * size, 5 * size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+2.5 * size, 0, -0.25 * size);
	drawsmallWall(1, 2.5 * size, 4.5 * size);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2.5 * size, 0, -0.25 * size);
	drawsmallWall(1, 2.5 * size, 4.5 * size);
	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, MyTextureObject[26]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5 * size, -0.98, -0.3 * size);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-5 * size, -0.98, 10 * size);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(5 * size, -0.98, 10 * size);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(5 * size, -0.98, -0.3 * size);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[23]);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.0, 5.0*size - 7.0, 5.0*size);
	glRotatef(timeofDog * 5, 0, 1, 0);
	glLightfv(GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE, lightColor2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

	glLightfv(GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE, lightColor3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor3);
	//glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);

	glLightfv(GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE, lightColor4);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor4);
	//glLightfv(GL_LIGHT2, GL_POSITION, lightPos4);

	glLightfv(GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE, lightColor5);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor5);
	//glLightfv(GL_LIGHT2, GL_POSITION, lightPos5);

	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);
	glCallList(myDiscoId);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix();
}