#include <iostream>
#include <cmath>
#include <cstring>
#include <thread>
#include <chrono>

int main() {
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    int color[1760];
    
    const char* shade = ".,-~:;=!*#$@";
    const char* colors[] = {
        "\x1b[34m",  // Blue
        "\x1b[36m",  // Cyan
        "\x1b[32m",  // Green
        "\x1b[92m",  // Bright Green
        "\x1b[93m",  // Bright Yellow
        "\x1b[91m",  // Bright Red
        "\x1b[95m",  // Bright Magenta
        "\x1b[97m"   // Bright White
    };
    
    // Clear screen
    std::cout << "\x1b[2J";
    
    while (true) {
        // Initialize buffers
        std::memset(b, ' ', sizeof(b));
        std::memset(z, 0, sizeof(z));
        std::memset(color, 0, sizeof(color));
        
        for (j = 0; j < 6.28; j += 0.07) {
            for (i = 0; i < 6.28; i += 0.02) {
                float c = std::sin(i);
                float d = std::cos(j);
                float e = std::sin(A);
                float f = std::sin(j);
                float g = std::cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = std::cos(i);
                float m = std::cos(B);
                float n = std::sin(B);
                float t = c * h * g - f * e;
                
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = static_cast<int>(8 * ((f * e - c * d * g) * m 
                    - c * d * e - f * g - l * d * n));
                
                if (y > 0 && y < 22 && x > 0 && x < 80 && o >= 0 && o < 1760) {
                    if (D > z[o]) {
                        z[o] = D;
                        if (N < 0) N = 0;
                        if (N > 11) N = 11;
                        b[o] = shade[N];
                        color[o] = N % 8;
                    }
                }
            }
        }
        
        // Move cursor to home position
        std::cout << "\x1b[H";
        
        // Print the frame
        for (k = 0; k < 1760; k++) {
            if (k % 80) {
                std::cout << colors[color[k]] << b[k];
            } else {
                std::cout << '\n';
            }
        }
        
        // Reset color
        std::cout << "\x1b[0m" << std::flush;
        
        A += 0.04;
        B += 0.02;
        
        // Sleep for 30ms
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    
    return 0;
}

// Pusing jir wkwkkw - Donut yang berputar bikin pusing! 😵‍💫
