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
#include <iostream>
using namespace std;

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

static const int MAX_CSIRGURU_COUNT = 10;

static const float HEAD_RADIUS = 0.5f;

static const float EPSILON = 0.001f;
static const float GRAVITY = 9.81f;

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
	Vector operator+(float a) {
		return Vector(x + a, y + a, z + a);
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

float toRad(float deg) {
	return deg * (M_PI / 180.0f);
}

float toDeg(float rad) {
	return rad * (180.0f / M_PI);
}

/*struct Camera {
	static const int XM = 600;
	static const int YM = 600;

	Vector eye, lookat, up;

	Camera() :eye(Vector()), lookat(Vector()), up(Vector()) {}

	Camera(Vector eye, Vector lookat, Vector up) :eye(eye), lookat(lookat), up(up) {}
};*/

enum ControllKeys { W, A, S, D, Q, E, keys_num };
bool keys_down[keys_num];

struct Camera {
	static const int XM = 600;
	static const int YM = 600;
	Vector fwd, pos;
	const float speed, mouse_speed;

	Camera(float speed = 5, float mouse_speed = 0.002f) : fwd(Vector(-1, -0.5f, -1).norm()), pos(5, 5, 5), speed(speed), mouse_speed(mouse_speed) { }

	void updatePos(float dt) {
		Vector up = Vector(0, 1, 0), right = (fwd % up).norm();
		up = (right % fwd).norm();

		if (keys_down[W] && !keys_down[S]) {
			pos = pos + fwd * speed * dt;
		}
		else if (keys_down[S] && !keys_down[W]) {
			pos = pos - fwd * speed * dt;
		}

		if (keys_down[D] && !keys_down[A]) {
			pos = pos + right * speed * dt;
		}
		else if (keys_down[A] && !keys_down[D]) {
			pos = pos - right * speed * dt;
		}

		if (keys_down[Q] && !keys_down[E]) {
			pos = pos + Vector(0, 1, 0) * speed * dt;
		}
		else if (keys_down[E] && !keys_down[Q]) {
			pos = pos - Vector(0, 1, 0) * speed * dt;
		}
	}

	void updateDir(int dx, int dy) {
		Vector y_axis = Vector(0, 1, 0), right = (fwd % y_axis).norm();
		Vector up = (right % fwd).norm();

		// Ha teljesen felfele / lefele nĂŠznĂŠnk, akkor ne forduljon ĂĄt a kamera
		float dot_up_fwd = (y_axis * fwd);
		if (dot_up_fwd > 0.95f && dy > 0) {
			dy = 0;
		}
		if (dot_up_fwd < -0.95f && dy < 0) {
			dy = 0;
		}

		// MĂłdosĂ­tsuk az nĂŠzeti irĂĄnyt
		fwd = fwd + (right * dx + up * dy) * mouse_speed;
		fwd = fwd.norm();
	}

	void applyMatrix() const {
		gluLookAt(pos.x, pos.y, pos.z, pos.x + fwd.x, pos.y + fwd.y, pos.z + fwd.z, 0, 1, 0);
	}
} camera;

struct Cylinder {
	float r, m;
	int slices;

	Cylinder() : r(1), m(1), slices(16) {}

	Cylinder(float r, float m, int slices) : r(r), m(m), slices(slices) {}

	void draw() {
		//Bottom face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(0, -1, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();

		//Top face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 1, 0);
		glVertex3f(0, m, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(0, 1, 0);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();

		//Sides
		glBegin(GL_TRIANGLE_STRIP);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(sinf(angle), 0, cosf(angle));
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();
	}
};

struct Cone {
	float r, m;
	int slices;

	Cone() : r(1), m(1), slices(16) {}

	Cone(float r, float m, int slices) : r(r), m(m), slices(slices) {}

	void draw() {
		float a = cosf(atanf(m / r)) * r;

		//Bottom face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			glNormal3f(0, -1, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();

		//Sides
		glBegin(GL_TRIANGLE_STRIP);
		for (float i = 0; i <= slices; i++)
		{
			float angle = 2 * M_PI / slices * i;

			Vector vr = Vector(sinf(angle) * r, 0, cosf(angle) * r);
			Vector normal = vr + (Vector(0, m, 0) - vr).norm() * a;

			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(0, m, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();
	}
};

struct Sphere {
	float r;
	int slices, stacks;

	Sphere() : r(1), slices(16), stacks(16) {}

	Sphere(float r, int slices, int stacks) : r(r), slices(slices), stacks(stacks) {}

	void draw() {
		glBegin(GL_TRIANGLE_STRIP);

		for (int i = 0; i < stacks; i++)
		{
			float b1 = M_PI / stacks * i - M_PI / 2;
			float b2 = M_PI / stacks * (i + 1) - M_PI / 2;

			for (int j = 0; j < slices; j++)
			{
				float a = 2 * M_PI / slices * j;
				Vector point1 = Vector(sinf(a) * cosf(b1) * r, sinf(b1) * r, cosf(a) * cosf(b1) * r);
				Vector point2 = Vector(sinf(a) * cosf(b2) * r, sinf(b2) * r, cosf(a) * cosf(b2) * r);

				glNormal3f(point1.x, point1.y, point1.z);
				glVertex3f(point1.x, point1.y, point1.z);

				glNormal3f(point2.x, point2.y, point2.z);
				glVertex3f(point2.x, point2.y, point2.z);
			}
		}

		glEnd();
	}
};

struct HalfCylinder {
	float r, m;
	int slices;

	HalfCylinder() : r(1), m(1), slices(16) {}

	HalfCylinder(float r, float m, int slices) : r(r), m(m), slices(slices) {}

	void draw() {
		//Bottom face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = M_PI / slices * i;

			glNormal3f(0, -1, 0);
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
		}
		glEnd();

		//Top face
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 1, 0);
		glVertex3f(0, m, 0);
		for (float i = 0; i <= slices; i++)
		{
			float angle = M_PI / slices * i;

			glNormal3f(0, 1, 0);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();

		//Sides
		glBegin(GL_TRIANGLE_STRIP);
		for (float i = 0; i <= slices; i++)
		{
			float angle = M_PI / slices * i;

			glNormal3f(sinf(angle), 0, cosf(angle));
			glVertex3f(sinf(angle) * r, 0, cosf(angle) * r);
			glVertex3f(sinf(angle) * r, m, cosf(angle) * r);
		}
		glEnd();

		glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glVertex3f(0, 0, r);
		glVertex3f(0, m, r);
		glVertex3f(0, m, -r);
		glVertex3f(0, 0, -r);
		glEnd();
	}
};

struct CatmullRom {
	Vector points[10];
	float t[10];
	int size;

	CatmullRom() : size(0) {}

	void addControlPoint(Vector point, float t) {
		if (size == 10)
			return;

		this->points[size] = point;
		this->t[size++] = t;
	}

	Vector getHermiteCurvePoint(int i, float t) {
		if (t > (this->t)[i + 1] && i + 1 < size) {
			i++;
		}
		float deltaT = t - (this->t)[i];
		return
			a3(i) * powf(deltaT, 3) +
			a2(i) * powf(deltaT, 2) +
			a1(i) * deltaT +
			a0(i);
	}

	Vector getDerivative(int i, float t) {
		if (t > (this->t)[i + 1] && i + 1 < size) {
			i++;
		}
		float deltaT = t - (this->t)[i];
		return
			a3(i) * 3 * powf(deltaT, 2) +
			a2(i) * 2 * deltaT +
			a1(i);
	}

	int getSize() { return size; }

	float getT(int i) { return t[i]; }

	Vector getPoint(int i) { return points[i]; }

private:
	Vector getVelocity(int i) {
		if (i == 0 || i == size) {
			return Vector();
		}

		return (((points[i + 1] - points[i]) / (t[i + 1] - t[i])) +
			((points[i] - points[i - 1]) / (t[i] - t[i - 1]))) / 2;
	}

	Vector a3(int i) {
		float deltaT = t[i + 1] - t[i];
		return
			(((points[i] - points[i + 1]) * 2) / powf(deltaT, 3)) +
			((getVelocity(i + 1) + getVelocity(i)) / powf(deltaT, 2));
	}

	Vector a2(int i) {
		float deltaT = t[i + 1] - t[i];
		return
			(((points[i + 1] - points[i]) * 3) / powf(deltaT, 2)) -
			((getVelocity(i + 1) + getVelocity(i) * 2) / deltaT);
	}

	Vector a1(int i) {
		return getVelocity(i);
	}

	Vector a0(int i) {
		return points[i];
	}
};

struct BezierCurve {
	Vector points[10];
	int size;

	BezierCurve() : size(0) {}

	void addControlPoint(Vector point) {
		if (size == 10)
			return;

		this->points[size++] = point;
	}

	Vector getBezierCruvePoint(float t) {
		Vector r = Vector();

		for (int i = 0; i < size; i++)
		{
			r = r + (points[i] * B(i, t));
		}

		return r;
	}

	Vector getDerivative(float t) {
		if (t == 0) {
			t += EPSILON;
		}
		Vector r = Vector();

		for (int i = 0; i < size; i++)
		{
			r = r + (points[i] * dB(i, t));
		}

		return r;
	}

private:
	float B(int i, float t) {
		int n = size - 1;

		float choose = 1;

		for (int j = 1; j <= i; j++)
		{
			choose *= (((float)n - (float)j + 1.0f) / (float)j);
		}

		return choose * powf(t, i) * powf(1 - t, n - i);
	}

	float dB(int i, float t) {
		int n = size - 1;

		float choose = 1;

		for (int j = 1; j <= i; j++)
		{
			choose *= (((float)n - (float)j + 1.0f) / (float)j);
		}

		return choose * (i*powf(t, i - 1) * powf(1 - t, n - i) - powf(t, i) * powf(1 - t, n - i - 1) * (n - i));
	}
};

struct Object {
	Vector position;
	Vector spinAxis;

	float directionAngle;
	float angle;
	float velocity;

	Vector getProjectileMotionPos(float t) {

		float y = velocity * t * sinf(angle) - GRAVITY / 2.0 * t * t;

		float d = velocity * t * cosf(angle);

		return Vector(sinf(directionAngle) * d, y, cosf(directionAngle) * d);
	}
};

//a CSIRGURU szeme
struct CsirguruEye : public Object {

	Sphere eyeBall;

	CsirguruEye() {
		eyeBall.r = 0.1f;
		eyeBall.slices = 8;
		eyeBall.stacks = 8;
	}

	void draw(bool shadow) {
		glColor3f(0, 0, 0);
		eyeBall.draw();
	}
};

//a CSIRGURU csőre
struct CsirguruBeak : public Object {

	Cone beak;

	CsirguruBeak() {
		beak.m = 0.2f;
		beak.r = 0.1f;
		beak.slices = 16;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(1, 0.647f, 0);
		}
		beak.draw();
	}
};

//a CSIRGURU taraja
struct CsirguruComb : public Object {

	Cone comb;

	CsirguruComb() {
		comb.m = 0.4f;
		comb.r = 0.1f;
		comb.slices = 10;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(1, 0, 0);
		}
		comb.draw();
	}
};

//a CSIGURU teste
struct CsirguruBody : public Object {

	int BEZIER_COUNT = 6;

	BezierCurve bezier[6];
	float t[6] = { 0, 1, 2, 3, 4, 5 };

	CsirguruBody() {

		int i = 0;
		Vector bCP = Vector(0, 1.35f, 0.9f);
		float bAngle = 0;
		float bSize = 1;
		addBezierCircle(i, bCP, bAngle, bSize);

		i = 1;
		bCP.y = 0.85f;
		bCP.z = 0.8f;
		bAngle = toRad(-30);
		bSize = 1;
		addBezierCircle(i, bCP, bAngle, bSize);

		i = 2;
		bCP.y = 0.15f;
		bCP.z = 0.4f;
		bAngle = toRad(-50);
		bSize = 2;
		addBezierCircle(i, bCP, bAngle, bSize);

		//This shall be the anchor point
		i = 3;
		bCP.y = 0;
		bCP.z = 0;
		bAngle = toRad(-90);
		bSize = 2.3f;
		addBezierCircle(i, bCP, bAngle, bSize);

		i = 4;
		bCP.y = 0.05f;
		bCP.z = -0.4f;
		bAngle = toRad(-130);
		bSize = 1.5f;
		addBezierCircle(i, bCP, bAngle, bSize);

		i = 5;
		bCP.y = 0.85f;
		bCP.z = -0.8f;
		bAngle = toRad(-180);
		bSize = 2 * EPSILON;
		addBezierCircle(i, bCP, bAngle, bSize);
	}

	void draw(bool shadow) {

		float bstep = 1.0f / 20.0f;

		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(0.9f, 0.9f, 0.9f);
		}

		for (float tb = 0; tb < 1; tb += bstep) {

			//TODO optimize mindent kétszer számol
			glBegin(GL_TRIANGLE_STRIP);

			CatmullRom cm1 = CatmullRom();
			CatmullRom cm2 = CatmullRom();

			for (int i = 0; i < BEZIER_COUNT; i++)
			{
				cm1.addControlPoint(bezier[i].getBezierCruvePoint(tb), t[i]);
				cm2.addControlPoint(bezier[i].getBezierCruvePoint(tb + bstep), t[i]);
			}

			CatmullRom cmNormal1 = CatmullRom();
			CatmullRom cmNormal2 = CatmullRom();

			for (int i = 0; i < BEZIER_COUNT; i++)
			{
				cmNormal1.addControlPoint(bezier[i].getDerivative(tb) % cm1.getDerivative(i, t[i] - EPSILON), t[i]);
				cmNormal2.addControlPoint(bezier[i].getDerivative(tb + bstep) % cm2.getDerivative(i, t[i] - EPSILON), t[i]);
			}

			for (int i = 0; i < cm1.getSize(); i++)
			{
				float step = (cm1.getT(i + 1) - cm1.getT(i)) / 4.0f;

				for (float t = cm1.getT(i); t <= cm1.getT(i + 1); t += step) {
					Vector cmPoint1 = cm1.getHermiteCurvePoint(i, t);
					Vector cmPoint2 = cm2.getHermiteCurvePoint(i, t);

					Vector normal1 = cmNormal1.getHermiteCurvePoint(i, t);
					Vector normal2 = cmNormal2.getHermiteCurvePoint(i, t);

					glNormal3f(normal1.x, normal1.y, normal1.z);
					glVertex3f(cmPoint1.x, cmPoint1.y, cmPoint1.z);
					glNormal3f(normal2.x, normal2.y, normal2.z);
					glVertex3f(cmPoint2.x, cmPoint2.y, cmPoint2.z);
				}
			}
			glEnd();
		}
	}

private:
	void addBezierCircle(int i, Vector cp, float angle, float size) {
		bezier[i].addControlPoint(Vector(cp.x, cp.y, cp.z));
		bezier[i].addControlPoint(Vector(cp.x + size / 2, cp.y, cp.z));
		bezier[i].addControlPoint(Vector(cp.x + size / 2, size * sinf(angle) + cp.y, size * cosf(angle) + cp.z));
		bezier[i].addControlPoint(Vector(cp.x - size / 2, size * sinf(angle) + cp.y, size * cosf(angle) + cp.z));
		bezier[i].addControlPoint(Vector(cp.x - size / 2, cp.y, cp.z));
		bezier[i].addControlPoint(Vector(cp.x, cp.y, cp.z));
	}
};

