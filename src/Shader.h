#pragma once

#include "GLFW/glfw3.h"
#include "Resource.h"
#include "gl/glew.h"

class Shader : public Resource
{
public:
 Shader(std::string name, const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);

 explicit Shader(std::string name);

 Shader(std::string dir, void *args);

 void *Create(std::string name, const char *vertexPath, const char *fragmentPath, const char *geometryPath);

 ~Shader() override;

 unsigned int CompileShader(unsigned int type, const std::string &source);

 void CheckCompileErrors(GLuint shader, std::string type);

 unsigned int GetID();

 void Use() const { glUseProgram(shader_ID); }

 const std::string &GetName() override { return name; };
#pragma region Uniform functions

 // ------------------------------------------------------------------------
 void setBool(const std::string &name, bool value) const
 {
  glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), (int)value);
 }

 // ------------------------------------------------------------------------
 void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), value); }

 // ------------------------------------------------------------------------
 void setFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(shader_ID, name.c_str()), value); }

 // ------------------------------------------------------------------------
 void setVec2(const std::string &name, const glm::vec2 &value) const
 {
  glUniform2fv(glGetUniformLocation(shader_ID, name.c_str()), 1, &value[0]);
 }

 void setVec2(const std::string &name, float x, float y) const
 {
  glUniform2f(glGetUniformLocation(shader_ID, name.c_str()), x, y);
 }

 // ------------------------------------------------------------------------
 void setVec3(const std::string &name, const glm::vec3 &value) const
 {
  glUniform3fv(glGetUniformLocation(shader_ID, name.c_str()), 1, &value[0]);
 }

 void setVec3(const std::string &name, float x, float y, float z) const
 {
  glUniform3f(glGetUniformLocation(shader_ID, name.c_str()), x, y, z);
 }

 // ------------------------------------------------------------------------
 void setVec4(const std::string &name, const glm::vec4 &value) const
 {
  glUniform4fv(glGetUniformLocation(shader_ID, name.c_str()), 1, &value[0]);
 }

 void setVec4(const std::string &name, float x, float y, float z, float w) const
 {
  glUniform4f(glGetUniformLocation(shader_ID, name.c_str()), x, y, z, w);
 }

 // ------------------------------------------------------------------------
 void setMat2(const std::string &name, const glm::mat2 &mat) const
 {
  glUniformMatrix2fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
 }

 // ------------------------------------------------------------------------
 void setMat3(const std::string &name, const glm::mat3 &mat) const
 {
  glUniformMatrix3fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
 }

 // ------------------------------------------------------------------------
 void setMat4(const std::string &name, const glm::mat4 &mat) const
 {
  glUniformMatrix4fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
 }

#pragma endregion

protected:
 unsigned int shader_ID = 0;
};