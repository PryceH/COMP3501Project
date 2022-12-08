#ifndef WALL_H_
#define WALL_H_

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
    class Wall : public SceneNode {

    public:
        // Create asteroid from given resources
        Wall(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture = NULL);

        // Destructor
        ~Wall();

        void CollideDetect(SceneNode* player);
        glm::vec3 GetMove(void) const;
        void SetMove(glm::vec3 move);
        // Update geometry configuration
        void Update(void);

    private:

        glm::quat angm_;


        glm::vec3 tran_;
        glm::mat4 root_;
        int time = 0;

    };

} // namespace game

#endif // TREE_H_
