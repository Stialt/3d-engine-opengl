#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "bitmap.h"
#include "objects.h"
#include <time.h>
#include <windows.h>
#include <MMSystem.h>

#define LIFT_UP 100
#define LIFT_DOWN 101


static int year = 0, day = 0, moonday = 0;
GLfloat myZ = 0.0, myX = 0.0, myY = 0.0, myAngle = 0.0, myVerticalAngle = 0.0;
GLfloat solidCubeR = 1.0, solidCubeG = 0.0, solidCubeB = 0.0;
GLfloat sphereEarthAngle = 0.0, sphereSunAngle = 0.0;
MyPoint currentLocation(0.0, 0.0);
int sign = 1;
bool showSquare = false;
GLint mouseX = -1, mouseY = -1;

bool anSphere = false;
bool liftClick = false;
int liftState = LIFT_UP;

GLfloat stepSize = 0.1;
GLint DayLength = 60; //in seconds
GLint FPS = 60;
GLint cityArea = 500;

GLfloat anR = 1.0, anG = 1.0, anB = 1.0, anX = 0.0, anZ = 0.0, anY = 0.0;

//gl essential functions
void init(void);
void display();


//my functions
void clickObject(float x, float z, float angle);
void smallBlueCubeAction();
void checkForSigns();
bool allowedSpace(float X, float Z);
void drawSign();
bool checkIntersections(float width, bool b, float x, float y, float z);


//mydisplay functions
void drawSky();
void GenCity();
void drawCity();

//Callback functions
void MyTimer(int value);
void MyPassive(int x, int y);
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void MyMouseClick(int button, int state, int x, int y);
void MyMainMenu(int entryID);


