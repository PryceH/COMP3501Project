#ifndef BOX_H_
#define BOX_H_

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
    class Box : public SceneNode {

    public:
        // Create asteroid from given resources
        Box(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture = NULL);

        // Destructor
        ~Box();

        bool GetOpen(void) const;
        glm::vec3 GetMove(void) const;
        void SetMove(glm::vec3 move);
        void SetOpen(bool open);
        // Update geometry configuration
        void Update(void);

    private:

        glm::quat angm_;


        glm::vec3 tran_;
        glm::mat4 root_;
        bool open_ = false;
        int time = 0;

    };

} // namespace game

#endif // TREE_H_