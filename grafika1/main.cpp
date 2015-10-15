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

//--------------------------------------------------------
// 2D-s pontok, ketiranyba lancolt listakent hasznalhato.
//--------------------------------------------------------
struct Vector {
	float x, y;
	long t;
	Vector* next;
	Vector* previous;

	Vector() {
		x = y = 0;
		t = 0;
		next = previous = nullptr;
	}

	Vector(float x0, float y0) {
		x = x0; y = y0; t = 0;
		next = previous = nullptr;
	}

	Vector(float x0, float y0, float t0) {
		x = x0; y = y0; t = t0;
		next = previous = nullptr;
	}

	Vector operator*(float a) {
		return Vector(x * a, y * a);
	}
	Vector operator+(const Vector& p) {
		return Vector(x + p.x, y + p.y);
	}
	Vector operator+(float a) {
		return Vector(x + a, y + a);
	}
	Vector operator-(const Vector& p) {
		return Vector(x - p.x, y - p.y);
	}
	Vector operator/(float a) {
		return Vector(x / a, y / a);
	}
	float operator*(const Vector& p) { 	// dot product
		return (x * p.x + y * p.y);
	}
	float Length() {
		return sqrt(x * x + y * y);
	}
};

// alkalmazas ablak felbontasa
const int screenWidth = 600;
const int screenHeight = 600;

//Mezo es ablak meret arany
const float ratio = 1000.0f / 600.0f;

//A mezo merete meterben
const int fieldWidth = 1000;
const int fieldHeight = 1000;

//Pontokhoz tartozo konstansok
const int circlePoints = 10;
const float circleRadius = 5.0f;

//Nagyitas es eltolas merteke. A hatter rajzolasahoz kell
int zoom = 1;
int offsetX = 250;
int offsetY = 250;

//Lancolt lista elemek
Vector* root;
Vector* last;
int pointCount = 0;

//A parabola fokusz pontja
Vector* focus;

//A parabola es a spline elso metszespontja
Vector* intersection;

// egy alkalmazas ablaknyi kep
Color image[screenWidth*screenHeight];

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

//X koordinatat at konvertal mezo koordinatava
int convertPixelX(int x) {
	switch (zoom)
	{
	case 1:
		return x * ratio;
	case 2:
		return (x * ratio) / zoom + offsetX;
	default:
		throw "Invalid argument";
	}
}

//Y koordinatat atkonvertal mezo koordinatava
int convertPixelY(int y) {
	switch (zoom)
	{
	case 1:
		return fieldHeight - y * ratio;
	case 2:
		return fieldHeight - ((y * ratio) / zoom + offsetY);
	default:
		throw "Invalid argument";
	}
}

//Kontrol pont sebessegenek kiszamitasa
Vector getVelocity(Vector* p) {
	if (p == root) return (((*(p->next) - *p) / (p->next->t - p->t)) +
		((*p - *(p->previous)) / p->t)) / 2;

	return (((*(p->next) - *p)/(p->next->t - p->t)) + 
		((*p - *(p->previous))/(p->t - p->previous->t))) / 2;
}

//a3-as egyutthato
Vector a3(Vector* p) {
	float deltaT = p == last ? p->next->t : p->next->t - p->t;
	return
		(((*p - *(p->next)) * 2) / powf(deltaT, 3)) +
		((getVelocity(p->next) + getVelocity(p)) / powf(deltaT, 2));
}

//a2-es egyutthato
Vector a2(Vector* p) {
	float deltaT = p == last ? p->next->t : p->next->t - p->t;
	return
		(((*(p->next) - *p) * 3) / powf(deltaT, 2)) -
		((getVelocity(p->next) + getVelocity(p) * 2) / deltaT);
}

//a1-es egyutthato
Vector a1(Vector* p) {
	return getVelocity(p);
}

//a0-as egyutthato
Vector a0(Vector* p) {
	return *p;
}

//Ket pont kozottti Hermite interpolacio pontjait hatarozza meg.
Vector getHermiteCurve(Vector* p, float t) {
	float deltaT = t - p->t;
	return
		a3(p) * powf(deltaT, 3) +
		a2(p) * powf(deltaT, 2) +
		a1(p) * deltaT +
		a0(p);
}

Vector deriveSpline(Vector* p, float t) {
	float deltaT = t - p->t;
	return
		a3(p) * 3 * powf(deltaT, 2) +
		a2(p) * 2 * deltaT +
		a1(p);
}

Vector deriveParabola(Vector& directrix1, Vector& directrix2, Vector& focus, Vector& curvePoint) {
	Vector i = Vector(directrix2.x - directrix1.x, directrix2.y - directrix1.y);
	i = i / i.Length();

	Vector focusProjection = directrix1 + (i * ((focus - directrix1) * i));
	i = i * ((curvePoint - focusProjection) * i);
	i = i / i.Length();

	Vector n = Vector(i.y, -i.x);

	float t = (curvePoint - focus) * i;

	return i + n * 2 * t / (2 * fabsf((n * (focus - directrix1))));
}

//Felrajzolja a Catmull-Rom spline a pontokhoz.
void drawCatmullRom() {
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);

	Vector *current = root;
	for (int i = 0; i < (pointCount == 2 ? 1 : pointCount); i++)
	{

		float t2 = current == last ? current->t + current->next->t : current->next->t;
		float step = (t2 - current->t) / 1000.0f;

		for (float t = current->t; t < t2; t += step)
		{
			Vector curve = getHermiteCurve(current, t);

			float wx = (fieldWidth / 2 - curve.x) / -(fieldWidth / 2);
			float wy = (fieldHeight / 2 - curve.y) / (fieldHeight / 2);

			glVertex2f(wx, wy);
		}
		current = current->next;
	}

	glEnd();
}

