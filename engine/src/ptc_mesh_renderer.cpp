#include "ptc_mesh_renderer.hpp"
#include "ptc_camera.hpp"
#include "ptc_console.hpp"
#include "ptc_light.hpp"
#include "ptc_material.hpp"
#include "ptc_mesh_filter.hpp"
#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"

bool MeshRenderer::FindMeshFilter()
{
    this->filterRef = GetComponent<MeshFilter>();
    return this->filterRef != nullptr;
}
MeshRenderer::MeshRenderer() { this->material = Material(); }
MeshRenderer::MeshRenderer(Material material)
{
    this->material = material;
}
void MeshRenderer::Start()
{
    if (!MeshRenderer::FindMeshFilter()) Console::WriteLine(Console::FormatString("No MeshFilter found on object: %s", parentObject->name.c_str()));
}
void MeshRenderer::Update()
{
    if (!this->filterRef || !Camera::main) return;

    Mesh* mesh = &filterRef->mesh;

    this->material.shader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->material.texture);
	this->material.shader.setInt("Texture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->material.specularMap);
	this->material.shader.setInt("SpecularMap", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->material.normalMap);
	this->material.shader.setInt("NormalMap", 2);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, parentObject->transform.position);
    model *= glm::mat4_cast(parentObject->transform.rotation);

	Object* cam = Object::Find("camera");
	Camera* camComp = cam->GetComponent<Camera>();

	model = glm::scale(model, parentObject->transform.scale);
	this->material.shader.setMatrix4x4("model", model);
	this->material.shader.setFloat4("baseCol", (this->material.shader.renderAsWireframe) ? glm::vec4(glm::vec3(0.0f), 1.0f) : (GetComponent<Light>() != nullptr) ? glm::vec4(GetComponent<Light>()->lightColor, 1.0f) * this->material.baseColor : this->material.baseColor);
	this->material.shader.setFloat("specularStrength", this->material.specularStrength);
	this->material.shader.setInt("specularExponent", this->material.specularExponent);
	this->material.shader.setFloat3("ambientLight", Light::ambientLight * glm::vec3(Light::ambientLightIntensity));

    this->material.shader.setInt("lightNum", (int)Light::GetLights(Point).size());
    this->material.shader.setFloat3v("lightPos", (GLsizei)Light::GetLights(Point).size(), Light::GetLightPositions(Point));
	this->material.shader.setFloat3v("lightCol", (GLsizei)Light::GetLights(Point).size(), Light::GetLightColors(Point));

	this->material.shader.setInt("dirLightNum", (int)Light::GetLights(Directional).size());
	this->material.shader.setFloat3v("dirLightDir", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightDirections(Directional));
	this->material.shader.setFloat3v("dirLightCol", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightColors(Directional));

	this->material.shader.setInt("spotLightNum", (int)Light::GetLights(Spot).size());
	this->material.shader.setFloat3v("spotLightDir", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightDirections(Spot));
	this->material.shader.setFloat3v("spotLightPos", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightPositions(Spot));
	this->material.shader.setFloat3v("spotLightCol", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightColors(Spot));
	this->material.shader.setFloat1v("spotLightCutoff", (GLsizei)Light::GetSpotlightCutoff().size(), Light::GetSpotlightCutoff());
	this->material.shader.setFloat1v("spotLightFocus", (GLsizei)Light::GetSpotlightFocus().size(), Light::GetSpotlightFocus());

	this->material.shader.setFloat3("cameraPos", Camera::main->parentObject->transform.position);

    glBindVertexArray(mesh->VAO);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    
    GL_CHECK_ERROR();
}

// void Mesh::drawInstance()
// {
// 	if (!visible || !enabled || !parentObject->enabled) return;
// 	Material* mat = GetComponent<Material>();

// 	mat->shader.use();
// 	GLint polygonMode[2];
// 	glGetIntegerv(GL_POLYGON_MODE, polygonMode);
// 	if (mat->shader.renderAsWireframe || Renderer::renderMode == GL_LINE)
// 	{
// 		glDisable(GL_CULL_FACE);
// 		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 	}

