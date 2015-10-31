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
#include <iostream>

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
	Color operator/(const Color& c) { 
		return Color(r / c.r, g / c.g, b / c.b); 
	}
	Color operator-(const Color& c) {
		return Color(r - c.r, g - c.g, b - c.b);
	}
};

const float c = 1.0f;

//Kibaszott sugar
struct Ray {
	Vector position, direction; //Kindulasi hely es iranya

	Ray() {

	}

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

	float shininess; //fenyesseg
public:
	virtual bool isReflective() = 0;
	virtual bool isRefractive() = 0; 
	virtual Color getDiffuseColor(Vector& position) = 0;
	virtual Color getShineColor(Vector& position) = 0;
	
	Vector reflect(Vector& direction, Vector& normal) {
		return direction - normal * (normal * direction) * 2.0f;
	}

	Vector refract(Vector& direction, Vector& normal) {
		return Vector();
	}

	/*Vector refract(Vector& direction, Vector& normal) {
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
	}*/

	Color shade(Vector& position, Vector& normal, Vector& viewDir, Vector& lightDir, Color& radIn) {
		Color radOut;

		float cosTheta = normal * lightDir; //Lambert trv
		if (cosTheta < 0) {
			return radOut;
		}

		Color kd = getDiffuseColor(position);

		radOut = radIn * kd * cosTheta;
		Vector halfway = (viewDir + lightDir).norm();

		float cosDelta = normal * halfway;
		if (cosDelta < 0) {
			return radOut;
		}

		Color ks = getShineColor(position);

		return radOut + radIn * ks * powf(cosDelta, shininess);
	}

	Color Fresnel(Vector& direction, Vector& normal) {
		float cosalpha = fabsf(normal * direction);
		return F0 + (Color(1, 1, 1) - F0) * powf(1 - cosalpha, 5);
	}
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
	Color kd, ks;
public:
	bool isReflective() {
		return true;
	}

	bool isRefractive() {
		return true;
	}

	Color getDiffuseColor(Vector& position) {
		return kd;
	}

	Color getShineColor(Vector& position) {
		return ks;
	}
};

//Rucskos anyag, a sugar szetverodik ezen, innen jon a szin
class RoughMaterial : public Material {
	Color kd, ks;
public:
	RoughMaterial() {

	}

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

	Color getDiffuseColor(Vector& position) {
		return kd;
	}

	Color getShineColor(Vector& position) {
		return ks;
	}
};

//Gooooolden
class GoldMaterial : public Material {
	Color k = Color(3.1f, 2.7f, 1.9f);
public:
	GoldMaterial() {
		this->n = Color(0.17f, 0.35f, 1.5f);
		this->F0 = (n - Color(1, 1, 1) * (n - Color(1, 1, 1)) + k*k) / ((n + Color(1, 1, 1))*(n + Color(1, 1, 1)) + k*k);
	}

	bool isReflective() {
		return true;
	}
	bool isRefractive() {
		return false;
	}

