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
int code = 0;
bool keys = false;
bool win = false;
bool open = false;
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
    resman_.LoadResource(Material, "Material", filename.c_str());
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
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/gwall1.png");
    resman_.LoadResource(Texture, "Gwall1", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/gwall2.png");
    resman_.LoadResource(Texture, "Gwall2", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/gwall3.png");
    resman_.LoadResource(Texture, "Gwall3", filename.c_str());
    filename = std::string(TEXTURE_DIRECTORY) + std::string("/wood/box.jpg");
    resman_.LoadResource(Texture, "Box", filename.c_str());

    filename = std::string(TEXTURE_DIRECTORY) + std::string("/Cover.png");
    resman_.LoadResource(Texture, "Cover", filename.c_str());

    filename = std::string(TEXTURE_DIRECTORY) + std::string("/Cover2.png");
    resman_.LoadResource(Texture, "Cover2", filename.c_str());
    // Create particles for explosion
    


    // Setup drawing to texture
    scene_.SetupDrawToTexture();

    // Create a torus
    resman_.CreateTorus("TorusMesh");
    resman_.CreateCylinder("SimpleCylinder", 4.0, 0.4, 10, 10);
    resman_.CreateCylinder("tree", 15.0, 1.0, 50, 50);
    resman_.CreateWall("wall");
    resman_.CreateSphereParticles("FireParticles");
    resman_.CreateMagicParticles("MagicParticles");
    resman_.CreateCylinder("self", 1, 1, 10, 45);
    resman_.CreateSphere("LightSource", 1);
    
}


void Game::SetupScene(void) {

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an instance of the torus mesh



    player = CreateInstance<SceneNode>("Player", "self", "Self");
    player->SetRadius(1.0);
    player->SetAngle(glm::pi<float>() / 2);
    player->SetPosition(glm::vec3(0, -10, 25));

    glm::quat rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0));
    game::SceneNode* floor = CreateInstance<SceneNode>("floor", "wall", "Material", "Land");
    floor->Rotate(rotation);
    floor->SetAngle(0.0);
    floor->SetPosition(glm::vec3(0, -2, 0));
    floor->Scale(glm::vec3(80, 80, 80));
    floor->SetPlayer(player);

    game::SceneNode* floor2 = CreateInstance<SceneNode>("floor2", "wall", "Material", "Rock");
    floor2->Rotate(rotation);
    floor2->SetAngle(0.0);
    floor2->SetPosition(glm::vec3(170, -2, 90));
    floor2->Scale(glm::vec3(80, 80, 80));
    floor2->SetPlayer(player);

    game::SceneNode* floor3 = CreateInstance<SceneNode>("floor3", "wall", "Material", "Rock");
    floor3->Rotate(glm::angleAxis(-glm::pi<float>() / 2 - glm::pi<float>() / 18, glm::vec3(1.0, 0.0, 0.0)));
    floor3->SetAngle(glm::pi<float>() / 18);
    floor3->SetPosition(glm::vec3(170, -18, -68));
    floor3->Scale(glm::vec3(80, 80, 80));
    floor3->SetPlayer(player);

    game::SceneNode* floor4 = CreateInstance<SceneNode>("step", "wall", "Material", "Rock");
    floor4->SetPosition(glm::vec3(130, -12, 10));
    floor4->Scale(glm::vec3(10, 10, 10));

    CreateSkyBox();

    CreateTreeField(3);
    CreateBlockA();
    CreateBlockB();
    Createbonfire("bonfire", 130, 0, 60);

    //cover
    game::SceneNode* cover = CreateInstance<SceneNode>("cover", "wall", "Normal", "Cover");
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    cover->Rotate(rotation);




    game::SceneNode* magicA = CreateInstance<SceneNode>("magicA", "MagicParticles", "ParticleMagic", "Magic");
    magicA->SetPosition(glm::vec3(22, -0.5, -22));
    magicA->SetPlayer(player);

    game::SceneNode* magicB = CreateInstance<SceneNode>("magicB", "MagicParticles", "ParticleMagic", "Magic");
    magicB->SetPosition(glm::vec3(130, -0.5, 100));
    magicB->SetPlayer(player);

    game::SceneNode* magicC = CreateInstance<SceneNode>("magicC", "MagicParticles", "ParticleMagic", "Magic");
    magicC->SetPosition(glm::vec3(130, -10.5, -35));
    magicC->SetPlayer(player);

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
                if (game_start && !win) {
                    scene_.GetNode("cover")->SetPosition(scene_.GetNode("cover")->GetPosition() + glm::vec3(0, -0.2, 0));
                }

                std::cout << "(" << camera_.GetPosition().x << ", " << camera_.GetPosition().z << ")" << "\n";

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


                last_time = current_time;
            }
        }
        //scene_.GetNode("ParticleInstance")->SetPosition(glm::vec3(0, 0, -0.5));
        SceneNode* sky_box = scene_.GetNode("front");
        sky_box->SetPosition(camera_.GetPosition()+glm::vec3(0,0,-1));
        sky_box = scene_.GetNode("back");
        sky_box->SetPosition(camera_.GetPosition() + glm::vec3(0, 0, 1));
        sky_box = scene_.GetNode("left");
        sky_box->SetPosition(camera_.GetPosition() + glm::vec3(1, 0, 0));
        sky_box = scene_.GetNode("right");
        sky_box->SetPosition(camera_.GetPosition() + glm::vec3(-1, 0, 0));
        sky_box = scene_.GetNode("top");
        sky_box->SetPosition(camera_.GetPosition() + glm::vec3(0, 1, 0));
        sky_box = scene_.GetNode("bottom");
        sky_box->SetPosition(camera_.GetPosition() + glm::vec3(0, -1, 0));
        // Draw the scene
        scene_.Update();
        // Draw the scene to a texture
        if (effect) {
            scene_.DrawToTexture(&camera_,&light_);
            scene_.DisplayTexture(resman_.GetResource("FlameEffect")->GetResource());
        }else if (effect2) {
            scene_.DrawToTexture(&camera_, &light_);
            scene_.DisplayTexture(resman_.GetResource("MagicEffect")->GetResource());
        }
        else {
            scene_.Draw(&camera_,&light_);
        }




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
    if (game_start && !win) {


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
            }else if (interaction == "magicC") {
                player->SetPosition(glm::vec3(115, -10, 80));
                game->camera_.SetPosition(glm::vec3(player->GetPosition().x, player->GetPosition().y + 11, player->GetPosition().z));
                game->camera_.SetView(glm::vec3(0.5, 1, 10.0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
                win = true;
            }
            else if (interaction == "Door" && keys) {
                door_open = true;
            }
            else if (interaction == "root1" || interaction == "root2" || interaction == "root3") {
                game->CheckCode(game, interaction);
              
            }
            else if (interaction == "boxtop") {
                keys = true;
                Box* box = (Box*)game->scene_.GetNode("boxtop");
                box->SetOpen(true);
                open = true;
            }
        }
    }
    else {
        if (win) {
            game->scene_.GetNode("cover")->SetTexture(game->resman_.GetResource("Cover2"));
            game->scene_.GetNode("cover")->SetPosition(glm::vec3(player->GetPosition().x, game->camera_.GetPosition().y, player->GetPosition().z) + glm::vec3(-0.2, 0, -4));

        }
        else {
            if (key == GLFW_KEY_K) {
                game_start = true;
            }
        }
        
    }
    
}
void Game::ChangeTreesTexture(Tree* br, Resource* texture1, Resource* texture2) {
    if (br->GetSon().size() == 0) {
        br->SetTexture(texture2);
    }
    else {
        for (Tree* br_son : br->GetSon()) {
            ChangeTreesTexture(br_son, texture1, texture2);
        }
        if (br->GetName() == "tree_trunk") {
            br->SetTexture(texture1);
        }else if (br->GetName() == "tree_branch") {
            br->SetTexture(texture2);
        }else {
            br->SetTexture(texture1);
        }
    }
}
void Game::CheckCode(Game* game, std::string name) {

    Tree* tree = (Tree*)game->scene_.GetNode(name);
    if (code != 3) {
        if (name == "root1" && code == 0) {
            code = 1;
            ChangeTreesTexture(tree, game->resman_.GetResource("Stone"), game->resman_.GetResource("Stone"));
            

        }else if (name == "root2" && code == 1) {
            code = 2;
            tree->SetTexture(game->resman_.GetResource("Stone"));
            for (Tree* br : tree->GetSon()) {
                for (Tree* br_br : br->GetSon()) {
                    for (Tree* br_br_br : br_br->GetSon()) {
                        br_br_br->SetTexture(game->resman_.GetResource("Stone"));
                    }
                    br_br->SetTexture(game->resman_.GetResource("Stone"));
                }
                br->SetTexture(game->resman_.GetResource("Stone"));
            }
            std::cout << "root2" << "\n";
        }else if (name == "root3" && code == 2) {
            code = 3;
            tree->SetTexture(game->resman_.GetResource("Stone"));
            for (Tree* br : tree->GetSon()) {
                for (Tree* br_br : br->GetSon()) {
                    for (Tree* br_br_br : br_br->GetSon()) {
                        br_br_br->SetTexture(game->resman_.GetResource("Stone"));
                    }
                    br_br->SetTexture(game->resman_.GetResource("Stone"));
                }
                br->SetTexture(game->resman_.GetResource("Stone"));
            }
            std::cout << "root3" << "\n";
            CreateBox(0, -1, 0);
            
        }
        else {
            code = 0;
            ChangeTreesTexture((Tree*)game->scene_.GetNode("root1"), game->resman_.GetResource("Wood"), game->resman_.GetResource("Land"));
            ChangeTreesTexture((Tree*)game->scene_.GetNode("root2"), game->resman_.GetResource("Wood"), game->resman_.GetResource("Land"));
            ChangeTreesTexture((Tree*)game->scene_.GetNode("root3"), game->resman_.GetResource("Wood"), game->resman_.GetResource("Land"));
        }
    }
    

}
void Game::ChangetoCastle() {
    block_locate = "BlockB";
    player->SetPosition(glm::vec3(115, 0, 80));
    light_.SetPosition(glm::vec3(scene_.GetNode("Fire")->GetPosition().x, scene_.GetNode("Fire")->GetPosition().y + 1, scene_.GetNode("Fire")->GetPosition().z));
    light_.SetColor(glm::vec3(1, 1, 0.8));
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
    light_.SetPosition(glm::vec3(0, 5, 0));
    light_.SetColor(glm::vec3(1, 1, 1));
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
void Game::CreateBox(float x, float y, float z) {
    game::Box* front = CreateInstance<Box>("box", "wall", "TextureMaterial", "Box");
    front->SetPosition(glm::vec3(x, y, z-1));
    front->SetScale(glm::vec3(1, 0.5, 1));

    game::Box* left = CreateInstance<Box>("box", "wall", "TextureMaterial", "Box");
    glm::quat rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0));
    left->Rotate(rotation);
    left->SetPosition(glm::vec3(x + 1, y, z));
    left->SetScale(glm::vec3(1, 0.5, 1));

    game::Box* right = CreateInstance<Box>("box", "wall", "TextureMaterial", "Box");
    rotation = glm::angleAxis(glm::pi<float>() / -2, glm::vec3(0.0, 1.0, 0.0));
    right->Rotate(rotation);
    right->SetPosition(glm::vec3(x - 1, y, z));
    right->SetScale(glm::vec3(1, 0.5, 1));

    game::Box* back = CreateInstance<Box>("box", "wall", "TextureMaterial", "Box");
    back->SetPosition(glm::vec3(x, y, z + 1));
    back->SetScale(glm::vec3(1, 0.5, 1));

    game::Box* top = CreateInstance<Box>("boxtop", "wall", "TextureMaterial", "Box");
    rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0));
    top->Rotate(rotation);
    top->SetPosition(glm::vec3(x, y + 0.5, z));
    top->SetScale(glm::vec3(1, 1, 0.5));
    top->SetPlayer(player);
    top->SetOpen(false);

    game::Box* bottom = CreateInstance<Box>("boxbottom", "wall", "TextureMaterial", "Box");
    rotation = glm::angleAxis(3 * glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0));
    bottom->Rotate(rotation);
    bottom->SetPosition(glm::vec3(x, y-0.5, z));
    bottom->SetScale(glm::vec3(1, 1, 0.5));
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
    Sky* front = CreateSkyBoxInstance("front", "wall", "Normal", "FrontTexture");
    glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    front->Rotate(rotation);

    Sky* left = CreateSkyBoxInstance("left", "wall", "Normal", "LeftTexture");
    rotation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(0.0, 1.0, 0.0));
    left->Rotate(rotation);

    Sky* right = CreateSkyBoxInstance("right", "wall", "Normal", "RightTexture");
    rotation = glm::angleAxis(glm::pi<float>() / -2, glm::vec3(0.0, 1.0, 0.0));
    right->Rotate(rotation);

    Sky* back = CreateSkyBoxInstance("back", "wall", "Normal", "BackTexture");

    Sky* top = CreateSkyBoxInstance("top", "wall", "Normal", "TopTexture");
    rotation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1.0, 0.0, 0.0));
    top->Rotate(rotation);
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 1.0, 0.0));
    top->Rotate(rotation);

    Sky* bottom = CreateSkyBoxInstance("bottom", "wall", "Normal", "BottomTexture");
    rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0));
    bottom->Rotate(rotation);
    rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0, 0.0, 1.0));
    bottom->Rotate(rotation);

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
    std::string tree_name = "tree_trunk";
    if (num == max_num - 1|| num == max_num - 2) {
        tex = "Land";
        tree_name = "tree_branch";
    }
    float scale = main_tree->GetScale().x / 2;
    //x side branches
    Tree* tree1 = CreateTreeInstance(tree_name, "tree", "TextureMaterial",tex);
    tree1->SetOrientation(main_tree->GetOrientation());
    tree1->SetScale(glm::vec3(scale, scale, scale));
    tree1->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, 4 * scale));
    tree1->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(1, 0, 0)));
    tree1->SetMove(glm::vec3(0, 4 * scale, 0));
    main_tree->SetSon(tree1);
    tree1->SetFather(main_tree);
    Branches_grow(tree1, num + 1, max_num);
    //-x side branches
    Tree* tree2 = CreateTreeInstance(tree_name, "tree", "TextureMaterial", tex);
    tree2->SetOrientation(main_tree->GetOrientation());
    tree2->SetScale(glm::vec3(scale, scale, scale));
    tree2->SetPosition(glm::vec3(0, 8 * scale + 4 * scale, -4 * scale));
    tree2->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * -90, glm::vec3(1, 0, 0)));
    tree2->SetMove(glm::vec3(0, 4 * scale, 0));
    tree2->SetFather(main_tree);
    main_tree->SetSon(tree2);
    Branches_grow(tree2, num + 1, max_num);
    //z side branches
    Tree* tree3 = CreateTreeInstance(tree_name, "tree", "TextureMaterial", tex);
    tree3->SetOrientation(main_tree->GetOrientation());
    tree3->SetScale(glm::vec3(scale, scale, scale));
    tree3->SetPosition(glm::vec3(-4 * scale, 8 * scale + 4 * scale, 0));
    tree3->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * 90, glm::vec3(0, 0, 1)));
    tree3->SetMove(glm::vec3(0, 4 * scale, 0));
    tree3->SetFather(main_tree);
    main_tree->SetSon(tree3);
    Branches_grow(tree3, num + 1, max_num);
    //-z side branches
    Tree* tree4 = CreateTreeInstance(tree_name, "tree", "TextureMaterial", tex);
    tree4->SetOrientation(main_tree->GetOrientation());
    tree4->SetScale(glm::vec3(scale, scale, scale));
    tree4->SetPosition(glm::vec3(4 * scale, 8 * scale + 4 * scale, 0));
    tree4->SetOrientation(glm::angleAxis((glm::pi<float>() / 360) * -90, glm::vec3(0, 0, 1)));
    tree4->SetMove(glm::vec3(0, 4 * scale, 0));
    tree4->SetFather(main_tree);
    main_tree->SetSon(tree4);
    Branches_grow(tree4, num + 1, max_num);



}