//a CSIRGURU lába
struct CsirguruThigh : public Object {

	Sphere joint;
	Cylinder thigh;

	CsirguruThigh() {
		joint.r = 0.25f;
		joint.slices = 16;
		joint.stacks = 16;

		thigh.r = 0.25f;
		thigh.m = 0.5f;
		thigh.slices = 16;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(0.9f, 0.9f, 0.9f);
		}
		joint.draw();
		thigh.draw();
	}
};

//a CSIRGURU lába
struct CsirguruLeg : public Object {

	Sphere joint;
	Cylinder leg;

	CsirguruLeg() {
		leg.r = 0.2f;
		leg.m = 0.8f;
		leg.slices = 16;

		joint.r = 0.2f;
		joint.slices = 16;
		joint.stacks = 16;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(1, 0.647f, 0);
		}
		leg.draw();
		joint.draw();
	}
};

struct CsirguruFeet : public Object {

	Cylinder feet;

	CsirguruFeet() {
		feet.m = 0.8f;
		feet.r = 0.2f;
		feet.slices = 8;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(1, 0.647f, 0);
		}
		feet.draw();
	}
};

struct CsirguruToe : public Object {

	HalfCylinder toe;

	CsirguruToe() {
		toe.m = 0.4f;
		toe.r = 0.2f;
		toe.slices = 8;
	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(1, 0.647f, 0);
		}
		toe.draw();
	}
};

