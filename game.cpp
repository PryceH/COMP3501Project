#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 1600;
const unsigned int window_height_g = 1200;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.5, 0.5, 10.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;

// behaviour
SceneNode* player;
bool player_jump = false;
bool game_start = false;
float player_jump_accerlation = 5.0;

Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
    effect = false;
    effect2 = false;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Load material to be applied to torus
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/three-term_shiny_blue");
    resman_.LoadResource(Material, "ShinyBlueMaterial", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
    resman_.LoadResource(Material, "TextureMaterial", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/normal_map");
    resman_.LoadResource(Material, "Normal", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/self");
    resman_.LoadResource(Material, "Self", filename.c_str());

    // Load material for screen-space effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_space");
    resman_.LoadResource(Material, "ScreenSpaceMaterial", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_effect");
    resman_.LoadResource(Material, "ScreenEffectMaterial", filename.c_str());
    //filename = std::string(MATERIAL_DIRECTORY) + std::string("/fire");
    //resman_.LoadResource(Material, "FireMaterial", filename.c_str());

    //Load texture
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/front.jpg");
    resman_.LoadResource(Texture, "FrontTexture", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/left.jpg");
    resman_.LoadResource(Texture, "LeftTexture", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/right.jpg");
    resman_.LoadResource(Texture, "RightTexture", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/back.jpg");
    resman_.LoadResource(Texture, "BackTexture", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/top.jpg");
    resman_.LoadResource(Texture, "TopTexture", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox/bottom.jpg");
    resman_.LoadResource(Texture, "BottomTexture", filename.c_str());



    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/front.tga");
    resman_.LoadResource(Texture, "FrontTexture2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/left.tga");
    resman_.LoadResource(Texture, "LeftTexture2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/right.tga");
    resman_.LoadResource(Texture, "RightTexture2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/back.tga");
    resman_.LoadResource(Texture, "BackTexture2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/up.tga");
    resman_.LoadResource(Texture, "TopTexture2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/skybox2/down.tga");
    resman_.LoadResource(Texture, "BottomTexture2", filename.c_str());


    filename = std::string(TEXTURE_DIRECTORY) + std::string("/flame/flame4x4orig.png");
    resman_.LoadResource(Texture, "Flame", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/download.jpg");
    resman_.LoadResource(Texture, "Wood", filename.c_str());

    filename = std::string(TEXTURE_DIRECTORY) + std::string("/Cover.png");
    resman_.LoadResource(Texture, "Cover", filename.c_str());

    // Create particles for explosion
    



    // Setup drawing to texture
    scene_.SetupDrawToTexture();

    // Create a torus
    resman_.CreateTorus("TorusMesh");
    resman_.CreateCylinder("SimpleCylinder", 1.0, 0.1, 10, 10);
    resman_.CreateCylinder("tree", 15.0, 1.0, 50, 50);
    //resman_.CreateFireParticles("FireParticles");
    resman_.CreateWall("wall");
    resman_.CreateSphereParticles("FireParticles");
    resman_.CreateCylinder("self", 3, 1, 10, 45);
}


void Game::SetupScene(void) {

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an instance of the torus mesh
    game::SceneNode* torus = CreateInstance<SceneNode>("TorusInstance1", "TorusMesh", "ShinyBlueMaterial");
    //game::SceneNode* particles = CreateInstance("ParticleInstance1", "FireParticles", "FireMaterial", "Flame");
    game::SceneNode* floor = CreateInstance<SceneNode>("floor", "wall", "TextureMaterial", "Wood");
    player = CreateInstance<SceneNode>("Player", "self", "Self");
    game::SceneNode* wall = CreateInstance<SceneNode>("Wall", "wall", "TextureMaterial", "Wood");

    player->SetRadius(1.0);
    glm::quat rotation = glm::angleAxis(glm::pi<float>() /2, glm::vec3(1.0, 0.0, 0.0));
    floor->Rotate(rotation);
    floor->SetPosition(glm::vec3(0, -2, 0));
    floor->Scale(glm::vec3(1000.5, 1000.5, 1000.5));
  

    rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0));
    wall->SetAngle(glm::pi<float>() / 2);
    wall->Rotate(rotation);
    wall->SetPosition(glm::vec3(100, 0, 0));
    wall->Scale(glm::vec3(10, 10, 10));
    wall->SetPlayer(player);

    CreateSkyBox();
    Createbonfire(0, -1, 4);
    CreateTreeField(5);
    // Scale the instance
    //particles->SetPosition(glm::vec3(2, 0, 0));
    torus->Scale(glm::vec3(1.5, 1.5, 1.5));

    //cover
    game::SceneNode* cover = CreateInstance("cover", "wall", "TextureMaterial", "Cover");
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    cover->Rotate(rotation);
    cover->SetPosition(camera_position_g + glm::vec3(0, 0, -4));
}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.01){
                if (game_start) {
                    scene_.GetNode("cover")->SetPosition(scene_.GetNode("cover")->GetPosition() + glm::vec3(0, -0.2, 0));
                }
                if (player->GetPosition().y > 0) {
                    if (player_jump) {
                        player->SetPosition(player->GetPosition() + glm::vec3(0, player_jump_accerlation, 0));
                        player_jump_accerlation -= 0.2;
                        if (player_jump_accerlation <= 0) {
                            player_jump = false;
                        }
                    }else {
                        player->SetPosition(player->GetPosition() + glm::vec3(0, -player_jump_accerlation, 0));
                        if (player_jump_accerlation < 5.0) {
                            player_jump_accerlation += 0.2;
                        }
                    }
                }
                else {
                    player->SetPosition(glm::vec3(player->GetPosition().x, 0, player->GetPosition().z));
                }
                light_.SetPosition(glm::vec3(cos(current_time) * 2, 0, sin(current_time) * 2));
                
                //scene_.Update();

                // Animate the torus
                SceneNode *node = scene_.GetNode("TorusInstance1");
                glm::quat rotation = glm::angleAxis(glm::pi<float>()/180.0f, glm::vec3(0.0, 1.0, 0.0));
                node->Rotate(rotation);

                node = scene_.GetNode("ParticleInstance1");
                rotation = glm::angleAxis(0.2f * glm::pi<float>() / 180.0f, glm::vec3(0.0, 1.0, 0.0));

                last_time = current_time;
            }
        }
        //scene_.GetNode("ParticleInstance")->SetPosition(glm::vec3(0, 0, -0.5));
        SceneNode* box = scene_.GetNode("front");
        box->SetPosition(camera_.GetPosition()+glm::vec3(0,0,-1));
        box = scene_.GetNode("back");
        box->SetPosition(camera_.GetPosition() + glm::vec3(0, 0, 1));
        box = scene_.GetNode("left");
        box->SetPosition(camera_.GetPosition() + glm::vec3(1, 0, 0));
        box = scene_.GetNode("right");
        box->SetPosition(camera_.GetPosition() + glm::vec3(-1, 0, 0));
        box = scene_.GetNode("top");
        box->SetPosition(camera_.GetPosition() + glm::vec3(0, 1, 0));
        box = scene_.GetNode("bottom");
        box->SetPosition(camera_.GetPosition() + glm::vec3(0, -1, 0));

        // Draw the scene
        scene_.Update();
        scene_.Draw(&camera_,&light_);

        // Draw the scene to a texture
        if (effect) {
            scene_.DrawToTexture(&camera_,&light_);
            scene_.DisplayTexture(resman_.GetResource("ScreenEffectMaterial")->GetResource());
        }
        if (effect2) {
            scene_.DrawToTexture(&camera_, &light_);
            scene_.DisplayTexture(resman_.GetResource("ScreenSpaceMaterial")->GetResource());
        }


        // Save the texture to a file for debug
        /*static int first = 1;
        if (first){
            scene_.SaveTexture("texture.ppm");
            first = 0;
        }*/

        // Process the texture with a screen-space effect and display
        // the texture
        //scene_.DisplayTexture(resman_.GetResource("ScreenSpaceMaterial")->GetResource());

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}



void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;

    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (game_start) {
        // Stop animation if space bar is pressed
    /*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }*/
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            game->effect = (game->effect == true) ? false : true;
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            game->effect2 = (game->effect2 == true) ? false : true;
        }

        // View control
        float rot_factor(glm::pi<float>() / 180);
        float trans_factor = 1.0;
        if (key == GLFW_KEY_UP) {
            game->camera_.Pitch(2 * rot_factor);
        }
        if (key == GLFW_KEY_DOWN) {
            game->camera_.Pitch(-2 * rot_factor);
        }
        if (key == GLFW_KEY_LEFT) {
            game->camera_.Yaw(2 * rot_factor);
        }
        if (key == GLFW_KEY_RIGHT) {
            game->camera_.Yaw(-2 * rot_factor);
        }

        if (key == GLFW_KEY_W) {
            player->Translate(glm::vec3(game->camera_.GetForward().x, 0, game->camera_.GetForward().z) * trans_factor);
            //game->camera_.Translate(glm::vec3(game->camera_.GetForward().x,0, game->camera_.GetForward().z) * trans_factor);
        }
        if (key == GLFW_KEY_S) {
            player->Translate(glm::vec3(-game->camera_.GetForward().x, 0, -game->camera_.GetForward().z) * trans_factor);
            //game->camera_.Translate(glm::vec3(-game->camera_.GetForward().x, 0, -game->camera_.GetForward().z) * trans_factor);
        }
        if (key == GLFW_KEY_A) {
            player->Translate(glm::vec3(-game->camera_.GetSide().x, 0, -game->camera_.GetSide().z) * trans_factor);
            //game->camera_.Translate(glm::vec3(-game->camera_.GetSide().x, 0, -game->camera_.GetSide().z) * trans_factor);
        }
        if (key == GLFW_KEY_D) {
            player->Translate(glm::vec3(game->camera_.GetSide().x, 0, game->camera_.GetSide().z) * trans_factor);
            //game->camera_.Translate(glm::vec3(game->camera_.GetSide().x, 0, game->camera_.GetSide().z) * trans_factor);
        }
        if (key == GLFW_KEY_SPACE) {
            if (!player_jump && player_jump_accerlation == 5.0) {
                player->Translate(glm::vec3(0, player_jump_accerlation, 0));
                //game->camera_.Translate(glm::vec3(0, player_jump_accerlation, 0));
                player_jump = true;
            }
        }
        
        if (key == GLFW_KEY_C) {
            game->ChangetoCastle();

        }
        if (key == GLFW_KEY_V) {
            game->ChangetoVillage();

        }
    }
    else {
        if (key == GLFW_KEY_K) {
            game_start = true;
        }
    }
    
    
}
void Game::ChangetoCastle() {
    scene_.GetNode("front")->SetTexture(resman_.GetResource("FrontTexture2"));
    scene_.GetNode("back")->SetTexture(resman_.GetResource("BackTexture2"));
    scene_.GetNode("left")->SetTexture(resman_.GetResource("LeftTexture2"));
    scene_.GetNode("right")->SetTexture(resman_.GetResource("RightTexture2"));
    scene_.GetNode("top")->SetTexture(resman_.GetResource("TopTexture2"));
    scene_.GetNode("bottom")->SetTexture(resman_.GetResource("BottomTexture2"));
}
void Game::ChangetoVillage() {
    scene_.GetNode("front")->SetTexture(resman_.GetResource("FrontTexture"));
    scene_.GetNode("back")->SetTexture(resman_.GetResource("BackTexture"));
    scene_.GetNode("left")->SetTexture(resman_.GetResource("LeftTexture"));
    scene_.GetNode("right")->SetTexture(resman_.GetResource("RightTexture"));
    scene_.GetNode("top")->SetTexture(resman_.GetResource("TopTexture"));
    scene_.GetNode("bottom")->SetTexture(resman_.GetResource("BottomTexture"));
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
    scene_.AddNode(ast);
    return ast;
}


void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        ast->SetPosition(glm::vec3(-300.0 + 600.0*((float) rand() / RAND_MAX), -300.0 + 600.0*((float) rand() / RAND_MAX), 600.0*((float) rand() / RAND_MAX)));
        ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
        ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
}

template <class Instance>
Instance *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!tex){
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }

    Instance *scn = new Instance(entity_name, geom, mat, tex);
    scene_.AddNode(scn);
    return scn;
}



