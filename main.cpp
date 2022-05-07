// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

struct Material {
    glm::vec3 color;
    float kd, ks, ka, ns;
    Material(glm::vec3 a, float b, float c, float d, float e) {
        color = a;
        kd = b;
        ks = c;
        ka = d;
        ns = e;
    }
};
struct Light {
    glm::vec3 position, color;
    float intensity;

    Light(glm::vec3 a, glm::vec3 b, float c) {
        position = a;
        color = b;
        intensity = c;
    }
};

vector<float> loadObj(string filename) {
    string line, text;
    ifstream in(filename);

    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec3> facesVertices;
    vector<glm::vec3> facesNormals;

    vector<float> vertexes;

    //read file, vertices and faces
    while (getline(in, line)) {
        //read in vertices
        if (line.rfind("v ", 0) == 0) {
            vector<string> tempVertex;
            istringstream iss(line);
            string temp;
            while (getline(iss, temp, ' ')) {
                //cout << temp << endl;
                tempVertex.push_back(temp);
            }
            float x = stof(tempVertex[1]);
            float y = stof(tempVertex[2]);
            float z = stof(tempVertex[3]);
            vertices.push_back(glm::vec3(x, y, z));
        }
        //read in normals
        if (line.rfind("vn ", 0) == 0) {
            vector<string> tempNormal;
            istringstream iss(line);
            string temp;
            while (getline(iss, temp, ' ')) {
                //cout << temp << endl;
                tempNormal.push_back(temp);
            }
            float x = stof(tempNormal[1]);
            float y = stof(tempNormal[2]);
            float z = stof(tempNormal[3]);
            normals.push_back(glm::vec3(x, y, z));
        }
        //read in faces
        if (line.rfind("f ", 0) == 0) {
            vector<string> tempFace;
            istringstream iss(line);
            string temp;
            while (getline(iss, temp, ' ')) {
                //cout << temp << endl;
                tempFace.push_back(temp);
            }
            //tempFace = "f, 1/1/1, 14/2/2, 13/3/3"
            //need to get <1,14,13> and <1,2,3>

            //vertices
            float a = stof(tempFace[1].substr(0, tempFace[1].find_first_of("/"))) - 1;
            float b = stof(tempFace[2].substr(0, tempFace[2].find_first_of("/"))) - 1;
            float c = stof(tempFace[3].substr(0, tempFace[3].find_first_of("/"))) - 1;
            facesVertices.push_back(glm::vec3(a, b, c));

            //normals
            float d = stof(tempFace[1].substr(tempFace[1].find_last_of("/") + 1)) - 1;
            float e = stof(tempFace[2].substr(tempFace[2].find_last_of("/") + 1)) - 1;
            float f = stof(tempFace[3].substr(tempFace[3].find_last_of("/") + 1)) - 1;
            facesNormals.push_back(glm::vec3(d, e, f));
        }
    }

    //process separate triangles data
    for (int i = 0; i < facesVertices.size(); i++) {
        vertexes.push_back(vertices[facesVertices[i].x].x);
        vertexes.push_back(vertices[facesVertices[i].x].y);
        vertexes.push_back(vertices[facesVertices[i].x].z);
        vertexes.push_back(normals[facesNormals[i].x].x);
        vertexes.push_back(normals[facesNormals[i].x].y);
        vertexes.push_back(normals[facesNormals[i].x].z);

        vertexes.push_back(vertices[facesVertices[i].y].x);
        vertexes.push_back(vertices[facesVertices[i].y].y);
        vertexes.push_back(vertices[facesVertices[i].y].z);
        vertexes.push_back(normals[facesNormals[i].y].x);
        vertexes.push_back(normals[facesNormals[i].y].y);
        vertexes.push_back(normals[facesNormals[i].y].z);

        vertexes.push_back(vertices[facesVertices[i].z].x);
        vertexes.push_back(vertices[facesVertices[i].z].y);
        vertexes.push_back(vertices[facesVertices[i].z].z);
        vertexes.push_back(normals[facesNormals[i].z].x);
        vertexes.push_back(normals[facesNormals[i].z].y);
        vertexes.push_back(normals[facesNormals[i].z].z);
    }

    return vertexes;
};

float userScaleFactor = 1.0f;
float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;

int main()
{

    string line, text;
    ifstream in("sourcePHONG.vs");
    while (getline(in, line))
    {
        text += line + "\n";
    }
    GLchar const* vertexShaderSource = text.c_str();

    string aline, atext;
    ifstream ain("sourcePHONG.fs");
    while (getline(ain, aline))
    {
        atext += aline + "\n";
    }
    GLchar const* fragmentShaderSource = atext.c_str();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();
    
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_DEPTH_TEST);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    //********** loading object
    vector<float> obj = loadObj("spheretris.obj");
    //color, kd, ks, ka, ns
    Material mat = Material(glm::vec3(0.5f, 0.5f, 0.5f), 0.5f, 0.0f, 0.1f, 64.0f);
    //position, color, intensity
    Light light = Light(glm::vec3(3.0f,-2.0f,5.0f), glm::vec3(1.0f), 4.0f);   

    unsigned int numVertices = (obj.size() / 6); 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(float), &obj.front(), GL_STATIC_DRAW);

    //position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    //normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3*sizeof(float)));

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        glfwSetKeyCallback(window, key_callback);
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //add depth buffer bit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         
        // draw our first triangle
        glUseProgram(shaderProgram);
        

        //projection
        glm::mat4 projection = glm::mat4(1.0f); 
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        //view
        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        //model
        glm::mat4 model = glm::mat4(1.0f);     
            //translate
            model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
            //rotate
            model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
            //scale
            glm::vec3 scaleFactor = glm::vec3(userScaleFactor);
            model = glm::scale(model, scaleFactor);

        //send final matrix to vertex shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

        //send material and light to fragment shader
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, &light.position[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, &light.color[0]);
        glUniform1f(glGetUniformLocation(shaderProgram, "lightIntensity"), light.intensity);

        glUniform3fv(glGetUniformLocation(shaderProgram, "objColor"), 1, &mat.color[0]);
        glUniform1f(glGetUniformLocation(shaderProgram, "ka"), mat.ka);
        glUniform1f(glGetUniformLocation(shaderProgram, "ks"), mat.ks);
        glUniform1f(glGetUniformLocation(shaderProgram, "kd"), mat.kd);
        glUniform1f(glGetUniformLocation(shaderProgram, "ns"), mat.ns);
        

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        userScaleFactor += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        userScaleFactor -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        rotX -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        rotX += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotY -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotY += 1.0f;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        userScaleFactor = 1.0f;
        rotX = 0.0f;
        rotY = 0.0f;
        rotZ = 0.0f;
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}