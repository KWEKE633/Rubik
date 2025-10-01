#include "cube.hpp"

Cube::Cube() {
    for (int i = 0; i < 9; ++i) state[i]    = Color::WHITE;  // Up
    for (int i = 9; i < 18; ++i) state[i]   = Color::YELLOW; // Down
    for (int i = 18; i < 27; ++i) state[i]  = Color::GREEN;  // Front
    for (int i = 27; i < 35; ++i) state[i]  = Color::BLUE;   // Back
    for (int i = 36; i < 45; ++i) state[i]  = Color::RED;    // Right
    for (int i = 45; i < 53; ++i) state[i]  = Color::ORANGE; // Left
}

void Cube::rotateF() {
    // 1. F面自体のステッカーを回転 (時計回り)
    Color temp_face[9];
    for (int i = 0; i < 9; ++i) temp_face[i] = state[18 + i];

    state[18] = temp_face[6]; state[19] = temp_face[3]; state[20] = temp_face[0];
    state[21] = temp_face[7]; state[22] = temp_face[4]; state[23] = temp_face[1];
    state[24] = temp_face[8]; state[25] = temp_face[5]; state[26] = temp_face[2];

    // 2. 隣接する4面のステッカーを移動
    Color temp_adj[3];
    // U面のFに接する3ステッカー (6, 7, 8) を一時保存
    temp_adj[0] = state[6]; temp_adj[1] = state[7]; temp_adj[2] = state[8];

    // L -> U
    state[6] = state[53]; state[7] = state[50]; state[8] = state[47];
    // D -> L
    state[53] = state[11]; state[50] = state[10]; state[47] = state[9];
    // R -> D
    state[11] = state[36]; state[10] = state[39]; state[9] = state[42];
    // (temp) U -> R
    state[36] = temp_adj[0]; state[39] = temp_adj[1]; state[42] = temp_adj[2];
}

void Cube::rotateB() {
    // 1. B面自体のステッカーを回転 (時計回り)
    Color temp_face[9];
    for (int i = 0; i < 9; ++i) temp_face[i] = state[27 + i];

    state[27] = temp_face[6]; state[28] = temp_face[3]; state[29] = temp_face[0];
    state[30] = temp_face[7]; state[31] = temp_face[4]; state[32] = temp_face[1];
    state[33] = temp_face[8]; state[34] = temp_face[5]; state[35] = temp_face[2];

    // 2. 隣接する4面のステッカーを移動 (U -> R -> D -> L -> U)
    Color temp_adj[3];
    // U面のBに接する3ステッカー (0, 1, 2) を一時保存
    temp_adj[0] = state[0]; temp_adj[1] = state[1]; temp_adj[2] = state[2];

    // L -> U
    state[0] = state[51]; state[1] = state[48]; state[2] = state[45];
    // D -> L
    state[51] = state[17]; state[48] = state[16]; state[45] = state[15];
    // R -> D
    state[17] = state[38]; state[16] = state[41]; state[15] = state[44];
    // (temp) U -> R
    state[38] = temp_adj[0]; state[41] = temp_adj[1]; state[44] = temp_adj[2];
}

void Cube::rotateL() {
    // 1. L面自体のステッカーを回転 (時計回り)
    Color temp_face[9];
    for (int i = 0; i < 9; ++i) temp_face[i] = state[45 + i];

    state[45] = temp_face[6]; state[46] = temp_face[3]; state[47] = temp_face[0];
    state[48] = temp_face[7]; state[49] = temp_face[4]; state[50] = temp_face[1];
    state[51] = temp_face[8]; state[52] = temp_face[5]; state[53] = temp_face[2];
    
    // 2. 隣接する4面のステッカーを移動 (U -> B -> D -> F -> U)
    Color temp_adj[3];
    // U面のLに接する3ステッカー (0, 3, 6) を一時保存
    temp_adj[0] = state[0]; temp_adj[1] = state[3]; temp_adj[2] = state[6];

    // F -> U
    state[0] = state[18]; state[3] = state[21]; state[6] = state[24];
    // D -> F
    state[18] = state[9]; state[21] = state[12]; state[24] = state[15];
    // B -> D
    state[9] = state[35]; state[12] = state[32]; state[15] = state[29];
    // (temp) U -> B
    state[35] = temp_adj[0]; state[32] = temp_adj[1]; state[29] = temp_adj[2];
}

