#include "box.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
namespace game {

    Box::Box(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
    }


    Box::~Box() {
    }



    void Box::SetMove(glm::vec3 move) {
        tran_ += move;
    }
    glm::vec3 Box::GetMove(void) const {

        return tran_;
    }

    void Box::SetOpen(bool open) {
        open_ = open;
    }
    bool Box::GetOpen(void) const {
        return open_;
    }

    void Box::Update(void) {
        //let the tree swaying.
        if (GetName().find("boxtop") == 0) {
            if (GetPlayer()->GetInteraction() == GetName()) {
                float distance = glm::distance(glm::vec2(GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().z), glm::vec2(GetPosition().x, GetPosition().z));
                if (distance < 10) {
                    GetPlayer()->SetInteraction(GetName());
                }
                else {
                    GetPlayer()->SetInteraction("Nothing");
                }
            }
            else if (GetPlayer()->GetInteraction() == "Nothing") {
                float distance = glm::distance(glm::vec2(GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().z), glm::vec2(GetPosition().x, GetPosition().z));
                if (distance < 10) {
                    GetPlayer()->SetInteraction(GetName());
                }
            }
        }

        // Open Animation
        if (GetOpen() && time < 50) {
            time++;
            Rotate(glm::angleAxis(glm::pi<float>() / 360, glm::vec3(0, 1, 0)));//rotate the tree by vator wind
            glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(1, 0, 0));
            glm::mat4 T2 = glm::translate(glm::mat4(1.0), GetPosition());
            glm::mat4 R = glm::mat4_cast(GetOrientation());
            glm::mat4 orbit = T2 * glm::inverse(T) * R * T;//orbit trans
            SetTrans(orbit);
        }
        else if (time >= 50) {
            SetTrans(GetTrans());
        }
        else {
            SetTrans(glm::translate(glm::mat4(1.0), GetPosition()) * glm::mat4_cast(GetOrientation()));
        }
    }

} // namespace game