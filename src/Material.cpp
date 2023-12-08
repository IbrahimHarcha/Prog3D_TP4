// Local includes
#include "Material.h"
#include "Shader.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

#include "Texture.h"


float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}

	if (m_programSky != 0) {
        glDeleteProgram(m_programSky);
    }
}

GLuint skyboxVAO, skyboxVBO;


void Material::init() {
	// TODO : Change shader by your
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	// m_texture = loadTexture2DFromFilePath("data/text/test1.png");
    m_texture = loadTexture2DFromFilePath("data/text/bump1.jpg");
	m_texture2 = loadTexture2DFromFilePath("data/text/bump2.jpg");
    // m_texture3 = loadTexture2DFromFilePath("data/text/bump1.jpg");

	// Skybox
	isSkybox = 1;

	m_programSky = load_shaders("shaders/unlit/vertexSkybox.glsl","shaders/unlit/fragmentSkybox.glsl");
	check();

	glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	m_cubeMap = loadCubemap({"data/skybox/right.jpg","data/skybox/left.jpg","data/skybox/top.jpg","data/skybox/bottom.jpg","data/skybox/front.jpg","data/skybox/back.jpg"});

}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind(glm::vec3 cameraPosition) {
	check();
	// glUseProgram(m_program);
	GLuint programToUse = isSkybox ? m_programSky : m_program;
    glUseProgram(programToUse);
    if(!isSkybox){
        GLint camId = getUniform("lightPosition");
        glUniform3fv(camId, 1, glm::value_ptr(cameraPosition));
    }
    internalBind();
}


void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != -1) {
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(getUniform("colorTexture"), 0);
	}

	// TODO : Add your custom parameters here
	if (m_texture2 != -1) {
    	glActiveTexture(GL_TEXTURE0+1); 
    	glBindTexture(GL_TEXTURE_2D, m_texture2);
    	glUniform1i(getUniform("bumpTexture"),1); 
	}

    if (m_texture3 != -1) {
    	glActiveTexture(GL_TEXTURE0+2); 
    	glBindTexture(GL_TEXTURE_2D, m_texture3);
    	glUniform1i(getUniform("normalTexture"),2);
	}

    if (m_cubeMap != -1 && isSkybox) {
        glActiveTexture(GL_TEXTURE0+3);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
    }
}


void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
    if(!isSkybox){
        glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	    glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	    glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
    }else{
        glUniformMatrix4fv(glGetUniformLocation(m_programSky, "view"), 1, false, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(m_programSky, "projection"), 1, false, glm::value_ptr(projectionMatrix));
    }
}

// GLint Material::getAttribute(const std::string& in_attributeName) {
// 	check();
// 	return glGetAttribLocation(m_program, in_attributeName.c_str());
// }

// GLint Material::getUniform(const std::string& in_uniformName) {
// 	check();
// 	return glGetUniformLocation(m_program, in_uniformName.c_str());
// }

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
    GLuint program = isSkybox ? m_programSky : m_program;
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}


GLint Material::getUniform(const std::string& in_uniformName) {

    GLuint program = isSkybox ? m_programSky : m_program;
    check();
    return glGetUniformLocation(program, in_uniformName.c_str());
}
