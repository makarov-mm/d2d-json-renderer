#pragma once

namespace Domain
{
    struct ArgbColor
    {
        unsigned char A;
        unsigned char R;
        unsigned char G;
        unsigned char B;

        ArgbColor(unsigned char a = 255,
            unsigned char r = 0,
            unsigned char g = 0,
            unsigned char b = 0)
            : A(a), R(r), G(g), B(b) {
        }
    };
}
