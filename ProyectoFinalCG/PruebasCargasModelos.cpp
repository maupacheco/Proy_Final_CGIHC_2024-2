/*
* 
* 05 - Carga de modelos e interacción
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <animatedmodel.h>
#include <iostream>

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 10.0f, 40.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 position(0.0f,0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     scaleV = 0.005f;
float     rotateCharacter = 0.0f;
float     rotateTable = 0.0f;

/**************************************************/
/*Variables para animacionB1*/
//Animacion Basica 1 RappiDelivey
bool animacionActiva = false;
float	motoPosX = -1.0f;
float	motoVelocidad = 0.2f;
bool	movDerecha_moto = true;
float	giraMoto = 90.0f;
/**************************************************/

// Shaders
Shader *staticShader;
Shader *dynamicShader;
Shader *proceduralShader;
Shader *wavesShader;


// Carga la información del modelo
Model	        *house;
Model			*piso;
Model			*pasto1;
Model			*rock;
Model			*picnic;
Model			*soportePS;
Model			*SolarPanel;
Model			*grassMesh;


//SALA NIKE
//Model			*mesaManiquieNike;
//Model			*cajaCobro;
//Model			*probadores;
//Model			*ShoesFinal;

// Model *chair, *table;
AnimatedModel* character;

//AguilaProcedural
//Model	*AguilaBlanca

//Modelo Eclipse - Animacion KeyFrames 04
Model* modelEclipseChasis;
Model* modelEclipseRearWheels;
Model* modelEclipseFrontalWheels;

//Eclipse
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);

/**************************************************/
/*Variables para el eclipse*/
// Aplicamos transformaciones del modelo
int state = 0;
float advanceCount = 0.0;
float rotCount = 0.0;
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;
int numberAdvance = 0;
int maxAdvance = 0.0;
bool avanceAutomatico = false;
// Variables animacion maquina de estados eclipse
float velocidadAnimacion = 0.5f; // Puedes ajustar este valor según sea necesario
float tiempoTranscurrido = 0.0f; // Declaración de la variable global
const float avance = 0.05f;
const float giroEclipse = 0.5f;
/**************************************************/

float proceduralTime = 0.0f;
float wavesTime = 0.0f;


float	  panel_offset = 0.0f;
float	  panel_rotation = 0.0f;
float	  panel_rotation1 = 0.0f;



// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

	// Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{
		
	}

	// glfw: Terminamos el programa y liberamos memoria
	glfwTerminate();
	return 0;
}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FBX Model Loading", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	dynamicShader = new Shader("shaders/09_vertex_skinning.vs", "shaders/09_fragment_skinning.fs");
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");

	// Máximo número de huesos: 100 por defecto
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// Actividad 2.0: Importar modelo de casa
	//house = new Model("models/ModelsMall/pisoExterior/pisoExterior.obj");
	house = new Model("models/ModelsMall/CCBlackFinal.fbx");
	piso = new Model("models/ModelsMall/pisoExterior/pisoExterior.obj");
	pasto1 = new Model("models/ModelsMall/smallGrass.fbx");
	rock = new Model("models/ModelsMall/rock.fbx");
	picnic = new Model("models/ModelsMall/picnicFinal.fbx");
	soportePS = new Model("models/ModelsMall/SolarPanels/soporteSolarPanel.obj");
	SolarPanel = new Model("models/ModelsMall/SolarPanels/SolarPanel.obj");
	grassMesh = new Model("models/IllumModels/grassMesh.fbx");




	//Sala Nike
	//mesaManiquieNike = new Model("models/ModelsMall/SalaNike/mesaManiquie1.fbx");
	//cajaCobro = new Model("models/ModelsMall/SalaNike/cajaCobro.fbx");
	//probadores = new Model("models/ModelsMall/SalaNike/probadores.fbx");
	//ShoesFinal = new Model("models/ModelsMall/SalaNike/ShoesStand/ShoesFinal.obj");

	//Ave - Aguila Animado Para Centro Comercial
	//AguilaBlanca = new Model("models/ModelsMall/AguilaBlancaFinal.obj");

	// Carro Eclipse
	modelEclipseChasis = new Model("models/ModelsMall/Eclipse/2003eclipse_chasis.obj");
	modelEclipseFrontalWheels = new Model("models/ModelsMall/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseRearWheels = new Model("models/ModelsMall/Eclipse/2003eclipse_rear_wheels.obj");


	// Dibujar en distintos modos de despliegue
	//	   GL_LINE: Modo Malla de alambre
	// 	   GL_POINT: Modo puntos
	// 	   GL_FILL: Modo sólido
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Actividad 3.0: Importar personaje
	character = new AnimatedModel("models/Pacman/Pac-Man_cel.fbx");

	// Actividad 4.0
	// Aquí cargamos los otros modelos de tipo estático
	// chair = new Model("models/chair.fbx");
	// table = new Model("models/table.fbx");

	return true;
}

bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	

	// Objeto estático (Comercial) Bien escalado y puesto
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.025f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		house->Draw(*staticShader);
	} 

	glUseProgram(0);

	//Piso Exterior Principal Escalado y modelado
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.525f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		piso->Draw(*staticShader);
	}

	glUseProgram(0);

	//Pasto 1
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.485f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		pasto1->Draw(*staticShader);

		// Aplicamos transformaciones del modelo
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(40.0f, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.385f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model1);

		pasto1->Draw(*staticShader);
	}

	glUseProgram(0);

	{
		// Activamos el shader de Phong
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-18.727f, 0.184485f, 25.388f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.007f, 0.003f, 0.005f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);

		grassMesh->Draw(*wavesShader);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.03965f, 0.184485f, 25.388f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.007f, 0.003f, 0.005f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);

		grassMesh->Draw(*wavesShader);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.64432f, 0.184485f, 25.388f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.007f, 0.003f, 0.005f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);

		grassMesh->Draw(*wavesShader);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(19.0824f, 0.184485f, 25.388f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.007f, 0.003f, 0.005f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);

		grassMesh->Draw(*wavesShader);

		wavesTime += 0.05;

	}

	glUseProgram(0);


	//mesaSalaNikeManique
	/*{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo ----> Modelo Mesa Manique
		glm::mat4 modelMesa = glm::mat4(1.0f);
		modelMesa = glm::translate(modelMesa, glm::vec3(-8.11834f, 6.95, -7.5f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMesa = glm::scale(modelMesa, glm::vec3(0.010f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelMesa);

		mesaManiquieNike->Draw(*staticShader);

		// Aplicamos transformaciones del modelo ----> Modelo Caja Registradora
		glm::mat4 modelCaja = glm::mat4(1.0f);
		modelCaja = glm::translate(modelCaja, glm::vec3(-8.11834f, 6.95, -3.5f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCaja = glm::scale(modelCaja, glm::vec3(0.010f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelCaja);

		cajaCobro->Draw(*staticShader);

		// Aplicamos transformaciones del modelo ----> Modelo Probadores
		glm::mat4 modelProbadores = glm::mat4(1.0f);
		modelProbadores = glm::translate(modelProbadores, glm::vec3(-7.7f, 7.0, -4.0f)); // translate it down so it's at the center of the scene
		modelProbadores = glm::rotate(modelProbadores, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelProbadores = glm::scale(modelProbadores, glm::vec3(0.008f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelProbadores);

		probadores->Draw(*staticShader);

		// Aplicamos transformaciones del modelo ----> Modelo Tenis
		glm::mat4 modelShoes = glm::mat4(1.0f);
		modelShoes = glm::translate(modelShoes, glm::vec3(-7.7f, 7.0, 0.0f)); // translate it down so it's at the center of the scene
		//modelShoes = glm::rotate(modelShoes, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShoes = glm::scale(modelShoes, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelShoes);

		ShoesFinal->Draw(*staticShader);


	}

	glUseProgram(0); */ 


	// Animacion Aguila 

	/*{
		// Activamos el shader de Phong
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);


		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -17.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 4.2f);
		proceduralShader->setFloat("height", 3.5f);

		AguilaBlanca->Draw(*proceduralShader);
		proceduralTime += 0.0010;

	}

	glUseProgram(0);*/ 

	// Actividad 3.1: Dibujar personaje
	// Objeto dinámico (Personaje animado)
	{
		// Actualización de la animación
		character->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, character->gBones);

		// Dibujamos el modelo
		//character->Draw(*dynamicShader);
	}

	// Desactivamos el shader actual
	glUseProgram(0);


	//Animacion Eclipse

	/*{

		// Activación del shader del personaje
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);
		

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipseChasis, glm::vec3(0.5, 0.5, 0.5));
		staticShader->setMat4("model", modelMatrixEclipseChasis);
		//Se dibuja
		modelEclipseChasis->Draw(*staticShader);


		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis); // (891)(892)(893)(894)

		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		staticShader->setMat4("model", modelMatrixFrontalWheels);
		//Se dibuja
		modelEclipseFrontalWheels->Draw(*staticShader);


		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		staticShader->setMat4("model", modelMatrixRearWheels);

		modelEclipseRearWheels->Draw(*staticShader);

	}

	glUseProgram(0);*/ 

	//Solar Panel
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		/**********************SOPORTE PANELES*************************/

		// Aplicamos transformaciones del modelo -- Soporte Panel Solar izq - inf
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(-25.0f, 0.0f, -8.0f)); // Traslación en -30 unidades en x y 10 unidades en z
		model1 = glm::translate(model1, glm::vec3(0.0, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model1);

		soportePS->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Soporte Panel Solar 2 izq - sup
		model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(-25.0f, 0.0f, -50.0f)); // Traslación en -30 unidades en x y 10 unidades en z
		model1 = glm::translate(model1, glm::vec3(0.0, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model1);

		soportePS->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Soporte Panel Solar 3 der - inf
		model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(20.0f, 0.0f, -8.0f)); // Traslación en -30 unidades en x y 10 unidades en z
		model1 = glm::translate(model1, glm::vec3(0.0, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model1);

		soportePS->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Soporte Panel Solar 4 der - sup
		model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(20.0f, 0.0f, -50.0f)); // Traslación en -30 unidades en x y 10 unidades en z
		model1 = glm::translate(model1, glm::vec3(0.0, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model1);

		soportePS->Draw(*staticShader);
		
		/**********************PANELES SOLARES*************************/

		// Aplicamos transformaciones del modelo -- Panel Solar izq - inf
		glm::mat4 modelSP = glm::mat4(1.0f);
		modelSP = glm::translate(modelSP, glm::vec3(-22.175f, 7.0f + panel_offset, -23.5f)); // Traslación en -30 unidades en x
		modelSP = glm::translate(modelSP, glm::vec3(0.0, 0.0f, 30.0f)); // translate it down so it's at the center of the scene
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation1), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSP = glm::scale(modelSP, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelSP);

		SolarPanel->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Panel Solar  2 izq - sup
		modelSP = glm::mat4(1.0f);
		modelSP = glm::translate(modelSP, glm::vec3(-22.175f, 7.0f + panel_offset, -65.4f)); // Traslación en -30 unidades en x
		modelSP = glm::translate(modelSP, glm::vec3(0.0, 0.0f, 30.0f)); // translate it down so it's at the center of the scene
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation1), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSP = glm::scale(modelSP, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelSP);

		SolarPanel->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Panel Solar 3 der - inf
		modelSP = glm::mat4(1.0f);
		modelSP = glm::translate(modelSP, glm::vec3(22.875f, 7.0f + panel_offset, -23.5f)); // Traslación en -30 unidades en x
		modelSP = glm::translate(modelSP, glm::vec3(0.0, 0.0f, 30.0f)); // translate it down so it's at the center of the scene
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation1), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSP = glm::scale(modelSP, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelSP);

		SolarPanel->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -- Panel Solar 4 der - sup
		modelSP = glm::mat4(1.0f);
		modelSP = glm::translate(modelSP, glm::vec3(22.875f, 7.0f + panel_offset, -65.4f)); // Traslación en -30 unidades en x
		modelSP = glm::translate(modelSP, glm::vec3(0.0, 0.0f, 30.0f)); // translate it down so it's at the center of the scene
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSP = glm::rotate(modelSP, glm::radians(panel_rotation1), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSP = glm::scale(modelSP, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelSP);

		SolarPanel->Draw(*staticShader);
	}

	glUseProgram(0);
	
	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//Animaciones Con implementacion de teclas panel Solar Animacion basica 01

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (panel_rotation1 < 35.0f)
			panel_rotation1 += 1.0f; // Incrementa la rotación solo si es menor que 35 grados
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (panel_rotation1 > -35.0f)
			panel_rotation1 -= 1.0f; // Decrementa la rotación solo si es mayor que -35 grados
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (panel_rotation < 35.0f)
			panel_rotation += 1.0f; // Incrementa la rotación solo si es menor que 35 grados
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (panel_rotation > -35.0f)
			panel_rotation -= 1.0f; // Decrementa la rotación solo si es mayor que -35 grados
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		if (panel_offset < 0.5f)
			panel_offset += 0.01f; // Incrementa el desplazamiento solo si es menor que 0.1 unidades
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		if (panel_offset > -0.5f)
			panel_offset -= 0.01f; // Decrementa el desplazamiento solo si es mayor que -0.1 unidades
	}



	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position = position + scaleV * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - scaleV * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		// rotateTable += 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		// rotateTable -= 0.05f;
	}
		
	
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
