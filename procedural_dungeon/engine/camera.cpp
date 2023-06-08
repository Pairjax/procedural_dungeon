#include "camera.h"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = YAW_DEFAULT;
    pitch = PITCH_DEFAULT;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    movement_speed = SPEED_DEFAULT;
    mouse_sensitivity = SENSITIVITY_DEFAULT;
    zoom = ZOOM_DEFAULT;
}

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
                float yaw = YAW_DEFAULT, float pitch = PITCH_DEFAULT):
                front(glm::vec3(0.0f, 0.0f, -1.0f)), 
                movement_speed(SPEED_DEFAULT), 
                mouse_sensitivity(SENSITIVITY_DEFAULT), 
                zoom(ZOOM_DEFAULT) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z,
                float up_x, float up_y, float up_z,
                float yaw, float pitch) :
                front(glm::vec3(0.0f, 0.0f, -1.0f)),
                movement_speed(SPEED_DEFAULT),
                mouse_sensitivity(SENSITIVITY_DEFAULT),
                zoom(ZOOM_DEFAULT) {
    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(up_x, up_y, up_z);
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard_input(Camera_Movement direction, float delta_time) {
    float velocity = movement_speed * delta_time;

    switch (direction) {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARD:
        position -= front * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    }
}

void Camera::process_mouse_movement_input(float x_offset, float y_offset,
    GLboolean constrain_pitch = true) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    // Restrain pitch bounds.
    if (constrain_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}

// Might want to change this to make it move between floors of the dungeon
void Camera::process_mouse_scroll(float y_offset) {
    zoom -= (float)y_offset;

    if (zoom < 1.0f) {
        zoom = 1.0f;
    }

    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::update_camera_vectors() {
    glm::vec3 front;

    // Get new front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Normalize to prevent slower movement
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}