// 	glBindVertexArray(Renderer::VAO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);

// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);

// 	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
// 	glEnableVertexAttribArray(2);

// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, mat->texture);
// 	mat->shader.setInt("Texture", 0);
// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, mat->specularMap);
// 	mat->shader.setInt("SpecularMap", 1);
// 	glActiveTexture(GL_TEXTURE2);
// 	glBindTexture(GL_TEXTURE_2D, mat->normalMap);
// 	mat->shader.setInt("NormalMap", 2);

// 	glm::mat4 model = glm::mat4(1.0f);
// 	model = glm::translate(model, parentObject->transform.position);
	
// 	Object* cam = Object::Find("camera");
// 	Camera* camComp = cam->GetComponent<Camera>();
// 	glm::mat4 billboardRotation(1.0f);
// 	switch (renderType) { // Implement the other 2 billboard types!
// 		case RenderType::NORMAL:
// 			model *= glm::mat4_cast(parentObject->transform.rotation);
// 			break;
// 		case RenderType::CAMERA_FACING_BILLBOARD:
// 			model *= glm::mat4(glm::quatLookAt(glm::normalize(cam->transform.position - parentObject->transform.position), cam->GetComponent<Camera>()->WorldUp));
// 			break;
// 		case RenderType::Y_AXIS_ONLY_BillBOARD:
// 			model *= glm::mat4(glm::quatLookAt(glm::normalize(glm::vec3(cam->transform.position.x, parentObject->transform.position.y, cam->transform.position.z) - parentObject->transform.position), cam->GetComponent<Camera>()->WorldUp));
// 			break;
// 		default:
// 			model *= glm::mat4_cast(parentObject->transform.rotation);
// 			break;
// 	}

// 	model = glm::scale(model, parentObject->transform.scale);
// 	mat->shader.setMatrix4x4("model", model);
// 	mat->shader.setFloat4("baseCol", (mat->shader.renderAsWireframe || polygonMode[1] == GL_LINE) ? glm::vec4(glm::vec3(0.0f), 1.0f) : (GetComponent<Light>() != nullptr) ? glm::vec4(GetComponent<Light>()->lightColor, 1.0f) * mat->baseColor : mat->baseColor);
// 	mat->shader.setFloat("specularStrength", GetComponent<Material>()->specularStrength);
// 	mat->shader.setInt("specularExponent", GetComponent<Material>()->specularExponent);
// 	mat->shader.setFloat3("ambientLight", Light::ambientLight * glm::vec3(Light::ambientLightIntensity));

// 	mat->shader.setInt("lightNum", (int)Light::GetLights(Point).size());
// 	mat->shader.setFloat3v("lightPos", (GLsizei)Light::GetLights(Point).size(), Light::GetLightPositions(Point));
// 	mat->shader.setFloat3v("lightCol", (GLsizei)Light::GetLights(Point).size(), Light::GetLightColors(Point));
	
// 	mat->shader.setInt("dirLightNum", (int)Light::GetLights(Directional).size());
// 	mat->shader.setFloat3v("dirLightDir", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightDirections(Directional));
// 	mat->shader.setFloat3v("dirLightCol", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightColors(Directional));

// 	mat->shader.setInt("spotLightNum", (int)Light::GetLights(Spot).size());
// 	mat->shader.setFloat3v("spotLightDir", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightDirections(Spot));
// 	mat->shader.setFloat3v("spotLightPos", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightPositions(Spot));
// 	mat->shader.setFloat3v("spotLightCol", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightColors(Spot));
// 	mat->shader.setFloat1v("spotLightCutoff", (GLsizei)Light::GetSpotlightCutoff().size(), Light::GetSpotlightCutoff());
// 	mat->shader.setFloat1v("spotLightFocus", (GLsizei)Light::GetSpotlightFocus().size(), Light::GetSpotlightFocus());

// 	mat->shader.setFloat3("cameraPos", Camera::main->parentObject->transform.position);
		
// 	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
// 	glEnable(GL_CULL_FACE);
// }