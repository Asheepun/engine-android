#ifndef H3D_H_
#define H3D_H_

#include "engine/geometry.h"
#include "engine/strings.h"
#include "engine/3d-no-driver.h"
#include "engine/assetManager.h"

#ifdef _WIN32
#include "glad/wgl.h"
#include "glad/gl.h"
#endif

#ifdef __linux__
#ifndef __ANDROID__
#include "glad/gl.h"
#endif
#endif

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#include <vector>

enum Type3D{
	TYPE_I8,
	TYPE_I16,
	TYPE_F32,
	TYPE_F64,
};

typedef struct Model{
	char name[STRING_SIZE];
	unsigned int VBO;
	unsigned int VAO;
	unsigned int numberOfTriangles;
}Model;

struct BoneModel{
	char name[STRING_SIZE];
	unsigned int VBO;
	unsigned int VAO;
	unsigned int n_triangles;
	//std::vector<Bone> bones;
	//std::vector<Mat4f> inverseBindMatrices;
};

struct Texture{
	char name[STRING_SIZE];
	unsigned int ID;
};

struct Shader{
	char name[STRING_SIZE];
	unsigned int ID;
};

/*
struct TextureAtlas{
	std::vector<SmallString> names;
	std::vector<Vec4f> textureCoordinates;
	Texture texture;
};
*/

struct TextureBuffer{
	unsigned int VBO;
	unsigned int TB;
	unsigned int n_elements;
	unsigned int elementSize;
};

/*
typedef struct VertexMesh{
	Vec3f *vertices;
	int length;
}VertexMesh;
*/

struct PointMesh{
	Vec3f *points;
	int n_points;
	char name[STRING_SIZE];
};

void Model_initFromMeshData(Model *, const unsigned char *, int);

void Model_initFromFile_mesh(Model *, const char *, AAssetManager *);

void BoneModel_initFromFile(BoneModel *, const char *);

//int BoneModel_getBoneIndexByName(BoneModel *, const char *);


unsigned char *generateMeshDataFromTriangleMesh(TriangleMesh, Vec2f *);

//void VertexMesh_initFromFile_mesh(VertexMesh *, const char *);

void PointMesh_initFromTriangleMesh(PointMesh *, TriangleMesh);

void Texture_init(Texture *, const char *, unsigned char *, int, int);

void Texture_initFromFile(Texture *, const char *, const char *);

void Texture_initFromFileAsAlphaMap(Texture *, const char *, const char *);

void Texture_initAsDepthMap(Texture *, int, int);

void Texture_initAsColorMap(Texture *, int, int);

//void TextureAtlas_init(TextureAtlas *, const char **, int);

void TextureBuffer_init(TextureBuffer *, void *, int);

void TextureBuffer_update(TextureBuffer *, int, int, void *);

void TextureBuffer_initAsVec4fArray(TextureBuffer *, Vec4f *, int);
void TextureBuffer_initAsMat4fArray(TextureBuffer *, Mat4f *, int, bool);

void Shader_init(Shader *, const char *, const char *, const char *);

bool checkTriangleMeshTriangleMeshCollision(TriangleMesh, TriangleMesh, Mat4f, Mat4f, Vec3f *, Vec3f *);

Vec3f PointMesh_GJKSupport(PointMesh *, Mat4f, Vec3f);

//free functions
void Model_free(Model *);
void TextureBuffer_free(TextureBuffer *);
void Texture_free(Texture *);

//uniform functions
void GL3D_uniformMat2f(unsigned int, const char *, Mat2f);

void GL3D_uniformMat3f(unsigned int, const char *, Mat3f);

void GL3D_uniformMat4f(unsigned int, const char *, Mat4f);

void GL3D_uniformMat4fArray(unsigned int, const char *, Mat4f *, int);

void GL3D_uniformVec2f(unsigned int, const char *, Vec2f);

void GL3D_uniformVec3f(unsigned int, const char *, Vec3f);

void GL3D_uniformVec4f(unsigned int, const char *, Vec4f);

void GL3D_uniformInt(unsigned int, const char *, int);

void GL3D_uniformFloat(unsigned int, const char *, float);

void GL3D_uniformTexture(unsigned int, const char *, unsigned int, unsigned int);

void GL3D_uniformTexture3D(unsigned int, const char *, unsigned int, unsigned int);

void GL3D_uniformTextureBuffer(unsigned int, const char *, unsigned int, unsigned int);

bool checkClosestLineTriangleMeshIntersection(Vec3f, Vec3f, TriangleMesh, Vec3f, float, Vec3f *, int *);

#endif
