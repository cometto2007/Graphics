#include "Tree.h"

Tree::Tree()
{
	Loader l = Loader::getInstance();
	objMesh = l.getTree();
	vector<Mesh*> childs = objMesh->getChilds();

	objMesh->setTexture(l.getSandText());
	childs[0]->setTexture(l.getGrassTex());
}

void Tree::Draw(const OGLRenderer& r)
{
	Loader l = Loader::getInstance();
	vector<Mesh*> childs = objMesh->getChilds();
	objMesh->Draw();
	
	r.BindTextureToSamplerAndUniform(12, l.getGrassMap(), "grassMap", r.GetCurrentShader(), GL_TEXTURE_2D);
	r.BindTextureToSamplerAndUniform(5, l.getDeptMap(), "deptTex", r.GetCurrentShader(), GL_TEXTURE_2D);

	
	r.BindTextureToSamplerAndUniform(6, childs[0]->GetTexture(), "diffuseTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	childs[0]->Draw();
	r.BindTextureToSamplerAndUniform(6, objMesh->GetTexture(), "diffuseTex", r.GetCurrentShader(), GL_TEXTURE_2D);
	objMesh->Draw();
}