void Game::CreateTreeField(int num_branches) {


    // Create root og tree
    Tree* root1 = CreateTreeInstance("root1", "tree", "TextureMaterial", "Wood");
    Branches_grow(root1, 0, num_branches);
    root1->SetPosition(glm::vec3(-20, 0, -20));
    root1->SetWind(glm::vec3(1, 0, 1));
    root1->SetPlayer(player);

    Tree* root2 = CreateTreeInstance("root2", "tree", "TextureMaterial", "Wood");
    Branches_grow(root2, 0, num_branches);
    root2->SetPosition(glm::vec3(20, 0, 20));
    root2->SetWind(glm::vec3(1, 0, 1));
    root2->SetPlayer(player);

    Tree* root3 = CreateTreeInstance("root3", "tree", "TextureMaterial", "Wood");
    Branches_grow(root3, 0, num_branches);
    root3->SetPosition(glm::vec3(-20, 0, 20));
    root3->SetWind(glm::vec3(1, 0, 1));
    root3->SetPlayer(player);
    // create branches
    
    //set the vator of wind
    


}

void Game::CreateBlockA() {
    std::vector<SceneNode*> wall_arr;
    int wall_coordinate[12][2] = { {-20, -30}, {0, -30}, {20, -30}, {30, -20}, {30, 0}, {30, 20}, {20, 30}, {0, 30}, {-20, 30}, {-30, 20}, {-30, 0}, {-30, -20} };
    float wall_angle[12] = { glm::pi<float>(), glm::pi<float>(), glm::pi<float>(),
        glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>() / 2,
        0.0, 0.0, 0.0,
        glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2, glm::pi<float>() * 3 / 2 };
    SceneNode* wall;
    for (int i = 0; i < 12; i++) {
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "Wall" + index;
        switch (i) {
            case 0: 
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall1");
            case 5:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall2");
                break;
            case 6:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall2");
                break;
            case 8:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall3");
                break;
            case 9:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall3");
                break;
            case 11:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall1");
                break;
            default:
                wall = CreateInstance<SceneNode>(name, "wall", "Material", "Gwall");
        }
        wall->Rotate(glm::angleAxis((float)wall_angle[i], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate[i][0], 0, wall_coordinate[i][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        wall_arr.push_back(wall);
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
                        glm::pi<float>()/ 2, glm::pi<float>() / 2, glm::pi<float>()/ 2, glm::pi<float>() / 2, glm::pi<float>() / 2, glm::pi<float>()/ 2, glm::pi<float>() / 2, glm::pi<float>() / 2,
                        0.0, 0.0, 0.0,
                        glm::pi<float>() / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2, glm::pi<float>()  / 2,
                        0.0, 0.0, 0.0
    };
    int wall_coordinate_complement[5][2] = { {110, -50}, {130, -50}, {150, -50}, {160, -40}, {100, -40}};
    float wall_angle_complement[5] = { 0.0, 0.0, 0.0, glm::pi<float>() / 2, glm::pi<float>() / 2 };
    SceneNode* wall;
    for (int i = 0; i < 25; i++) {
        std::stringstream ss;
        ss << i + 12;
        std::string index = ss.str();
        std::string name = "Wall" + index;
        if (i == 12) {
            wall = CreateInstance<SceneNode>("Door", "wall", "Material", "Door");
        }
        else {
            wall = CreateInstance<SceneNode>(name, "wall", "Material", "Cwall");
        }
        wall->SetAngle(wall_angle[i]);
        wall->Rotate(glm::angleAxis((float)wall_angle[i], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate[i][0], 0, wall_coordinate[i][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        if (wall->GetName() == "Door") {
            wall->SetPosition(glm::vec3(wall_coordinate[i][0], 0, wall_coordinate[i][1]));
            wall->SetPlayer(player);
        }
        wall_arr.push_back(wall);
    }
    for (int i = 0; i < 5; i++) {
        std::stringstream ss;
        ss << i + 12 + 25;
        std::string index = ss.str();
        std::string name = "Wall" + index;

        wall = CreateInstance<SceneNode>(name, "wall", "Material", "Cwall");
        wall->SetAngle(wall_angle_complement[i]);
        wall->Rotate(glm::angleAxis((float)wall_angle_complement[i], glm::vec3(0.0, 1.0, 0.0)));
        wall->SetPosition(glm::vec3(wall_coordinate_complement[i][0], -20, wall_coordinate_complement[i][1]));
        wall->Scale(glm::vec3(10, 10, 10));
        wall_arr_complement.push_back(wall);
    }
}
} // namespace game
