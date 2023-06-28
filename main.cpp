#include <iostream>
#include "file.hpp"
#include "test.hpp"


int main() {
    system("chcp 65001");
    std::string filename;
    std::cout << "Введите название файла: ";
    std::cin >> filename;
    copy_file(filename, "out.gif");
    CustomFileStream file("../out.gif",
                          std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Файл не открылся";
        return 0;
    }
    file.print();

    int c = 1;
    std::string side;
    std::string dop;
    std::string name;
    int x1, y1, x2, y2;
    while (c != 0) {
        std::cout << "0. Завершить" << std::endl
                  << "1. Обрезка (x1,y1,x2,y2)" << std::endl
                  << "2. Поворот на 90 градусов ('r' или 'l')" << std::endl
                  << "3. Склеивание (сторона('u','r','d','l'), сдвиг('u','r','d','l'), название файла)" << std::endl
                  << "4. Негатив" << std::endl
                  << "5. Прозрачное наложение (x,y, название файла)" << std::endl
                  << "6. Наложение (x,y, название файла)" << std::endl
                  << "7. Тесты" << std::endl;
        std::cin >> c;
        if (c == 1) {
            std::cout << "Введите x1,y1,x2,y2 через пробел:  ";
            std::cin >> x1 >> y1 >> x2 >> y2;
            file.set(x1, y1, x2, y2);
            break;
        } else if (c == 2) {
            std::cout << "Введите в какую сторону: ";
            std::cin >> side;
            file.flip(side);
        } else if (c == 3) {
            std::cout << "Введите с какой стороны приклеить: ";
            std::cin >> side;
            std::cout << std::endl << "Введите в какую сторону сдвигать изображение: ";
            std::cin >> dop;
            std::cout << std::endl << "Введите название файла: ";
            std::cin >> name;
            CustomFileStream dopFile("../" + name,
                                     std::ios::in | std::ios::out | std::ios::binary);
            file.join(side, dop, &dopFile);
        } else if (c == 4) {
            file.negative();
        } else if (c == 5 or c == 6) {
            std::cout << "Введите x1,y1 через пробел: ";
            std::cin >> x1 >> y1;
            std::cout << std::endl << "Введите название файла: ";
            std::cin >> name;
            CustomFileStream dopFile("../" + name,
                                     std::ios::in | std::ios::out | std::ios::binary);
            if (c == 5) {
                file.overlay_d(x1, y1, &dopFile);
            } else if (c == 6) {
                file.overlay(x1, y1, &dopFile);
            }
        } else if (c == 7){
            init();
        } else if (c == 0){
            break;
        } else {
            std::cout << "Введите верное значение" << std::endl;
            while (true){
                std::cin >> c;
                if (c == 0 or c == 1 or c == 2 or c == 3 or c == 4 or c == 5 or c == 6 or c == 7){
                    break;
                } else {
                    std::cout << "Введите верное значение" << std::endl;
                }
            }
        }
    }

    file.close();
//    init();
    return 0;
}