void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	LoadGLTextures();
	GenCity();
	srand(time(NULL));
	initializeDogs();
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat mat_solid[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat mat_zero[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_notzero[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_sky[] = { 0.5 / 3, 0.8 / 3, 1.0 / 3, 1.0 };


	GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightPosition[] = { -10.0, 0.0, -300.0, 1.0 };

	GLfloat light1_ambient[] = { 0.5 / 2, 0.8 / 2, 1.0, 0.5 };
	GLfloat light1_diffuse[] = { 0.5 / 2, 0.8 / 2, 1.0, 0.5 };
	GLfloat light1_specular[] = { 0.5 / 2, 0.8 / 2, 1.0, 0.5 };
	GLfloat light1Position[] = { 10.0, 0.0, 300.0, 1.0 };


	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_notzero);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_solid);

	//Movement and view functions
	glRotatef(myVerticalAngle, 1.0, 0.0, 0.0);
	glRotatef(myAngle, 0.0, 1.0, 0.0);
	glTranslatef(myX, myY, myZ);
	//~movement

	//Floor
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[2]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(-1600.0, -1, -1600.0);
	glTexCoord2d(0.0, 2000.0); glVertex3f(-1600.0, -1, 1600.0);
	glTexCoord2d(2000.0, 2000.0); glVertex3f(1600.0, -1, 1600.0);
	glTexCoord2d(2000.0, 0.0); glVertex3f(1600.0, -1, -1600.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//~~~

	//Sky (and walls of sky)
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_sky);
	drawSky();
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_notzero);
	//~~~

	//Sun
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(sphereSunAngle, 1.0, 0.0, 0.0);
	glTranslatef(-10.0, 0.0, -300.0);
	//Light of Sun
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glRotatef(sphereEarthAngle, 0.0, 1.0, 0.0);
	//glutSolidSphere(5.0, 20, 20);

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
	//Textured Sun
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[3]);
	glCallList(mySunID);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//~~

	//Moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(sphereSunAngle, 1.0, 0.0, 0.0);
	glTranslatef(10.0, 0.0, 300.0);
	//Light of Moon
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Position);

	glRotatef(sphereEarthAngle, 0.0, 1.0, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	//glutSolidSphere(5.0, 20, 20);

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
	//Textured Moon
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[4]);
	glCallList(myMoonId);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//~~~Moon


	glMaterialfv(GL_FRONT, GL_EMISSION, mat_notzero);

	//Big Cube
	glPushMatrix();
	glTranslatef(0.0, 0.0, -500.0);
	GLfloat bigCubeEmission[] = { solidCubeR, solidCubeG, solidCubeB, 1.0 };
	glColor3f(solidCubeR, solidCubeG, solidCubeB);
	glMaterialfv(GL_FRONT, GL_EMISSION, bigCubeEmission);
	glutSolidCube(2.0);
	glPopMatrix();

	//Small blue cube
	glPushMatrix();
	glTranslatef(0.0, 0.0, -500.0);
	GLfloat smallCubeEmission[] = { 0.0, 0.0, 1.0, 1.0 };
	glColor3f(0.0, 0.0, 1.0);
	glMaterialfv(GL_FRONT, GL_EMISSION, smallCubeEmission);
	glTranslatef(1.25, 0.0, 0.0);
	glutSolidCube(0.5);
	if (showSquare) {
		glColor3f(0.0, 1.0, 0.0);
		glutWireCube(0.55);
	}
	glPopMatrix();
	//~~~

	//Sphere Earth
	glPushMatrix();
	GLfloat mat1 = 0.25;
	GLfloat mat_emission[] = { mat1, mat1, mat1, 0.6 };
	GLfloat material_ambient[] = { mat1, mat1, mat1, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	glEnable(GL_BLEND);
	//glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(3.0, 0.0, 0.0);
	glRotatef(sphereEarthAngle, 0.0, 1.0, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);		//use this to justify position of continents on sphere
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glCallList(myEarthID);
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glPopMatrix();
	//~~


	//City Drawing
	glPushMatrix();
	drawCity();
	glPopMatrix();
	//~


	//Dog
	glPushMatrix();
	drawDogs();
	glPopMatrix();
	//~~~



	//Disco Room with dogs
	glPushMatrix();
	glTranslatef(200, 0, 200);
	//drawWall(2, 1, 1);
	drawRoom(3);
	glPopMatrix();
	//

	//Blending Sphere
	if (anSphere) {
		GLfloat mat_transparent[] = { 1.0, 0.4, 0.3, 0.6 };
		GLfloat mat_emission_annoying_sphere[] = { anR, anG, anB, 0.6 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission_annoying_sphere);
		glPushMatrix();
		//glTranslatef(0.0, 0.0, -5.0);
		glTranslatef(anX, -anY, anZ);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0, 0.3, 0.3, 0.9);
		glDepthMask(GL_FALSE);

		glutSolidSphere(1.0, 100, 100);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glPopMatrix();
	}
	//~~~sphere

	glPopMatrix();
	//~~~world


	//draws sign of possibility to click
	if (showSquare)
		drawSign();



	glutSwapBuffers();

}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Engine Kim Denis");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(500, MyTimer, 1);
	glutPassiveMotionFunc(MyPassive);
	glutMouseFunc(MyMouseClick);

	GLint SubMenu1 = glutCreateMenu(MyMainMenu);//
	glutAddMenuEntry("10", 1010);
	glutAddMenuEntry("30", 1030);
	glutAddMenuEntry("60", 1060);
	glutAddMenuEntry("120", 1120);
	glutCreateMenu(MyMainMenu);

	GLint SubMenu2 = glutCreateMenu(MyMainMenu);//
	glutAddMenuEntry("10", 2010);
	glutAddMenuEntry("30", 2030);
	glutAddMenuEntry("60", 2060);
	glutAddMenuEntry("120", 2120);
	glutCreateMenu(MyMainMenu);

	GLint SubMenu3 = glutCreateMenu(MyMainMenu);//
	glutAddMenuEntry("Normal", 3001);
	glutAddMenuEntry("Run", 3005);
	glutAddMenuEntry("Very fast", 3010);
	glutAddMenuEntry("Thunderbolt mode", 3100);
	glutCreateMenu(MyMainMenu);

	GLint SubMenu4 = glutCreateMenu(MyMainMenu);//
	glutAddMenuEntry("10", 4001);
	glutAddMenuEntry("50", 4005);
	glutAddMenuEntry("500", 4050);
	glutAddMenuEntry("1000", 4100);
	glutAddMenuEntry("9000", 4900);
	glutCreateMenu(MyMainMenu);

	GLint SubMenu5 = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("White", 5001);
	glutAddMenuEntry("Red", 5002);
	glutAddMenuEntry("Green", 5003);
	glutAddMenuEntry("Blue", 5004);
	glutAddMenuEntry("No", 5005);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);//
	glutAddSubMenu("FPS~", SubMenu1);//    
	glutAddSubMenu("DAYtime~", SubMenu2);//    
	glutAddSubMenu("WalkSpeed~", SubMenu3);//    
	glutAddSubMenu("Number of buildings~", SubMenu4);
	glutAddSubMenu("Blending Sphere~", SubMenu5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);//

	glutMainLoop();
	return 0;
}