struct CsirguruHead : public Object {

	Sphere head;

	CsirguruHead() {
		head.r = HEAD_RADIUS;
		head.slices = 20;
		head.stacks = 20;
	}

	void explode(Vector center) {

	}

	void draw(bool shadow) {
		if (shadow) {
			glColor3f(0, 0, 0);
		}
		else {
			glColor3f(0.9f, 0.9f, 0.9f);
		}
		head.draw();
	}
};

//sexy beast
struct Csirguru {
	CsirguruBody body;
	CsirguruThigh thigh;
	CsirguruLeg leg;
	CsirguruFeet feet;
	CsirguruToe toe;
	CsirguruHead head;
	CsirguruEye eyeLeft, eyeRight;
	CsirguruBeak beak;
	CsirguruComb comb1, comb2, comb3, comb4, comb5, comb6, comb7;

	Vector position, jumpOrigin;

	float faceDirection;

	bool toeAnchored, exploded, jumping, landed;
	float kneeAngle, ankleAngle, toeAngle;
	long timeOfExplosion, timeOfJump, timeOfLanding;

	long animPrepareDuration, animAccelerateDuration, animDecelerateDuration;
	float animPerpareAnkleStart, animPerpareAnkleEnd, animPrepareToeStart, animPrepareToeEnd;
	float animAccelerateKneeStart, animAccelerateKneeEnd;
	float animInAirAnkleStart, animInAirAnkleEnd, animInAirToeStart, animInAirToeEnd;
	float animDecelerateAnkleStart, animDecelerateAnkleEnd;

