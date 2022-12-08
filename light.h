#ifndef LIGHT_H_
#define LIGHT_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace game {

    class Light {

    public:
        Light(void);
        ~Light();

        glm::vec3 GetPosition(void) const;


        void SetPosition(glm::vec3 position);

        void Translate(glm::vec3 trans);

        void SetupShader(GLuint program);

        void Update(void);

    private:
        glm::vec3 position_;

        glm::mat4 view_matrix_;


    };

} // namespace game

#endif // Light_H_
