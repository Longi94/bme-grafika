//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization fﬂggv nyt kiv ve, a lefoglalt adat korrekt felszabadÌtÂsa n lkﬂl 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Tran Thanh Long
// Neptun : K4H9ZB
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
	float x, y, z;

	Vector() {
		x = y = z = 0;
	}
	Vector(float x0, float y0, float z0 = 0) {
		x = x0; y = y0; z = z0;
	}
	Vector operator*(float a) {
		return Vector(x * a, y * a, z * a);
	}
	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { 	// dot product
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%(const Vector& v) { 	// cross product
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() { return sqrt(x * x + y * y + z * z); }
};

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
	float r, g, b;

	Color() {
		r = g = b = 0;
	}
	Color(float r0, float g0, float b0) {
		r = r0; g = g0; b = b0;
	}
	Color operator*(float a) {
		return Color(r * a, g * a, b * a);
	}
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
};

struct Point {
	float x;
	float y;
	float vx;
	float vy;
	long t;
	Point *next;
};

const int screenWidth = 600;	// alkalmazas ablak felbontasa
const int screenHeight = 600;

const float ratio = 1000.0f / 600.0f;

const int fieldWidth = 1000;
const int fieldHeight = 1000;

//Pontokhoz tartozo konstansok
const int circlePoints = 10;
const float circleRadius = 5.0f;

int zoom = 1;
int offsetX = 250;
int offsetY = 250;

//Lancolt lista elemek
Point *root;
Point *last;
int pointCount = 0;

//A parabola fokusz pontja
Point *focus;

Color image[screenWidth*screenHeight];	// egy alkalmazas ablaknyi kep

//Feher korvonalu piros pottyot rajzol a megadott parameterek alapjan
void drawCircle(float cx, float cy, float r, int segments)
{
	float wx = (fieldWidth / 2 - cx) / -(fieldWidth / 2);
	float wy = (fieldHeight / 2 - cy) / (fieldHeight / 2);
	float wr = r / 1000.0f;

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < segments; i++)
	{
		float theta = 2.0f * M_PI * float(i) / float(segments);//get the current angle 

		float x = wr * cosf(theta);//calculate the x component 
		float y = wr * sinf(theta);//calculate the y component 

		glVertex2f(x + wx, y + wy);//output vertex 
	}
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		float theta = 2.0f * M_PI * float(i) / float(segments);//get the current angle 

		float x = wr * cosf(theta);//calculate the x component 
		float y = wr * sinf(theta);//calculate the y component 

		glVertex2f(x + wx, y + wy);//output vertex 
	}
	glEnd();
}

int convertParabolaX(int x) {
	switch (zoom)
	{
	case 1:
		return x * ratio;
	case 2:
		return (x * ratio) / zoom + offsetX;
	}
}

int convertParabolaY(int y) {
	switch (zoom)
	{
	case 1:
		return fieldHeight - y * ratio;
	case 2:
		return fieldHeight - ((y * ratio) / zoom + offsetY);
	}
}

int convertX(int x) {
	switch (zoom)
	{
	case 1:
		return x;
	case 2:
		return (x - offsetX) * zoom;
	}
}

int convertY(int y) {
	switch (zoom)
	{
	case 1:
		return y;
	case 2:
		return (y - (500 - offsetY)) * zoom;
	}
}


//Pont vonaltol valo tavolsagat szamitja ki
float distanceFromLine(float x1, float y1, float x2, float y2, float px, float py) {
	return fabsf((y2 - y1)*px - (x2 - x1)*py + x2*y1 - y2*x1) / sqrtf(powf(y2 - y1, 2) + powf(x2 - x1, 2));
}

//Pont ponttol valo tavolsagat szamitja ki
float distanceFromPoint(float x1, float y1, float x2, float y2) {
	return sqrtf(powf(y2 - y1, 2) + powf(x2 - x1, 2));
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
	glViewport(0, 0, screenWidth, screenHeight);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay() {
	glClearColor(0, 0, 0, 1);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	if (pointCount >= 3) {

		//There is a parabola
		Point *p1 = root;
		Point *p2 = root->next;
		Point *focus = root->next->next;

		for (int y = 0; y < 600; y++)
		{
			for (int x = 0; x < 600; x++)
			{
				if (distanceFromPoint(focus->x, focus->y, convertParabolaX(x), convertParabolaY(y)) > 
					distanceFromLine(p1->x, p1->y, p2->x, p2->y, convertParabolaX(x), convertParabolaY(y))) {
					image[y*screenWidth + x] = Color(0, 0.5f, 0.5f);
				}
				else {
					image[y*screenWidth + x] = Color(1, 1, 0);
				}
			}
		}

		glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
	}

	Point *current = root;
	for (int i = 0; i < pointCount; i++)
	{
		drawCircle(convertX(current->x), convertY(current->y), circleRadius * zoom, circlePoints);
		current = current->next;
	}

	glutSwapBuffers();// Buffercsere: rajzolas vege
}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	
	if (key == ' ') {
		// TODO zoom in and start moving, disable all inputs
		zoom = 2;
		glutPostRedisplay();
	}

	if (key == 'w') {
		offsetY++;
	}
	if (key == 'a') {
		offsetX--;
	}
	if (key == 's') {
		offsetY--;
	}
	if (key == 'd') {
		offsetX++;
	}
	glutPostRedisplay();

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (pointCount == 0) {
			root = new Point;
			last = root;
		} else {
			last->next = new Point;
			last = last->next;
		}

		last->x = x * ratio;
		last->y = y * ratio;
		last->t = glutGet(GLUT_ELAPSED_TIME);
		pointCount++;

		if (pointCount == 3) {
			focus = last;
		}

		glutPostRedisplay();
	}
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
	glutInit(&argc, argv); 				// GLUT inicializalasa
	glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
	glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

	glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

	glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();

	onInitialization();					// Az altalad irt inicializalast lefuttatjuk

	glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);

	glutMainLoop();					// Esemenykezelo hurok

	return 0;
}
