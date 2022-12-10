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
glm::vec3 camera_position_g(0.5, 1, 10.0);
glm::vec3 camera_look_at_g(0.0, camera_position_g.y, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;

// behaviour
SceneNode* player;
bool player_jump = false;
bool game_start = false;
bool door_open = false;
float player_jump_accerlation = 5.0;
std::string block_locate = "BlockA";

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
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/lit");
    resman_.LoadResource(Material, "Light", filename.c_str());

    // Load material for screen-space effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_space_magic");
    resman_.LoadResource(Material, "MagicEffect", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/screen_effect");
    resman_.LoadResource(Material, "FlameEffect", filename.c_str());

    // Load material to be applied to particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/magic");
    resman_.LoadResource(Material, "ParticleMagic", filename.c_str());
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/fire");
    resman_.LoadResource(Material, "FireMaterial", filename.c_str());

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
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/flame/magic.png");
    resman_.LoadResource(Texture, "Magic", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/download.jpg");
    resman_.LoadResource(Texture, "Wood", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/stone.jpg");
    resman_.LoadResource(Texture, "Stone", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/rocky.png");
    resman_.LoadResource(Texture, "Rock", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/land.png");
    resman_.LoadResource(Texture, "Land", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/door.jpg");
    resman_.LoadResource(Texture, "Door", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/cwall.png");
    resman_.LoadResource(Texture, "Cwall", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/gwall.png");
    resman_.LoadResource(Texture, "Gwall", filename.c_str());

    filename = std::string(TEXTURE_DIRECTORY) + std::string("/Cover.png");
    resman_.LoadResource(Texture, "Cover", filename.c_str());

    // Create particles for explosion
    



    // Setup drawing to texture
    scene_.SetupDrawToTexture();

    // Create a torus
    resman_.CreateTorus("TorusMesh");
    resman_.CreateCylinder("SimpleCylinder", 4.0, 0.4, 10, 10);
    resman_.CreateCylinder("tree", 15.0, 1.0, 50, 50);
    //resman_.CreateFireParticles("FireParticles");
    resman_.CreateWall("wall");
    resman_.CreateSphereParticles("FireParticles");
    resman_.CreateMagicParticles("MagicParticles");
    resman_.CreateCylinder("self", 1, 1, 10, 45);
}


void Game::SetupScene(void) {
    CreateTreeField(5);

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an instance of the torus mesh
    //game::SceneNode* torus = CreateInstance<SceneNode>("TorusInstance1", "TorusMesh", "ShinyBlueMaterial");
    
    
    player = CreateInstance<SceneNode>("Player", "self", "Self");
    //player->SetBlending(true);
    player->SetRadius(1.0);
    player->SetAngle(glm::pi<float>() / 2);
    player->SetPosition(glm::vec3(0,-10,25));

    glm::quat rotation = glm::angleAxis(glm::pi<float>() /2, glm::vec3(1.0, 0.0, 0.0));
    game::SceneNode* floor = CreateInstance<SceneNode>("floor", "wall", "TextureMaterial", "Land");
    floor->Rotate(rotation);
    floor->SetAngle(0.0);
    floor->SetPosition(glm::vec3(0, -2, 0));
    floor->Scale(glm::vec3(80, 80, 80));
    floor->SetPlayer(player);

    game::SceneNode* floor2 = CreateInstance<SceneNode>("floor2", "wall", "TextureMaterial", "Rock");
    floor2->Rotate(rotation);
    floor2->SetAngle(0.0);
    floor2->SetPosition(glm::vec3(170, -2, 90));
    floor2->Scale(glm::vec3(80, 80, 80));
    floor2->SetPlayer(player);

    game::SceneNode* floor3 = CreateInstance<SceneNode>("floor3", "wall", "TextureMaterial", "Rock");
    floor3->Rotate(glm::angleAxis(-glm::pi<float>() / 2 - glm::pi<float>() / 18, glm::vec3(1.0, 0.0, 0.0)));
    floor3->SetAngle(glm::pi<float>() / 18);
    floor3->SetPosition(glm::vec3(170, -18, -68));
    floor3->Scale(glm::vec3(80, 80, 80));
    floor3->SetPlayer(player);

    game::SceneNode* floor4 = CreateInstance<SceneNode>("step", "wall", "TextureMaterial", "Rock");
    floor4->SetPosition(glm::vec3(130, -12, 10));
    floor4->Scale(glm::vec3(10, 10, 10));

    CreateSkyBox();
    
    CreateTreeField(5);
    CreateBlockA();
    CreateBlockB();
    Createbonfire("bonfire", 130, 0, 60);
    // Scale the instance
    //particles->SetPosition(glm::vec3(2, 0, 0));
    //torus->Scale(glm::vec3(1.5, 1.5, 1.5));
    //particles->SetBlending(true);
    //cover
    game::SceneNode* cover = CreateInstance<SceneNode>("cover", "wall", "TextureMaterial", "Cover");
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    cover->Rotate(rotation);
    cover->SetPosition(camera_position_g + glm::vec3(0, 0, -4));

    //game::SceneNode* flame = CreateInstance<SceneNode>("fire", "FireParticles", "FireMaterial", "Flame");
    //flame->SetPosition(glm::vec3(0,1,-1));
    game::SceneNode* magicA = CreateInstance<SceneNode>("magicA", "MagicParticles", "ParticleMagic", "Magic");
    magicA->SetPosition(glm::vec3(22, -0.5, -22));
    magicA->SetPlayer(player);

    game::SceneNode* magicB = CreateInstance<SceneNode>("magicB", "MagicParticles", "ParticleMagic", "Magic");
    magicB->SetPosition(glm::vec3(130, -0.5, 100));
    magicB->SetPlayer(player);
    light_.SetPosition(glm::vec3(0,10,0));
}


void Game::MainLoop(void){
    ChangetoCastle();
    scene_.GetNode("cover")->SetPosition(glm::vec3(player->GetPosition().x, camera_.GetPosition().y, player->GetPosition().z) + glm::vec3(-0.2, 0, -4));
    

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        camera_.SetPosition(glm::vec3(player->GetPosition().x, player->GetPosition().y + 11, player->GetPosition().z));
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.01){
                if (game_start) {
                    scene_.GetNode("cover")->SetPosition(scene_.GetNode("cover")->GetPosition() + glm::vec3(0, -0.2, 0));
                }

                // door animation
                if (door_open) {
                    SceneNode* door = scene_.GetNode("Door");
                    if (door->GetPosition().y > -20) {
                        door->Translate(glm::vec3(0, -2, 0));
                    }
                }

                // terrain hieght algorithm
                SceneNode* reference_floor;
                if (block_locate == "BlockA") {
                    reference_floor = scene_.GetNode("floor");

                }else if(block_locate == "BlockB") {
                    reference_floor = scene_.GetNode("floor2");

                }
                else if (block_locate == "BlockC") {
                    reference_floor = scene_.GetNode("floor3");

                }
                float y = reference_floor->GetHight() - 10;
                player->SetPosition(glm::vec3(player->GetPosition().x, y, player->GetPosition().z));
                
                // fire distance
                SceneNode* fire = scene_.GetNode("Fire");
                float distance = glm::distance(glm::vec2(fire->GetPosition().x, fire->GetPosition().z), glm::vec2(player->GetPosition().x, player->GetPosition().z));
                if (distance < 10) {
                    effect = true;
                }
                else {
                    effect = false;
                }

                // magic distance
                SceneNode* magic;
                if (block_locate == "BlockA") {

                    magic = scene_.GetNode("magicA");

                }
                else if (block_locate == "BlockB") {

                    magic = scene_.GetNode("magicB");

                }
                if (block_locate != "BlockC") {
                    distance = glm::distance(glm::vec2(magic->GetPosition().x, magic->GetPosition().z), glm::vec2(player->GetPosition().x, player->GetPosition().z));
                    if (distance < 10) {
                        effect2 = true;
                    }
                    else {
                        effect2 = false;
                    }
                }
                //scene_.Update();

                // Animate the torus
                //SceneNode *node = scene_.GetNode("TorusInstance1");
                //glm::quat rotation = glm::angleAxis(glm::pi<float>() / 180, glm::vec3(0.0, 1.0, 0.0));
                //node->Rotate(rotation);

                //node = scene_.GetNode("ParticleInstance1");
                //rotation = glm::angleAxis(glm::pi<float>() / (180*5), glm::vec3(0.0, 1.0, 0.0));

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
            scene_.DisplayTexture(resman_.GetResource("FlameEffect")->GetResource());
        }
        if (effect2) {
            scene_.DrawToTexture(&camera_, &light_);
            scene_.DisplayTexture(resman_.GetResource("MagicEffect")->GetResource());
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
        //if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        //    game->effect = (game->effect == true) ? false : true;
        //}
        //if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        //    game->effect2 = (game->effect2 == true) ? false : true;
        //}

        // View control
        float rot_factor = 2 * (glm::pi<float>() / 180);
        float trans_factor = 1.0;
        if (key == GLFW_KEY_UP) {
            game->camera_.Pitch(rot_factor);
        }
        if (key == GLFW_KEY_DOWN) {
            game->camera_.Pitch(-rot_factor);
        }
        if (key == GLFW_KEY_LEFT) {
            game->camera_.Yaw(rot_factor);
            player->SetAngle(player->GetAngle()+rot_factor);
            if (player->GetAngle() >= 2 * glm::pi<float>()) {
                player->SetAngle(player->GetAngle() - 2 * glm::pi<float>());
            }
        }
        if (key == GLFW_KEY_RIGHT) {
            game->camera_.Yaw(-rot_factor);
            player->SetAngle(player->GetAngle()-rot_factor);
            if (player->GetAngle() < 0) {
                player->SetAngle(player->GetAngle() + 2 * glm::pi<float>());
            }
        }

        if (key == GLFW_KEY_W) {
            game->camera_.Translate(glm::vec3(game->camera_.GetForward().x,0, game->camera_.GetForward().z) * trans_factor);
            glm::vec3 pos = player->GetPosition();
            glm::vec3 move = glm::vec3(game->camera_.GetForward().x, 0, game->camera_.GetForward().z) * trans_factor;
            if (block_locate == "BlockA") {
                if (pos.x > -30 && pos.x + move.x < 30 && pos.z + move.z > -30 && pos.z + move.z < 30) {
                    player->Translate(move);
                }
            }else if (block_locate == "BlockB") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > 10 && pos.z + move.z < 110) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z <= 10 && door_open) {
                    player->Translate(move);
                    block_locate = "BlockC";
                }
            }else if (block_locate == "BlockC") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > -50 && pos.z + move.z <= 10) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z > 10) {
                    player->Translate(move);
                    block_locate = "BlockB";
                }
            }
        }
        if (key == GLFW_KEY_S) {
            glm::vec3 pos = player->GetPosition();
            glm::vec3 move = glm::vec3(-game->camera_.GetForward().x, 0, -game->camera_.GetForward().z) * trans_factor;
            if (block_locate == "BlockA") {
                if (pos.x + move.x > -30 && pos.x + move.x < 30 && pos.z + move.z > -30 && pos.z + move.z < 30) {
                    player->Translate(move);
                }
            }
            else if (block_locate == "BlockB") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > 10 && pos.z + move.z < 110) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z <= 10 && door_open) {
                    player->Translate(move);
                    block_locate = "BlockC";
                }
            }
            else if (block_locate == "BlockC") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > -50 && pos.z + move.z <= 10) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z > 10) {
                    player->Translate(move);
                    block_locate = "BlockB";
                }
            }
            //game->camera_.Translate(glm::vec3(-game->camera_.GetForward().x, 0, -game->camera_.GetForward().z) * trans_factor);
        }
        if (key == GLFW_KEY_A) {
            glm::vec3 pos = player->GetPosition();
            glm::vec3 move = glm::vec3(-game->camera_.GetSide().x, 0, -game->camera_.GetSide().z) * trans_factor;
            if (block_locate == "BlockA") {
                if (pos.x + move.x > -30 && pos.x + move.x < 30 && pos.z + move.z > -30 && pos.z + move.z < 30) {
                    player->Translate(move);
                }
            }
            else if (block_locate == "BlockB") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > 10 && pos.z + move.z < 110) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z <= 10 && door_open) {
                    player->Translate(move);
                    block_locate = "BlockC";
                }
            }
            else if (block_locate == "BlockC") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > -50 && pos.z + move.z <= 10) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z > 10) {
                    player->Translate(move);
                    block_locate = "BlockB";
                }
            }
            //game->camera_.Translate(glm::vec3(-game->camera_.GetSide().x, 0, -game->camera_.GetSide().z) * trans_factor);
        }
        if (key == GLFW_KEY_D) {
            glm::vec3 pos = player->GetPosition();
            glm::vec3 move = glm::vec3(game->camera_.GetSide().x, 0, game->camera_.GetSide().z) * trans_factor;
            if (block_locate == "BlockA") {
                if (pos.x + move.x > -30 && pos.x + move.x < 30 && pos.z + move.z > -30 && pos.z + move.z < 30) {
                    player->Translate(move);
                }
            }
            else if (block_locate == "BlockB") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > 10 && pos.z + move.z < 110) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z <= 10 && door_open) {
                    player->Translate(move);
                    block_locate = "BlockC";
                }
            }
            else if (block_locate == "BlockC") {
                if (pos.x + move.x > 100 && pos.x + move.x < 160 && pos.z + move.z > -50 && pos.z + move.z <= 10) {
                    player->Translate(move);
                }
                else if (pos.x + move.x > 120 && pos.x + move.x < 140 && pos.z + move.z > 10) {
                    player->Translate(move);
                    block_locate = "BlockB";
                }
            }
            //game->camera_.Translate(glm::vec3(game->camera_.GetSide().x, 0, game->camera_.GetSide().z) * trans_factor);
        }
        if (key == GLFW_KEY_SPACE) {
            game->camera_.Translate(glm::vec3(0, 2.0, 0)* trans_factor);
        }
        
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            std::string interaction = player->GetInteraction();

            if (interaction == "magicA") {
                game->ChangetoCastle();
            }
            else if (interaction == "magicB") {
                game->ChangetoVillage();
            }
            else if (interaction == "Door") {
                door_open = true;
            }

        }
    }
    else {
        if (key == GLFW_KEY_K) {
            game_start = true;
        }
    }
    
    
}
void Game::ChangetoCastle() {
    block_locate = "BlockB";
    player->SetPosition(glm::vec3(115, 0, 80));
    light_.SetPosition(scene_.GetNode("Fire")->GetPosition());
    scene_.GetNode("front")->SetTexture(resman_.GetResource("BackTexture2"));
    scene_.GetNode("back")->SetTexture(resman_.GetResource("FrontTexture2"));
    scene_.GetNode("left")->SetTexture(resman_.GetResource("LeftTexture2"));
    scene_.GetNode("right")->SetTexture(resman_.GetResource("RightTexture2"));
    scene_.GetNode("top")->SetTexture(resman_.GetResource("TopTexture2"));
    scene_.GetNode("bottom")->SetTexture(resman_.GetResource("BottomTexture2"));
}
void Game::ChangetoVillage() {
    block_locate = "BlockA";
    player->SetPosition(glm::vec3(0, 0, 0));
    light_.SetPosition(glm::vec3(0, 10, 0));
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
void Game::Createbonfire(std::string name, float x, float y, float z) {
    std::stringstream ss;
    std::string index;
    std::string final_name;
    ss << 1;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c1 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c1->SetPosition(glm::vec3(x+0.4,y,z));
    glm::quat rotation = glm::angleAxis(glm::pi<float>()/4, glm::vec3(0.0, 0.0, 1.0));
    c1->Rotate(rotation);


    ss << 2;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c2 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c2->SetPosition(glm::vec3(x-0.4, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(0.0, 0.0, 1.0));
    c2->Rotate(rotation);


    ss << 3;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c3 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c3->SetPosition(glm::vec3(x - 0.4, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, 1.0));
    c3->Rotate(rotation);


    ss << 4;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c4 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c4->SetPosition(glm::vec3(x, y, z));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, -1.0));
    c4->Rotate(rotation);
    

    ss << 5;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c5 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c5->SetPosition(glm::vec3(x, y, z - 0.4));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(-1.0, 0.0, 0.0));
    c5->Rotate(rotation);


    ss << 6;
    index = ss.str();
    final_name = name + index;
    game::SceneNode* c6 = CreateInstance<SceneNode>(final_name, "SimpleCylinder", "TextureMaterial", "Wood");
    c6->SetPosition(glm::vec3(x, y, z+0.4));
    rotation = glm::angleAxis(glm::pi<float>() / -4, glm::vec3(1.0, 0.0, 0.0));
    c6->Rotate(rotation);
    game::SceneNode* particles = CreateInstance<SceneNode>("Fire", "FireParticles", "FireMaterial", "Flame");
    particles->SetPosition(glm::vec3(x, y, z));
    //particles->SetBlending(true);

    //game::SceneNode* particles = CreateInstance<SceneNode>("ParticleInstance1", "FireParticles", "FireMaterial", "Flame");
    //particles->SetPosition(glm::vec3(x, y+1, z));
    ////particles->SetBlending(true);
}
Tree* Game::CreateTreeInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {

    // Get resources
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
    Tree* tree = new Tree(entity_name, geom, mat,tex);
    scene_.AddNode(tree);
    return tree;
}
void Game::Branches_grow(Tree* main_tree, int num, int max_num) {
    if (num == max_num) {
        return;
    }
    std::string tex = "Wood";
    if (num == max_num - 1|| num == max_num - 2) {
        tex = "Land";
    }
    num += 1;
    float scale = main_tree->GetScale().x / 2;
    //x side branches
    Tree* tree1 = CreateTreeInstance("tree", "tree", "Light",tex);
    tree1->SetOrientation(main_tree->GetOrientation());
    tree1->SetScale(glm::vec3(scale, scale, scale));
    tree1->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, 4 * scale));
    tree1->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(1, 0, 0)));
    tree1->SetMove(glm::vec3(0, 4 * scale, 0));
    main_tree->SetSon(tree1);
    tree1->SetFather(main_tree);
    Branches_grow(tree1, num, max_num);
    //-x side branches
    Tree* tree2 = CreateTreeInstance("tree", "tree", "Light", tex);
    tree2->SetOrientation(main_tree->GetOrientation());
    tree2->SetScale(glm::vec3(scale, scale, scale));
    tree2->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, -4 * scale));
    tree2->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * -90, glm::vec3(1, 0, 0)));
    tree2->SetMove(glm::vec3(0, 4 * scale, 0));
    tree2->SetFather(main_tree);
    main_tree->SetSon(tree2);
    Branches_grow(tree2, num, max_num);
    //z side branches
    Tree* tree3 = CreateTreeInstance("tree", "tree", "Light", tex);
    tree3->SetOrientation(main_tree->GetOrientation());
    tree3->SetScale(glm::vec3(scale, scale, scale));
    tree3->SetPosition(glm::vec3(-4 * scale, 8 * scale + 4 * scale, 0));
    tree3->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(0, 0, 1)));
    tree3->SetMove(glm::vec3(0, 4 * scale, 0));
    tree3->SetFather(main_tree);
    main_tree->SetSon(tree3);
    Branches_grow(tree3, num, max_num);
    //-z side branches
    Tree* tree4 = CreateTreeInstance("tree", "tree", "Light", tex);
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
    Tree* root = CreateTreeInstance("root", "tree", "Light", "Wood");
    root->SetPosition(glm::vec3(-25, 0, -25));
    Branches_grow(root, 0, 3);
    root->SetWind(glm::vec3(1, 0, 1));
    root = CreateTreeInstance("root", "tree", "Light", "Wood");
    root->SetPosition(glm::vec3(25, 0, 25));
    Branches_grow(root, 0, 3);
    root->SetWind(glm::vec3(1, 0, 1));
    root = CreateTreeInstance("root", "tree", "Light", "Wood");
    root->SetPosition(glm::vec3(-25, 0, 25));
    Branches_grow(root, 0, 3);
    root->SetWind(glm::vec3(1, 0, 1));
    // create branches
    
    //set the vator of wind
    


}

