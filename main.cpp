#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <array>

#include "cube.hpp"

// ===== シェーダー =====
const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 uColor;
    void main() {
        FragColor = vec4(uColor, 1.0f);
    }
)glsl";

// ===== グローバル変数 =====
unsigned int VAO = 0;
unsigned int shaderProgram = 0;
const glm::vec3 COLOR_R(1.0f, 0.0f, 0.0f), COLOR_O(1.0f, 0.5f, 0.0f), COLOR_B(0.0f, 0.0f, 1.0f);
const glm::vec3 COLOR_G(0.0f, 1.0f, 0.0f), COLOR_W(1.0f, 1.0f, 1.0f), COLOR_Y(1.0f, 1.0f, 0.0f);
const glm::vec3 COLOR_BLACK(0.1f, 0.1f, 0.1f);

// 状態管理
Cube g_logical_cube;
std::array<Color, 54> g_visual_colors;

// アニメーション管理
std::queue<std::string> g_animation_queue;
bool g_is_animating = false;
float g_animation_progress = 0.0f, g_animation_speed = 3.0f;
char g_rotating_face = ' ';
int g_rotation_direction = 0;
float g_delta_time = 0.0f, g_last_frame = 0.0f;


// ===== ヘルパー関数 =====
glm::vec3 colorEnumToVec3(Color c) {
    switch (c) {
        case Color::WHITE:  return COLOR_W;
        case Color::YELLOW: return COLOR_Y;
        case Color::GREEN:  return COLOR_G;
        case Color::BLUE:   return COLOR_B;
        case Color::RED:    return COLOR_R;
        case Color::ORANGE: return COLOR_O;
        case Color::BLACK:
        default:            return COLOR_BLACK;
    }
}

Color getCurrentColor(int index) {
    if (g_is_animating) {
        return g_visual_colors[index];
    } else {
        return g_logical_cube.getColor(index);
    }
}

void drawFace(glm::mat4 model, glm::vec3 color) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uColor"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void schedule_moves(const std::string& sequence) {
    std::stringstream ss(sequence);
    std::string move;
    while (ss >> move) {
        g_animation_queue.push(move);
    }
}