void drawSplineTangent(Vector* controlPoint, float t, Vector& curvePoint) {

	Vector steep = deriveSpline(controlPoint, t);

	float m = steep.y / steep.x;
	float b = curvePoint.y + steep.y - m*(curvePoint.x + steep.x);

	float splineTangentX1 = 0;
	float splineTangentY1 = b;
	float splineTangentX2 = 1000.0f;
	float splineTangentY2 = m*splineTangentX2 + b;

	float wx1 = (fieldWidth / 2 - splineTangentX1) / -(fieldWidth / 2);
	float wy1 = (fieldHeight / 2 - splineTangentY1) / (fieldHeight / 2);
	float wx2 = (fieldWidth / 2 - splineTangentX2) / -(fieldWidth / 2);
	float wy2 = (fieldHeight / 2 - splineTangentY2) / (fieldHeight / 2);

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(wx1, wy1);
	glVertex2f(wx2, wy2);
	glEnd();
}

void drawParabolaTangent(Vector& directrix1, Vector& directrix2, Vector& focus, Vector& curvePoint) {
	Vector steep = deriveParabola(directrix1, directrix2, focus, curvePoint);

	float m = steep.y / steep.x;
	float b = curvePoint.y + steep.y - m*(curvePoint.x + steep.x);

	float splineTangentX1 = 0;
	float splineTangentY1 = b;
	float splineTangentX2 = 1000.0f;
	float splineTangentY2 = m*splineTangentX2 + b;

	float wx1 = (fieldWidth / 2 - splineTangentX1) / -(fieldWidth / 2);
	float wy1 = (fieldHeight / 2 - splineTangentY1) / (fieldHeight / 2);
	float wx2 = (fieldWidth / 2 - splineTangentX2) / -(fieldWidth / 2);
	float wy2 = (fieldHeight / 2 - splineTangentY2) / (fieldHeight / 2);

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(wx1, wy1);
	glVertex2f(wx2, wy2);
	glEnd();
}

bool isInParabola(Vector& directrix1, Vector& directrix2, Vector& focus, Vector& p) {
	Vector i = Vector(directrix2.x - directrix1.x, directrix2.y - directrix1.y);
	i = i / i.Length();
	Vector n = Vector(i.y, -i.x);

	return ((p - focus) * (p - focus) - (n * (p - directrix1)) * (n * (p - directrix1))) < 0;
}

//Megkeresi a parabola es a CM spline elso metszes pontjat (masodik es harmadik pont kozott)
void findFirstIntersection() {
	Vector cm1 = *(root->next);
	Vector cm2 = *(cm1.next);

	Vector directrix1 = *root;
	Vector directrix2 = *(root->next);
	Vector focus = *(root->next->next);

	float step = (cm2.t - cm1.t) / 1000.0f;

	float t = cm1.t;

	bool found = false;

	while (t < cm2.t && !found) {

		Vector curvePoint = getHermiteCurve(&cm1, t);

		if (isInParabola(directrix1, directrix2, focus, curvePoint)) {

			found = true;

			drawSplineTangent(&cm1, t, curvePoint);
			drawParabolaTangent(directrix1, directrix2, focus, curvePoint);
		}
		t += step;
	}
}

void drawParabola() {
	Vector directrix1 = *root;
	Vector directrix2 = *(root->next);
	Vector focus = *(root->next->next);

	for (int y = 0; y < 600; y++)
	{
		for (int x = 0; x < 600; x++)
		{
			Vector p = Vector(convertPixelX(x), convertPixelY(y));
			if (isInParabola(directrix1, directrix2, focus, p)) {
				image[y*screenWidth + x] = Color(1, 0, 0);
			}
			else {
				image[y*screenWidth + x] = Color(0, 0, 1);
			}
		}
	}

	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
	glViewport(0, 0, screenWidth, screenHeight);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay() {
	glClearColor(0, 0, 0, 1);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	//Parabola rajzolasa
	if (pointCount >= 3) {
		drawParabola();
	}

	//Catmull-rom spline rajzolasa
	if (pointCount >= 2) {
		drawCatmullRom();
	}

	//Pontok rajzolasa
	Vector *current = root;
	for (int i = 0; i < pointCount; i++)
	{
		drawCircle(current->x, current->y, circleRadius, circlePoints);
		current = current->next;
	}

	//Metszepont megkeresese
	if (pointCount >= 3) {
		findFirstIntersection();
	}

	glutSwapBuffers();// Buffercsere: rajzolas vege
}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {

	if (key == ' '  && zoom != 2) {
		// TODO zoom in and start moving, disable all inputs
		zoom = 2;
		gluOrtho2D(-0.5, 0.5, -0.5, 0.5);
	}

	if (key == 'w') {
		offsetY++;
		glTranslatef(0, -0.002f, 0);
	}
	if (key == 'a') {
		offsetX--;
		glTranslatef(0.002f, 0, 0);
	}
	if (key == 's') {
		offsetY--;
		glTranslatef(0, 0.002f, 0);
	}
	if (key == 'd') {
		offsetX++;
		glTranslatef(-0.002f, 0, 0);
	}
	glutPostRedisplay();

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {

	if (zoom == 2) return;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (pointCount == 0) {
			root = new Vector(x * ratio, y * ratio, glutGet(GLUT_ELAPSED_TIME));
			last = root;
		}
		else {
			Vector* previous = last;
			last->next = new Vector(x * ratio, y * ratio, glutGet(GLUT_ELAPSED_TIME));
			last = last->next;
			last->previous = previous;

			last->next = root;
			root->previous = last;
		}
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