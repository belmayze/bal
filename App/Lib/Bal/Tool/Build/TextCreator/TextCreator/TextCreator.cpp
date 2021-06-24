// TextCreator.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>

int main()
{
    std::ofstream file("jis.txt");
    if (!file) { return 1; }

    {
        // ASCII
        for (int i = 0x20; i < 0xA0; ++i)
        {
            if (0x7F <= i)
            {
                file << " ";
            }
            else
            {
                file << static_cast<char>(i);
            }
        }
        file << std::endl;
    }
    {
        // 半角
        for (int i = 0xA0; i < 0xE0; ++i)
        {
            if (i <= 0xA0)
            {
                file << " ";
            }
            else
            {
                file << static_cast<char>(i);
            }
        }
        file << std::endl;
    }
    {
        // 全角
        {
            // 0x8140～0x81FF
            for (int i = 0x40; i < 0x100; ++i)
            {
                if ((i == 0x7F) ||
                    (0xAD <= i && i <= 0xB7) ||
                    (0xC0 <= i && i <= 0xC7) ||
                    (0xCF <= i && i <= 0xD9) ||
                    (0xE9 <= i && i <= 0xEF) ||
                    (0xF8 <= i && i <= 0xFB) ||
                    (0xFD <= i && i <= 0xFF))
                {
                    file << "　"; // 未使用領域
                }
                else
                {
                    file << static_cast<char>(0x81) << static_cast<char>(i);
                }
                if (i == 0x7F || i == 0xBF|| i == 0xFF) { file << std::endl; }
            }
            // 0x8240～0x82FF
            for (int i = 0x40; i < 0x100; ++i)
            {
                if ((0x40 <= i && i <= 0x4E) ||
                    (0x59 <= i && i <= 0x5F) ||
                    (0x7A <= i && i <= 0x80) ||
                    (0x9B <= i && i <= 0x9E) ||
                    (0xF2 <= i && i <= 0xFF))
                {
                    file << "　"; // 未使用領域
                }
                else
                {
                    file << static_cast<char>(0x82) << static_cast<char>(i);
                }
                if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
            }
            // 0x8340～0x83FF
            for (int i = 0x40; i < 0x100; ++i)
            {
                if ((i == 0x7F) ||
                    (0x97 <= i && i <= 0x9E) ||
                    (0xB7 <= i && i <= 0xBE) ||
                    (0xD7 <= i && i <= 0xFF))
                {
                    file << "　"; // 未使用領域
                }
                else
                {
                    file << static_cast<char>(0x83) << static_cast<char>(i);
                }
                if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
            }
            // 0x8440～0x84FF
            for (int i = 0x40; i < 0x100; ++i)
            {
                if ((0x61 <= i && i <= 0x6F) ||
                    (0x7F <= i && i <= 0x7F) ||
                    (0x92 <= i && i <= 0x9E) ||
                    (0xBF <= i && i <= 0xFF))
                {
                    file << "　"; // 未使用領域
                }
                else
                {
                    file << static_cast<char>(0x84) << static_cast<char>(i);
                }
                if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
            }
            // 0x8540～0x85FF（未使用）
            // 0x8640～0x86FF（未使用）
            // 0x8740～0x87FF
            for (int i = 0x40; i < 0x100; ++i)
            {
                if ((0x5E <= i && i <= 0x5E) ||
                    (0x76 <= i && i <= 0x7D) ||
                    (0x7F <= i && i <= 0x7F) ||
                    (0x9D <= i && i <= 0xFF))
                {
                    file << "　"; // 未使用領域
                }
                else
                {
                    file << static_cast<char>(0x87) << static_cast<char>(i);
                }
                if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
            }
            // 0x8840～0x9FFF
            for (int j = 0x88; j < 0x9F; ++j)
            {
                for (int i = 0x40; i < 0x100; ++i)
                {
                    if ((j == 0x88 && 0x40 <= i && i <= 0x9E) || // 第一水準切り替え
                        (j == 0x98 && 0x73 <= i && i <= 0x9E) || // 第二水準切り替え
                        (i == 0x7F) ||            // すべてで下2桁の0x7Fは未使用
                        (0xFD <= i && i <= 0xFF)) // すべてで下2桁の0xFD～0xFFは未使用
                    {
                        file << "　"; // 未使用領域
                    }
                    else
                    {
                        file << static_cast<char>(j) << static_cast<char>(i);
                    }
                    if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
                }
            }
            // 0xE040～0xEAFF
            for (int j = 0xE0; j < 0xEB; ++j)
            {
                for (int i = 0x40; i < 0x100; ++i)
                {
                    if ((j == 0xEA && 0xA5 <= i && i <= 0xFF) || // 第二水準終了
                        (i == 0x7F) ||            // すべてで下2桁の0x7Fは未使用
                        (0xFD <= i && i <= 0xFF)) // すべてで下2桁の0xFD～0xFFは未使用
                    {
                        file << "　"; // 未使用領域
                    }
                    else
                    {
                        file << static_cast<char>(j) << static_cast<char>(i);
                    }
                    if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
                }
            }
            // 0xEB40～0xEBFF（未使用）
            // 0xEC40～0xECFF（未使用）
            // 0xED40～0xEDFF（未使用）
            // 0xEE40～0xEEFF（未使用）
            // 0xEF40～0xEFFF（未使用）
            // 0xF040～0xF0FF（未使用）
            // 0xF140～0xF1FF（未使用）
            // 0xF240～0xF2FF（未使用）
            // 0xF340～0xF3FF（未使用）
            // 0xF440～0xF4FF（未使用）
            // 0xF540～0xF5FF（未使用）
            // 0xF640～0xF6FF（未使用）
            // 0xF740～0xF7FF（未使用）
            // 0xF840～0xF8FF（未使用）
            // 0xF940～0xF9FF（未使用）
            // 0xFA40～0xFCFF
            for (int j = 0xFA; j < 0xFD; ++j)
            {
                for (int i = 0x40; i < 0x100; ++i)
                {
                    if ((j == 0xFC && 0x4C <= i && i <= 0xFF) || // 終了
                        (i == 0x7F) ||            // すべてで下2桁の0x7Fは未使用
                        (0xFD <= i && i <= 0xFF)) // すべてで下2桁の0xFD～0xFFは未使用
                    {
                        file << "　"; // 未使用領域
                    }
                    else
                    {
                        file << static_cast<char>(j) << static_cast<char>(i);
                    }
                    if (i == 0x7F || i == 0xBF || i == 0xFF) { file << std::endl; }
                }
            }
        }
    }

    return 0;
}