// ===== main関数 =====
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " \"<move sequence>\"" << std::endl;
        return 1;
    }
    std::string scramble = argv[1];
    std::string solution = g_logical_cube.solve_dummy(scramble);
    schedule_moves(scramble);
    schedule_moves(solution);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Rubik's Cube Solver", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vShader);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f,  0.5f, 0.0f, -0.5f,  0.5f, 0.0f };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        float current_frame = (float)glfwGetTime();
        g_delta_time = current_frame - g_last_frame;
        g_last_frame = current_frame;

        if (!g_is_animating && !g_animation_queue.empty()) {
            g_is_animating = true;
            g_animation_progress = 0.0f;
            std::string move = g_animation_queue.front();
            g_animation_queue.pop();
            g_rotating_face = move[0];
            g_rotation_direction = 1;
            if (move.length() > 1) {
                if (move[1] == '\'') g_rotation_direction = -1;
                else if (move[1] == '2') g_rotation_direction = 2;
            }
            for (int i = 0; i < 54; ++i) {
                g_visual_colors[i] = g_logical_cube.getColor(i);
            }
        }

        if (g_is_animating) {
            g_animation_progress += g_animation_speed * g_delta_time;
            if (g_animation_progress >= 1.0f) {
                g_is_animating = false;
                std::string move_str(1, g_rotating_face);
                if(g_rotation_direction == -1) move_str += "'";
                if(g_rotation_direction == 2) move_str += "2";
                g_logical_cube.applyMove(move_str);
            }
        }
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f));
        view = glm::rotate(view, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // main.cppの描画ループを以下に置き換え

        float spacing = 1.0f, cubie_size = 0.5f, sticker_scale = 0.9f;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for (int z = -1; z <= 1; ++z) {
                    if (x == 0 && y == 0 && z == 0) continue;
                    
                    glm::vec3 position(x * spacing, y * spacing, z * spacing);
                    glm::mat4 model_transform = glm::translate(glm::mat4(1.0f), position);

                    if (g_is_animating) {
                        float angle_deg = 90.0f * g_animation_progress;
                        if (g_rotation_direction == 2) angle_deg *= 2.0f;
                        float angle_rad = glm::radians(angle_deg * g_rotation_direction);

                        glm::vec3 rotation_axis(0.0f);
                        bool is_rotating = false;

                        if (g_rotating_face == 'F' && z == 1) { is_rotating = true; rotation_axis = glm::vec3(0,0,1); }
                        if (g_rotating_face == 'B' && z == -1){ is_rotating = true; rotation_axis = glm::vec3(0,0,-1); }
                        if (g_rotating_face == 'R' && x == 1) { is_rotating = true; rotation_axis = glm::vec3(1,0,0); }
                        if (g_rotating_face == 'L' && x == -1){ is_rotating = true; rotation_axis = glm::vec3(-1,0,0); }
                        if (g_rotating_face == 'U' && y == 1) { is_rotating = true; rotation_axis = glm::vec3(0,1,0); }
                        if (g_rotating_face == 'D' && y == -1){ is_rotating = true; rotation_axis = glm::vec3(0,-1,0); }

                        if (is_rotating) {
                            model_transform = glm::rotate(glm::mat4(1.0f), angle_rad, rotation_axis) * model_transform;
                        }
                    }
                    
                    // ★★★ ここに不足していたインデックス宣言を追加 ★★★
                    int u_idx = (-z + 1) * 3 + (x + 1);
                    int d_idx = 9 + (z + 1) * 3 + (x + 1);
                    int f_idx = 18 + (-y + 1) * 3 + (x + 1);
                    int b_idx = 27 + (-y + 1) * 3 + (-x + 1);
                    int r_idx = 36 + (-y + 1) * 3 + (-z + 1);
                    int l_idx = 45 + (-y + 1) * 3 + (z + 1);
                    
                    glm::mat4 face_model;
                    face_model = glm::translate(model_transform, glm::vec3(cubie_size, 0, 0)); face_model = glm::rotate(face_model, glm::radians(90.0f), glm::vec3(0,1,0)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, x == 1 ? colorEnumToVec3(getCurrentColor(r_idx)) : COLOR_BLACK);
                    
                    face_model = glm::translate(model_transform, glm::vec3(-cubie_size, 0, 0)); face_model = glm::rotate(face_model, glm::radians(-90.0f), glm::vec3(0,1,0)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, x == -1 ? colorEnumToVec3(getCurrentColor(l_idx)) : COLOR_BLACK);

                    face_model = glm::translate(model_transform, glm::vec3(0, cubie_size, 0)); face_model = glm::rotate(face_model, glm::radians(-90.0f), glm::vec3(1,0,0)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, y == 1 ? colorEnumToVec3(getCurrentColor(u_idx)) : COLOR_BLACK);

                    face_model = glm::translate(model_transform, glm::vec3(0, -cubie_size, 0)); face_model = glm::rotate(face_model, glm::radians(90.0f), glm::vec3(1,0,0)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, y == -1 ? colorEnumToVec3(getCurrentColor(d_idx)) : COLOR_BLACK);

                    face_model = glm::translate(model_transform, glm::vec3(0, 0, cubie_size)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, z == 1 ? colorEnumToVec3(getCurrentColor(f_idx)) : COLOR_BLACK);
                    
                    face_model = glm::translate(model_transform, glm::vec3(0, 0, -cubie_size)); face_model = glm::rotate(face_model, glm::radians(180.0f), glm::vec3(0,1,0)); face_model = glm::scale(face_model, glm::vec3(sticker_scale));
                    drawFace(face_model, z == -1 ? colorEnumToVec3(getCurrentColor(b_idx)) : COLOR_BLACK);
                }
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
