#define TINYOBJLOADER_IMPLEMENTATION
#include "model.h"


Model::Model()
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    parentPosition = glm::mat4(1.0f);
}

void Model::loadFromObj(std::string path, int hasTextures)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str()))
    {
        throw std::runtime_error(err);
    }

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            //add vertices
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
            if (hasTextures == 1)
            {
                //add texture coordinates
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else
            {
                //add blank texture coordinates
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            triangles.push_back(triangles.size());
            triangles.push_back(triangles.size());
            triangles.push_back(triangles.size());
        }
    }
    GLInit();
}

void Model::GLInit()
{
    // generate and bind the buffers assosiated with this chunk in order to assign
    // vertices and color to the mesh
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // load the shaders from their corresponding files
    GLuint vertexShader = loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader =
        loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);

    // compile the GPU programs
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // catch any errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // create a program from the shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    // finilize the program and use it
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the array buffer to contain sections the size of a Vertex struct, and
    // pass a pointer to the vector containing them
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
        &vertices[0], GL_STATIC_DRAW);

    // pass and bind triangle data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        triangles.size() * sizeof(GLuint), &triangles[0],
        GL_STATIC_DRAW);

    // pass vertex positions to shader program
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    GLint texcoordsAttrib = glGetAttribLocation(shaderProgram, "texCoords");
    glEnableVertexAttribArray(texcoordsAttrib);
    glVertexAttribPointer(texcoordsAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    uniColor = glGetUniformLocation(shaderProgram, "objColor");
    uniTrans = glGetUniformLocation(shaderProgram, "model");
    uniView = glGetUniformLocation(shaderProgram, "view");
    uniProj = glGetUniformLocation(shaderProgram, "proj");
    uniParent = glGetUniformLocation(shaderProgram, "parentPos");
    uniIsTexOn = glGetUniformLocation(shaderProgram, "isTexOn");
    uniTexture = glGetUniformLocation(shaderProgram, "textureArray");
    uniLayer = glGetUniformLocation(shaderProgram, "layer");
    uniMRIView = glGetUniformLocation(shaderProgram, "MRIView");

    glUniform4f(uniColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glUniform1i(uniIsTexOn, 0);
    glUniform1i(uniView, view);
}

void Model::render(Camera &camera)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
    glUniformMatrix4fv(uniParent, 1, GL_FALSE, glm::value_ptr(parentPosition));
    glUniform1f(uniLayer, layer);
    glUniform1i(uniMRIView, view);

    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
}

void Model::render(Camera &camera, float r, float g, float b, float a)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
    glUniformMatrix4fv(uniParent, 1, GL_FALSE, glm::value_ptr(parentPosition));

    glUniform4f(uniColor, r, g, b, a);

    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
}

void Model::addTexture(int id)
{
    glUniform1i(uniIsTexOn, 1);
    glUniform1i(uniTexture, id);
}

GLuint Model::loadShader(const char *filepath, GLenum type)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        return 0;
    }

    long len;
    if (fseek(file, 0, SEEK_END) != 0 || (len = ftell(file)) == -1L)
    {
        fclose(file);
        return 0;
    }
    rewind(file);

    char *buffer = (char *)malloc(len);
    if (fread(buffer, 1, len, file) != len)
    {
        fclose(file);
        free(buffer);
        return 0;
    }
    fclose(file);

    GLuint shader = glCreateShader(type);
    if (shader == 0)
    {
        free(buffer);
        return 0;
    }

    glShaderSource(shader, 1, (const char *const *)&buffer, (GLint *)&len);
    free(buffer);
    return shader;
}