	float velocity, minTimeInAir, jumpAngle;

	Csirguru() {
		position = Vector();
		toeAnchored = true;
		exploded = false;
		jumping = false;
		landed = false;

		kneeAngle = toRad(135);
		ankleAngle = toRad(90);
		toeAngle = toRad(135);

		animPrepareDuration = 500;
		animAccelerateDuration = 400; 
		animDecelerateDuration = 400;

		animPerpareAnkleStart = 90;
		animPerpareAnkleEnd = 30;
		animPrepareToeStart = 135;
		animPrepareToeEnd = 165;

		animAccelerateKneeStart = 0.414f;
		animAccelerateKneeEnd = 1.6f;

		float bodyAccel = 2 * 1.186f / powf(animAccelerateDuration / 1000.0f, 2);
		velocity = bodyAccel * animAccelerateDuration / 1000.0f;
		minTimeInAir = (2 * velocity * sinf(jumpAngle)) / GRAVITY * 1000.0f;

		jumpAngle = 2 * M_PI / 3;

		animInAirAnkleStart = 180;
		animInAirAnkleEnd = 105;
		animInAirToeStart = 90;
		animInAirToeEnd = 165;

		animDecelerateAnkleStart = 105;
		animDecelerateAnkleEnd = 30;
	}

	void explode(Vector center, long t) {
		timeOfExplosion = t;
		exploded = true;

		randomizeProjectile(body);
		randomizeProjectile(thigh);
		randomizeProjectile(leg);
		randomizeProjectile(feet);
		randomizeProjectile(toe);
		randomizeProjectile(head);
		randomizeProjectile(eyeLeft);
		randomizeProjectile(eyeRight);
		randomizeProjectile(beak);
		randomizeProjectile(comb1);
		randomizeProjectile(comb2);
		randomizeProjectile(comb3);
		randomizeProjectile(comb4);
		randomizeProjectile(comb5);
		randomizeProjectile(comb6);
		randomizeProjectile(comb7);
	}

