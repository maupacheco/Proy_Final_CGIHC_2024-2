/*
Proyecto Final CGIHC 2024-2
Alumno: Pacheco Salgado Mauricio
Codigo implementado del profesor: Sergio Teodoro Vite
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
#include <material.h>
#include <light.h>
#include <cubemap.h>

#include <irrKlang.h>
using namespace irrklang;

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 10.0f, 35.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));



// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;


glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     trdpersonOffset = 1.0f;
float     scaleV = 0.025f;
float     rotateCharacter = 0.0f;
float	  door_offset = 0.0f;
float	  door_rotation = 0.0f;

/**************************************************/
/*Variables para animacionB1*/
//Animacion Basica 1 RappiDelivey
bool animacionActiva = false;
float	motoPosX = -1.0f;
float	motoVelocidad = 0.2f;
bool	movDerecha_moto = true;
float	giraMoto = 90.0f;
/**************************************************/


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
const float avance = 0.09f;
const float giroEclipse = 0.5f;
/**************************************************/


/**************************************************/
/*Variables para el Pac-Man*/
//Variables globales para pacman
int modelSelected = 0;
/**************************************************/


// Shaders
Shader* mLightsShader;
Shader* proceduralShader;
Shader* wavesShader;
Shader* cubemapShader;
Shader* dynamicShader;
Shader* staticShader;
Shader* basicShader;


// Carga la información del modelo
Model* centroComercial;
Model* piso;
Model* door;
Model* moon;
Model* gridMesh;
//Indice para las luces
Model* lightDummy;

/**************************************************/
//Carga de modelos

//Modelo AguilaBlanca - Animacion Procedural 01
Model* AguilaBlanca;
//Modelo MotoDelivery - Animacion Basica 02
Model* RappiDelivery;
//Modelo - Animacion Hombre Paseando Perro Basica 03
AnimatedModel* modelMan;
AnimatedModel* modelDog;
//Modelo Eclipse - Animacion KeyFrames 04
Model* modelEclipseChasis;
Model* modelEclipseRearWheels;
Model* modelEclipseFrontalWheels;
//Modelo Pacman - Animacion KeyFrames 05
AnimatedModel* modelPacManDescanso;
AnimatedModel* modelPacManCorriendo;

//Modelos ---> Sala Nike 01
Model			*mesaManiquieNike;
Model			*cajaCobro;
Model			*probadores;
Model			*ShoesFinal;


/*Modelos Extras*/
Model* picnic;
Model* pasto1;
Model* rock;

/**************************************************/
//Definiciones de los modelos

//Eclipse
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
//Modelo Pacman
glm::mat4 modelMatrixPacmanCorriendo = glm::mat4(1.0f);
glm::mat4 modelMatrixPacmanDescanso = glm::mat4(1.0f);
int animationPacmanIndex = 0;
//Modelo Man y Dog
glm::mat4 modelMatrixMan = glm::mat4(1.0f);
glm::mat4 modelMatrixDog = glm::mat4(1.0f);


float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap* mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

// Materiales
Material material01;
float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

//// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara


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

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final CGIHC 2024-2 - Pacheco Salgado Mauricio", NULL, NULL);
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
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	basicShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");


	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	/*********************Se cargan los modelos del centro comercial*************************/

	//Estructura Centro Comercial
	centroComercial = new Model("models/ModelsMall/CCBlackFinal.fbx");

	//Piso Centro Comercial
	piso = new Model("models/ModelsMall/pisoExterior/pisoExterior.obj");

	//Ave - Aguila Animado Para Centro Comercial --> Animacion 01 - Procedural
	AguilaBlanca = new Model("models/ModelsMall/AguilaBlancaFinal.obj");

	//MotoRappi Para Centro Comercial --> Animacion 02 Basica
	RappiDelivery = new Model("models/ModelsMall/RappiDelivery.fbx");

	// Carro Eclipse
	modelEclipseChasis = new Model("models/ModelsMall/Eclipse/2003eclipse_chasis.obj");
	modelEclipseFrontalWheels = new Model("models/ModelsMall/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseRearWheels = new Model("models/ModelsMall/Eclipse/2003eclipse_rear_wheels.obj");

	//Model Pacman
	modelPacManDescanso = new AnimatedModel("models/ModelsMall/Pacman/Pac-Man_Descanso.fbx");
	modelPacManCorriendo = new AnimatedModel("models/ModelsMall/Pacman/Pac-Man_Corriendo.fbx");

	//Model ManDog
	modelMan = new AnimatedModel("models/ModelsMall/manWalkingFinal.fbx");
	modelDog = new AnimatedModel("models/ModelsMall/DogGolden.fbx");

	/**********Modelos Extras Al exterior*********/
	picnic = new Model("models/ModelsMall/picnicFinal.fbx");
	rock = new Model("models/ModelsMall/rock.fbx");

	/********** Modelos SALA NIKE 01 *********/
	mesaManiquieNike = new Model("models/ModelsMall/SalaNike/mesaManiquie1.fbx");
	cajaCobro = new Model("models/ModelsMall/SalaNike/cajaCobro.fbx");
	probadores = new Model("models/ModelsMall/SalaNike/probadores.fbx");
	ShoesFinal = new Model("models/ModelsMall/SalaNike/ShoesStand/ShoesFinal.obj");
	

	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.png",
		"textures/cubemap/01/negx.png",
		"textures/cubemap/01/posy.png",
		"textures/cubemap/01/negy.png",
		"textures/cubemap/01/posz.png",
		"textures/cubemap/01/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration

	Light light01;
	light01.Position = glm::vec3(5.0f, 2.0f, 5.0f);
	light01.Color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light01.Power = glm::vec4(60.0f, 60.0f, 60.0f, 1.0f);
	light01.alphaIndex = 11;
	gLights.push_back(light01);


	/*Light light02;
	light02.Position = glm::vec3(-5.0f, 2.0f, 5.0f);
	light02.Color = glm::vec4(0.0f, 0.2f, 0.0f, 1.0f);
	light02.Power = glm::vec4(60.0f, 60.0f, 60.0f, 1.0f);
	gLights.push_back(light02);


	Light light03;
	light03.Position = glm::vec3(5.0f, 2.0f, -5.0f);
	light03.Color = glm::vec4(0.0f, 0.0f, 0.2f, 1.0f);
	light03.Power = glm::vec4(60.0f, 60.0f, 60.0f, 1.0f);
	gLights.push_back(light03);

	Light light04;
	light04.Position = glm::vec3(-5.0f, 2.0f, -5.0f);
	light04.Color = glm::vec4(0.2f, 0.2f, 0.0f, 1.0f);
	light04.Power = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
	gLights.push_back(light04);*/

	lightDummy = new Model("models/IllumModels/lightDummy.fbx");

	// Configuración de propiedades materiales
	// Tabla: http://devernay.free.fr/cours/opengl/materials.html
	material01.ambient = glm::vec4(0.25f, 0.20725f, 0.20725f, 1.0f);
	material01.diffuse = glm::vec4(1.0f, 0.829f, 0.829f, 1.0f);
	material01.specular = glm::vec4(0.296648f, 0.296648f, 0.296648f, 1.0f);
	material01.transparency = 1.0f;

	SoundEngine->play2D("sound/biodynamic.mp3", true);


	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection;
	glm::mat4 view;

	if (activeCamera) {
		// Cámara en primera persona
		
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
	}
	else {
		// cámara en tercera persona
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();
	}

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}

	//Centro Comercial
	{
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.025f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", material01.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material01.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material01.specular);
		mLightsShader->setFloat("transparency", material01.transparency);

		centroComercial->Draw(*mLightsShader);

		// model = glm::mat4(1.0f);

		// Actividad 5.1
		// Efecto de puerta corrediza
		// model = glm::translate(model, glm::vec3(0.418f + door_offset, 0.0f, 6.75f));

		// Efecto de puerta con bisagra
		// model = glm::rotate(model, glm::radians(door_rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// mLightsShader->setMat4("model", model);
		// door->Draw(*mLightsShader);
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

	/**********Modelos Extras Estaticos Dibujados*****************/
	
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo -----> Picnic
		glm::mat4 modelPicnic = glm::mat4(1.0f);
		modelPicnic = glm::translate(modelPicnic, glm::vec3(-15.0f, 0.5f, 25.0f)); // translate it down so it's at the center of the scene
		modelPicnic = glm::scale(modelPicnic, glm::vec3(1.75f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelPicnic);

		picnic->Draw(*staticShader);

		// Aplicamos transformaciones del modelo -----> Rock1
		glm::mat4 modelRock = glm::mat4(1.0f);
		modelRock = glm::translate(modelRock, glm::vec3(-40.0f, 0.0f, 25.0f)); // translate it down so it's at the center of the scene
		modelRock = glm::rotate(modelRock, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelRock = glm::scale(modelRock, glm::vec3(1.5f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", modelRock);

		rock->Draw(*staticShader);
	}

	glUseProgram(0);

	//mesaSalaNikeManique
	{
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

	glUseProgram(0); 

	// Animacion Aguila --> Procedural 01 

	{
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
		proceduralTime += 0.010;

	}

	glUseProgram(0);


	//Animacion DeliveryRappi
	
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo --> RappiDelivery
		glm::mat4 modelRappi = glm::mat4(1.0f);
		modelRappi = glm::translate(modelRappi, glm::vec3(motoPosX, 0.0f, 12.0f));
		modelRappi = glm::scale(modelRappi, glm::vec3(1.25));
		modelRappi = glm::rotate(modelRappi, glm::radians(giraMoto), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader->setMat4("model", modelRappi);

		RappiDelivery->Draw(*staticShader);
	}
	glUseProgram(0);
	

	//Animacion Eclipse
	
	{

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
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.4, 0.4, 0.4));
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

	glUseProgram(0);

	// Pacman animado

	{
		modelPacManDescanso->UpdateAnimation(deltaTime);
		modelPacManCorriendo->UpdateAnimation(deltaTime);


		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);


		if (animationPacmanIndex == 1) {
			glm::mat4 modelMatrixPacmanCorriendoBody = glm::mat4(modelMatrixPacmanCorriendo);
			modelMatrixPacmanCorriendoBody = glm::translate(modelMatrixPacmanCorriendoBody, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrixPacmanCorriendoBody = glm::rotate(modelMatrixPacmanCorriendoBody, glm::radians(180.0f), glm::vec3(0.0, 1.0f, 0.0f));
			modelMatrixPacmanCorriendoBody = glm::scale(modelMatrixPacmanCorriendoBody, glm::vec3(0.007f, 0.007f, 0.007f));
			dynamicShader->setMat4("model", modelMatrixPacmanCorriendoBody);
			dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, modelPacManCorriendo->gBones);
			// Dibujamos el modelo
			modelPacManCorriendo->Draw(*dynamicShader);
		}
		else {
			glm::mat4 modelMatrixPacmanDescansoBody = glm::mat4(modelMatrixPacmanDescanso);
			modelMatrixPacmanDescansoBody = glm::translate(modelMatrixPacmanDescansoBody, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrixPacmanDescansoBody = glm::rotate(modelMatrixPacmanDescansoBody, glm::radians(180.0f), glm::vec3(0.0, 1.0f, 0.0f));
			modelMatrixPacmanDescansoBody = glm::scale(modelMatrixPacmanDescansoBody, glm::vec3(0.007f, 0.007f, 0.007f));
			dynamicShader->setMat4("model", modelMatrixPacmanDescansoBody);
			dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, modelPacManDescanso->gBones);
			// Dibujamos el modelo
			modelPacManDescanso->Draw(*dynamicShader);
		}

		glUseProgram(0);



	}

	//Hombre Paseando
	
	{
		modelMan->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 modelManBody = glm::mat4(modelMatrixMan);
		modelManBody = glm::rotate(modelManBody, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		modelManBody = glm::scale(modelManBody, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		dynamicShader->setMat4("model", modelManBody);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, modelMan->gBones);

		// Dibujamos el modelo
		modelMan->Draw(*dynamicShader);
	}

	glUseProgram(0);


	//Model Dog Paseando

	{
		modelDog->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 modelDogBody = glm::mat4(modelMatrixDog);
		modelDogBody = glm::translate(modelDogBody, glm::vec3(0.0f, 0.0f, 3.0f));
		modelDogBody = glm::rotate(modelDogBody, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		modelDogBody = glm::scale(modelDogBody, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", modelDogBody);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, modelDog->gBones);

		// Dibujamos el modelo
		modelDog->Draw(*dynamicShader);
	}

	glUseProgram(0);

	// Deplegamos los indicadores auxiliares de cada fuente de iluminación
	{
		basicShader->use();

		basicShader->setMat4("projection", projection);
		basicShader->setMat4("view", view);

		glm::mat4 model;

		for (size_t i = 0; i < gLights.size(); ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, gLights[i].Position);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			basicShader->setMat4("model", model);

			lightDummy->Draw(*basicShader);
		}

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

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;


	//Animaciones Con implementacion de teclas

	//Animacion Basica 1 RappiDelivery 
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		// Si se presiona la tecla 'C', cambiar el estado de la animación
		animacionActiva = !animacionActiva;
	}
	// Si la animación está activa
	if (animacionActiva) {
		if (movDerecha_moto) {
			motoPosX += motoVelocidad * deltaTime * 50.05;
			if (motoPosX >= 43.80f) {
				motoPosX = 43.80f;
				giraMoto = -90.0f;
				movDerecha_moto = false;
			}
		}
		else {
			motoPosX -= motoVelocidad * deltaTime * 50.05;
			if (motoPosX <= -43.80f) {
				motoPosX = -43.80f;
				giraMoto = 90.0f;
				movDerecha_moto = true;
			}
		}
	}


	//Animacion Recorrido Coche Eclipse
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		// Cambio al nuevo estado para iniciar el recorrido automático
		if (state == 3) { // Si la animación está detenida, reanudarla
			state = 0;
		}
		else { // Si no, detener la animación
			state = 3;
		}
	}
	// Maquina de estados para el carro eclipse
	switch (state) {
	case 0:
		if (numberAdvance == 0)
			maxAdvance = 32.0;
		else if (numberAdvance == 1)
			maxAdvance = 60.0;
		else if (numberAdvance == 2)
			maxAdvance = 115.0;
		else if (numberAdvance == 3)
			maxAdvance = 115.0;
		else if (numberAdvance == 4)
			maxAdvance = 115.0;
		else if (numberAdvance == 5)
			maxAdvance = 115.0;
		else if (numberAdvance == 6)
			maxAdvance = 115.0;
		state = 1;
		break;
	case 1:
		modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance * velocidadAnimacion));
		advanceCount += avance;
		rotWheelsX += 0.05f;
		rotWheelsY -= 0.02f;
		if (rotWheelsY < 0)
			rotWheelsY = 0;
		if (advanceCount > maxAdvance) {
			advanceCount = 0;
			numberAdvance++;
			if (numberAdvance > 6)
				numberAdvance = 3; // Volver al inicio del recorrido
			state = 2;
		}
		break;
	case 2:
		modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.025f * velocidadAnimacion));
		modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse), glm::vec3(0, 1, 0));
		rotCount += giroEclipse;
		rotWheelsX += 0.05f;
		rotWheelsY += 0.02f;
		if (rotWheelsY > 0.25f)
			rotWheelsY = 0.25f;
		if (rotCount >= 90.0f) {
			rotCount = 0;
			state = 0;
			if (numberAdvance > 6)
				numberAdvance = 3;
		}
		break;
	default:
		break;
	}

	
	//ASIGNACION DE TECLAS PACMAN

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		animationPacmanIndex = 0;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

		modelMatrixPacmanCorriendo = glm::rotate(modelMatrixPacmanCorriendo, 0.02f, glm::vec3(0, 1, 0));
		modelMatrixPacmanDescanso = glm::rotate(modelMatrixPacmanDescanso, 0.02f, glm::vec3(0, 1, 0));
		animationPacmanIndex = 1;

	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {


		modelMatrixPacmanCorriendo = glm::rotate(modelMatrixPacmanCorriendo, -0.02f, glm::vec3(0, 1, 0));
		modelMatrixPacmanDescanso = glm::rotate(modelMatrixPacmanDescanso, -0.02f, glm::vec3(0, 1, 0));
		animationPacmanIndex = 1;

	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		modelMatrixPacmanCorriendo = glm::translate(modelMatrixPacmanCorriendo, glm::vec3(0.0, 0.0, 0.02));
		modelMatrixPacmanDescanso = glm::translate(modelMatrixPacmanDescanso, glm::vec3(0.0, 0.0, 0.02));
		animationPacmanIndex = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {


		modelMatrixPacmanCorriendo = glm::translate(modelMatrixPacmanCorriendo, glm::vec3(0.0, 0.0, -0.02));
		modelMatrixPacmanDescanso = glm::translate(modelMatrixPacmanDescanso, glm::vec3(0.0, 0.0, -0.02));
		animationPacmanIndex = 1;
	}
	

	//PACAMAN controls run
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		modelMatrixPacmanCorriendo = glm::rotate(modelMatrixPacmanCorriendo, 0.02f, glm::vec3(0, 1, 0));
		modelMatrixPacmanDescanso = glm::rotate(modelMatrixPacmanDescanso, 0.02f, glm::vec3(0, 1, 0));
		animationPacmanIndex = 1;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		modelMatrixPacmanCorriendo = glm::rotate(modelMatrixPacmanCorriendo, -0.02f, glm::vec3(0, 1, 0));
		modelMatrixPacmanDescanso = glm::rotate(modelMatrixPacmanDescanso, -0.02f, glm::vec3(0, 1, 0));
		animationPacmanIndex = 1;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		modelMatrixPacmanCorriendo = glm::translate(modelMatrixPacmanCorriendo, glm::vec3(0.0, 0.0, 0.02));
		modelMatrixPacmanDescanso = glm::translate(modelMatrixPacmanDescanso, glm::vec3(0.0, 0.0, 0.02));
		animationPacmanIndex = 1;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		modelMatrixPacmanCorriendo = glm::translate(modelMatrixPacmanCorriendo, glm::vec3(0.0, 0.0, -0.02));
		modelMatrixPacmanDescanso = glm::translate(modelMatrixPacmanDescanso, glm::vec3(0.0, 0.0, -0.02));
		animationPacmanIndex = 1;
	}


	//ASIGNACION DE TECLAS Dog y Man

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {

		modelMatrixMan = glm::rotate(modelMatrixMan, 0.02f, glm::vec3(0, 1, 0));
		modelMatrixDog = glm::rotate(modelMatrixDog, 0.02f, glm::vec3(0, 1, 0));

	}
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {


		modelMatrixMan = glm::rotate(modelMatrixMan, -0.02f, glm::vec3(0, 1, 0));
		modelMatrixDog = glm::rotate(modelMatrixDog, -0.02f, glm::vec3(0, 1, 0));


	}
	else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {

		modelMatrixMan = glm::translate(modelMatrixMan, glm::vec3(0.0, 0.0, 0.02));
		modelMatrixDog = glm::translate(modelMatrixDog, glm::vec3(0.0, 0.0, 0.02));

	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		
		modelMatrixMan = glm::translate(modelMatrixMan, glm::vec3(0.0, 0.0, -0.02));
		modelMatrixDog = glm::translate(modelMatrixDog, glm::vec3(0.0, 0.0, -0.02));
	}

	//Luz
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		gLights.at(0).Position.x += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		gLights.at(0).Position.x -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		gLights.at(0).Position.z += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		gLights.at(0).Position.z -= 0.1f;
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
