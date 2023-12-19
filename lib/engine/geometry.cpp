#include "engine/geometry.h"

#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//GENERAL MATH FUNCTIONS
float normalize(float x){
	return x / fabs(x);
}

float square(float x){
	return x * x;
}

unsigned int ___x = 1;
unsigned int ___a = 1103515245;
unsigned int ___c = 12345;
unsigned int ___m = 2147483648;

float getRandom(){

	//return (float)rand() / (float)RAND_MAX;
	___x = (___a * ___x + ___c) % ___m;
	float r = (float)___x;
	r /= (float)___m;

	return r;
	
}

void setRandomSeed(int seed){
	___x = seed;
}

bool checkEqualsFloat(float x, float y, float tolerance){
	return fabs(x - y) < tolerance;
}

int getSign(float x){
	return 1 * (x > 0) + -1 * (x < 0) + 0 * (x == 0) + 0;
}

int sign(float x){
	return 1 * (x > 0) + -1 * (x < 0) + 0 * (x == 0) + 0;
}

int min(int a, int b){
	return (a <= b) * a + (a > b) * b;
}

int max(int a, int b){
	return (a >= b) * a + (a < b) * b;
}

//VEC2F FUNCTIONS

Vec2f getVec2f(float x, float y){
	Vec2f v = { x, y };
	return v;
}

void Vec2f_set(Vec2f *v, float x, float y){
	v->x = x;
	v->y = y;
}

void Vec2f_log(Vec2f v){
	printf("{ x: %f, y: %f }\n", v.x, v.y);
}

void Vec2f_add(Vec2f *v1, Vec2f v2){
	v1->x += v2.x;
	v1->y += v2.y;
}

void Vec2f_sub(Vec2f *v1, Vec2f v2){
	v1->x -= v2.x;
	v1->y -= v2.y;
}

void Vec2f_mul(Vec2f *v1, Vec2f v2){
	v1->x *= v2.x;
	v1->y *= v2.y;
}

void Vec2f_div(Vec2f *v1, Vec2f v2){
	v1->x /= v2.x;
	v1->y /= v2.y;
}

void Vec2f_mulByFloat(Vec2f *v1, float a){
	v1->x *= a;
	v1->y *= a;
}

void Vec2f_divByFloat(Vec2f *v1, float a){
	v1->x /= a;
	v1->y /= a;
}

void Vec2f_normalize(Vec2f *v){
	Vec2f_divByFloat(v, getMagVec2f(*v));
}

