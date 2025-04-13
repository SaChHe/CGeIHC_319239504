/*
Pr�ctica 7: Iluminaci�n 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture baseColor;
Texture normal;
Texture metallic;
Texture roughness;
Texture emissive;

bool luzLamparaEncendida = true;
bool luzEsferaEncendida = false;
bool encender_lampara = true; 

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model cuerpo_coche;
Model cofre;
Model llanta_frontal_1;
Model llanta_frontal_2;
Model llanta_posterior_1;
Model llanta_posterior_2;
Model lampara;
Model esfera;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

bool spotlightCofreEncendido;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	baseColor = Texture("dragonballs/textures/DragonBall_BaseColor.png");
	baseColor.LoadTextureA();
	
	normal = Texture("dragonballs/textures/DragonBall_Normal.png");
	normal.LoadTextureA();
	metallic = Texture("dragonballs/textures/DragonBall_Metallic.png");
	metallic.LoadTextureA();
	roughness = Texture("dragonballs/textures/DragonBall_Roughness.png");
	roughness.LoadTextureA();
	emissive = Texture("dragonballs/textures/DragonBall_Emissive.png");
	emissive.LoadTextureA();

	cuerpo_coche = Model();
	cuerpo_coche.LoadModel("Models/carro_texturizado.obj");
	cofre = Model();
	cofre.LoadModel("Models/cofre_texturizado.obj");
	llanta_frontal_1 = Model();
	llanta_frontal_1.LoadModel("Models/llanta_frontal_texturizada.obj");
	llanta_frontal_2 = Model();
	llanta_frontal_2.LoadModel("Models/llanta_frontal_texturizada.obj");
	llanta_posterior_1 = Model();
	llanta_posterior_1.LoadModel("Models/llanta_posterior_texturizada.obj");
	llanta_posterior_2 = Model();
	llanta_posterior_2.LoadModel("Models/llanta_posterior_texturizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");
	
	esfera = Model();
	esfera.LoadModel("dragonballs/source/Dragonballs.fbx");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.5f, 0.0f,    // Luz de esfera (naranja)
		0.8f, 1.5f,
		5.0f, 5.0f, -5.0f,     // Posición inicial
		0.2f, 0.1f, 0.02f);
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//luz puntual blanca
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,  //color blanco
		0.5f, 1.0f,
		3.0f, 0.0f, -1.0f,                       //posición
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.8f,  //luz  blanca
		1.5f, 3.0f,                             
		0.0f, 0.0f, 0.0f,                        
		-1.0f, -0.2f, 0.0f,                      
		1.0f, 0.1f, 0.01f,                      
		30.0f);
	spotLightCount++;
	//luz creada 
	spotLights[2] = SpotLight(1.0f, 0.2f, 0.1f,  //luz roja
		1.5f, 2.0f,                              
		0.0f, 0.0f, 0.0f,                        
		1.0f, -0.2f, 0.0f,                       
		1.0f, 0.1f, 0.01f,                       
		25.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 0.5f, 0.0f,   //luz naranja
		1.0f, 2.0f,                              
		0.0f, 0.0f, 0.0f,                         
		-0.7f, 0.7f, 0.0f,                        
		1.0f, 0.0f, 0.0f,                       
		25.0f);                                   
	spotLightCount++;





	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		
		


		//control de luz del cofre
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			
			spotLights[3].SetColor(1.0f, 0.5f, 0.0f); //color naranja cuando se enciende 

			glm::vec3 posicionCoche = glm::vec3(mainWindow.getmover(), -2.0f, -2.0f);

			glm::vec3 offsetCofre = glm::vec3(-1.5f * 0.2f, 1.8f * 0.2f, 0.0f * 0.2f);
			glm::vec3 posicionSpot = posicionCoche + offsetCofre;
			glm::vec3 direccionDiagonal = glm::normalize(glm::vec3(-0.7f, 0.7f, 0.0f));

			spotLights[3].SetFlash(posicionSpot, direccionDiagonal);
		}
		else {
			spotLights[3].SetColor(0.0f, 0.0f, 0.0f); //se apaga la luz
		}

		//control de luces del coche
		if (mainWindow.getsKeys()[GLFW_KEY_X]) { //el coche avanza
			
			spotLights[1].SetColor(1.0f, 1.0f, 0.8f); //se enciende luz blanca

			glm::vec3 posicionCoche = glm::vec3(mainWindow.getmover(), -2.0f, -2.0f);
			glm::vec3 offsetDelantero = glm::vec3(-3.0f * 0.2f, 1.0f * 0.2f, 0.0f * 0.2f);
			glm::vec3 posicionDelantera = posicionCoche + offsetDelantero;
			glm::vec3 direccionDelantera = glm::normalize(glm::vec3(-1.0f, -0.2f, 0.0f));

			spotLights[1].SetFlash(posicionDelantera, direccionDelantera);

			
			spotLights[2].SetColor(0.0f, 0.0f, 0.0f);//se apafa luz trasera
		}
		else if (mainWindow.getsKeys()[GLFW_KEY_Z]) { 
			
			spotLights[2].SetColor(1.0f, 0.2f, 0.1f); //se enciende luz roja

			glm::vec3 posicionCoche = glm::vec3(mainWindow.getmover(), -2.0f, -2.0f);
			glm::vec3 offsetTrasero = glm::vec3(3.0f * 0.2f, 1.0f * 0.2f, 0.0f * 0.2f);
			glm::vec3 posicionTrasera = posicionCoche + offsetTrasero;
			glm::vec3 direccionTrasera = glm::normalize(glm::vec3(1.0f, -0.2f, 0.0f));

			spotLights[2].SetFlash(posicionTrasera, direccionTrasera);

			
			spotLights[1].SetColor(0.0f, 0.0f, 0.0f);//se apaga la luz delantera
		}
		else {
			
			spotLights[1].SetColor(0.0f, 0.0f, 0.0f); //  apagadas cuando no se mueve el coche
			spotLights[2].SetColor(0.0f, 0.0f, 0.0f);
		}
		
//control luces puntuales


		if (mainWindow.getsKeys()[GLFW_KEY_M]) {
			encender_lampara = !encender_lampara; // Alternar estado

			// Redefinir completamente la luz (usando el operador =)
			pointLights[1] = PointLight(
				encender_lampara ? 1.0f : 0.0f,  // R (1.0 si está encendida, 0.0 si no)
				encender_lampara ? 1.0f : 0.0f,  // G
				encender_lampara ? 1.0f : 0.0f,  // B
				encender_lampara ? 0.5f : 0.0f,  // AmbientIntensity
				encender_lampara ? 1.0f : 0.0f,  // DiffuseIntensity
				3.0f, 0.0f, -1.0f,             // Posición (fija)
				0.3f, 0.1f, 0.1f                 // Atenuación (constante, lineal, cuadrática)
			);
		}
		//luz naranja de esfera
		if (mainWindow.getsKeys()[GLFW_KEY_E]) {
			luzEsferaEncendida = !luzEsferaEncendida;
			pointLights[0].SetColor(luzEsferaEncendida ? 1.0f : 0.0f,
				luzEsferaEncendida ? 0.5f : 0.0f,
				luzEsferaEncendida ? 0.0f : 0.0f);
		}

		// Actualizar posición de la luz de la esfera
		pointLights[0].position = glm::vec3(5.0f, 5.0f, -5.0f);  // Alternativa si SetPos no funciona

		// Actualizar posición de la luz de la lámpara
		pointLights[1].position = glm::vec3(3.0f, 0.0f, -3.0f);

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();
		
		glm::mat4 modelDragonBall(1.0f);
		modelDragonBall = glm::translate(modelDragonBall, glm::vec3(5.0f, 5.0f, -5.0f)); // Posición frente a la cámara
		modelDragonBall = glm::scale(modelDragonBall, glm::vec3(6.0f)); // Ajusta la escala según necesidad
		shaderList[0].UseShader();
		// Configurar shader y propiedades de material
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDragonBall));
		glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f); // Color base blanco (para no alterar la textura)

		baseColor.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esfera.RenderModel();

		//Coche, se mueve para adelante con X y para atras con Z
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0, -2.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmover(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, glm::radians(mainWindow.getmuevellanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		cuerpo_coche.RenderModel();


		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(35.0f, 12.0f, 25.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevellanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		model = modelaux;
		llanta_frontal_1.RenderModel();



		//declracion de llantas, enfrente Y, para atras U
		modelaux = model;
		model = glm::translate(model, glm::vec3(34.0f, 12.0f, -20.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevellanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		model = modelaux;
		llanta_posterior_2.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(-40.0f, 12.0f, -18.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevellanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		model = modelaux;
		llanta_posterior_1.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(-45.0f, 12.0f, 25.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevellanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		model = modelaux;
		llanta_frontal_2.RenderModel();


		//cofre se mueve para arriba con O y para abajo con P
		modelaux = model;
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		model = glm::translate(model, glm::vec3(-27.0f, 32.5f, -0.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevecofre()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		model = modelaux;
		cofre.RenderModel();

		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -15.0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		
		
		

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}