	void randomizeProjectile(Object& obj) {
		obj.angle = toRad(rand() % 45 + 45);
		obj.directionAngle = toRad(rand() % 360);
		obj.velocity = rand() % 5 + 5;
		obj.spinAxis = Vector((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
	}

	void animateValues(long t) {
		if (!jumping) {
			faceDirection = rand() % 360;
			timeOfJump = t;
			jumpOrigin = position;
			jumping = true;
			return;
		}

		long dt = t - timeOfJump;

		if (dt <= animPrepareDuration) {
			float d = (float)dt / (float)animPrepareDuration;

			ankleAngle = (animPerpareAnkleEnd - animPerpareAnkleStart) * d + animPerpareAnkleStart;
			toeAngle = (animPrepareToeEnd - animPrepareToeStart) * d + animPrepareToeStart;

			ankleAngle = toRad(ankleAngle);
			toeAngle = toRad(toeAngle);
			return;
		}

		dt -= animPrepareDuration;
		if (dt <= animAccelerateDuration) {

			float kneeAccel = 2 * (animAccelerateKneeEnd - animAccelerateKneeStart) / powf(animAccelerateDuration / 1000.0f, 2);

			float kneePosY = kneeAccel / 2.0f * powf(dt / 1000.0f, 2) + animAccelerateKneeStart;

			ankleAngle = 2 * asinf(kneePosY / 2 / 0.8f);
			toeAngle = M_PI - ankleAngle / 2;
			return;
		}

		dt -= animAccelerateDuration;
		if (toeAnchored && !landed) {
			toeAnchored = false;
			body.position.y = 3.2f;
			position = position + body.position;
			jumpOrigin = position;
		}

		Vector projectilePos = jumpOrigin + getProjectileMotionPos(dt / 1000.0f);

		Vector thighP = projectilePos + Vector(0, -1.6f, 0);
		Vector legP = thighP + Vector(0, sinf(M_PI / 2 + M_PI) * leg.leg.m, cosf(M_PI / 2 + M_PI) * leg.leg.m);
		Vector toeP = legP + Vector(0, -sinf(M_PI / 2 - M_PI + toRad(105)) * feet.feet.m, cosf(M_PI / 2 - M_PI + toRad(105)) * feet.feet.m);

		if (toeP.y >= 0 && !landed || dt < minTimeInAir) {
			position = projectilePos;

			float d = fminf(dt, minTimeInAir) / minTimeInAir;

			kneeAngle = M_PI;

			ankleAngle = (animInAirAnkleEnd - animInAirAnkleStart) * d + animInAirAnkleStart;
			toeAngle = (animInAirToeEnd - animInAirToeStart) * d + animInAirToeStart;

			ankleAngle = toRad(ankleAngle);
			toeAngle = toRad(toeAngle);

			timeOfLanding = t;
			return;
		}

		landed = true;

		if (!toeAnchored) {
			toeAnchored = true;
			position.y = 0;
			position = position + Vector(sinf(toRad(faceDirection)) * toe.position.z, 0, cosf(toRad(faceDirection)) * toe.position.z);
		}

		dt = t - timeOfLanding;
		if (dt <= animDecelerateDuration) {
			float ankleAccel = 2 * (animDecelerateAnkleStart - animDecelerateAnkleEnd) / powf(animDecelerateDuration / 1000.0f, 2);

			ankleAngle = ankleAccel / 2.0f * powf((animDecelerateDuration - dt) / 1000.0f, 2) + animDecelerateAnkleEnd;

			ankleAngle = toRad(ankleAngle);
			return;
		}

		dt -= animDecelerateDuration;
		if (dt <= animPrepareDuration) {
			float d = (float)dt / (float)animPrepareDuration;

			ankleAngle = animPerpareAnkleEnd - (animPerpareAnkleEnd - animPerpareAnkleStart) * d;
			toeAngle = animPrepareToeEnd - (animPrepareToeEnd - animPrepareToeStart) * d;

			ankleAngle = toRad(ankleAngle);
			toeAngle = toRad(toeAngle);
			return;
		}

		jumping = false;
		landed = false;
	}

	void draw(long t, bool shadow) {

		if (!exploded) {

			animateValues(t);

			if (toeAnchored) {
				toe.position = Vector();
				feet.position = toe.position;
				leg.position = feet.position + Vector(0, sinf(toeAngle) * feet.feet.m, cosf(toeAngle) * feet.feet.m);
				thigh.position = leg.position + Vector(0, cosf(3 * M_PI / 2 - toeAngle - ankleAngle) * leg.leg.m, sinf(3 * M_PI / 2 - toeAngle - ankleAngle) * leg.leg.m);
				body.position = thigh.position + Vector(0, 1.6f, 0);
			}
			else {
				body.position = Vector();
				thigh.position = body.position + Vector(0, -1.6f, 0);
				leg.position = thigh.position + Vector(0, sinf(M_PI / 2 + kneeAngle) * leg.leg.m, cosf(M_PI / 2 + kneeAngle) * leg.leg.m);
				feet.position = leg.position;
				toe.position = feet.position + Vector(0, -sinf(M_PI / 2 - kneeAngle + ankleAngle) * feet.feet.m, cosf(M_PI / 2 - kneeAngle + ankleAngle) * feet.feet.m);
			}

			head.position = Vector(body.position.x, body.position.y + 1.35f, body.position.z + 0.9f + (HEAD_RADIUS * 4.0f / 5.0f));
			eyeLeft.position = Vector(head.position.x + sinf(M_PI / 4) * (HEAD_RADIUS - 10 * EPSILON), head.position.y, head.position.z + cosf(M_PI / 4) * (HEAD_RADIUS - 10 * EPSILON));
			eyeRight.position = Vector(head.position.x + sinf(-M_PI / 4) * (HEAD_RADIUS - 10 * EPSILON), head.position.y, head.position.z + cosf(-M_PI / 4) * (HEAD_RADIUS - 10 * EPSILON));
			beak.position = Vector(head.position.x, head.position.y, head.position.z + (HEAD_RADIUS - 0.02f));
			float comb1Angle = toRad(45);
			comb1.position = Vector(head.position.x, head.position.y + sinf(comb1Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb1Angle) * (HEAD_RADIUS - 0.02f));
			float comb2Angle = toRad(60);
			comb2.position = Vector(head.position.x, head.position.y + sinf(comb2Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb2Angle) * (HEAD_RADIUS - 0.02f));
			float comb3Angle = toRad(75);
			comb3.position = Vector(head.position.x, head.position.y + sinf(comb3Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb3Angle) * (HEAD_RADIUS - 0.02f));
			float comb4Angle = toRad(90);
			comb4.position = Vector(head.position.x, head.position.y + sinf(comb4Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb4Angle) * (HEAD_RADIUS - 0.02f));
			float comb5Angle = toRad(105);
			comb5.position = Vector(head.position.x, head.position.y + sinf(comb5Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb5Angle) * (HEAD_RADIUS - 0.02f));
			float comb6Angle = toRad(120);
			comb6.position = Vector(head.position.x, head.position.y + sinf(comb6Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb6Angle) * (HEAD_RADIUS - 0.02f));
			float comb7Angle = toRad(135);
			comb7.position = Vector(head.position.x, head.position.y + sinf(comb7Angle) * (HEAD_RADIUS - 0.02f), head.position.z + cosf(comb7Angle) * (HEAD_RADIUS - 0.02f));
		}

		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(faceDirection, 0, 1, 0);

		float dt = (t - timeOfExplosion) / 1000.0f;

		Vector pos = body.position;
		if (exploded) pos = body.position + body.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, body.spinAxis.x, body.spinAxis.y, body.spinAxis.z);
				body.draw(shadow);
			} glPopMatrix();
		}