float getMagVec2f(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

Vec2f getAddVec2f(Vec2f v1, Vec2f v2){
	return getVec2f(v1.x + v2.x, v1.y + v2.y);
}

Vec2f getSubVec2f(Vec2f v1, Vec2f v2){
	return getVec2f(v1.x - v2.x, v1.y - v2.y);
}

Vec2f getMulVec2fFloat(Vec2f v, float a){
	return getVec2f(v.x * a, v.y * a);
}

Vec2f getDivVec2fFloat(Vec2f v, float a){
	return getVec2f(v.x / a, v.y / a);
}

float getDistanceVec2f(Vec2f v1, Vec2f v2){
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

Vec2f getNormalizedVec2f(Vec2f v){
	return getVec2f(v.x / getMagVec2f(v), v.y / getMagVec2f(v));
}

Vec2f getInverseVec2f(Vec2f v){
	return getVec2f(1 / v.x, 1 / v.y);
}

float getDotVec2f(Vec2f v1, Vec2f v2){
	return v1.x * v2.x + v1.y * v2.y;
}

//VEC3F FUNCTIONS
Vec3f getVec3f(float x, float y, float z){
	Vec3f v = { x, y, z };
	return v;
}

Vec3f getVec3f(float a){
	Vec3f v = { a, a, a };
	return v;
}

void Vec3f_log(Vec3f v){
	printf("{ x: %f, y: %f, z: %f }\n", v.x, v.y, v.z);
}

void Vec3f_add(Vec3f *v1, Vec3f v2){
	v1->x += v2.x;
	v1->y += v2.y;
	v1->z += v2.z;
}

void Vec3f_sub(Vec3f *v1, Vec3f v2){
	v1->x -= v2.x;
	v1->y -= v2.y;
	v1->z -= v2.z;
}

void Vec3f_mulByFloat(Vec3f *v, float f){
	v->x *= f;
	v->y *= f;
	v->z *= f;
}

void Vec3f_mulByVec3f(Vec3f *v1, Vec3f v2){
	v1->x *= v2.x;
	v1->y *= v2.y;
	v1->z *= v2.z;
}

void Vec3f_divByFloat(Vec3f *v, float x){
	v->x /= x;
	v->y /= x;
	v->z /= x;
}

void Vec3f_normalize(Vec3f *v){
	Vec3f_divByFloat(v, getMagVec3f(*v));
}

void Vec3f_inverse(Vec3f *v){
	v->x *= -1;
	v->y *= -1;
	v->z *= -1;
}

void Vec3f_rotate(Vec3f *point, float rotX, float rotY, float rotZ){

	//x axis
	float mag = sqrt(point->y * point->y + point->z * point->z);

	float deg = atan2(point->z, point->y);

	deg += rotX;

	point->y = mag * cos(deg);
	point->z = mag * sin(deg);

	//y axis
	mag = sqrt(point->x * point->x + point->z * point->z);

	deg = atan2(point->x, point->z);

	deg += rotY;

	point->x = mag * sin(deg);
	point->z = mag * cos(deg);

	//z axis
	mag = sqrt(point->x * point->x + point->y * point->y);

	deg = atan2(point->y, point->x);

	deg += rotZ;

	point->x = mag * cos(deg);
	point->y = mag * sin(deg);

}

float getMagVec3f(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool checkEqualsVec3f(Vec3f v1, Vec3f v2, float tolerance){
	return fabs(v1.x - v2.x) < tolerance
		&& fabs(v1.y - v2.y) < tolerance
		&& fabs(v1.z - v2.z) < tolerance;
}

Vec3f getAddVec3f(Vec3f v1, Vec3f v2){
	return getVec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3f getSubVec3f(Vec3f v1, Vec3f v2){
	return getVec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3f getMulVec3fFloat(Vec3f v, float a){
	return getVec3f(v.x * a, v.y * a, v.z * a);
}

Vec3f getDivVec3fFloat(Vec3f v, float a){
	return getVec3f(v.x / a, v.y / a, v.z / a);
}

float getDistanceVec3f(Vec3f v1, Vec3f v2){
	return getMagVec3f(getSubVec3f(v1, v2));
}

float getDotVec3f(Vec3f v1, Vec3f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3f getCrossVec3f(Vec3f v1, Vec3f v2){
	Vec3f v = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};

	return v;
}

float getAngleBetweenVec3f(Vec3f v1, Vec3f v2){
	float a = getMagVec3f(getSubVec3f(v1, v2));
	float b = getMagVec3f(v1);
	float c = getMagVec3f(v2);

	return acos((a * a - b * b - c * c) / (-2 * b * c));
}

float getAreaFromTriangleVec3f(Vec3f v1, Vec3f v2, Vec3f v3){
	return getMagVec3f(getCrossVec3f(getSubVec3f(v1, v2), getSubVec3f(v1, v3))) / 2;
	//float a = getMagVec3f(getSubVec3f(v1, v2));
	//float b = getMagVec3f(getSubVec3f(v1, v3));
	//float c = getMagVec3f(getSubVec3f(v2, v3));

	//return (b * c / 2) * sqrt(1 - getSquare((b * b + c * c - a * a) / (2 * b * c)));
}

Vec3f getNormalFromTriangleVec3f(Vec3f v1, Vec3f v2, Vec3f v3){
	Vec3f N = getCrossVec3f(getSubVec3f(v2, v1), getSubVec3f(v3, v1));

	Vec3f_normalize(&N);

	return N;
}

bool checkLineToTriangleIntersectionVec3f(Vec3f l1, Vec3f l2, Vec3f t1, Vec3f t2, Vec3f t3, Vec3f *collisionPoint_p){

	Vec3f u = getSubVec3f(t2, t1);
	Vec3f v = getSubVec3f(t3, t1);

	Vec3f N = getCrossVec3f(u, v);

	Vec3f lineDirection = getSubVec3f(l2, l1);

	Vec3f P = getSubVec3f(l1, getMulVec3fFloat(lineDirection, getDotVec3f(getSubVec3f(l1, t1), N) / getDotVec3f(lineDirection, N)));

	*collisionPoint_p = P;

	Vec3f b = getSubVec3f(P, t1);

	float a11 = getDotVec3f(u, u);
	float a12 = getDotVec3f(u, v);
	float a21 = a12;
	float a22 = getDotVec3f(v, v);

	float b1 = getDotVec3f(b, u);
	float b2 = getDotVec3f(b, v);

	float denominator = 1.0 / (a11 * a22 - a21 * a12);

	float x = (b1 * a22 - b2 * a21) * denominator;
	float y = (b2 * a11 - b1 * a12) * denominator;

	return x >= 0.0 && y >= 0.0 && x + y <= 1.0;

}

bool checkSphereToTriangleCollision(Vec3f s, float r, Vec3f t1, Vec3f t2, Vec3f t3){

	//check collision with triangle plane
	Vec3f u = getSubVec3f(t2, t1);
	Vec3f v = getSubVec3f(t3, t1);

	Vec3f N = getCrossVec3f(u, v);

	float t = getDotVec3f(getSubVec3f(s, t1), N) / getDotVec3f(N, N);

	if(fabs(t * getMagVec3f(N)) > r){
		return false;
	}
	
	//check if projected point is inside triangle
	Vec3f P = getAddVec3f(s, getMulVec3fFloat(N, t));

	Vec3f b = getSubVec3f(P, t1);

	float a11 = getDotVec3f(u, u);
	float a12 = getDotVec3f(u, v);
	float a21 = a12;
	float a22 = getDotVec3f(v, v);

	float b1 = getDotVec3f(b, u);
	float b2 = getDotVec3f(b, v);

	float denominator = 1.0 / (a11 * a22 - a21 * a12);

	float x = (b1 * a22 - b2 * a21) * denominator;
	float y = (b2 * a11 - b1 * a12) * denominator;

	if(x >= 0.0 && y >= 0.0 && x + y <= 1.0){
		return true;
	}

	Vec3f w = getSubVec3f(t2, t3);

	//check collision against triangle edges
	Vec3f s1 = getSubVec3f(t1, s);
	Vec3f s2 = getSubVec3f(t2, s);
	Vec3f s3 = getSubVec3f(t3, s);

	float x1 = getDotVec3f(s1, u) / getDotVec3f(u, u);
	float x2 = getDotVec3f(s1, v) / getDotVec3f(v, v);
	float x3 = getDotVec3f(s3, w) / getDotVec3f(u, u);

	Vec3f ps1 = getAddVec3f(s1, getMulVec3fFloat(u, -x1));
	Vec3f ps2 = getAddVec3f(s2, getMulVec3fFloat(v, -x2));
	Vec3f ps3 = getAddVec3f(s3, getMulVec3fFloat(w, -x3));

	if(getDotVec3f(ps1, ps1) <= r * r
	&& x1 >= 0.0 && x1 <= 1.0
	|| getDotVec3f(ps2, ps2) <= r * r
	&& x2 >= 0.0 && x2 <= 1.0
	|| getDotVec3f(ps3, ps3) <= r * r
	&& x3 >= 0.0 && x3 <= 1.0){
		return true;
	}

	if(getDotVec3f(s1, s1) <= r * r
	|| getDotVec3f(s2, s2) <= r * r
	|| getDotVec3f(s3, s3) <= r * r){
		return true;
	}

	return false;

}

Vec3f mulVec3fMat4f(Vec3f v, Mat4f m, float w){
	Vec4f v4 = m * getVec4f(v.x, v.y, v.z, w);
	return getVec3f(v4.x, v4.y, v4.z);

}

/*
void Vec3f_mulByMat4f(Vec3f *v_p, Mat4f m, float w){

	Vec4f v4 = getVec4f(v_p->x, v_p->y, v_p->z, w);

	v4 = m * v4;
	//Vec4f_mulByMat4f(&v4, m);

	v_p->x = v4.x;
	v_p->y = v4.y;
	v_p->z = v4.z;

}
*/

//VEC4F FUNCTIONS


Vec4f getVec4f(float x, float y, float z, float w){
	Vec4f v = { x, y, z, w };
	return v;
}

Vec4f getVec4f(float a){
	Vec4f v = { a, a, a, a };
	return v;
}

float getMagVec4f(Vec4f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

void Vec4f_log(Vec4f v){
	printf("{ %f, %f, %f, %f }\n", v.x, v.y, v.z, v.w);
	
}

void Vec4f_mulByMat4f(Vec4f *v_p, Mat4f m){

	Vec4f newV = {
		v_p->x * m.values[0][0] + v_p->y * m.values[0][1] + v_p->z * m.values[0][2] + v_p->w * m.values[0][3],
		v_p->x * m.values[1][0] + v_p->y * m.values[1][1] + v_p->z * m.values[1][2] + v_p->w * m.values[1][3],
		v_p->x * m.values[2][0] + v_p->y * m.values[2][1] + v_p->z * m.values[2][2] + v_p->w * m.values[2][3],
		v_p->x * m.values[3][0] + v_p->y * m.values[3][1] + v_p->z * m.values[3][2] + v_p->w * m.values[3][3],
	};

	*v_p = newV;

}

//MAT4F FUNCTIONS

void Mat4f_log(Mat4f m){

	printf("{\n");
	for(int i = 0; i < 4; i++){
		printf("\t");
		for(int j = 0; j < 4; j++){
			printf("%f, ", m.values[i][j]);
		}
		printf("\n");
	}
	printf("}\n");

}

/*
void Mat4f_mulByMat4f(Mat4f *m1_p, Mat4f m2){

	Mat4f newMatrix;
	memset(newMatrix.values, 0, 16 * sizeof(float));

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				newMatrix.values[i][j] += m1_p->values[i][k] * m2.values[k][j];
			}
		}
	}

	memcpy(m1_p->values, newMatrix.values, 16 * sizeof(float));
	
}
*/

Mat4f getIdentityMat4f(){

	Mat4f identityMat4f = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	return identityMat4f;

}

/*
Mat4f getRotationMat4f(float x, float y, float z){

	Mat4f xRotationMat4f = {
		1, 0, 	   0, 		0,
		0, cos(x), -sin(x),	0,
		0, sin(x), cos(x), 	0,
		0, 0, 	   0, 		1,
	};

	Mat4f yRotationMat4f = {
		cos(y),  0, sin(y), 0,
		0, 		 1, 0, 		0,
		-sin(y), 0, cos(y), 0,
		0, 		 0, 0, 		1,
	};

	Mat4f zRotationMat4f = {
		cos(z),	-sin(z), 0, 0,
		sin(z),	cos(z),  0, 0,
		0, 		0, 		 1, 0,
		0, 		0, 		 0, 1,
	};

	
	Mat4f_mulByMat4f(&xRotationMat4f, yRotationMat4f);
	Mat4f_mulByMat4f(&xRotationMat4f, zRotationMat4f);

	return xRotationMat4f;
	
}
*/

Mat4f getTranslationMat4f(float x, float y, float z){

	Mat4f translationMat4f = {
		1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0,
	};

	return translationMat4f;

}

Mat4f getTranslationMat4f(Vec3f v){
	return getTranslationMat4f(v.x, v.y, v.z);
}

Mat4f getScalingMat4f(float x, float y, float z){

	Mat4f scalingMat4f = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1,
	};

	return scalingMat4f;

}

Mat4f getScalingMat4f(Vec3f v){
	return getScalingMat4f(v.x, v.y, v.z);
}

Mat4f getScalingMat4f(float scale){
	return getScalingMat4f(scale, scale, scale);
}

Mat4f getPerspectiveMat4f(float fov, float aspectRatio, float far, float near){

	Mat4f perspectiveMat4f = {
		1 / (aspectRatio * tan(fov / 2)), 	0, 				  0,   0,
		0, 									1 / tan(fov / 2), 0,   0,
		0, 									0,				  (float)((far + near) / (far - near)), -(float)(2.0 * far * near / (far - near)),
		//0, 									0,				  1.0, 0.0,
		0, 									0, 				  1,   0,
	};
	
	return perspectiveMat4f;


}

Mat4f getOrthographicMat4f(float scalingFactor, float aspectRatio, float far, float near){

	Mat4f matrix = {
		1 / (aspectRatio * scalingFactor),  0, 				   0,							0,
		0, 				 					1 / scalingFactor, 0,							0,
		0, 				 					0,				   (float)(1.0 / ((far - near))), 0,
		0, 				 					0, 				   0,   						1.0,
		
	};

	return matrix;

}

Mat4f getLookAtMat4f(Vec3f pos, Vec3f direction){

	Vec3f right = getCrossVec3f(getVec3f(0.0, 1.0, 0.0), direction);
	Vec3f up = getCrossVec3f(direction, right);

	Vec3f_normalize(&right);
	Vec3f_normalize(&up);

	Mat4f matrix = getTranslationMat4f(-pos.x, -pos.y, -pos.z);

	Mat4f lookAtMatrix = { 
		right.x,	 right.y, 	  right.z, 	   0.0,
		up.x, 	   	 up.y, 		  up.z, 	   0.0,
		direction.x, direction.y, direction.z, 0.0,
		0.0, 		 0.0,  		  0.0, 		   1.0,
	};

	/*
	Mat4f matrix = { 
		right.x,	 right.y, 	  right.z, 	   -pos.x,
		up.x, 	   	 up.y, 		  up.z, 	   -pos.y,
		direction.x, direction.y, direction.z, -pos.z,
		0.0, 		 0.0,  		  0.0, 		   1.0,
	};
	*/

	matrix *= lookAtMatrix;

	return matrix;

}

Mat4f getQuaternionMat4f(Vec4f q){

	Mat4f matrix = { 
		(float)(1.0 - 2.0 * (q.y * q.y + q.z * q.z)), (float)(2.0 * (q.x * q.y - q.z * q.w)),		(float)(2.0 * (q.x * q.z + q.y * q.w)), 0.0,
		(float)(2.0 * (q.x * q.y + q.z * q.w)), 	  (float)(1.0 - 2.0 * (q.x * q.x + q.z * q.z)), (float)(2.0 * (q.y * q.z - q.x * q.w)), 0.0,
		(float)(2.0 * (q.x * q.z - q.y * q.w)), 	  (float)(2.0 * (q.y * q.z + q.x * q.w)),		(float)(1.0 - 2.0 * (q.x * q.x + q.y * q.y)), 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	return matrix;
	
}

//VECTOR FUNCTIONS

float length(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

float length(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float length(Vec4f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vec2f normalize(Vec2f v){
	return v / length(v);
}

Vec3f normalize(Vec3f v){
	return v / length(v);
}

Vec4f normalize(Vec4f v){
	return v / length(v);
}

Vec4f normalizeQuaternion(Vec4f q){
	Vec3f axis = normalize(getVec3f(q.x, q.y, q.z));
	return getVec4f(axis.x, axis.y, axis.z, q.w);
}

Vec3f floor(Vec3f v){
	return getVec3f(floor(v.x), floor(v.y), floor(v.z));
}

float dot(Vec2f v1, Vec2f v2){
	return v1.x * v2.x + v1.y * v2.y;
}

float dot(Vec3f v1, Vec3f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float dot(Vec4f v1, Vec4f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float lerp(float a, float b, float t){
	return a + (b - a) * t;
}

Vec2f lerp(Vec2f v1, Vec2f v2, float t){
	return v1 + (v2 - v1) * t;
}

Vec3f lerp(Vec3f v1, Vec3f v2, float t){
	return v1 + (v2 - v1) * t;
}

Vec4f lerp(Vec4f v1, Vec4f v2, float t){
	return v1 + (v2 - v1) * t;
}

float smoothStep(float x1, float x2, float t){
	return lerp(x1, x2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec3f smoothStep(Vec3f v1, Vec3f v2, float t){
	return lerp(v1, v2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec4f smoothStep(Vec4f v1, Vec4f v2, float t){
	return lerp(v1, v2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec3f cross(Vec3f v1, Vec3f v2){
	return getVec3f(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vec4f getQuaternion(Vec3f axis, float angle){

	Vec3f normalizedAxis = normalize(axis);

	return getVec4f(
		sin(angle / 2.0) * normalizedAxis.x,
		sin(angle / 2.0) * normalizedAxis.y,
		sin(angle / 2.0) * normalizedAxis.z,
		cos(angle / 2.0)
	);

}

Vec4f mulQuaternions(Vec4f q1, Vec4f q2){
	Mat4f matrix = {
		q2.w, q2.z, -q2.y, q2.x,
		-q2.z, q2.w, q2.x, q2.y,
		q2.y, -q2.x, q2.w, q2.z,
		-q2.x, -q2.y, -q2.z, q2.w
	};
	return matrix * q1;
	/*
	return getVec4f(
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
		q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
		//q1.x * q2.x - q1.y * q2.y - q1.z * q2.z - q1.w * q2.w,
		//q1.x * q2.y + q1.y * q2.x + q1.z * q2.w - q1.w * q2.z,
		//q1.x * q2.z - q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		//q1.x * q2.y + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x
	);
	*/
}

Vec3f getRotatedVec3f(Vec3f v, Vec3f axis, float angle){

	Vec4f v4 = getVec4f(v.x, v.y, v.z, 1.0);

	Vec4f_mulByMat4f(&v4, getQuaternionMat4f(getQuaternion(axis, angle)));

	return getVec3f(v4.x, v4.y, v4.z);

}

//MATRIX FUNCTIONS

float det(Mat2f m){
	return m.values[0][0] * m.values[1][1] - m.values[0][1] * m.values[1][0];
}

float det(Mat3f m){

	Mat2f m1 = {
		m[1][1], m[1][2],
		m[2][1], m[2][2],
	};

	Mat2f m2 = {
		m[1][0], m[1][2],
		m[2][0], m[2][2],
	};

	Mat2f m3 = {
		m[1][0], m[1][1],
		m[2][0], m[2][1],
	};

	return m[0][0] * det(m1) - m[0][1] * det(m2) + m[0][2] * det(m3);

}

float det(Vec3f v1, Vec3f v2, Vec3f v3){
	return dot(cross(v1, v2), v3);
}

float det(Mat4f m){
	
	Mat3f m1 = {
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f m2 = {
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f m3 = {
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f m4 = {
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};

	return m[0][0] * det(m1) - m[0][1] * det(m2) + m[0][2] * det(m3) - m[0][3] * det(m4);

}

Mat2f getRotationMat2f(float rotation){
	
	Mat2f matrix = {
		cos(rotation), -sin(rotation),
		sin(rotation), cos(rotation),
	};

	return matrix;

}

Mat4f inverse(Mat4f m){
	
	Mat3f a00 = {
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a01 = {
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a02 = {
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a03 = {
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	Mat3f a10 = {
		m[0][1], m[0][2], m[0][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a11 = {
		m[0][0], m[0][2], m[0][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a12 = {
		m[0][0], m[0][1], m[0][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a13 = {
		m[0][0], m[0][1], m[0][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	
	Mat3f a20 = {
		m[0][1], m[0][2], m[0][3],
		m[1][1], m[1][2], m[1][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a21 = {
		m[0][0], m[0][2], m[0][3],
		m[1][0], m[1][2], m[1][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a22 = {
		m[0][0], m[0][1], m[0][3],
		m[1][0], m[1][1], m[1][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a23 = {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	
	Mat3f a30 = {
		m[0][1], m[0][2], m[0][3],
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
	};
	
	Mat3f a31 = {
		m[0][0], m[0][2], m[0][3],
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
	};
	
	Mat3f a32 = {
		m[0][0], m[0][1], m[0][3],
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
	};
	
	Mat3f a33 = {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
	};

	float d = det(m);
	
	Mat4f inverseM = {
		det(a00) / d, -det(a10) / d, det(a20) / d, -det(a30) / d,
		-det(a01) / d, det(a11) / d, -det(a21) / d, det(a31) / d,
		det(a02) / d, -det(a12) / d, det(a22) / d, -det(a32) / d,
		-det(a03) / d, det(a13) / -d, det(a23) / d, det(a33) / d,
	};

	return inverseM;
	
}

void plog(Mat4f m){
	printf("{\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%f, ", m[i][j]);
		}
		printf("\n");
	}
	printf("}\n");
}

bool checkTriangleTriangleCollisionVec3f(Vec3f a1, Vec3f a2, Vec3f a3, Vec3f b1, Vec3f b2, Vec3f b3, Vec3f *output_collisionPoint){

	Vec3f aNormal = normalize(cross(a2 - a1, a3 - a1));
	Vec3f bNormal = normalize(cross(b2 - b1, b3 - b1));

	Vec3f aPoints[3];
	int n_aPoints = 0;

	float t = (dot(b1, bNormal) - dot(a1, bNormal)) / dot(a2 - a1, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a1 + (a2 - a1) * t;
		n_aPoints++;
	}

	t = (dot(b1, bNormal) - dot(a2, bNormal)) / dot(a3 - a2, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a2 + (a3 - a2) * t;
		n_aPoints++;
	}

	t = (dot(b1, bNormal) - dot(a3, bNormal)) / dot(a1 - a3, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a3 + (a1 - a3) * t;
		n_aPoints++;
	}

	Vec3f bPoints[3];
	int n_bPoints = 0;

	t = (dot(a1, aNormal) - dot(b1, aNormal)) / dot(b2 - b1, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b1 + (b2 - b1) * t;
		n_bPoints++;
	}

	t = (dot(a1, aNormal) - dot(b2, aNormal)) / dot(b3 - b2, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b2 + (b3 - b2) * t;
		n_bPoints++;
	}

	t = (dot(a1, aNormal) - dot(b3, aNormal)) / dot(b1 - b3, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b3 + (b1 - b3) * t;
		n_bPoints++;
	}

	if(n_aPoints == 2
	&& n_bPoints == 2){
		if(dot(aPoints[0] - bPoints[0], aPoints[0] - bPoints[1]) < 0.0
		|| dot(aPoints[1] - bPoints[0], aPoints[1] - bPoints[1]) < 0.0
		|| dot(bPoints[0] - aPoints[0], bPoints[0] - aPoints[1]) < 0.0
		|| dot(bPoints[1] - aPoints[0], bPoints[1] - aPoints[1]) < 0.0){

			if(output_collisionPoint != NULL){
				*output_collisionPoint = (bPoints[0] + bPoints[1]) / 2.0;
			}

			return true;
			/*
			collision.pos += (bPoints[0] + bPoints[1]) / 2.0;

			if(dot(rigidBody_p->velocity * -1.0, aNormal) > dot(rigidBody_p->velocity * -1.0, collision.normal)
			|| getMagVec3f(collision.normal) < 0.01){
				collision.normal = aNormal;
			}

			n_hits++;
			*/

		}
	}

	return false;

}


