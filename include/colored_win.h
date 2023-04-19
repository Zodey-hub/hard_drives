/**
 * @file colored_win.h
 * @brief Header file for the colored namespace that provides a class for colored console output.
 */
#ifndef COLORED_WIN_H_INCLUDED
#define COLORED_WIN_H_INCLUDED

#include <windows.h>
#include <iostream>

namespace colored
{

    enum colors
    {
        default_grey = 7,
        grey = 8,
        blue = 9,
        green = 10,
        cyan = 11,
        red = 12,
        pink = 13,
        yellow = 14,
        white = 15,
    };

    /**
     * @brief A class that provides colored console output.
     * @example colored::colored coloring;
     * std::cout << coloring.print_colored("this is white ", colored::white) << coloring.print_colored("this is blue ", colored::blue) << "this has the default color" << std::endl;
     *
     */
    class Colored
    {
    private:
        HANDLE hConsole;
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        WORD wOldColorAttrs;

    public:
        /**
         * @brief Constructor that initializes the console handle and retrieves the console screen buffer info.
         */
        Colored()
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
            wOldColorAttrs = csbiInfo.wAttributes;
        };

        /**
         * @brief Destructor that closes the console handle.
         */
        ~Colored()
        {
            CloseHandle(hConsole);
        };

        /**
         * @brief Prints the given data to the console using the specified color.
         * @param data The data to print to the console.
         * @param color The color to use for the console output.
         * @return A nullpointer.
         */
        char *print_colored(const char *data, colors color)
        {
            SetConsoleTextAttribute(hConsole, color);
            std::cout << data;
            SetConsoleTextAttribute(hConsole, wOldColorAttrs);
            return nullptr;
        }
    };

};
#endif // COLORED_WIN_H_INCLUDED
