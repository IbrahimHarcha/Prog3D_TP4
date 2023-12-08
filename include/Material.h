#ifndef MATERIAL_H_DEFINED
#define MATERIAL_H_DEFINED
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>

struct Material {
	// Shader program
	GLuint m_program;
	GLuint m_programSky;

	bool isSkybox;

	// Material parameters
	glm::vec4 m_color;
	GLint m_texture;
	GLint m_texture2;
	GLint m_texture3;
	
	GLint m_cubeMap;

	inline void check() {
		if (m_program == 0) {
			throw std::runtime_error("Shader program not initialized");
		}
	}

	Material(): m_program(0) {

	}

	virtual ~Material();

	virtual void init();

	virtual void clear();

	void bind(glm::vec3 cameraPosition);
	
	void bindSkybox();

	virtual void internalBind();

	void setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix);

	GLint getAttribute(const std::string& in_attributeName);

	GLint getUniform(const std::string& in_uniformName);
};

#endif
