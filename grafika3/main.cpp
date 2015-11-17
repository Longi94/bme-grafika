//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization fßggvÊnyt kivÊve, a lefoglalt adat korrekt felszabadítåsa nÊlkßl 
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
	Vector operator/(float a) {
		return Vector(x / a, y / a, z / a);
	}
	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { //dot
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%(const Vector& v) { //cross
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() { return sqrt(x * x + y * y + z * z); }
	Vector norm() {
		return Vector(x / Length(), y / Length(), z / Length());
	}
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

struct Camera {
	static const int XM = 600;
	static const int YM = 600;

	Vector eye, lookat, up;

	Camera(Vector eye, Vector lookat, Vector up) :eye(eye), lookat(lookat), up(up) {}
};

class Object {
	Vector position;
public:
	void fling(Vector dir) {

	}
};

//a CSIRGURU szeme
class CsirguruEye : public Object {

};

//a CSIRGURU csőre
class CsirguruBeak : public Object {

};

//a CSIRGURU taraja
class CsirguruComb : public Object {

};

//a CSIGURU teste
class CsirguruBody : public Object {

};

//a CSURGIRI lába
class CsirguruLeg : public Object {
	Object thigh, leg, foot, toe;
};

//sexy beast
class Csirguru {
protected:
	CsirguruEye eyeLeft, eyeRight;
	CsirguruBeak beak;
	CsirguruBody body;
	CsirguruComb comb;
	CsirguruLeg Leg;
public:
	void jump() {
		//TODO random irány, fancy animation
	}

	void explode(Vector center) {
		//TODO fling all body parts
	}
};

class Bomb : public Object {
public:
	void explode() {
		//loop through csirgurus
	}
};

class Field {
public:
	void draw() {
		glBegin(GL_QUADS);

		glColor3f(1, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);
		glVertex3f(10, 0, 10);
		glVertex3f(0, 0, 10);

		glEnd();
	}
};

class Scene {
protected:
	static const Vector AMBIENT_LIGHT;
	static const Vector SUN_LIGHT;

	Csirguru* csirgurus; //láncolt lista kéne

	Field field;
public:
	//static Camera camera;
	Scene() {}

	void init() {
		field = Field();
	}

	void draw() {
		field.draw();
	}

	void addCsirguru(Vector pos) {

	}

	void dropBomb(Vector pos) {

	}
};

const Vector Scene::AMBIENT_LIGHT = Vector(0.1f, 0.1f, 0.1f);
const Vector Scene::SUN_LIGHT = Vector();

Color image[Camera::XM*Camera::YM];

void glVertex3f(const Vector& v) {
	glVertex3f(v.x, v.y, v.z);
}

void glQuad(Vector& a, Vector& b, Vector& c, Vector& d) {
	Vector normal = ((b - a) % (c - a)).norm();
	glColor3f(fabs(normal.x), fabs(normal.y), fabs(normal.z));
	glVertex3f(a); glVertex3f(b); glVertex3f(c); glVertex3f(d);
}

void drawCube(Vector& size) {
	glBegin(GL_QUADS); {
		/*       (E)-----(A)
				 /|      /|
				/ |     / |
			  (F)-----(B) |
			   | (H)---|-(D)
			   | /     | /
			   |/      |/
			  (G)-----(C)        */

		Vector s = size / 2;

		Vector A(+s.x, +s.y, -s.z), B(+s.x, +s.y, +s.z), C(+s.x, -s.y, +s.z), D(+s.x, -s.y, -s.z),
			E(-s.x, +s.y, -s.z), F(-s.x, +s.y, +s.z), G(-s.x, -s.y, +s.z), H(-s.x, -s.y, -s.z);

		glQuad(A, B, C, D); glQuad(E, H, G, F); glQuad(A, E, F, B);
		glQuad(D, C, G, H); glQuad(B, F, G, C); glQuad(A, D, H, E);

	} glEnd();
}

Scene scene;

void onInitialization() {
	glViewport(0, 0, Camera::XM, Camera::YM);

	//Scene::camera = Camera(Vector(5, 10, 5), Vector(5, 0, 5), Vector(0, 0, -1));

	/*glMatrixMode(GL_PROJECTION);
	gluPerspective(90, 1, 0.1, 11);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(Scene::camera.eye.x, Scene::camera.eye.y, Scene::camera.eye.z,
		Scene::camera.lookat.x, Scene::camera.lookat.y, Scene::camera.lookat.z,
		Scene::camera.up.x, Scene::camera.up.y, Scene::camera.up.z);*/

	//Rendes 3d
	glEnable(GL_DEPTH_TEST);
	//Világítás engedélyezése
	glEnable(GL_LIGHTING);
	//Ambiens fény?
	glEnable(GL_COLOR_MATERIAL);


	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1, 0.1, 20);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(-3, 2, -1, 0, 0, 0, 0, 1, 0);

	scene = Scene();
	scene.init();
}

void onDisplay() {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glColor3f(1, 1, 1);
	drawCube(Vector(1, 1, 1));

	scene.draw();

	glutSwapBuffers();

}

void onKeyboard(unsigned char key, int x, int y) {
	if (key == 'd') glutPostRedisplay(); 		// d beture rajzold ujra a kepet

	switch (key)
	{
	case 'a':
		//TODO move crosshair left
		break;
	case 'w':
		//TODO move crosshair up
		break;
	case 'd':
		//TODO move crosshair right
		break;
	case /*'y'*/'s':
		//TODO move crosshair down
		break;
	case ' ':
		//TODO drop da bomb
		break;
	default:
		//nothing
		break;
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {

}

void onMouse(int button, int state, int x, int y) {

}

void onMouseMotion(int x, int y) {

}

void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME);
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
