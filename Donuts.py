import math
import time
import os

def main():
    A = 0.0
    B = 0.0
    
    shade = ".,-~:;=!*#$@"
    colors = [
        "\033[34m",  # Blue
        "\033[36m",  # Cyan
        "\033[32m",  # Green
        "\033[92m",  # Bright Green
        "\033[93m",  # Bright Yellow
        "\033[91m",  # Bright Red
        "\033[95m",  # Bright Magenta
        "\033[97m"   # Bright White
    ]
    
    # Clear screen
    print("\033[2J", end="")
    
    while True:
        # Initialize buffers
        z = [0] * 1760
        b = [' '] * 1760
        color = [0] * 1760
        
        j = 0
        while j < 6.28:
            i = 0
            while i < 6.28:
                c = math.sin(i)
                d = math.cos(j)
                e = math.sin(A)
                f = math.sin(j)
                g = math.cos(A)
                h = d + 2
                D = 1 / (c * h * e + f * g + 5)
                l = math.cos(i)
                m = math.cos(B)
                n = math.sin(B)
                t = c * h * g - f * e
                
                x = int(40 + 30 * D * (l * h * m - t * n))
                y = int(12 + 15 * D * (l * h * n + t * m))
                o = x + 80 * y
                N = int(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n))
                
                if 0 < y < 22 and 0 < x < 80 and 0 <= o < 1760:
                    if D > z[o]:
                        z[o] = D
                        if N < 0:
                            N = 0
                        if N > 11:
                            N = 11
                        b[o] = shade[N]
                        color[o] = N % 8
                
                i += 0.02
            j += 0.07
        
        # Move cursor to home position
        print("\033[H", end="")
        
        # Print the frame
        for k in range(1760):
            if k % 80:
                print(f"{colors[color[k]]}{b[k]}", end="")
            else:
                print()
        
        # Reset color
        print("\033[0m", end="", flush=True)
        
        A += 0.04
        B += 0.02
        time.sleep(0.03)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\033[0mBerhenti... Sampai jumpa! 👋")