// sky box
Sky* Game::CreateSkyBoxInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {
    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }
    Resource* tex = NULL;
    if (texture_name != "") {
        tex = resman_.GetResource(texture_name);
        if (!tex) {
            throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
        }
    }

    // Create asteroid instance
    Sky* sky = new Sky(entity_name, geom, mat,tex);
    scene_.AddNode(sky);
    return sky;
}
void Game::CreateSkyBox() {
    Sky* front = CreateSkyBoxInstance("front", "wall", "TextureMaterial", "FrontTexture");
    glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    front->Rotate(rotation);
    //front->SetPosition(camera_.GetPosition() + glm::vec3(0,0,-1));

    Sky* left = CreateSkyBoxInstance("left", "wall", "TextureMaterial", "LeftTexture");
    rotation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(0.0, 1.0, 0.0));
    left->Rotate(rotation);
    //left->SetPosition(camera_.GetPosition() + glm::vec3(1, 0, 0));

    Sky* right = CreateSkyBoxInstance("right", "wall", "TextureMaterial", "RightTexture");
    rotation = glm::angleAxis(glm::pi<float>() / -2, glm::vec3(0.0, 1.0, 0.0));
    right->Rotate(rotation);
    //right->SetPosition(camera_.GetPosition() + glm::vec3(-1, 0, 0));

    Sky* back = CreateSkyBoxInstance("back", "wall", "TextureMaterial", "BackTexture");
    //back->SetPosition(camera_.GetPosition() + glm::vec3(0, 0, 1));

    Sky* top = CreateSkyBoxInstance("top", "wall", "TextureMaterial", "TopTexture");
    rotation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1.0, 0.0, 0.0));
    top->Rotate(rotation);
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    top->Rotate(rotation);
    //top->SetPosition(camera_.GetPosition() + glm::vec3(0, 1, 0));

    Sky* bottom = CreateSkyBoxInstance("bottom", "wall", "TextureMaterial", "BottomTexture");
    rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0));
    bottom->Rotate(rotation);
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 0.0, 1.0));
    bottom->Rotate(rotation);
    //bottom->SetPosition(camera_.GetPosition() + glm::vec3(0, -1, 0));

}
void Game::Createbonfire(float x, float y, float z) {
    game::SceneNode* c1 = CreateInstance<SceneNode>("c1", "SimpleCylinder", "Normal", "Wood");
    c1->SetPosition(glm::vec3(x+0.1,y,z));
    glm::quat rotation = glm::angleAxis(glm::pi<float>()/4, glm::vec3(0.0, 0.0, 1.0));
    c1->Rotate(rotation);

    game::SceneNode* c2 = CreateInstance<SceneNode>("c2", "SimpleCylinder", "Normal", "Wood");
    c2->SetPosition(glm::vec3(x-0.1, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(0.0, 0.0, 1.0));
    c2->Rotate(rotation);

    game::SceneNode* c3 = CreateInstance<SceneNode>("c3", "SimpleCylinder", "Normal", "Wood");
    c3->SetPosition(glm::vec3(x - 0.1, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, 1.0));
    c3->Rotate(rotation);

    game::SceneNode* c4 = CreateInstance<SceneNode>("c4", "SimpleCylinder", "Normal", "Wood");
    c4->SetPosition(glm::vec3(x, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, -1.0));
    c4->Rotate(rotation);

    game::SceneNode* c5 = CreateInstance<SceneNode>("c5", "SimpleCylinder", "Normal", "Wood");
    c5->SetPosition(glm::vec3(x, y, z - 0.1));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(-1.0, 0.0, 0.0));
    c5->Rotate(rotation);

    game::SceneNode* c6 = CreateInstance<SceneNode>("c6", "SimpleCylinder", "Normal", "Wood");
    c6->SetPosition(glm::vec3(x, y, z+0.1));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, 0.0));
    c6->Rotate(rotation);
}
Tree* Game::CreateTreeInstance(std::string entity_name, std::string object_name, std::string material_name) {

    // Get resources
    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }

    // Create asteroid instance
    Tree* tree = new Tree(entity_name, geom, mat);
    scene_.AddNode(tree);
    return tree;
}
void Game::Branches_grow(Tree* main_tree, int num, int max_num) {
    if (num == max_num) {
        return;
    }
    num += 1;
    float scale = main_tree->GetScale().x / 2;
    //x side branches
    Tree* tree1 = CreateTreeInstance("tree", "tree", "Normal");
    tree1->SetOrientation(main_tree->GetOrientation());
    tree1->SetScale(glm::vec3(scale, scale, scale));
    tree1->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, 4 * scale));
    tree1->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(1, 0, 0)));
    tree1->SetMove(glm::vec3(0, 4 * scale, 0));
    main_tree->SetSon(tree1);
    tree1->SetFather(main_tree);
    Branches_grow(tree1, num, max_num);
    //-x side branches
    Tree* tree2 = CreateTreeInstance("tree", "tree", "Normal");
    tree2->SetOrientation(main_tree->GetOrientation());
    tree2->SetScale(glm::vec3(scale, scale, scale));
    tree2->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, -4 * scale));
    tree2->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * -90, glm::vec3(1, 0, 0)));
    tree2->SetMove(glm::vec3(0, 4 * scale, 0));
    tree2->SetFather(main_tree);
    main_tree->SetSon(tree2);
    Branches_grow(tree2, num, max_num);
    //z side branches
    Tree* tree3 = CreateTreeInstance("tree", "tree", "Normal");
    tree3->SetOrientation(main_tree->GetOrientation());
    tree3->SetScale(glm::vec3(scale, scale, scale));
    tree3->SetPosition(glm::vec3(-4 * scale, 8 * scale + 4 * scale, 0));
    tree3->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(0, 0, 1)));
    tree3->SetMove(glm::vec3(0, 4 * scale, 0));
    tree3->SetFather(main_tree);
    main_tree->SetSon(tree3);
    Branches_grow(tree3, num, max_num);
    //-z side branches
    Tree* tree4 = CreateTreeInstance("tree", "tree", "Normal");
    tree4->SetOrientation(main_tree->GetOrientation());
    tree4->SetScale(glm::vec3(scale, scale, scale));
    tree4->SetPosition(glm::vec3(4 * scale, 8 * scale + 4 * scale, 0));
    tree4->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * -90, glm::vec3(0, 0, 1)));
    tree4->SetMove(glm::vec3(0, 4 * scale, 0));
    tree4->SetFather(main_tree);
    main_tree->SetSon(tree4);
    Branches_grow(tree4, num, max_num);



}

void Game::CreateTreeField(int num_branches) {

    // Create root og tree
    Tree* root = CreateTreeInstance("root", "tree", "Normal");
    root->SetPosition(glm::vec3(0, 0, -100));
    // create branches
    Branches_grow(root, 0, 4);
    //set the vator of wind
    root->SetWind(glm::vec3(1, 0, 1));

}
} // namespace game
