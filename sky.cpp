#include "Sky.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
namespace game {

    Sky::Sky(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material,texture) {
    }


    Sky::~Sky() {
    }



    void Sky::SetMove(glm::vec3 move) {
        tran_ += move;
    }
    glm::vec3 Sky::GetMove(void) const {

        return tran_;
    }



    void Sky::Update(void) {
        //let the tree swaying.
        

    }

} // namespace game
