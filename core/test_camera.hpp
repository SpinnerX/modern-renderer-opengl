#pragma once
#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    forward,
    backward,
    left,
    right,
    up,
    down
};

// Default camera values
const float yaw         = -90.0f;
const float pitch       =  0.0f;
// const float SPEED       =  2.5f;
// const float SENSITIVITY =  0.1f;
const float zoom        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class camera{
public:
    struct camera_configuration{
    };

    static constexpr float rotation_angle = 176.80f;

    // constructor with vectors
    camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float p_yaw = yaw, float p_pitch = pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.f), camera_mouse_sensitivity(0.1f), MouseSensitivity(0.1f), Zoom(zoom){
        Position = position;
        WorldUp = up;
        Yaw = p_yaw;
        Pitch = p_pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float p_yaw, float p_pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.f), camera_mouse_sensitivity(0.1f), MouseSensitivity(0.1f), Zoom(zoom){
        Position = glm::vec3(pos_x, pos_y, pos_z);
        WorldUp = glm::vec3(up_x, up_y, up_z);
        Yaw = p_yaw;
        Pitch = p_pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime){
        float velocity = MovementSpeed * deltaTime;

        if (direction == forward)
            Position += Front * velocity;
        if (direction == backward)
            Position -= Front * velocity;
        if (direction == left)
            Position -= Right * velocity;
        if (direction == right)
            Position += Right * velocity;
        
        if(direction == up){
            Position += Up * velocity;
        }

        if(direction == down){
            Position -= Up * velocity;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset){
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void SetCameraMovementSpeed(float Sensitivity){
        camera_movement_sensitivity = Sensitivity;
        MovementSpeed = camera_movement_sensitivity;
    }

    void SetCameraMouseSpeed(float Sensitivity){
        camera_mouse_sensitivity = Sensitivity;
        // MouseSensitivity = camera_mouse_sensitivity;
    }

    float GetCameraSensitivity() const { return camera_mouse_sensitivity; }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors(){
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Left = glm::normalize(glm::cross(-Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
        Down  = glm::normalize(glm::cross(-Right, WorldUp));
    }

public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Down;
    glm::vec3 Right;
    glm::vec3 Left;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    // float camera_mouse_sensitivity = 0.1f;
    float camera_mouse_sensitivity = 2.5f;
    float camera_movement_sensitivity = 2.5f;
};