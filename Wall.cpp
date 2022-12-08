#include "Wall.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
namespace game {

    Wall::Wall(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
    }


    Wall::~Wall() {
    }

    void Wall::CollideDetect(SceneNode* player) {
        float a = player->GetPosition().z - GetPosition().z;
        float b = player->GetPosition().x - GetPosition().x;
        float dis = glm::distance(player->GetPosition(), GetPosition());
        float dis_collision = dis * sin(atan(b / a) - GetAngle());

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