		pos = head.position;
		if (exploded) pos = head.position + head.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				head.draw(shadow);
			} glPopMatrix();
		}

		pos = eyeLeft.position;
		if (exploded) pos = eyeLeft.position + eyeLeft.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				eyeLeft.draw(shadow);
			} glPopMatrix();
		}

		pos = eyeRight.position;
		if (exploded) pos = eyeRight.position + eyeRight.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				eyeRight.draw(shadow);
			} glPopMatrix();
		}

		pos = beak.position;
		if (exploded) pos = beak.position + beak.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, beak.spinAxis.x, beak.spinAxis.y, beak.spinAxis.z);
				glRotatef(90, 1, 0, 0);
				beak.draw(shadow);
			} glPopMatrix();
		}

		pos = comb1.position;
		if (exploded) pos = comb1.position + comb1.getProjectileMotionPos(dt);
		drawComb(comb1, pos, 45, dt, shadow);

		pos = comb2.position;
		if (exploded) pos = comb2.position + comb2.getProjectileMotionPos(dt);
		drawComb(comb2, pos, 30, dt, shadow);

		pos = comb3.position;
		if (exploded) pos = comb3.position + comb3.getProjectileMotionPos(dt);
		drawComb(comb3, pos, 15, dt, shadow);

		pos = comb4.position;
		if (exploded) pos = comb4.position + comb4.getProjectileMotionPos(dt);
		drawComb(comb4, pos, 0, dt, shadow);

		pos = comb5.position;
		if (exploded) pos = comb5.position + comb5.getProjectileMotionPos(dt);
		drawComb(comb5, pos, -15, dt, shadow);

		pos = comb6.position;
		if (exploded) pos = comb6.position + comb6.getProjectileMotionPos(dt);
		drawComb(comb6, pos, -30, dt, shadow);

		pos = comb7.position;
		if (exploded) pos = comb7.position + comb7.getProjectileMotionPos(dt);
		drawComb(comb7, pos, -45, dt, shadow);

		pos = thigh.position;
		if (exploded) pos = thigh.position + thigh.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, thigh.spinAxis.x, thigh.spinAxis.y, thigh.spinAxis.z);
				thigh.draw(shadow);
			} glPopMatrix();
		}

		pos = leg.position;
		if (exploded) pos = leg.position + leg.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, leg.spinAxis.x, leg.spinAxis.y, leg.spinAxis.z);
				if (toeAnchored) {
					glRotatef(toDeg(toeAngle + ankleAngle) - 270, -1, 0, 0);
				}
				else {
					glRotatef(toDeg(kneeAngle) - 180, -1, 0, 0);
				}
				leg.draw(shadow);
			} glPopMatrix();
		}

		pos = feet.position;
		if (exploded) pos = feet.position + feet.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, feet.spinAxis.x, feet.spinAxis.y, feet.spinAxis.z);
				if (toeAnchored) {
					glRotatef(toDeg(toeAngle) - 90, -1, 0, 0);
				}
				else {
					glRotatef(toDeg(kneeAngle - ankleAngle) - 180, -1, 0, 0);
				}
				glRotatef(90, 0, 1, 0);
				feet.draw(shadow);
			} glPopMatrix();
		}

		pos = toe.position;
		if (exploded) pos = toe.position + toe.getProjectileMotionPos(dt);

		if (pos.y + position.y >= 0) {
			glPushMatrix(); {
				glTranslatef(pos.x, pos.y, pos.z);
				if (exploded) glRotatef(360.0f * dt, toe.spinAxis.x, toe.spinAxis.y, toe.spinAxis.z);
				if (toeAnchored) {
					glRotatef(-90, -1, 0, 0);
				}
				else {
					glRotatef(toDeg(kneeAngle - toeAngle - ankleAngle), -1, 0, 0);
				}
				glRotatef(90, 0, 1, 0);
				toe.draw(shadow);
			} glPopMatrix();
		}

		glPopMatrix();
	}

