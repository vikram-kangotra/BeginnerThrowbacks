#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
    public:
        Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f)) :
           m_pos(pos),
           m_rot(rot),
           m_scale(scale){}

        glm::mat4 GetModel() const;

        glm::vec3& GetPos() {return m_pos;};
        glm::vec3& GetRot() {return m_rot;};
        glm::vec3& GetScale() {return m_scale;};

        void SetPos(const glm::vec3& pos) { m_pos = pos;}
        void SetRot(const glm::vec3& rot) { m_rot = rot;}
        void SetScale(const glm::vec3& scale) { m_scale = scale;}

        virtual ~Transform(){}
    protected:

    private:
        glm::vec3 m_pos;
        glm::vec3 m_rot;
        glm::vec3 m_scale;
};

#endif // TRANSFORM_H
