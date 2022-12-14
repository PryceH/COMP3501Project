#include "Tree.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
namespace game {

    Tree::Tree(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
    }


    Tree::~Tree() {
    }



    void Tree::SetMove(glm::vec3 move) {
        tran_ = move;
    }
    glm::vec3 Tree::GetMove(void) const {

        return tran_;
    }


    std::vector<Tree*> Tree::GetSon() {
        return all_son;
    }

    void Tree::SetFather(Tree* root) {
        father_ = root;
    }

    void Tree::SetWind(glm::vec3 wind) {
        for (int i = 0; i < GetSon().size(); i++) {
            GetSon()[i]->SetWind(wind);

        }
        wind_ = wind;


    }


    void Tree::SetSon(Tree* node) {
        all_son.push_back(node);
    }
    Tree* Tree::GetFather() {
        if (father_ != NULL) {
            return father_;
        }
        return NULL;

    }
    void Tree::SetPlayer(SceneNode* player) {

        player_ = player;
    }
    SceneNode* Tree::GetPlayer(void) const {

        return player_;
    }
    void Tree::Update(void) {
        //let the tree swaying.
        int move;
        if (time == 50) {
            time = 0;
        }
        if (time < 25) {
            move = 1;
        }
        if (time >= 25) {
            move = -1;
        }
        time += 1;
        if (GetFather() == NULL) {//root of tree

            glm::mat4 scaling = glm::scale(glm::mat4(1.0), GetScale());
            glm::mat4 rotation = glm::mat4_cast(GetOrientation());
            glm::mat4 translation = glm::translate(glm::mat4(1.0), GetPosition());
            glm::mat4 final = translation * rotation * scaling;

            SetTrans(final);

        }
        else {//braches of tree

            Rotate(glm::angleAxis((glm::pi<float>() / 3600) * move, wind_));//rotate the tree by vator wind
            glm::mat4 T = glm::translate(glm::mat4(1.0), tran_);
            glm::mat4 T2 = glm::translate(glm::mat4(1.0), GetPosition());
            glm::mat4 R = glm::mat4_cast(GetOrientation());
            glm::mat4 orbit = T2 * glm::inverse(T) * R * T;//orbit trans
            SetTrans(GetFather()->GetTrans() * orbit);
        }


        if (GetName().find("root") == 0) {
            if (GetPlayer()->GetInteraction() == GetName()) {
                float distance = glm::distance(glm::vec2(GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().z), glm::vec2(GetPosition().x, GetPosition().z));
                if (distance < 30) {
                    GetPlayer()->SetInteraction(GetName());
                }
                else {
                    GetPlayer()->SetInteraction("Nothing");
                }
            }
            else if (GetPlayer()->GetInteraction() == "Nothing") {
                float distance = glm::distance(glm::vec2(GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().z), glm::vec2(GetPosition().x, GetPosition().z));
                if (distance < 30) {
                    GetPlayer()->SetInteraction(GetName());
                }
            }
        }

    }

} // namespace game
