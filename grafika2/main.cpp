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

const float LIGHT_C = 1.0f;
const float EPSILON = 0.0001f;

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
	Color operator/(float a) {
		return Color(r / a, g / a, b / a);
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

const Color AMBIENT(0.1f, 0.1f, 0.1f);

//Kibaszott sugar
struct Ray {
	Vector position, direction; //Kindulasi hely es iranya

	Ray(Vector position, Vector direction) {
		this->position = position;
		this->direction = direction.norm();
	}
};

//Anyag interfesz (abstract)
class Material {
protected:
	//Smooth
	Color F0; //Fresnel, anyagra jellemzo const: ((n-1)*(n-1) + k*k) / ((n+1)*(n+1) + k*k)
	Color N; //toresmutato

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
		float n = (N.r + N.g + N.b) / 3.0f;
		float ior = n;

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

//Gooooolden
class GoldMaterial : public Material {
public:
	GoldMaterial() {
		Color k = Color(3.1f, 2.7f, 1.9f);
		this->N = Color(0.17f, 0.35f, 1.5f);
		this->F0 = (N - Color(1, 1, 1) * (N - Color(1, 1, 1)) + k*k) / ((N + Color(1, 1, 1))*(N + Color(1, 1, 1)) + k*k);
	}

	bool isReflective() {
		return true;
	}
	bool isRefractive() {
		return false;
	}

	Color getDiffuseColor(Vector& position) {
		return Color(); //gold
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

class GlassMaterial : public Material {
public:
	GlassMaterial() {
		Color k = Color(0, 0, 0);
		this->N = Color(1.5, 1.5, 1.5);
		this->F0 = (N - Color(1, 1, 1) * (N - Color(1, 1, 1)) + k*k) / ((N + Color(1, 1, 1))*(N + Color(1, 1, 1)) + k*k);
	}

	bool isReflective() {
		return true;
	}
	bool isRefractive() {
		return true;
	}

	Color getDiffuseColor(Vector& position) {
		return Color();
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

class Checkers : public Material {
	float squareSize;
	Color c1, c2;
public:
	Checkers(float bandWidth, Color c1, Color c2) {
		this->squareSize = bandWidth;
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

		if ((fmodf(position.x, squareSize * 2) > squareSize) == (fmodf(position.z, squareSize * 2) > squareSize)) {
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

class Wonky : public Material {
	Vector center;
	Color c1, c2;
public:
	Wonky(Vector center, Color c1, Color c2) {
		this->center = center;
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
		float x = (position.x - center.x) * 1.5f;
		float y = (position.y - center.y) * 1.5f;

		float n = sinf(x + y) + sinf(x * y) - fabsf(sinf(x * x + y * y));

		if (n > 0) {
			return c1;
		}
		else {
			return c2;
		}
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

class Blobs : public Material {
	Vector center;
	Color c1, c2;
public:
	Blobs(Vector center, Color c1, Color c2) {
		this->center = center;
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
		float x = (position.z - center.z) * 3;
		float y = (position.y - center.y) * 3;

		float n = sinf(sinf(x) + cosf(y)) - cosf(sinf(x * y) + cosf(x));

		if (n > 0) {
			return c1;
		}
		else {
			return c2;
		}
	}
	Color getShineColor(Vector& position) {
		return Color();
	}
};

class CheckersDiagonal : public Material {
	Color c1, c2;
public:
	CheckersDiagonal(Color c1, Color c2) {
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
		float x = position.x * 3;
		float y = position.y * 3;

		float n = sin(x) - cos(y);

		if (n > 0) {
			return c1;
		}
		else {
			return c2;
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
	virtual Hit intersect(Ray& ray, float elapsedTime) = 0;
};

//Sik
class Plane : public Intersectable {
	Vector position, normal;
public:
	Plane() {

	}

	Plane(Vector position, Vector normal, Material* material) {
		this->position = position;
		this->normal = normal.norm();
		this->material = material;
	}

	Hit intersect(Ray& ray, float elapsedTime) {

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

		hit.t = d / LIGHT_C;

		return hit;
	}
};

//http://www.bmsc.washington.edu/people/merritt/graphics/quadrics.html
class QuadricSurfaces : public Intersectable {
protected:
	float A, B, C, D, E, F, G, H, I, J;
public:
	Hit intersect(Ray& ray, float elapsedTime) {
		float Aq = A * powf(ray.direction.x, 2) + B * powf(ray.direction.y, 2) + C * powf(ray.direction.z, 2) +
			D * ray.direction.x * ray.direction.y +
			E * ray.direction.x * ray.direction.z +
			F * ray.direction.y * ray.direction.z;

		float Bq = 2 * A * ray.position.x * ray.direction.x + 2 * B * ray.position.y * ray.direction.y + 2 * C * ray.position.z * ray.direction.z +
			D * (ray.position.x * ray.direction.y + ray.position.y * ray.direction.x) +
			E * (ray.position.x * ray.direction.z + ray.position.z * ray.direction.x) +
			F * (ray.position.y * ray.direction.z + ray.position.z * ray.direction.y) +
			G * ray.direction.x + H * ray.direction.y + I * ray.direction.z;

		float Cq = A * powf(ray.position.x, 2) + B * powf(ray.position.y, 2) + C * powf(ray.position.z, 2) +
			D * ray.position.x * ray.position.y +
			E * ray.position.x * ray.position.z +
			F * ray.position.y * ray.position.z +
			G * ray.position.x + H * ray.position.y + I * ray.position.z + J;

		float disc = Bq * Bq - 4 * Aq * Cq;

		float t;
		if (fabsf(Aq) < EPSILON) {
			t = -Cq / Bq;
		}
		else {
			if (disc < 0) {
				return Hit();
			}

			t = (-Bq - sqrtf(disc)) / (2 * Aq);

			if (t < 0) {
				t = (-Bq + sqrtf(disc)) / (2 * Aq);
			}
		}

		Vector intersection = ray.position + ray.direction.norm() * t;
		Vector normal = Vector(2 * A * intersection.x + D * intersection.y + E * intersection.z + G,
			2 * B * intersection.y + D * intersection.x + F * intersection.z + H,
			2 * C * intersection.z + E * intersection.x + F * intersection.y + I).norm();

		Hit hit;
		hit.normal = normal;
		hit.position = intersection;
		hit.t = t;
		hit.material = material;

		return hit;
	}
};

class Ellipsoid : public QuadricSurfaces {
	Vector center, i, velocity;
	float a, b;
public:
	Ellipsoid() {

	}

	Ellipsoid(Vector center, float a, float b, Vector i, Vector velocity, Material* material) {
		this->center = center;
		this->velocity = velocity;
		this->a = a;
		this->b = b;
		this->i = i;
		this->material = material;

		float f = sqrtf(a*a - b*b);

		Vector F1 = center + i*f;
		Vector F2 = center - i*f;

		float x1 = F1.x;
		float y1 = F1.y;
		float z1 = F1.z;
		float x2 = F2.x;
		float y2 = F2.y;
		float z2 = F2.z;

		float F1LS = F1.Length() * F1.Length();
		float F2LS = F2.Length() * F2.Length();

		A = powf(x1 - x2, 2) - 4 * a*a;
		B = powf(y1 - y2, 2) - 4 * a*a;
		C = powf(z1 - z2, 2) - 4 * a*a;
		D = 2 * (x1 - x2) * (y1 - y2);
		E = 2 * (x1 - x2) * (z1 - z2);
		F = 2 * (y1 - y2) * (z1 - z2);
		G = 4 * a*a*(x1 + x2) + F1LS * (x2 - x1) + F2LS * (x1 - x2);
		H = 4 * a*a*(y1 + y2) + F1LS * (y2 - y1) + F2LS * (y1 - y2);
		I = 4 * a*a*(z1 + z2) + F1LS * (z2 - z1) + F2LS * (z1 - z2);
		J = (powf(F1LS - F2LS, 2) / 4.0f) + 2 * a * a * (2 * a*a - F1LS + F2LS - 2 * (x2*x2 + y2*y2 + z2*z2));

		if (A != 0) {
			B /= A;
			C /= A;
			D /= A;
			E /= A;
			F /= A;
			G /= A;
			H /= A;
			I /= A;
			J /= A;
			A /= A;
		}
	}

	Hit intersect(Ray& ray, float elapsedTime) {

		Vector p = ray.position - (velocity * elapsedTime);
		Vector d = ray.direction * LIGHT_C + velocity;

		return QuadricSurfaces::intersect(Ray(p, d), elapsedTime);
	}
};

class Paraboloid : public QuadricSurfaces {
public:
	Paraboloid() {

	}
	Paraboloid(Vector center, float a, Material* material) {
		this->material = material;
		A = D = E = F = 0;
		B = C = 1;

		G = a*a;
		H = -2 * center.y;
		I = -2 * center.z;
		J = -a*a*center.x + center.y*center.y + center.z*center.z;
	}
};

// Fenyforras
struct LightSource {
	Vector position, velocity;
	Color color;
	float power;

	LightSource(float power = 20) {
		this->power = power;
	}

	Color getLuminance(Vector& position) {
		float d = (this->position - position).Length();
		return (color / (d*d)) * power;
	}
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

Vector ELLIPSOID_START_POS(9, 1, 9);
Vector ELLIPSOID_SPEED = Vector(-1, 1, -1).norm() * 0.5f;
Vector LIGHT_SOURCE_START_POS(4, 7, 6);
Vector LIGHT_SOURCE_SPEED = Vector(1, 0, -1).norm() * 0.1f;
Vector CAMERA_POS(9.5f, 1, 0.5f);
Vector CAMERA_LOOK_AT_THIS_POINT(5, 5, 5);

Color image[Camera::XM*Camera::YM];

Camera camera;
Intersectable* objects[7];

Plane wallLeft;
Plane wallFront;
Plane wallBack;
Plane wallTop;
Plane wallBottom;

Ellipsoid ellipsoid;
Paraboloid paraboloid;

ConcentricCircles concetricCircles(Vector(5, 0, 5), 1.0f, Color(0.32f, 0.18f, 0.66f), Color(1, 1, 1));
Checkers checkers(1.0f, Color(0.32f, 0.18f, 0.66f), Color(1, 1, 1));
CheckersDiagonal checkersDiagonal(Color(0.32f, 0.18f, 0.66f), Color(1, 1, 1));
Wonky wonky(Vector(5, 5, 10), Color(0.32f, 0.18f, 0.66f), Color(1, 1, 1));
Blobs blobs(Vector(10, 5, 5), Color(0.32f, 0.18f, 0.66f), Color(1, 1, 1));

GoldMaterial goldMaterial;
GlassMaterial glassMaterial;

LightSource light;

void init() {
	//camera init
	Vector up = Vector(0, 1, 0);

	camera = Camera();
	camera.eye = CAMERA_POS;
	camera.lookat = CAMERA_LOOK_AT_THIS_POINT;

	//magic
	camera.lookat = ((camera.lookat - camera.eye).norm() * 2) + camera.eye;
	camera.right = (camera.lookat - camera.eye) % up;
	camera.right = camera.right.norm() * 2;
	camera.up = camera.right % (camera.lookat - camera.eye);
	camera.up = camera.up.norm() * 2;

	//flat walls init
	wallLeft = Plane(Vector(10, 10, 10), Vector(-1, 0, 0), &blobs);
	wallFront = Plane(Vector(10, 10, 10), Vector(0, 0, -1), &wonky);
	wallBack = Plane(Vector(0, 0, 0), Vector(0, 0, 1), &checkersDiagonal);
	wallTop = Plane(Vector(10, 10, 10), Vector(0, -1, 0), &checkers);
	wallBottom = Plane(Vector(0, 0, 0), Vector(0, 1, 0), &concetricCircles);

	paraboloid = Paraboloid(Vector(2, 5, 5), sqrtf(25), &goldMaterial);

	objects[0] = &wallLeft;
	objects[1] = &paraboloid;
	objects[2] = &wallFront;
	objects[3] = &wallBack;
	objects[4] = &wallTop;
	objects[5] = &wallBottom;
}

Hit firstIntersect(Ray ray, float elapsedTime) {
	Hit firstHit;

	for (int i = 0; i < 7; i++) {
		Intersectable* obj = objects[i];
		if (obj != 0) {
			Hit hit = obj->intersect(ray, elapsedTime);

			if (hit.t > 0 && (firstHit.t < 0 || firstHit.t > hit.t)) {
				firstHit = hit;
			}
		}
	}

	return firstHit;
}

const int MAX_DEPTH = 10;

Color trace(Ray ray, int depth, float elapsedTime) {

	if (depth > MAX_DEPTH) {
		return AMBIENT;
	}

	Hit hit = firstIntersect(ray, elapsedTime);
	if (hit.t < 0) {
		return AMBIENT;
	}

	float hitTime = elapsedTime - hit.t;

	Color outRadiance;

	Vector lightPosAtHit = light.position + light.velocity * hitTime; // A fény helyzete az ütközés idejében

	//Kiszámolt másodfokú egyenlet együtthatói
	float La = powf(light.velocity.Length(), 2) - 1;
	float Lb = 2 * (light.velocity.x*(hit.position.x - lightPosAtHit.x) + light.velocity.y*(hit.position.y - lightPosAtHit.y) + light.velocity.z*(hit.position.z - lightPosAtHit.z));
	float Lc = powf(hit.position.x - lightPosAtHit.x, 2) + powf(hit.position.y - lightPosAtHit.y, 2) + powf(hit.position.z - lightPosAtHit.z, 2);

	float disc = Lb*Lb - 4 * La*Lc; //Nem kéne hogy negatív legyen

	float t;
	if (fabsf(La) < EPSILON) {
		t = -Lc / Lb;
	}
	else {

		t = (-Lb - sqrtf(disc)) / (2 * La);

		if (t < 0) {
			t = (-Lb + sqrtf(disc)) / (2 * La);
		}
	}

	Vector lightOrigin = light.position + light.velocity * (hitTime - t); //Innen jött a fény

	Vector lightDir = lightOrigin - hit.position;

	Ray shadowRay = Ray(hit.position + hit.normal * EPSILON, lightDir.norm());
	Hit shadowHit = firstIntersect(shadowRay, elapsedTime);
	if (shadowHit.t < 0 || shadowHit.t > lightDir.Length()) {
		outRadiance = outRadiance + hit.material->shade(hit.position, hit.normal, ray.direction, lightDir.norm(), light.getLuminance(hit.position));
	}

	if (hit.material->isReflective()) {
		Vector reflectionDir = hit.material->reflect(ray.direction, hit.normal);
		Ray reflectedRay(hit.position + hit.normal * EPSILON, reflectionDir);
		outRadiance = outRadiance + trace(reflectedRay, depth + 1, elapsedTime - hit.t) * hit.material->Fresnel(ray.direction, hit.normal);
	}

	if (hit.material->isRefractive()) {
		Vector reflectionDir = hit.material->refract(ray.direction, hit.normal);
		Ray refractedRay(hit.position - hit.normal * EPSILON, reflectionDir);
		outRadiance = outRadiance + trace(refractedRay, depth + 1, elapsedTime - hit.t) * (Color(1, 1, 1) - hit.material->Fresnel(ray.direction, hit.normal));
	}

	return outRadiance;
}

void build(float elapsedTime) {
	//Init ellipsoid
	ellipsoid = Ellipsoid(ELLIPSOID_START_POS, 1, 0.25f, Vector(0, 0, 1).norm(), ELLIPSOID_SPEED, &glassMaterial);
	objects[6] = &ellipsoid;

	//light init
	light.color = Color(1, 1, 1);
	light.velocity = LIGHT_SOURCE_SPEED;
	light.position = LIGHT_SOURCE_START_POS;

	for (int Y = 0; Y < Camera::XM; Y++)
		for (int X = 0; X < Camera::YM; X++) {
			Ray ray = camera.getRay(X, Y);
			image[Y*Camera::XM + X] = trace(ray, 0, elapsedTime);
		}
}

void onInitialization()
{
	glViewport(0, 0, Camera::XM, Camera::YM);
	init();
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
	if (key == ' ') {
		int time = glutGet(GLUT_ELAPSED_TIME);
		build(time / 1000.0f);
		glutPostRedisplay();
	}
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
