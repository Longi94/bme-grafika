//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization f??ggv?©nyt kiv?©ve, a lefoglalt adat korrekt felszabad?­t??sa n?©lk??l 
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
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
	Color operator/(const Color& c) { 
		return Color(r / c.r, g / c.g, b / c.b); 
	}
	Color operator-(const Color& c) {
		return Color(r - c.r, g - c.g, b - c.b);
	}
};

//Kibaszott sugar
struct Ray {
	Vector position, direction; //Kindulasi hely es iranya

	Ray(Vector position, Vector direction) {
		this->position = position;
		this->direction = direction;
	}
};

//Anyag interfesz (abstract)
class Material {
protected:
	//Smooth
	Color F0; //Fresnel, anyagra jellemzo const: ((n-1)*(n-1) + k*k) / ((n+1)*(n+1) + k*k)
	Color n; //toresmutato

	//Rough
	Color kd, ks; //diffuz tul (szin)
	float shininess; //fenyesseg
public:
	virtual bool isReflective() = 0;
	virtual bool isRefractive() = 0;
	virtual Vector reflect(Vector& direction, Vector& normal) = 0;
	virtual Vector refract(Vector& direction, Vector& normal) = 0;
	virtual Color shade(Vector& normal, Vector& viewDir, Vector& lightDir, Color& radIn) = 0;
	virtual Color Fresnel(Vector& direction, Vector& normal) = 0;
};

//Talalat!!
struct Hit {
	float t; //Ido
	Vector position, normal; //Az utkozes poxicioja, es a felulet normal vektora
	Material *material; //A felulet anyaga

	Hit() {
		t = -1;
	}
};

//Sima felulet, a sugar siman visszaverodik es/vagy torik
class SmoothMaterial : public Material {
public:
	SmoothMaterial(Color n, Color k) {
		this->n = n;
		this->F0 = (n - Color(1, 1, 1) * (n - Color(1, 1, 1)) + k*k) / ((n + Color(1, 1, 1))*(n + Color(1, 1, 1)) + k*k);
	}

	bool isReflective() {
		return true;
	}

	bool isRefractive() {
		return true;
	}

	Vector reflect(Vector& direction, Vector& normal) {
		return direction - normal * (normal * direction) * 2.0f;
	}

	Vector refract(Vector& direction, Vector& normal) {
		Color ior = n;

		float cosalpha = -(normal * direction);
		if (cosalpha < 0) {
			cosalpha = -cosalpha;
			normal = normal * (-1);
			ior = 1.0 / n;
		}

		float disc = 1 - (1 - powf(cosalpha, 2)) / powf(ior, 2);
		if (disc < 0) {
			return reflect(direction, normal);
		}
		return direction / ior + normal * (cosalpha / ior - sqrtf(disc));
	}

	Color Fresnel(Vector& direction, Vector& normal) {
		float cosalpha = fabsf(normal * direction);
		return F0 + (Color(1, 1, 1) - F0) * powf(1 - cosalpha, 5);
	}

	Color shade(Vector& normal, Vector& viewDir, Vector& lightDir, Color& radIn) {
		return (Color(0, 0, 0)); //unused
	}
};

//Rucskos anyag, a sugar szetverodik ezen, innen jon a szin
class RoughMaterial : public Material {
public:
	RoughMaterial(Color kd, Color ks, float shininess) {
		this->kd = kd;
		this->ks = ks;
		this->shininess = shininess;
	}

	bool isReflective() {
		return false;
	}

	bool isRefractive() {
		return false;
	}

	Vector reflect(Vector& direction, Vector& normal) {
		return Vector(0, 0, 0); //unused
	}

	Vector refract(Vector& direction, Vector& normal) {
		return Vector(0, 0, 0); //unused
	}

	Color Fresnel(Vector& direction, Vector& normal) {
		return Color(0, 0, 0); //unused
	}

	Color shade(Vector& normal, Vector& viewDir, Vector& lightDir, Color& radIn) {
		Color radOut = Color(0, 0, 0);

		float cosTheta = normal * lightDir; //Lambert trv
		if (cosTheta < 0) {
			return radOut;
		}

		radOut = radIn * kd * cosTheta;
		Vector halfway = (normal + halfway).norm();

		float cosDelta = normal * halfway;
		if (cosDelta < 0) {
			return radOut;
		}

		return radOut + radIn * ks * powf(cosDelta, shininess);
	}
};

//Utkozo kepes anyagok ososztalya
class Intersectable {
protected:
	Material* material;
public:
	virtual Hit intersect(const Ray& ray) = 0;
};

//Sik
class Plane : public Intersectable {
	Vector position, normal;
public:
	Plane(Vector position, Vector normal, Material* material) {
		this->position = position;
		this->normal = normal;
		this->material = material;
	}

	Hit intersect(const Ray& ray) {

	}
};

//Ellipsoid
class Ellipsoid : public Intersectable {

public:
	Hit intersect(const Ray& ray) {
		// TODO
	}
};

//Ellipsoid
class Paraboloid : public Intersectable {
	Vector point, normal, focus;
public:
	Hit intersect(const Ray& ray) {
		// TODO
	}
};

const float c = 1.0f;

// Fenyforras
struct LightSource {
	Vector position;
	Color color;
};

struct Camera {
	static const int XM = 600;
	static const int YM = 600;

	Vector eye;
	Vector lookat;

	Vector up;
	Vector right;

	Ray getRay(int x, int y) {
		Vector pixel = lookat + right * (2 * x / XM - 1) + up * (2 * x / XM - 1);
		Vector direction = pixel = eye;
		return Ray(eye, direction);
	}
};

Color image[Camera::XM*Camera::YM];

Camera camera;
Intersectable* objects[7];

Plane wallLeft;
Plane wallRight;
Plane wallFront;
Plane wallBack;
Plane wallTop;
Plane wallBottom;

RoughMaterial rough;

void build() {
	//camera init
	camera = Camera();
	camera.eye = Vector(5, 5, 0);
	camera.lookat = Vector(5, 5, 2);
	camera.up = Vector(0, 1, 0);
	camera.right = Vector(-1, 0, 0);

	rough = RoughMaterial();

	//flat walls init
	wallLeft = Plane(Vector(10, 10, 10), Vector(-1, 0, 0));
}

Hit firstIntersect(Ray ray) {
	Hit firstHit;

	return firstHit;
}

Color trace(Ray& ray) {
	return Color(0, 0, 0);
}

void onInitialization()
{
	glViewport(0, 0, Camera::XM, Camera::YM);
	build();
}

void onDisplay()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawPixels(Camera::XM, Camera::YM, GL_RGB, GL_FLOAT, image);

	glutSwapBuffers();

}

void onKeyboard(unsigned char key, int x, int y)
{

}

void onKeyboardUp(unsigned char key, int x, int y)
{

}

void onMouse(int button, int state, int x, int y)
{

}

void onMouseMotion(int x, int y)
{

}

void onIdle()
{

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