void Game::CreateBlockA() {
    std::vector<SceneNode*> wall_arr;
    int wall_coordinate[12][2] = { {-20, -30}, {0, -30}, {20, -30}, {30, -20}, {30, 0}, {30, 20}, {20, 30}, {0, 30}, {-20, 30}, {-30, 20}, {-30, 0}, {-30, -20} };
    float wall_angle[12] = { 0.0, 0.0, 0.0, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2,
        glm::pi<float>(), glm::pi<float>(), glm::pi<float>(), glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2 };
    for (int i = 0; i < 12; i++) {
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Wall" + index;
        wall_arr.push_back(CreateInstance<SceneNode>(name, "wall", "TextureMaterial", "Gwall"));
    }
    int index = 0;
    for (SceneNode* wall : wall_arr) {
        wall->SetAngle(wall_angle[index]);
        wall->Rotate(glm::angleAxis((float)wall_angle[index], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate[index][0], 0, wall_coordinate[index][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        index++;
    }
}

void Game::CreateBlockB() {
    std::vector<SceneNode*> wall_arr;
    std::vector<SceneNode*> wall_arr_complement;
    int wall_coordinate[25][2] = { {110, 110}, {130, 110}, {150, 110}, 
                                    {160, 100}, {160, 80}, {160, 60}, {160, 40}, {160, 20}, {160, 0}, {160, -20}, {160, -40},
                                    {110, 10}, {130, 10}, {150, 10},
                                    {100, 100}, {100, 80}, {100, 60}, {100, 40}, {100, 20}, {100, 0}, {100, -20}, {100, -40},
                                    {110, -50}, {130, -50}, {150, -50},
    };
    float wall_angle[25] = { 0.0, 0.0, 0.0, 
                        glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2,
                        glm::pi<float>(), glm::pi<float>(), glm::pi<float>(),
                        glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2,
                        glm::pi<float>(), glm::pi<float>(), glm::pi<float>()
    };
    int wall_coordinate_complement[5][2] = { {110, -50}, {130, -50}, {150, -50}, {160, -40}, {100, -40}};
    float wall_angle_complement[5] = { 0.0, 0.0, 0.0, glm::pi<float>() / 2, glm::pi<float>() / 2 };
    for (int i = 0; i < 25; i++) {
        std::stringstream ss;
        ss << i + 12;
        std::string index = ss.str();
        std::string name = "Wall" + index;
        if (i == 12) {
            wall_arr.push_back(CreateInstance<SceneNode>("Door", "wall", "TextureMaterial", "Door"));
        }
        else {
            wall_arr.push_back(CreateInstance<SceneNode>(name, "wall", "TextureMaterial", "Cwall"));
        }
    }
    for (int i = 0; i < 5; i++) {
        std::stringstream ss;
        ss << i + 12 + 25;
        std::string index = ss.str();
        std::string name = "Wall" + index;
        wall_arr_complement.push_back(CreateInstance<SceneNode>(name, "wall", "TextureMaterial", "Cwall"));
    }

    int index = 0;
    for (SceneNode* wall : wall_arr) {
        wall->SetAngle(wall_angle[index]);
        wall->Rotate(glm::angleAxis((float)wall_angle[index], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate[index][0], 0, wall_coordinate[index][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        if (wall->GetName() == "Door") {
            wall->SetPosition(glm::vec3(wall_coordinate[index][0], 0, wall_coordinate[index][1]));
            wall->SetPlayer(player);
        }
        index++;
    }
    index = 0;
    for (SceneNode* wall : wall_arr_complement) {
        wall->SetAngle(wall_angle_complement[index]);
        wall->Rotate(glm::angleAxis((float)wall_angle_complement[index], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate_complement[index][0], -20, wall_coordinate_complement[index][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        index++;
    }
}
} // namespace game
