#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

namespace game {

    // Abstraction of an asteroid
    class Tree : public SceneNode {

    public:
        // Create asteroid from given resources
        Tree(const std::string name, const Resource* geometry, const Resource* material);

        // Destructor
        ~Tree();


        glm::vec3 GetMove(void) const;
        void SetMove(glm::vec3 move);
        void SetSon(Tree* son);
        void SetWind(glm::vec3 wind);
        std::vector<Tree*> GetSon();
        Tree* GetFather();
        void SetFather(Tree* root);
        // Update geometry configuration
        void Update(void);

    private:

        glm::quat angm_;


        glm::vec3 tran_;
        glm::vec3 wind_;
        std::vector<Tree*> all_son;
        Tree* father_ = NULL;
        glm::mat4 root_;
        int time = 0;

    };

} // namespace game

#endif // TREE_H_
