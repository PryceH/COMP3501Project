#include "Wall.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
namespace game {

    Wall::Wall(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
    }


    Wall::~Wall() {
    }

    void Wall::CollideDetect(SceneNode player) {
        //Do nothing
    }

    void Wall::SetMove(glm::vec3 move) {
        tran_ += move;
    }
    glm::vec3 Wall::GetMove(void) const {

        return tran_;
    }



    void Wall::Update(void) {
        //let the tree swaying.


    }

} // namespace game
