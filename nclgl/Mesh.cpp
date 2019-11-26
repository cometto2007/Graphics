#include "Mesh.h"

Mesh::Mesh(void) {
	for (int i = 0; i < MAX_BUFFER; i++) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);

	numVertices = 0;
	texture = 0;
	bumpTexture = 0;
	
	tangents = NULL;
	normals = NULL;
	textureCoords = NULL;
	vertices = NULL;
	colours = NULL;
	indices = NULL;
	numIndices = 0;
	type = GL_TRIANGLES;
}

Mesh::~Mesh(void) {
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &bumpTexture);
	delete[] textureCoords;
	delete[] vertices;
	delete[] colours;
	delete[] indices;
	delete[] normals;
	delete[] tangents;
}

void Mesh::Draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTexture);

	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(type, 0, numVertices);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}



Mesh* Mesh::GenerateTriangle() {
	Mesh* m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
	m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(0.8f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m->BufferData();
	return m;
}



Mesh* Mesh::GenerateQuad()
{
	Mesh* m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateCube()
{
	Mesh* m = new Mesh();
	m->numVertices = 8;
	m->numIndices = 36;
	m->type = GL_TRIANGLES;

	m->vertices = new Vector3[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->indices = new unsigned int[m->numIndices];

	m->vertices[0] = Vector3(0.0, 0.0f, 0.0f);
	m->vertices[1] = Vector3(0.5f, 0.0f, 0.0f);
	m->vertices[2] = Vector3(0.5f, 0.5f, 0.0f);
	m->vertices[3] = Vector3(0.0f, 0.5f, 0.0f);
	m->vertices[4] = Vector3(0.0, 0.0f, 0.5f);
	m->vertices[5] = Vector3(0.5f, 0.0f, 0.5f);
	m->vertices[6] = Vector3(0.5f, 0.5f, 0.5f);
	m->vertices[7] = Vector3(0.0f, 0.5f, 0.5f);
	
	m->indices[0] = 0;
	m->indices[1] = 1;
	m->indices[2] = 2;
	m->indices[3] = 0;
	m->indices[4] = 2;
	m->indices[5] = 3;
	m->indices[6] = 7;
	m->indices[7] = 4;
	m->indices[8] = 6;
	m->indices[9] = 6;
	m->indices[10] = 4;
	m->indices[11] = 5;
	m->indices[12] = 5;
	m->indices[13] = 2;
	m->indices[14] = 6;
	m->indices[15] = 2;
	m->indices[16] = 5;
	m->indices[17] = 1;
	m->indices[18] = 4;
	m->indices[19] = 7;
	m->indices[20] = 3;
	m->indices[21] = 0;
	m->indices[22] = 4;
	m->indices[23] = 3;
	m->indices[24] = 0;
	m->indices[25] = 4;
	m->indices[26] = 5;
	m->indices[27] = 5;
	m->indices[28] = 1;
	m->indices[29] = 0;
	m->indices[30] = 7;
	m->indices[31] = 3;
	m->indices[32] = 6;
	m->indices[33] = 6;
	m->indices[34] = 3;
	m->indices[35] = 2;

	for (int i = 0; i < m->numVertices; i++) {
		m->colours[i] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GeneratePoint()
{
	Mesh* m = new Mesh();
	m->numVertices = 1;
	m->type = GL_POINTS;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f, 0.0f, 0.0f);
	
	m->BufferData();
	return m;
}

void* Mesh::getColorBufferPointer()
{
	void* p = nullptr;
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
	p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return p;
}

void Mesh::BufferData() {
	glBindVertexArray(arrayObject);
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);

	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	if (colours) {
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	if (normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	if (tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	glBindVertexArray(0);
}

void Mesh::GenerateNormals()
{
	if (!normals) {
		normals = new Vector3[numVertices];
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		normals[i] = Vector3();
	}
	if (indices) { // Generate per - vertex normals
		for (GLuint i = 0; i < numIndices; i += 3) {
			unsigned int a = indices[i];
			unsigned int b = indices[i + 1];
			unsigned int c = indices[i + 2];
			Vector3 normal = Vector3::Cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));
			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	} else { // It 's just a list of triangles , so generate face normals
		for (GLuint i = 0; i < numVertices; i += 3) {
			Vector3 & a = vertices[i];
			Vector3 & b = vertices[i + 1];
			Vector3 & c = vertices[i + 2];
			Vector3 normal = Vector3::Cross(b - a, c - a);
			
			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}
	
	for (GLuint i = 0; i < numVertices; ++i) {
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents()
{
	if (!tangents) {
		tangents = new Vector3[numVertices];
	}
	if (!textureCoords) {
		return; // Can 't use tex coords if there aren 't any !
	}

	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i] = Vector3();
	}
	
	if (indices) {
		for (GLuint i = 0; i < numIndices; i += 3) {
			int a = indices[i];
			int b = indices[i + 1];
			int c = indices[i + 2];
			
			Vector3 tangent = GenerateTangent(vertices[a], vertices[b],
				vertices[c], textureCoords[a],
				textureCoords[b], textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	} else {
		for (GLuint i = 0; i < numVertices; i += 3) {
			Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1],
				vertices[i + 2], textureCoords[i],
				textureCoords[i + 1], textureCoords[i + 2]);
			tangents[i] += tangent;
			tangents[i + 1] += tangent;
			tangents[i + 2] += tangent;
		}
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3& a, const Vector3& b, const Vector3& c, const Vector2& ta, const Vector2& tb, const Vector2& tc)
{
	Vector2 coord1 = tb - ta;
	Vector2 coord2 = tc - ta;
	
	Vector3 vertex1 = b - a;
	Vector3 vertex2 = c - a;
	
	Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);
	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}
