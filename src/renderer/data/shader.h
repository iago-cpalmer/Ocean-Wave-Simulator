#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct {
	unsigned int ShaderProgram;

	unsigned int ModelMatrixUniform;
	unsigned int ViewMatrixUniform;
	unsigned int ProjectionMatrixUniform;
} Shader;

typedef struct
{
	unsigned int Id;
	unsigned int GenerationId;
} ShaderHandle;

void create_shader(Shader* rpShader, const char* rVertexShaderFile, const char* rFragmentShaderFile);

int read_shader_from_file(std::string& rSt, const char* rPath);

inline void use_shader(Shader* rpShader)
{
	glUseProgram(rpShader->ShaderProgram);
}

inline void set_uniform_1i(Shader* rpShader, const char* rUniformName, unsigned int rValue)
{
	glUniform1i(glGetUniformLocation(rpShader->ShaderProgram, rUniformName), rValue);
}

inline void set_uniform_vec3(Shader* rpShader, const char* rUniformName, glm::vec3 rValue)
{
	glUniform3f(glGetUniformLocation(rpShader->ShaderProgram, rUniformName), rValue.x, rValue.y, rValue.z);
}

inline void set_uniform_vec2(Shader* rpShader, const char* rUniformName, glm::vec2 rValue)
{
	glUniform2f(glGetUniformLocation(rpShader->ShaderProgram, rUniformName), rValue.x, rValue.y);
}

inline void set_uniform_float(Shader* rpShader, const char* rUniformName, float rValue)
{
	glUniform1f(glGetUniformLocation(rpShader->ShaderProgram, rUniformName), rValue);
}

inline void set_uniform_int(Shader* rpShader, const char* rUniformName, int rValue)
{
	glUniform1i(glGetUniformLocation(rpShader->ShaderProgram, rUniformName), rValue);
}


inline void shader_set_model_matrix(Shader* rpShader, glm::mat4x4 rMatrix)
{
	glUniformMatrix4fv(rpShader->ModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(rMatrix));
}

inline void shader_set_view_matrix(Shader* rpShader, glm::mat4x4 rMatrix)
{
	glUniformMatrix4fv(rpShader->ViewMatrixUniform, 1, GL_FALSE, glm::value_ptr(rMatrix));
}

inline void shader_set_projection_matrix(Shader* rpShader, glm::mat4x4 rMatrix)
{
	glUniformMatrix4fv(rpShader->ProjectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(rMatrix));
}



#endif // !SHADER_H
