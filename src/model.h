#ifndef MODEL_H
#define MODEL_H
#define GLEW_STATIC

#include <GL/glew.h>

#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include "Camera.h"

#include "tiny_obj_loader.h"
class Model
{
    GLuint loadShader(const char *filepath, GLenum type);
    void GLInit();

    unsigned int VAO, VBO, EBO;
    GLuint shaderProgram;
    GLint uniTrans, uniView, uniProj, uniColor, uniParent;
    std::vector<GLuint> triangles;
    std::vector<float> vertices;
    std::vector<float> normals;

public:
    Model();
    void loadFromObj(std::string path, int hasTextures);
    void loadFromNV(std::string path);
    void render(Camera &camera);
    void render(Camera &camera, float r, float g, float b, float a);
    glm::mat4 model;
    glm::mat4 parentPosition;
};

#endif // MODEL_H
