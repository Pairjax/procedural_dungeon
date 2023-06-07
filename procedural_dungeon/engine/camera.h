#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW_DEFAULT = -90.0f;
const float PITCH_DEFAULT = 0.0f;
const float SPEED_DEFAULT = 2.5f;
const float SENSITIVITY_DEFAULT = 0.1f;
const float ZOOM_DEFAULT = 45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    // Euler Angles
    float yaw;
    float pitch;

    // Options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // Vector constructor
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {}

    // Scalar constructor
    Camera(float pos_x, float pos_y, float pos_z, 
        float up_x, float up_y, float up_z, 
        float yaw, float pitch) {}

    // Returns view matrix calculated from Euler Angles and LookAt Matrix
    glm::mat4 GetViewMatrix() {}

    void process_keyboard_input(Camera_Movement direction, float delta_time) {}

    void process_mouse_movement_input(float x_offset, float y_offset, 
                                    GLboolean constrain_pitch) {}

    void process_mouse_scroll(float y_offset) {}

private:
    void update_camera_vectors() {}
};