void MyMainMenu(int entryID) { //
	switch (entryID / 1000) {
	case 1:
		FPS = entryID % 1000;
		break;
	case 2:
		DayLength = entryID % 1000;
		break;
	case 3:
		stepSize = (entryID % 1000) / 10.0;
		break;
	case 4:
		numofBuildings = (entryID % 1000) * 10;
		if (numofBuildings > 1000)
			cityArea = 2000;
		else
			cityArea = 500;
		city = new building[numofBuildings];
		GenCity();
		glutPostRedisplay();
		break;
	case 5:
		MyPoint currentP(myX, myZ);
		MyPoint PointView;
		PointView = currentP.addDistanceVector(3.0, myAngle);

		anX = -PointView.x;
		anY = myY;
		anZ = -PointView.z;
		switch (entryID % 10) {
		case 1:
			anSphere = true;
			anR = anG = anB = 1.0;
			break;
		case 2:
			anSphere = true;
			anR = 1.0; anG = anB = 0.0;
			break;
		case 3:
			anSphere = true;
			anG = 1.0; anR = anB = 0.0;
			break;
		case 4:
			anSphere = true;
			anB = 1.0; anR = anG = 0.0;
			break;
		case 5:
			anSphere = false;
			break;
		}
	}
}



//List of object coordinates and sizes
/*
1. Big Solid Cube: 0.0, 0.0, 0.0, size = 2
2. Small solid cube on edge of 1.: -1.25, 0.0, 0.0, size = 0.5
3. Earth: -3.0, 0.0, 0.0, Radius = 1.0
*/
void clickObject(float x, float z, float angle) {
	MyPoint currentP(x, z);
	MyPoint PointView;
	PointView = currentP.addDistanceVector(1.0, angle);
	printf("MyPoint of View: %.2f, %.2f\n", PointView.x, PointView.z);
	//check which object clicked
	MyPoint instantPoint;
	float DistanceToBlueCube = instantPoint.findDistanceBetweenPoints(PointView, MyPoint(-1.25, 500.0));
	float DistanceToBlueCubefromLoc = instantPoint.findDistanceBetweenPoints(currentLocation, MyPoint(-1.25, 500.0));
	printf("Distance to Blue Cube: %.2f\n", DistanceToBlueCube);
	printf("Distance from Locatio: %.2f\n", DistanceToBlueCubefromLoc);
	if (DistanceToBlueCube < 0.5) {
		smallBlueCubeAction();
	}

}

void smallBlueCubeAction() {
	if (solidCubeR == 1.0) {
		solidCubeR = 0.0;
		solidCubeB = 1.0;
		solidCubeG = 1.0;
		printf("SmallBlueCube clicked\n");
	}
	else if (solidCubeB == 1.0) {
		solidCubeR = 1.0;
		solidCubeG = 0.0;
		solidCubeB = 0.0;
		printf("SmallBlueCube clicked\n");
	}
	//liftClick = true;
	sign *= -1;
}

void checkForSigns() {
	MyPoint currentP(myX, myZ);
	MyPoint PointView;
	PointView = currentP.addDistanceVector(1.0, myAngle);
	MyPoint instantPoint;
	float DistanceToBlueCube = instantPoint.findDistanceBetweenPoints(PointView, MyPoint(-1.25, 500.0));
	//for blue small cube
	if (DistanceToBlueCube < 0.5 and myY > -3) {
		showSquare = true;
		printf("SmallBLueCubefound\n");
	}
	else {
		showSquare = false;
	}
}

