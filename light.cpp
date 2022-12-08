#include <stdexcept>
#define GLM_FORCE_RADIANS

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "light.h"

namespace game {

    Light::Light(void) {
    }


    Light::~Light() {
    }


    glm::vec3 Light::GetPosition(void) const {

        return position_;
    }



    void Light::SetPosition(glm::vec3 position) {

        position_ = position;
    }



    void Light::Translate(glm::vec3 trans) {

        position_ += trans;
    }


    void Light::SetupShader(GLuint program) {


        // Set view matrix in shader
        GLint light = glGetUniformLocation(program, "light_position");
        glUniform3fv(light, 1, glm::value_ptr(position_));

    }

    void Light::Update(void) {

    }



} // namespace game