private:
	void drawComb(CsirguruComb& comb, Vector& pos, int angle, float dt, bool shadow) {
		if (pos.y + position.y < 0) return;

		glPushMatrix(); {
			glTranslatef(pos.x, pos.y, pos.z);
			if (exploded) glRotatef(360.0f * dt, comb.spinAxis.x, comb.spinAxis.y, comb.spinAxis.z);
			glRotatef(angle, 1, 0, 0);
			comb1.draw(shadow);
		} glPopMatrix();
	}

	Vector getProjectileMotionPos(float t) {
		float y = velocity * t * sinf(jumpAngle) - GRAVITY / 2.0 * t * t;
		float d = velocity * t * cosf(jumpAngle);
		return Vector(sinf(M_PI + toRad(faceDirection)) * d, y, cosf(M_PI + toRad(faceDirection)) * d);
	}
};

struct Bomb : public Object {
	void explode() {
		//loop through csirgurus
	}
};

struct Field {
	void draw() {
		glEnable(GL_TEXTURE_2D);

		applyTexture();

		glBegin(GL_QUADS);

		glColor3f(1, 1, 1);
		glNormal3f(0, 1, 0);

		float step = 1;

		for (float i = 0; i < 100; i += step)
		{
			for (float j = 0; j < 100; j += step)
			{
				glTexCoord2f(i / 100.0f, j / 100.0f);
				glVertex3f(i, 0, j);

				glTexCoord2f((i + step) / 100.0f, j / 100.0f);
				glVertex3f(i + step, 0, j);

				glTexCoord2f((i + step) / 100.0f, (j + step) / 100.0f);
				glVertex3f(i + step, 0, j + step);

				glTexCoord2f(i / 100.0f, (j + step) / 100.0f);
				glVertex3f(i, 0, j + step);
			}
		}

		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	void applyTexture() {
		Color texture[16][16];

		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (i % 2 == 0) {
					if (j % 2 == 0) {
						texture[i][j] = Color(249.0f / 255.0f, 220.0f / 255.0f, 36.0f / 255.0f);
					}
					else {
						texture[i][j] = Color(235.0f / 255.0f, 138.0f / 255.0f, 68.0f / 255.0f);
					}
				}
				else {
					if (j % 2 == 0) {
						texture[i][j] = Color(142.0f / 255.0f, 186.0f / 255.0f, 67.0f / 255.0f);
					}
					else {
						texture[i][j] = Color(75.0f / 255.0f, 116.0f / 255.0f, 71.0f / 255.0f);
					}
				}
			}
		}

		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_FLOAT, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
};

struct CsirguruLinkedList {
	Csirguru csirguru;

	CsirguruLinkedList* next;
	CsirguruLinkedList* previous;

	CsirguruLinkedList() {
		next = 0;
		previous = 0;

		csirguru = Csirguru();
	}
};

struct Scene {
	static const Vector SUN_LIGHT;

	CsirguruLinkedList* first;
	CsirguruLinkedList* last;
	
	int csirguruCount = 0;

	Field field;

	float lightColor[4];
	float lightDir[4];
	//Camera camera;

