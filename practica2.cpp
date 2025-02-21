//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()

{
	//0
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	//1
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	//2
	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	//3
	GLfloat vertices_trianguloazul[] = {
		//  X      Y		  Z					  R     G     B
		-1.0f,  -1.0f,		 0.5f,			    0.0f,	0.0f,	1.0f,
		 1.0f,  -1.0f,		0.5f,				0.0f,	0.0f,	1.0f,		//vertices triangulo azul
		 0.0f,   1.0f,		0.5f,				0.0f,	0.0f,	1.0f,
	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18); 
	meshColorList.push_back(trianguloazul);

	//4
	GLfloat vertices_trianguloverde[] = {
		//  X      Y      Z       R     G     B
		-1.0f,  -1.0f,   0.5f,    0.0f, 0.5f, 0.0f,
		 1.0f,  -1.0f,   0.5f,    0.0f, 0.5f, 0.0f,
		 0.0f,   1.0f,   0.5f,    0.0f, 0.5f, 0.0f,
	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	//5
	GLfloat vertices_cuadradorojo[] = {
		//  X      Y      Z       R     G     B
		-0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36); // 6 vértices x 6 datos = 36
	meshColorList.push_back(cuadradorojo);

	//6
	GLfloat vertices_cuadradocafe[] = {
		//  X      Y      Z       R       G       B
		-0.5f, -0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f, -0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
		-0.5f, -0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
		 0.5f,  0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
		-0.5f,  0.5f,  0.5f,   0.478f, 0.255f, 0.067f,
	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_letra_S[]{

		-0.4f,  0.1f, 0.5f,	 0.312f, 0.491f, 0.828f,
		-0.2f,  0.1f, 0.5f,	 0.312f, 0.491f, 0.828f,
		- 0.2f,  0.2f, 0.5f,	 0.312f, 0.491f, 0.828f,//estos vértices hacen la parte superior de la S
		- 0.4f,  0.1f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f,  0.2f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.4f,  0.2f, 0.5f,  0.312f, 0.491f, 0.828f,


		- 0.4f,  0.0f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.3f,  0.0f, 0.5f, 0.312f, 0.491f, 0.828f,
		- 0.3f,  0.1f, 0.5f,	 0.312f, 0.491f, 0.828f,//esos vértices hacen la parte izquierda central de la S
		- 0.4f,  0.0f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.3f,  0.1f, 0.5f, 0.312f, 0.491f, 0.828f,
		- 0.4f,  0.1f, 0.5f, 0.312f, 0.491f, 0.828f,


		- 0.3f, -0.1f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f, -0.1f, 0.5f,	  0.312f, 0.491f, 0.828f,	//estos vértices hacen la parte derecha central de la S
		- 0.2f,  0.0f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.3f, -0.1f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f,  0.0f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.3f,  0.0f, 0.5f,  0.312f, 0.491f, 0.828f,


		- 0.4f, -0.2f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f, -0.2f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f, -0.1f, 0.5f,	  0.312f, 0.491f, 0.828f,		//estos vértices hacen la parte inferior de la S
		- 0.4f, -0.2f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.2f, -0.1f, 0.5f,  0.312f, 0.491f, 0.828f,
		- 0.4f, -0.1f, 0.5f,  0.312f, 0.491f, 0.828f,


	};

	MeshColor* letra_S = new MeshColor();
	letra_S->CreateMeshColor(vertices_letra_S, 144);
	meshColorList.push_back(letra_S);
	
	GLfloat vertices_letra_C[]{
		-0.1f,  0.1f, 0.0f,  0.017f, 0.651f, 0.940,
		 0.1f,  0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		 0.1f,  0.2f, 0.0f,	0.017f, 0.651f, 0.940,	//parte superior de la C
		-0.1f,  0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		 0.1f,  0.2f, 0.0f, 0.017f, 0.651f, 0.940,
		 -0.1f,  0.2f, 0.0f, 0.017f, 0.651f, 0.940,

		-0.1f, -0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		 0.0f, -0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		 0.0f,  0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		-0.1f, -0.1f, 0.0f,	0.017f, 0.651f, 0.940,		//parte central de la C
		0.0f,  0.1f, 0.0f, 0.017f, 0.651f, 0.940,
		-0.1f,  0.1f, 0.0f, 0.017f, 0.651f, 0.940,


		-0.1f, -0.2f, 0.0f, 0.017f, 0.651f, 0.940,
		0.1f, -0.2f, 0.0f, 0.017f, 0.651f, 0.940,
		0.1f, -0.1f, 0.0f,	0.017f, 0.651f, 0.940,	//parte inferior de l C
		-0.1f, -0.2f, 0.0f, 0.017f, 0.651f, 0.940,
		0.1f, -0.1f, 0.0f,0.017f, 0.651f, 0.940,
		-0.1f, -0.1f, 0.0f, 0.017f, 0.651f, 0.940,
	};

	MeshColor* letra_C = new MeshColor();
	letra_C->CreateMeshColor(vertices_letra_C, 108);
	meshColorList.push_back(letra_C);

	GLfloat vertices_letra_H[]{
		 0.2f, -0.2f, 0.0f,   0.773f, 0.210f, 0.345f,
		 0.25f, -0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		 0.25f,  0.2f, 0.0f,	0.773f, 0.210f, 0.345f,	//parte izquierda de la H
		 0.2f, -0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		 0.25f,  0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		 0.2f,   0.2f, 0.0f, 0.773f, 0.210f, 0.345f,


		 0.25f,  0.0f, 0.0f, 0.773f, 0.210f, 0.345f,
		 0.35f,  0.0f, 0.0f, 0.773f, 0.210f, 0.345f,
		 0.35f,  0.1f, 0.0f, 0.773f, 0.210f, 0.345f,		//parte central de la H
		 0.25f,  0.0f, 0.0f, 0.773f, 0.210f, 0.345f,
		  0.35f,  0.1f, 0.0f, 0.773f, 0.210f, 0.345f,
			0.25f,  0.1f, 0.0f, 0.773f, 0.210f, 0.345f,

		  0.35f, -0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		  0.40f, -0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		  0.40f,  0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		  0.35f, -0.2f, 0.0f, 0.773f, 0.210f, 0.345f,		//parte derecha de la H 
		  0.40f,  0.2f, 0.0f, 0.773f, 0.210f, 0.345f,
		  0.35f,  0.2f, 0.0f, 0.773f, 0.210f, 0.345f,

	};

	MeshColor* letra_H = new MeshColor();
	letra_H->CreateMeshColor(vertices_letra_H, 108);
	meshColorList.push_back(letra_H);

}


void CreateShaders()
{
	//0
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	//1
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, "rojo.frag");
	shaderList.push_back(*shaderRojo);

	//2
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, "verde.frag");
	shaderList.push_back(*shaderVerde);

	//3
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShader, "azul.frag");
	shaderList.push_back(*shaderAzul);

	//4
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, "cafe.frag");
	shaderList.push_back(*shaderCafe);

	//5
	Shader* shaderVerdeOsc = new Shader();
	shaderVerdeOsc->CreateFromFiles(vShader, "verdeOscuro.frag");
	shaderList.push_back(*shaderVerdeOsc);

	//6
	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);


}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f); //fondo blanco
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//						
		//////letra S
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();

		//////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.2f, 0.0f, -2.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[7]->RenderMeshColor();

		////letra C
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();

		//////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[8]->RenderMeshColor();


		////letra H
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();

		//////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.2f, 0.0f, -2.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[9]->RenderMeshColor();

		//cuadrdo rojo
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		////triangulo azul
		shaderList[3].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, -2.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		////cuadrado verde inferior
		shaderList[2].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		////cuadrado verde lado izquierdo
		shaderList[2].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, -0.2f, -1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		////cuadrado verde lado derechi
		shaderList[2].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.3f, -0.2f, -1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		////cuardado cafe derecho
		shaderList[4].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.85f, -1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();



		////cuadrado cafe izquierdo
		shaderList[4].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.85f, -1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();



		////triangulo verde derecho
		shaderList[5].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.45f, -1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		////triangulo verde izquierdo
		shaderList[5].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.45f, -1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/