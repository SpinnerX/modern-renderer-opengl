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

        p_pair->projection = glm::mat4(1.f);

        p_pair->projection =
        glm::perspective(glm::radians(p_camera.field_of_view),
                            aspect_ratio,
                            p_camera.plane.x,
                            p_camera.plane.y);
        // p_pair->projection[1][1] *= -1;
        p_pair->view = glm::mat4(1.f);

        // This is converting a glm::highp_vec4 to a glm::quat
        // (quaternion)
        // glm::quat quaternion = glm::quat({ p_transform.quaternion.w,
        //                                     p_transform.quaternion.x,
        //                                     p_transform.quaternion.y,
        //                                     p_transform.quaternion.z });
        glm::quat quaternion = to_quat(p_transform.quaternion);
        p_pair->view =
        glm::translate(p_pair->view, p_transform.position) *
        glm::mat4_cast(quaternion);

        p_pair->view = glm::inverse(p_pair->view);
    });

    // query all camera objects
    auto query_camera_objects =
          scene_registry.query_builder<flecs::pair<tags::editor, projection_view>, perspective_camera>() .build();

    // creating camera entity
    flecs::entity camera_entity = scene_registry.entity("Camera");
    camera_entity.add<flecs::pair<tags::editor, projection_view>>();
    camera_entity.set<transform>({
        .position = {0.0f, 0.0f, 3.0f},
    });
    camera_entity.set<perspective_camera>({
        .plane = { 0.1f, 1000.f },
        .is_active = true,
        .field_of_view = 45.f,
    });

    // Creating a sphere
    flecs::entity mesh_entity = scene_registry.entity("Some Mesh");
    mesh_entity.set<transform>({
        .position = {0.0f, 0.0f, -3.0f},
        .rotation = {0.0f, 0.0f, 1.0f}
    });
    mesh_entity.set<mesh_renderer>({
        .model_path = "assets/sphere.obj",
        .textures_path = {"assets/wood.png"}
    });

    flecs::entity mesh_entity2 = scene_registry.entity("Some Mesh 2");
    mesh_entity2.set<transform>({
        .position = {0.0f, 0.5f, -6.0f},
        .rotation = {0.0f, 0.0f, 1.0f}
    });
    mesh_entity2.set<mesh_renderer>({
        .model_path = "assets/robot.obj",
        .textures_path = {"assets/container_diffuse.png", "assets/robo-pose/textures/Texture_1K.jpg", "assets/robo-pose/textures/LP_BodyNormalsMap_1K.jpg", "assets/robo-pose/textures/specular.jpeg", "assets/robo-pose/textures/diffuse.jpeg"}
    });

    renderer geometry_renderer("Renderer");

    glm::mat4 proj_view(1.f);
    
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)){
        auto current_time = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
        start_time = current_time;

        scene_registry.progress(delta_time);

        geometry_renderer.background_color({0.2f, 0.3f, 0.3f, 1.0f});

        // game logic input events
        transform* camera_transform = camera_entity.get_mut<transform>();
        float movement_speed = 10.f;
        float rotation_speed = 1.f;
        float velocity = movement_speed * delta_time;
        float rotation_velocity = rotation_speed * delta_time;
        glm::quat to_quaternion = to_quat(camera_transform->quaternion);
        glm::vec3 up = glm::rotate(to_quaternion, glm::vec3(0.f, 1.f, 0.f));
          glm::vec3 forward =
            glm::rotate(to_quaternion, glm::vec3(0.f, 0.f, -1.f));
          glm::vec3 right =
            glm::rotate(to_quaternion, glm::vec3(1.0f, 0.0f, 0.0f));

        if (is_key_pressed(window, key_w)) {
            camera_transform->position += forward * velocity;
            // test_camera.ProcessKeyboard(Camera_Movement::forward, delta_time);
        }
        if (is_key_pressed(window, key_s)) {
            // std::println("S Presed!");
            camera_transform->position -= forward * velocity;
            // test_camera.ProcessKeyboard(Camera_Movement::backward, delta_time);
        }

        if (is_key_pressed(window, key_d)) {
            // std::println("D Presed!");
            camera_transform->position += right * velocity;
            // test_camera.ProcessKeyboard(Camera_Movement::right, delta_time);
        }
        if (is_key_pressed(window, key_a)) {
            // std::println("A Presed!");
            camera_transform->position -= right * velocity;
            // test_camera.ProcessKeyboard(Camera_Movement::left, delta_time);
        }
        if (is_key_pressed(window, key_q)) {
            camera_transform->rotation.y += rotation_velocity;
        }
        if (is_key_pressed(window, key_e)) {
            camera_transform->rotation.y -= rotation_velocity;
        }

        if(is_key_pressed(window, key_left_shift)) {
            camera_transform->position += up * velocity;
        }

        if(is_key_pressed(window, key_space)) {
            camera_transform->position -= up * velocity;
        }

        camera_transform->set_rotation(camera_transform->rotation);

        query_camera_objects.each(
              [&](flecs::entity,
                flecs::pair<tags::editor, projection_view> p_pair,
                perspective_camera& p_camera) {
            if (!p_camera.is_active) {
                return;
            }

            proj_view = p_pair->projection * p_pair->view;
        });

        geometry_renderer.begin(proj_view);
        auto query_transforms = scene_registry.query_builder<transform, mesh_renderer>().build();
        query_transforms.each([&](flecs::entity p_entity, transform&, mesh_renderer& p_mesh){
            const transform* t = p_entity.get<transform>();
            geometry_renderer.submit(p_entity.id(), t, p_mesh);
        });

        geometry_renderer.end();
        

        glfwSwapBuffers(window);

        poll_events();
    }
    return 0;
}