	Scene() {
		first = 0;
		last = 0;

		field = Field();
		//camera = Camera();
	}

	void init() {
		lightColor[0] = 1;
		lightColor[1] = 1;
		lightColor[2] = 1;
		lightColor[3] = 1;

		lightDir[0] = -0.7f;
		lightDir[1] = 1;
		lightDir[2] = 1;
		lightDir[3] = 0;
	}

	void render(long t) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera.applyMatrix();

		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
		glEnable(GL_LIGHT0);

		glPushMatrix();
		field.draw();
		glPopMatrix();

		CsirguruLinkedList* current = first;
		while (current != 0) {
			current->csirguru.draw(t, false);
			current = current->next;
		}

		float shadow[4][4] = {
			1, 0, 0, 0,
			-lightDir[0] / lightDir[1], 0, -lightDir[2] / lightDir[1], 0,
			0, 0, 1, 0,
			0, EPSILON, 0, 1
		};
		glMultMatrixf(&shadow[0][0]);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);

		CsirguruLinkedList* currentShadow = first;
		while (currentShadow != 0) {
			currentShadow->csirguru.draw(t, true);
			currentShadow = currentShadow->next;
		}
	}

	void addCsirguru() {
		if (csirguruCount == MAX_CSIRGURU_COUNT) {
			cout << "csirguru full" << endl;
			return;
		}

		csirguruCount++;

		if (first == 0) {
			first = new CsirguruLinkedList();
			last = first;
		}
		else {
			last->next = new CsirguruLinkedList();
			last->next->previous = last;
			last = last->next;
		}

		cout << "csirguru added, count: " << csirguruCount << endl;
	}

	void dropBomb(Vector pos) {

	}
};

const Vector Scene::SUN_LIGHT = Vector();

Color image[Camera::XM*Camera::YM];
Scene scene;

int csirgurusAdded = 0;

void onInitialization() {
	glViewport(0, 0, Camera::XM, Camera::YM);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	scene = Scene();
	scene.init();

	//scene.camera = Camera(Vector(3, 2, -3), Vector(5, 0, 5), Vector(0, 1, 0));

	//Rendes 3d
	glEnable(GL_DEPTH_TEST);
	//Normál vaktorokat egység vektorokként kezelni
	glEnable(GL_NORMALIZE);
	//Világítás engedélyezése
	glEnable(GL_LIGHTING);


	glMatrixMode(GL_PROJECTION);
	gluPerspective(54, 1, 0.2, 200);
	glMatrixMode(GL_MODELVIEW);
	camera.applyMatrix();
	/*gluLookAt(scene.camera.eye.x, scene.camera.eye.y, scene.camera.eye.z,
		scene.camera.lookat.x, scene.camera.lookat.y, scene.camera.lookat.z,
		scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);*/

		//Ambiens fény?
	glEnable(GL_COLOR_MATERIAL);
	//Irányfényforrás
	glLightfv(GL_LIGHT0, GL_DIFFUSE, scene.lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, scene.lightDir);
	glEnable(GL_LIGHT0);

	glEnable(GL_SMOOTH);
}

long elapsedTime = 0;

void onDisplay() {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render(elapsedTime);
	glutSwapBuffers();
}

bool randomInit = false;

void onKeyboard(unsigned char key, int x, int y) {
	/*switch (key)
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
	case 'y''s':
		//TODO move crosshair down
		break;
	case ' ':
		//TODO drop da bomb
		break;
	default:
		//nothing
		break;
	}*/

	switch (key) {
	case 'w': case 'W':
		keys_down[W] = true;
		break;
	case 's': case 'S':
		keys_down[S] = true;
		break;
	case 'a': case 'A':
		keys_down[A] = true;
		break;
	case 'd': case 'D':
		keys_down[D] = true;
		break;
	case 'q': case 'Q':
		keys_down[Q] = true;
		break;
	case 'e': case 'E':
		keys_down[E] = true;
		break;
	case ' ':
		elapsedTime = glutGet(GLUT_ELAPSED_TIME);
		break;
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		keys_down[W] = false;
		break;
	case 's': case 'S':
		keys_down[S] = false;
		break;
	case 'a': case 'A':
		keys_down[A] = false;
		break;
	case 'd': case 'D':
		keys_down[D] = false;
		break;
	case 'q': case 'Q':
		keys_down[Q] = false;
		break;
	case 'e': case 'E':
		keys_down[E] = false;
		break;
	}
}

int last_x, last_y;
void onMouse(int button, int state, int x, int y) {
	last_x = x;
	last_y = y;
}

void onMouseMotion(int x, int y) {
	camera.updateDir(x - last_x, last_y - y);
	last_x = x;
	last_y = y;
}

void onIdle() {
	float time = glutGet(GLUT_ELAPSED_TIME);
	float dt = (time - elapsedTime) / 1000.0f;

	elapsedTime = time;
	if (time > csirgurusAdded * 1000) {
		scene.addCsirguru();
		csirgurusAdded++;
	}

	camera.updatePos(dt);
	glutPostRedisplay();
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
