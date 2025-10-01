#ifndef CUBE_HPP
#define CUBE_HPP

#include <array>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include <string>

// using namespace std;

enum class Color {
    WHITE,  // 0 (Up)
    YELLOW, // 1 (Down)
    GREEN,  // 2 (Front)
    BLUE,   // 3 (Back)
    RED,    // 4 (Right)
    ORANGE, // 5 (Left)
    BLACK
};

struct Corner {
    int position; // 0-7 のどこにあるか
    int orientation; // 向き (0, 1, 2)
};

// 12個のエッジピース
struct Edge {
    int position; // 0-11 のどこにあるか
    int orientation; // 向き (0, 1)
};

class Cube {
    public:
        Cube();
        std::string solve();
        // "F", "R'", "U2" のような単一の回転指示を適用
        void applyMove(const std::string& move);

        // "F R' U2" のような一連の回転指示を適用
        void applyMoveSequence(const std::string& sequence);
        void print() const;
        std::string solve_dummy(const std::string& sequence);
        Color getColor(int index) const;

    private:
        void rotateF();
        void rotateB();
        void rotateL();
        void rotateR();
        void rotateU();
        void rotateD();
    // 6面 * 9ステッカー = 54個のステッカーの色を保持
    // [0-8]:U, [9-17]:D, [18-26]:F, [27-35]:B, [36-44]:R, [45-53]:L
        std::array<Color, 54> state;

    // 色を文字に変換して出力するためのヘルパー関数
        char colorToChar(Color c) const;
        // ヒューリスティック値を計算する
        int heuristic() const;
    
    // IDA* のための再帰的な探索関数
        bool search(int depth, std::vector<std::string>& path);

        std::array<Corner, 8> corners;
        std::array<Edge, 12> edges;
};

#endif