void Cube::rotateR() {
    Color temp_face[9];
    for(int i = 0; i < 9; ++i) temp_face[i] = state[36 + i];
    state[36] = temp_face[6]; state[37] = temp_face[3]; state[38] = temp_face[0];
    state[39] = temp_face[7]; state[40] = temp_face[4]; state[41] = temp_face[1];
    state[42] = temp_face[8]; state[43] = temp_face[5]; state[44] = temp_face[2];

    Color temp_adj[3];
    temp_adj[0] = state[2]; temp_adj[1] = state[5]; temp_adj[2] = state[8]; // U
    state[2] = state[20]; state[5] = state[23]; state[8] = state[26]; // F -> U
    state[20] = state[15]; state[23] = state[12]; state[26] = state[9]; // D -> F
    state[15] = state[29]; state[12] = state[32]; state[9] = state[35]; // B -> D
    state[29] = temp_adj[0]; state[32] = temp_adj[1]; state[35] = temp_adj[2]; // U -> B
}

void Cube::rotateU() {
    Color temp_face[9];
    for(int i = 0; i < 9; ++i) temp_face[i] = state[0 + i];
    state[0] = temp_face[6]; state[1] = temp_face[3]; state[2] = temp_face[0];
    state[3] = temp_face[7]; state[4] = temp_face[4]; state[5] = temp_face[1];
    state[6] = temp_face[8]; state[7] = temp_face[5]; state[8] = temp_face[2];

    Color temp_adj[3];
    temp_adj[0] = state[18]; temp_adj[1] = state[19]; temp_adj[2] = state[20]; // F
    state[18] = state[36]; state[19] = state[37]; state[20] = state[38]; // R -> F
    state[36] = state[27]; state[37] = state[28]; state[38] = state[29]; // B -> R
    state[27] = state[45]; state[28] = state[46]; state[29] = state[47]; // L -> B
    state[45] = temp_adj[0]; state[46] = temp_adj[1]; state[47] = temp_adj[2]; // F -> L
}

void Cube::rotateD() {
    // 1. D面自体のステッカーを回転 (時計回り)
    Color temp_face[9];
    for (int i = 0; i < 9; ++i) temp_face[i] = state[9 + i];
    
    state[9] = temp_face[6]; state[10] = temp_face[3]; state[11] = temp_face[0];
    state[12] = temp_face[7]; state[13] = temp_face[4]; state[14] = temp_face[1];
    state[15] = temp_face[8]; state[16] = temp_face[5]; state[17] = temp_face[2];

    // 2. 隣接する4面のステッカーを移動 (F -> L -> B -> R -> F)
    Color temp_adj[3];
    // F面のDに接する3ステッカー (24, 25, 26) を一時保存
    temp_adj[0] = state[24]; temp_adj[1] = state[25]; temp_adj[2] = state[26];

    // R -> F
    state[24] = state[42]; state[25] = state[43]; state[26] = state[44];
    // B -> R
    state[42] = state[33]; state[43] = state[34]; state[44] = state[35];
    // L -> B
    state[33] = state[51]; state[34] = state[52]; state[35] = state[53];
    // (temp) F -> L
    state[51] = temp_adj[0]; state[52] = temp_adj[1]; state[53] = temp_adj[2];
}

