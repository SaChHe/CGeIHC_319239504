//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks


void CrearCubo()
{
    unsigned int cubo_indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };
    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
    unsigned int indices_piramide_triangular[] = {
        0,1,2,
        1,3,2,
        3,0,2,
        1,0,3
    };
    GLfloat vertices_piramide_triangular[] = {
        -0.5f, -0.5f,0.0f,   //0
         0.5f,-0.5f,0.0f,    //1
         0.0f, 0.5f, -0.5f,  //2
         0.0f,-0.5f,-0.5f,   //3
    };
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
    meshList.push_back(obj1);
}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

    //constantes utilizadas en los ciclos for
    int n, i;
    //cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    //ciclo for para crear los vértices de las paredes del cilindro
    for (n = 0; n <= (res); n++) {
        if (n != res) {
            x = R * cos((n)*dt);
            z = R * sin((n)*dt);
        }
        else {
            //caso para terminar el círculo
            x = R * cos((0) * dt);
            z = R * sin((0) * dt);
        }
        for (i = 0; i < 6; i++) {
            switch (i) {
            case 0:
                vertices.push_back(x);
                break;
            case 1:
                vertices.push_back(y);
                break;
            case 2:
                vertices.push_back(z);
                break;
            case 3:
                vertices.push_back(x);
                break;
            case 4:
                vertices.push_back(0.5f);
                break;
            case 5:
                vertices.push_back(z);
                break;
            }
        }
    }

    //ciclo for para crear la circunferencia inferior
    for (n = 0; n <= (res); n++) {
        x = R * cos((n)*dt);
        z = R * sin((n)*dt);
        for (i = 0; i < 3; i++) {
            switch (i) {
            case 0:
                vertices.push_back(x);
                break;
            case 1:
                vertices.push_back(-0.5f);
                break;
            case 2:
                vertices.push_back(z);
                break;
            }
        }
    }

    //ciclo for para crear la circunferencia superior
    for (n = 0; n <= (res); n++) {
        x = R * cos((n)*dt);
        z = R * sin((n)*dt);
        for (i = 0; i < 3; i++) {
            switch (i) {
            case 0:
                vertices.push_back(x);
                break;
            case 1:
                vertices.push_back(0.5f);
                break;
            case 2:
                vertices.push_back(z);
                break;
            }
        }
    }

    //Se generan los indices de los vértices
    for (i = 0; i < (int)vertices.size(); i++)
        indices.push_back(i);

    //se genera el mesh del cilindro
    Mesh* cilindro = new Mesh();
    cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

    //constantes utilizadas en los ciclos for
    int n, i;
    //cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    //vértice superior (punta del cono)
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    //ciclo for para crear la circunferencia de la base del cono
    for (n = 0; n <= (res); n++) {
        x = R * cos((n)*dt);
        z = R * sin((n)*dt);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    //cerramos el ciclo
    vertices.push_back(R * cos(0) * dt);
    vertices.push_back(-0.5f);
    vertices.push_back(R * sin(0) * dt);

    for (i = 0; i < res + 2; i++)
        indices.push_back(i);

    //se genera el mesh del cono
    Mesh* cono = new Mesh();
    cono->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
    vector<unsigned int> piramidecuadrangular_indices = {
        0,3,4,
        3,2,4,
        2,1,4,
        1,0,4,
        0,1,2,
        0,2,4
    };
    vector<GLfloat> piramidecuadrangular_vertices = {
         0.5f,-0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,
         0.0f, 0.5f, 0.0f,
    };
    Mesh* piramide = new Mesh();
    piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
    meshList.push_back(piramide);
}

// Tetraedro


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShader);
    shaderList.push_back(*shader2);
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CrearCubo();                // [0]
    CrearPiramideTriangular();  // [1]
    CrearCilindro(5, 1.0f);     // [2]
    CrearCono(25, 2.0f);        // [3]
    CrearPiramideCuadrangular();// [4]
    CreateShaders();
  

    // Cámara (posiciones y velocidad de movimiento/rotación)
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f, 0.0f,
        0.3f, 0.3f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
        mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f, 100.0f);

    sp.init();  // inicializar esfera
    sp.load();  // enviar la esfera al shader

    glm::mat4 model(1.0f);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        //Recibir eventos del usuario
        glfwPollEvents();
        //Cámara
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        //Limpiar la ventana y el buffer de profundidad
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activamos el primer shader (shaderList[0])
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        // Solo es necesario setear projection y view una vez (si no cambiamos de shader ni de proyección)
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        // --- PIRÁMIDE GRANDE (NEGRA) ---
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        
        model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        //pirámides verdes
       //IZQUIERDA ARRIBA
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.0f, 0.38f, -2.42f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));
       
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.2f, -0.04f, -2.2f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DERECHA ARRIBA
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.2f, -0.04f, -2.2f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DRECHA ABAJO
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.4f, -0.46f, -2.0f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //IZQUIERDA ABAJO
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.4f, -0.46f, -2.0f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, -0.46f, -2.0f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        //verde invertida
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, 0.05f, -2.25f));

        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

      
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, -0.38f, -2.05f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.2f, -0.38f, -2.05f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

      
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

       


        //PIRÁMIDES ROJAS
       //color rojo
       
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.05f, 0.38f, -2.48f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.035f, -0.02f, -2.48f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DERECHA ARRIBA
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.24f, -0.02f, -2.28f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DRECHA ABAJO
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.45f, -0.45f, -2.08f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //IZQUIERDA ABAJO
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 



        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.03f, -0.46f, -2.5f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.23f, -0.46f, -2.29f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        //roja invertida
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.08f, -0.01f, -2.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.28f, -0.45f, -2.3f));
        
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.09f, -0.45f, -2.54f));
        
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 



       //color azul


        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.05f, 0.38f, -2.48f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.035f, -0.02f, -2.48f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DERECHA ARRIBA
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.24f, -0.02f, -2.28f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DRECHA ABAJO
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.45f, -0.45f, -2.08f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //IZQUIERDA ABAJO
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 
       
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.03f, -0.46f, -2.5f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.23f, -0.46f, -2.29f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //azul invertida
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        //azul invertida
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.08f, -0.01f, -2.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.28f, -0.45f, -2.3f));
        
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.09f, -0.45f, -2.54f));
        
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 


        //OTRO AZUL CIELO
 
       //IZQUIERDA ARRIBA
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.0f, -0.52f, -2.45f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.2f, -0.53f, -2.25f));
        
        model = glm::scale(model, glm::vec3(0.36f, 0.36f, 0.36f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DERECHA ARRIBA
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.2f, -0.53f, -2.25f));
        
        model = glm::scale(model, glm::vec3(0.36f, 0.36f, 0.36f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //DRECHA ABAJO
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.38f, -0.53f, -2.05f));
        
        model = glm::scale(model, glm::vec3(0.36f, 0.36f, 0.36f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //IZQUIERDA ABAJO
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.38f, -0.53f, -2.05f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, -0.53f, -2.05f));
        
        model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

       
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, -0.6f, -2.42f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.2f, -0.6f, -2.23f));
        
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 

        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-0.2f, -0.6f, -2.23f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


        color = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[1]->RenderMesh(); 



        glUseProgram(0);

        // Intercambiamos buffers
        mainWindow.swapBuffers();
    }

    return 0;
}
