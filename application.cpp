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

    // 1. Setting up hardcode shader as a string (this would be loaded as a file)

    // std::array<shader_info, 2> sources = {
    //     shader_info{"shader_samples/shader1_triangle/test.vert", shader_stage::vertex},
    //     shader_info{"shader_samples/shader1_triangle/test.frag", shader_stage::fragment}
    // };
    // std::array<shader_info, 2> sources = {
    //     shader_info{"shader_samples/shader2_camera/test.vert", shader_stage::vertex},
    //     shader_info{"shader_samples/shader2_camera/test.frag", shader_stage::fragment}
    // };
    // shader triangle_shader(sources);

    // std::println("loaded = {}", triangle_shader.loaded());

    // if(!triangle_shader.loaded()) {
    //     std::println("Triangle shader could not load one or many of the shader sources provided!!!");
    // }

    // 5. load your vertices
    // opengl requires that you have your vertex buffer created with an ID
    // then a vertex array to handle vertices
    
//    std::vector<float> vertices = {
//         // positions          // colors           // texture coords
//          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
//     };

//     std::vector<uint32_t> indices = {
//         0, 1, 3, // first triangle
//         1, 2, 3  // second triangle
//     };

//     vertex_array vao(vertices, indices);
    
//     /*
//     Equivalent to: (in test.vert glsl shader)
//     layout(location = 0) in vec3 aPos;
//     layout(location = 1) in vec3 aColor;
//     layout(location = 2) in vec2 aTexCoords;
//     */
//     std::array<vertex_attribute_element, 3> elements = {
//         vertex_attribute_element{ .name = "aPos", .type = GL_FLOAT, .size = 3, },
//         vertex_attribute_element{ .name = "aColor", .type = GL_FLOAT, .size = 3, },
//         vertex_attribute_element{ .name = "aTexCoords", .type = GL_FLOAT, .size = 2, }
//     };
//     vao.vertex_attributes(elements);

    // texture wood_texture(std::filesystem::path("assets/wood.png"));
    // texture wall_texture(std::filesystem::path("assets/awesomeface.png"));

    // triangle_shader.bind();
    // triangle_shader.write("texture1", 0);
    // triangle_shader.write("texture2", 1);

    camera test_camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

    renderer geometry_renderer("Renderer");

    // query all camera objects
    auto query_camera_objects =
          scene_registry.query_builder<flecs::pair<tags::editor, projection_view>, perspective_camera>() .build();

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

        // Setting up view/proj matrices
        /*
        transform* camera_transform = camera_entity.get_mut<transform>();

        float movement_speed = 10.f;
        float rotation_speed = 1.f;
        float velocity = movement_speed * delta_time;
        float rotation_velocity = rotation_speed * delta_time;

        glm::quat to_quaternion = to_quathp(camera_transform->quaternion);

        glm::vec3 up = glm::rotate(to_quaternion, glm::vec3(0.f, 1.f, 0.f));
        glm::vec3 forward = glm::rotate(to_quaternion, glm::vec3(0.f, 0.f, -1.f));
        glm::vec3 right = glm::rotate(to_quaternion, glm::vec3(1.0f, 0.0f, 0.0f));

        if (is_key_pressed(window, key_left_shift)) {
            if (is_mouse_pressed(window, mouse_button_middle)) {
                camera_transform->position += up * velocity;
            }

            if (is_mouse_pressed(window, mouse_button_right)) {
                camera_transform->position -= up * velocity;
            }
        }

        if (is_key_pressed(window, key_w)) {
            // std::println("W Presed!");
            camera_transform->position += forward * velocity;
        }
        if (is_key_pressed(window, key_s)) {
            // std::println("S Presed!");
            camera_transform->position -= forward * velocity;
        }

        if (is_key_pressed(window, key_d)) {
            // std::println("D Presed!");
            camera_transform->position += right * velocity;
        }
        if (is_key_pressed(window, key_a)) {
            // std::println("A Presed!");
            camera_transform->position -= right * velocity;
        }

        if (is_key_pressed(window, key_q)) {
            // std::println("Q Presed!");
            camera_transform->rotation.y += rotation_velocity;
        }
        if (is_key_pressed(window, key_e)) {
            // std::println("E Presed!");
            camera_transform->rotation.y -= rotation_velocity;
        }

        camera_transform->set_rotation(camera_transform->rotation);

        query_camera_objects.each(
              [&](flecs::entity,
                  flecs::pair<tags::editor, projection_view> p_pair,
                  perspective_camera& p_camera) {
            if (!p_camera.is_active) {
                return;
            }
            // proj_view = glm::mat4(1.f);
            // proj_view = p_pair->projection * p_pair->view;
            triangle_shader.write("projection", p_pair->projection);
            triangle_shader.write("view", p_pair->view);
        });
        */
        // glm::mat4 projection = glm::perspective(glm::radians(test_camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
        // glm::mat4 view = test_camera.GetViewMatrix();

        // // // test_vec = model * test_vec;
        // triangle_shader.write("projection", projection);
        // triangle_shader.write("view", view);
        

        // glm::vec3 position(0.0f, 0.0f, -3.0f);
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, position);
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // triangle_shader.write("model", model);

        // // draw our first triangle
        // triangle_shader.bind();
        // wood_texture.bind();
        // wall_texture.bind(1);
        // glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr);

        geometry_renderer.begin(test_camera, aspect_ratio);
        geometry_renderer.end();
        

        glfwSwapBuffers(window);

        poll_events();
    }
    return 0;
}