char Cube::colorToChar(Color c) const {
    switch (c) {
        case Color::WHITE:  return 'W';
        case Color::YELLOW: return 'Y';
        case Color::GREEN:  return 'G';
        case Color::BLUE:   return 'B';
        case Color::RED:    return 'R';
        case Color::ORANGE: return 'O';
        default:            return ' ';
    }
}

void Cube::applyMove(const std::string& move) {
    if (move.length() == 0) return;

    int count = 1;
    if (move.length() > 1) {
        if (move[1] == '\'') {
            count = 3; // 逆回転は時計回りを3回
        } else if (move[1] == '2') {
            count = 2;
        }
    }

    for (int i = 0; i < count; ++i) {
        switch (move[0]) {
            case 'F': rotateF(); break;
            case 'R': rotateR(); break;
            case 'U': rotateU(); break;
            case 'B': rotateB(); break;
            case 'L': rotateL(); break;
            case 'D': rotateD(); break;
        }
    }
}

// "F R' U2" のような一連の回転指示を適用
void Cube::applyMoveSequence(const std::string& sequence) {
    std::stringstream ss(sequence);
    std::string move;
    while (ss >> move) {
        applyMove(move);
    }
}

void Cube::print() const {
    const char separator[] = " | ";
    std::cout << "-----------------" << std::endl;
    // Up Face
    std::cout << "      " << colorToChar(state[0]) << " " << colorToChar(state[1]) << " " << colorToChar(state[2]) << std::endl;
    std::cout << "      " << colorToChar(state[3]) << " " << colorToChar(state[4]) << " " << colorToChar(state[5]) << std::endl;
    std::cout << "      " << colorToChar(state[6]) << " " << colorToChar(state[7]) << " " << colorToChar(state[8]) << std::endl;
    std::cout << "-----------------" << std::endl;

    // Left, Front, Right, Back Faces
    for (int row = 0; row < 3; ++row) {
        // L
        std::cout << colorToChar(state[45 + row * 3]) << " " << colorToChar(state[46 + row * 3]) << " " << colorToChar(state[47 + row * 3]);
        std::cout << separator;
        // F
        std::cout << colorToChar(state[18 + row * 3]) << " " << colorToChar(state[19 + row * 3]) << " " << colorToChar(state[20 + row * 3]);
        std::cout << separator;
        // R
        std::cout << colorToChar(state[36 + row * 3]) << " " << colorToChar(state[37 + row * 3]) << " " << colorToChar(state[38 + row * 3]);
        std::cout << separator;
        // B
        std::cout << colorToChar(state[27 + row * 3]) << " " << colorToChar(state[28 + row * 3]) << " " << colorToChar(state[29 + row * 3]);
        std::cout << std::endl;
    }
    std::cout << "-----------------" << std::endl;

    // Down Face
    std::cout << "      " << colorToChar(state[9]) << " " << colorToChar(state[10]) << " " << colorToChar(state[11]) << std::endl;
    std::cout << "      " << colorToChar(state[12]) << " " << colorToChar(state[13]) << " " << colorToChar(state[14]) << std::endl;
    std::cout << "      " << colorToChar(state[15]) << " " << colorToChar(state[16]) << " " << colorToChar(state[17]) << std::endl;
    std::cout << "-----------------" << std::endl;
}

std::string Cube::solve_dummy(const std::string& sequence) {
    std::stringstream ss(sequence);
    std::string move;
    std::vector<std::string> moves;
    while (ss >> move) {
        moves.push_back(move);
    }
    
    std::reverse(moves.begin(), moves.end());
    
    std::string inverseSequence = "";
    for (const auto& m : moves) {
        if (m.length() == 1) {
            inverseSequence += m + "' ";
        } else if (m[1] == '\'') {
            inverseSequence += m[0];
            inverseSequence += " ";
        } else {
            inverseSequence += m + " ";
        }
    }
    return inverseSequence;
}

Color Cube::getColor(int index) const {
    if (index >= 0 && index < 54) {
        return state[index];
    }
    return Color::BLACK;
}