bool allowedSpace(float X, float Z) {
	//check if point in allowed space or not
	//check for Big Cube:
	MyPoint instantPoint;
	if (myY < -20) {
		return true;
	}
	if (X <= 1.0 && X >= -1.0 && Z <= 1.0 + 500.0  && Z >= -1.0 + 500.0) {
		printf("Collision with Big Cube\n");
		return false;
	}
	//check for small Blue Cube:
	if (X <= 0.25 - 1.25 && X >= -0.25 - 1.25 && Z <= 0.25 + 500.0  && Z >= -0.25 + 500.0) {
		printf("Collision with SmallBLue Cube\n");
		return false;
	}

	if (instantPoint.findDistanceBetweenPoints(MyPoint(-3.0, 0), MyPoint(X, Z)) <= 1.5) {
		printf("Collision with Earth\n");
		return false;
	}
	for (int i = 0; i < objectsSpace.size(); i++) {
		if (objectsSpace[i].checkSpace(X, Z) == false) {
			return false;
		}
	}
	return true;

}

//--------------CALLBACK FUNCTIONS--------------------------
void MyTimer(int value) {
	objectsSpace.clear();
	sphereEarthAngle += sign * 1.0;
	sphereSunAngle += 360.0 / FPS / DayLength;
	timeofDog += 0.1;
	MoveDogs();
	if (int(timeofDog * 10) % 50 == 0)
		DogsChangeDir();
	if (sphereEarthAngle >= 360)
		sphereEarthAngle -= 360;
	if (sphereSunAngle >= 360)
		sphereSunAngle -= 360;
	if (sphereSunAngle >= 180.0 && sphereSunAngle <= 360.0) {
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	else {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
	if (liftClick == true && liftState == LIFT_UP) {
		myY -= 10.0 / FPS;
		if (myY < -50.0) {
			liftClick = false;
			liftState = LIFT_DOWN;
		}
	}
	else if (liftClick == true && liftState == LIFT_DOWN) {
		myY += 10.0 / FPS;
		if (myY > 0.0) {
			myY = 0.0;
			liftClick = false;
			liftState = LIFT_UP;
		}
	}
	checkForSigns();
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, MyTimer, 1);
}

void MyPassive(int x, int y) {
	if (mouseX != -1 && mouseY != -1) {
		if (abs(x - mouseX < 50))
			myAngle = myAngle + (x - mouseX) * 1.0;
		if (abs(y - mouseY < 10))
			myVerticalAngle = myVerticalAngle + (y - mouseY) * 1.0;
	}
	mouseX = x;
	mouseY = y;
}

void MyMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		clickObject(myX, myZ, myAngle);
	}
}