	Color getDiffuseColor(Vector& position) {
		return Color(1, 0.84f, 0); //gold
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

class GlassMaterial : public Material {
	Color k = Color(0, 0, 0);
public:
	GlassMaterial() {
		this->n = Color(1.5, 1.5, 1.5);
		this->F0 = (n - Color(1, 1, 1) * (n - Color(1, 1, 1)) + k*k) / ((n + Color(1, 1, 1))*(n + Color(1, 1, 1)) + k*k);
	}

	bool isReflective() {
		return true;
	}
	bool isRefractive() {
		return true;
	}

	Color getDiffuseColor(Vector& position) {
		return Color(1, 1, 1); //gold
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

class ConcentricCircles : public Material {
	Vector center;
	float bandWidth;
	Color c1, c2;
public:
	ConcentricCircles(Vector center, float bandWidth, Color c1, Color c2) {
		this->center = center;
		this->bandWidth = bandWidth;
		this->c1 = c1;
		this->c2 = c2;
	}
	bool isReflective() {
		return false;
	}
	bool isRefractive() {
		return false;
	}

	Color getDiffuseColor(Vector& position) {
		float d = (position - center).Length();

		if (fmodf(d, bandWidth) > (bandWidth / 2.0)) {
			return c2;
		}
		else {
			return c1;
		}
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

//Utkozo kepes anyagok ososztalya
class Intersectable {
protected:
	Material* material;
public:
	virtual Hit intersect(Ray& ray) = 0;
};

//Sik
class Plane : public Intersectable {
	Vector position, normal;
public:
	Plane() {

	}

	Plane(Vector position, Vector normal, Material* material) {
		this->position = position;
		this->normal = normal;
		this->material = material;
	}

	Hit intersect(Ray& ray) {

		float dot = normal * ray.direction;

		if (dot == 0) { //sugar parhozamos a sikkal
			return Hit();
		}

		Hit hit = Hit();
		hit.material = material;
		hit.normal = normal;

		//https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Algebraic_form
		float x = ((position - ray.position) * normal) / dot;
		if (x < 0) {
			return Hit(); //A metszett hatrafele van
		}
		Vector intersection = ray.direction * x + ray.position;

		hit.position = intersection;

		float d = (intersection - ray.position).Length();

		hit.t = d / c;

		return hit;
	}
};

//Ellipsoid
class Ellipsoid : public Intersectable {

public:
	Hit intersect(Ray& ray) {
		// TODO
	}
};

//Paraboloid
class Paraboloid : public Intersectable {
	Vector point, normal, focus;
public:
	Hit intersect(Ray& ray) {
		// TODO
	}
};

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

	Ray getRay(float x, float y) {
		Vector pixel = lookat + right * (2 * x / XM - 1) + up * (2 * y / YM - 1);
		Vector direction = pixel - eye;
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

RoughMaterial roughRed;
RoughMaterial roughBlue;
RoughMaterial roughYellow;
RoughMaterial roughCyan;
RoughMaterial roughMagenta;

ConcentricCircles wallBottomMaterial(Vector(5, 0, 5), 1.0f, Color(1, 0, 0), Color(1, 1, 1));

GoldMaterial goldMaterial;

LightSource light;

void build() {
	//camera init
	camera = Camera();
	camera.eye = Vector(5, 5, 0.1f);
	camera.lookat = Vector(5, 5, 2);
	camera.up = Vector(0, 2, 0);
	camera.right = Vector(-2, 0, 0);

	roughRed = RoughMaterial(Color(1, 0, 0), Color(1, 0, 0), 0);
	roughBlue = RoughMaterial(Color(0, 0, 1), Color(1, 0, 0), 0);
	roughYellow = RoughMaterial(Color(1, 1, 0), Color(1, 0, 0), 0);
	roughCyan = RoughMaterial(Color(0, 1, 1), Color(1, 0, 0), 0);
	roughMagenta = RoughMaterial(Color(1, 0, 1), Color(1, 0, 0), 0);

	//flat walls init
	wallLeft = Plane(Vector(10, 10, 10), Vector(-1, 0, 0), &roughRed);
	wallRight = Plane(Vector(0, 0, 0), Vector(1, 0, 0), &goldMaterial);
	wallFront = Plane(Vector(10, 10, 10), Vector(0, 0, -1), &roughBlue);
	wallBack = Plane(Vector(0, 0, 0), Vector(0, 0, 1), &roughYellow);
	wallTop = Plane(Vector(10, 10, 10), Vector(0, -1, 0), &roughCyan);
	wallBottom = Plane(Vector(0, 0, 0), Vector(0, 1, 0), &wallBottomMaterial);

	objects[0] = &wallLeft;
	objects[1] = &wallRight;
	objects[2] = &wallFront;
	objects[3] = &wallBack;
	objects[4] = &wallTop;
	objects[5] = &wallBottom;

	//light init
	light.color = Color(1, 1, 1);
	light.position = Vector(5, 5, 5);
}

Hit firstIntersect(Ray ray) {
	Hit firstHit;

	for (Intersectable* obj : objects) {
		if (obj != 0) {
			Hit hit = obj->intersect(ray);

			if (hit.t > 0 && (firstHit.t < 0 || firstHit.t > hit.t)) {
				firstHit = hit;
			}
		}
	}

	return firstHit;
}

const int maxDepth = 10;

Color trace(Ray ray, int depth) {

	Color outRadiance;

	if (depth > maxDepth) {
		return outRadiance;
	}

	Hit hit = firstIntersect(ray);
	if (hit.t < 0) {
		return outRadiance;
	}

	Vector lightDir = light.position - hit.position;

	Ray shadowRay = Ray(hit.position, lightDir);
	Hit shadowHit = firstIntersect(shadowRay);
	if (shadowHit.t < 0 || shadowHit.t > lightDir.Length()) {
		//lightDist = (hit.position - light.position).Length();
		outRadiance = outRadiance + hit.material->shade(hit.position, hit.normal, ray.direction, lightDir.norm(), light.color); //nem jooo
	}

	if (hit.material->isReflective()) {
		Vector reflectionDir = hit.material->reflect(ray.direction, hit.normal);
		Ray reflectedRay(hit.position, reflectionDir);
		outRadiance = outRadiance + trace(reflectedRay, depth + 1) * hit.material->Fresnel(ray.direction, hit.normal);
	}
	if (hit.material->isRefractive()) {
		Vector reflectionDir = hit.material->refract(ray.direction, hit.normal);
		Ray refractedRay(hit.position, reflectionDir);
		outRadiance = outRadiance + trace(refractedRay, depth + 1) * (Color(1, 1, 1) - hit.material->Fresnel(ray.direction, hit.normal));
	}

	return outRadiance;
}

void onInitialization()
{
	glViewport(0, 0, Camera::XM, Camera::YM);
	build();

	for (int Y = 0; Y < Camera::XM; Y++)
		for (int X = 0; X < Camera::YM; X++) {
			Ray ray = camera.getRay(X, Y);
			image[Y*Camera::XM + X] = trace(ray, 0);
		}
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
