#include <glad/glad.h>
#include <filesystem>
#include <core/shader.hpp>
#include <GLFW/glfw3.h>
#include <print>
#include <array>
#include <vector>
#include <span>
#include <print>
#include <core/event.hpp>
#include <core/vertex_array.hpp>
#include <core/texture.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <core/test_camera.hpp>
#include <flecs.h>
#include <renderer/components.hpp>
#include <renderer/renderer.hpp>
#include <core/test_camera.hpp>

int main(){
    if(!glfwInit()){
        std::print("glfwInit could not be initialized!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    int width = 800;
    int height = 600;
    std::string title = "Hello Window";
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* p_window, /*NOLINT*/int p_width, int p_height){
        glViewport(0, 0, p_width, p_height);
    });

    // Required to setup glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::println("Could not load glad!!!");
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    float delta_time = 0.f;

    float aspect_ratio = (float)width / (float)height;
    flecs::world scene_registry;

    scene_registry.system<flecs::pair<tags::editor, projection_view>, transform, perspective_camera>().each([aspect_ratio](flecs::pair<tags::editor, projection_view> p_pair, transform p_transform, const perspective_camera& p_camera){
        if(!p_camera.is_active) {
            return;
        }

        // p_pair->projection = glm::mat4(1.f);

        p_pair->projection =
        glm::perspective(glm::radians(p_camera.field_of_view),
                            aspect_ratio,
                            p_camera.plane.x,
                            p_camera.plane.y);
        // p_pair->projection[1][1] *= -1;
        // p_pair->view = glm::mat4(1.f);

        // This is converting a glm::highp_vec4 to a glm::quat
        // (quaternion)
        glm::quat quaternion = glm::quat({ p_transform.quaternion.w,
                                            p_transform.quaternion.x,
                                            p_transform.quaternion.y,
                                            p_transform.quaternion.z });
        p_pair->view =
        glm::translate(p_pair->view, p_transform.position) *
        glm::mat4_cast(quaternion);

        p_pair->view = glm::inverse(p_pair->view);
    });

    // creating camera entity

    // flecs::entity camera_entity = scene_registry.entity("Camera");
    // camera_entity.add<flecs::pair<tags::editor, projection_view>>();
    // camera_entity.set<transform>({
    //     .position = {0.0f, 0.0f, 3.0f},
    // });
    // camera_entity.set<perspective_camera>({
    //     .plane = { 0.1f, 1000.f },
    //     .is_active = true,
    //     .field_of_view = 45.f,
    // });

    flecs::entity mesh_entity = scene_registry.entity("Some Mesh");
    mesh_entity.set<transform>({
        .position = {0.0f, 0.0f, -3.0f},
        .rotation = {0.0f, 0.0f, 1.0f}

    });

    renderer geometry_renderer("Renderer");

    // query all camera objects
    auto query_camera_objects =
          scene_registry.query_builder<flecs::pair<tags::editor, projection_view>, perspective_camera>() .build();
    
    camera test_camera(glm::vec3(0.0f, 0.0f, 3.0f));

    while(!glfwWindowShouldClose(window)){
        auto current_time = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
        start_time = current_time;

        // glClearColor(0.5f, 1.0f, 1.0f, 1.f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // game logic input events

        if (is_key_pressed(window, key_w)) {
            // camera_transform->position += forward * velocity;
            test_camera.ProcessKeyboard(Camera_Movement::forward, delta_time);
        }
        if (is_key_pressed(window, key_s)) {
            // std::println("S Presed!");
            // camera_transform->position -= forward * velocity;
            test_camera.ProcessKeyboard(Camera_Movement::backward, delta_time);
        }

        if (is_key_pressed(window, key_d)) {
            // std::println("D Presed!");
            // camera_transform->position += right * velocity;
            test_camera.ProcessKeyboard(Camera_Movement::right, delta_time);
        }
        if (is_key_pressed(window, key_a)) {
            // std::println("A Presed!");
            // camera_transform->position -= right * velocity;
            test_camera.ProcessKeyboard(Camera_Movement::left, delta_time);
        }

        glm::mat4 projection = glm::perspective(glm::radians(test_camera.Zoom), aspect_ratio, 0.1f, 100.0f);
        glm::mat4 view = test_camera.GetViewMatrix();

        glm::mat4 proj_view = projection * view;

        geometry_renderer.begin(proj_view);
        auto query_transforms = scene_registry.query_builder<transform>().build();
        query_transforms.each([&](flecs::entity p_entity, transform&){
            const transform* t = p_entity.get<transform>();
            geometry_renderer.submit(p_entity.id(), t, {});
        });

        geometry_renderer.end();
        

        glfwSwapBuffers(window);

        poll_events();
    }
    return 0;
}