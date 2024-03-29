#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "asteroid.h"
#include "sky.h"
#include "box.h"
#include "tree.h"
#include "light.h"
namespace game {

    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            // Call Init() before calling any other method
            void Init(void); 
            // Set up resources for the game
            void SetupResources(void);
            // Set up initial scene
            void SetupScene(void);
            // Run the game: keep the application active
            void MainLoop(void); 

            void Branches_grow(Tree* main_tree, int num, int current_num);

        private:
            // GLFW window
            GLFWwindow* window_;


            // Scene graph containing all nodes to render
            SceneGraph scene_;

            // Resources available to the game
            ResourceManager resman_;

            // Camera abstraction
            Camera camera_;
            Light light_;

            // Flag to turn animation on/off
            bool animating_;
            bool effect;
            bool effect2;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
 
            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Asteroid field
            // Create instance of one asteroid
            Asteroid *CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name);
            // Create entire random asteroid field
            void CreateAsteroidField(int num_asteroids = 1500);

            //sky box
            Sky* CreateSkyBoxInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));
            void CreateSkyBox();
            void CreateBox(float x, float y, float z);
            void Createbonfire(std::string name, float x, float y, float z);
            void ChangetoCastle();
            void ChangetoVillage();

            // Create tree
            Tree* CreateTreeInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));
            void CreateTreeField(int num_branches);
            void CheckCode(Game* game, std::string name);
            void Open();
            void ChangeTreesTexture(Tree* br, Resource* texture1, Resource* texture2);

            // Create Map blocks
            void CreateBlockA();
            void CreateBlockB();

            // Create Instance
            template <class Instance> 
            Instance *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

    }; // class Game

} // namespace game

#endif // GAME_H_
