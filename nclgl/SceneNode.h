#pragma once
#include <vector>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include "OBJMesh.h"

class SceneNode {
public:
	SceneNode(Mesh* m = nullptr, Vector4 colour = Vector4(1, 1, 1, 1), OBJMesh * mesh = nullptr);
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Shader* GetShader() const { return shader; }
	void SetShader(Shader* s) { shader = s; }

	Shader* GetShadowShader() const { return shadowShader; }
	void SetShadowShader(Shader* s) { shadowShader = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	OBJMesh* getObjMesh() const { return objMesh; }
	void setObjMesh(OBJMesh* m) { objMesh = m; }

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

	void AddChild(SceneNode* s);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer& r);

	std::vector<SceneNode*>::const_iterator getChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator getChildIteratorEnd() { return children.end(); }

protected:
	SceneNode* parent;
	Mesh* mesh;
public:
	Matrix4 texture_matrix() const
	{
		return textureMatrix;
	}

	void set_texture_matrix(const Matrix4& texture_matrix)
	{
		textureMatrix = texture_matrix;
	}

protected:
	OBJMesh* objMesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;
	Shader* shader;
	Shader* shadowShader;
	std::vector<SceneNode*> children;

	Matrix4 textureMatrix;

	float distanceFromCamera;
	float boundingRadius;

};