void keyboard(unsigned char key, int x, int y) {
	float myoldX = myX, myoldZ = myZ, myoldAngle = myAngle;
	switch (key) {
	case 'f':
		clickObject(myX, myZ, myAngle);
		break;
	case 'd':
		myX += stepSize * cos((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
		myZ += stepSize * sin((myAngle + 90.0 + 90.0) / 360.0 * 2 * PI);
		break;
	case 'w':
		myX += stepSize * cos((myAngle + 90.0) / 360.0 * 2 * PI);
		myZ += stepSize * sin((myAngle + 90.0) / 360.0 * 2 * PI);
		break;
	case 'a':
		myX += stepSize * cos((myAngle - 90.0 + 90.0) / 360.0 * 2 * PI);
		myZ += stepSize * sin((myAngle - 90.0 + 90.0) / 360.0 * 2 * PI);
		break;
	case 's':
		myX += stepSize * cos((myAngle + 180.0 + 90.0) / 360.0 * 2 * PI);
		myZ += stepSize * sin((myAngle + 180.0 + 90.0) / 360.0 * 2 * PI);
		break;
	case 'q':
		myAngle -= 5.0;
		break;
	case 'e':
		myAngle += 5.0;
		break;
	case 'g':
		GenCity();
		break;
	case 'l':
		liftClick = true;
		break;
	case 27:
		exit(0);
	default: break;
	}

	currentLocation.x = myX;
	currentLocation.z = myZ;

	if (!allowedSpace(myX, myZ)) {
		myX = myoldX;
		myZ = myoldZ;
		myAngle = myoldAngle;
	}

	printf("%.2f %.2f %.2f\n", myX, myZ, myAngle);
	glutPostRedisplay();

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.01, 20000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0 + myX, 0.0, 5.0 + myZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/*

Function that draws sign for clickable

*/
void drawSign() {
	glColor3f(0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.0015, -0.0015, -0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.0015, 0.0015, -0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0015, 0.0015, -0.02);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0015, -0.0015, -0.02);
	glDisable(GL_TEXTURE_2D);
	glEnd();
}


void drawSky() {
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[5]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(-1600.0, 500, -1600.0);
	glTexCoord2d(0.0, 1.0); glVertex3f(-1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 1.0); glVertex3f(1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 0.0); glVertex3f(1600.0, 500, -1600.0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(-1600.0, -500, -1600.0);
	glTexCoord2d(0.0, 1.0); glVertex3f(-1600.0, -500, 1600.0);
	glTexCoord2d(1.0, 1.0); glVertex3f(-1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 0.0); glVertex3f(-1600.0, 500, -1600.0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(1600.0, -500, -1600.0);
	glTexCoord2d(0.0, 1.0); glVertex3f(1600.0, -500, 1600.0);
	glTexCoord2d(1.0, 1.0); glVertex3f(1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 0.0); glVertex3f(1600.0, 500, -1600.0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(-1600.0, -500, -1600.0);
	glTexCoord2d(0.0, 1.0); glVertex3f(-1600.0, 500, -1600.0);
	glTexCoord2d(1.0, 1.0); glVertex3f(1600.0, 500, -1600.0);
	glTexCoord2d(1.0, 0.0); glVertex3f(1600.0, -500, -1600.0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3f(-1600.0, -500, 1600.0);
	glTexCoord2d(0.0, 1.0); glVertex3f(-1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 1.0); glVertex3f(1600.0, 500, 1600.0);
	glTexCoord2d(1.0, 0.0); glVertex3f(1600.0, -500, 1600.0);
	glEnd();

	//glCallList(mySkyId);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void GenCity() {
	srand(time(NULL));
	int x, z;
	for (int i = 0; i < numofBuildings; i++) {

		while (1) {
			x = (rand() % cityArea) - 260 - cityArea / 2.0;
			z = (rand() % cityArea) - 260 - cityArea / 2.0;
			bool b;
			if (rand() % 2 == 0) {
				b = true;
			}
			else
				b = false;

			int size = rand() % 7 + 1;
			int floors = size;
			if (size == 3) floors = 4;
			if (size == 4) floors = 5;
			if (size >= 5) floors = 10;
			float width = 2.0 * floors;
			if (floors >= 4) {
				width /= 1.5;
			}
			else if (floors >= 10) {
				width /= 2.0;
			}

			if (checkIntersections(width, b, x, 0.0, z)) {
				city[i].isRec = b;
				city[i].x = x;
				city[i].z = z;
				city[i].size = size;
				mySpace instance(true, x, z, width);
				objectsSpace.push_back(instance);
				break;
			}
			else if (numofBuildings > 1000) {
				city[i].isRec = false;
				city[i].x = 0;
				city[i].z = 0;
				city[i].size = 0;
				break;
			}
		}

	}

}

void drawCity() {
	for (int i = 0; i < numofBuildings; i++) {
		glPushMatrix();
		drawBuilding(city[i].size, city[i].isRec, city[i].x, 0.0, city[i].z);
		glPopMatrix();
	}
}

bool checkIntersections(float width, bool b, float x, float y, float z) {
	width += 0.5;
	for (int i = 0; i < objectsSpace.size(); i++) {
		mySpace obj = objectsSpace[i];
		float x2 = obj.x;
		float z2 = obj.z;
		float width2 = obj.width;
		if (x - width < x2 + width2 && x - width > x2 - width2 && z - width < z2 + width2 && z - width> z2 - width2)
			return false;

		if (x - width < x2 + width2 && x - width > x2 - width2 && z + width < z2 + width2 && z + width> z2 - width2)
			return false;

		if (x + width < x2 + width2 && x + width > x2 - width2 && z + width < z2 + width2 && z + width> z2 - width2)
			return false;

		if (x + width < x2 + width2 && x + width > x2 - width2 && z - width < z2 + width2 && z - width> z2 - width2)
			return false;

		if (x2 - width2 < x + width && x2 - width2 > x - width && z2 - width2 < z + width && z2 - width2 > z - width)
			return false;

		if (x2 + width2 < x + width && x2 + width2 > x - width && z2 - width2 < z + width && z2 - width2 > z - width)
			return false;

		if (x2 + width2 < x + width && x2 + width2 > x - width && z2 + width2 < z + width && z2 + width2 > z - width)
			return false;

		if (x2 - width2 < x + width && x2 - width2 > x - width && z2 + width2 < z + width && z2 + width2 > z - width)
			return false;
	}
	return true;
}