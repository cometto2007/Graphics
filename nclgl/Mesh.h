#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};

class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	void setTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();
	static Mesh* GenerateCube();
	static Mesh* GeneratePoint();

	void* getColorBufferPointer();
	void setTransformation(Matrix4& transform) {
		for (size_t i = 0; i < numVertices; i++)
			vertices[i] = transform * vertices[i];
	}

protected:
	void BufferData();
	void GenerateNormals();
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 & a, const Vector3 & b,
		const Vector3 & c, const Vector2 & ta,
		const Vector2 & tb, const Vector2 & tc);

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	GLuint texture;
	GLuint numIndices;
	GLuint bumpTexture;
	unsigned int* indices;

	Vector3* tangents;
	Vector2* textureCoords;
	Vector3* vertices;
	Vector3* normals;
	Vector4* colours;
};

