#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include "map"
#include "cmath"
#include <string>
#include <vector>


void copy_file(const std::string &filename, const std::string &copyname) {
    std::ifstream in("../" + filename, std::ios::binary);
    if (in) {
        std::ofstream out("../" + copyname, std::ios::binary);
        char byte;
        while (in.get(byte)) {
            out.put(byte);
        }
        in.close();
        out.close();
    }
}


int bin_to_dec(std::string binary) {
    // Перевод из двоичного в десятичное
    int decimal = 0;
    int power = 1;

    for (int i = int(binary.length()) - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            decimal += power;
        }
        power *= 2;
    }

    return decimal;
}


std::string dec_to_bin(int decimal, int length) {
    // Перевод из десятичного в двоичное
    std::string binary;

    while (decimal > 0) {
        int remainder = decimal % 2;
        binary.insert(0, std::to_string(remainder));
        decimal /= 2;
    }

    // Добавляем недостающие нули в начало
    while (binary.length() < length) {
        binary.insert(0, "0");
    }

    return binary;
}


class CustomFileStream : public std::fstream {
private:
    struct GIFHeader {
        char signature[3];
        char version[3];
        uint16_t width;
        uint16_t height;
        long long weight;
    };
    GIFHeader header{};
    int count = -1;
    int count_colors;

    void init() {
        /**
         * Инициализируем пиксели, переменную c(номер байта начала блока изображений), lzw код, header
        **/
        read(reinterpret_cast<char *>(&header), 10);
        seekg(0, std::ios::end);
        std::streampos fileSize = tellg();
        auto fileSizeLL = static_cast<long long>(fileSize);
        seekg(0, std::ios::beg);
        header.weight = fileSizeLL;

        // Чтение данных из файла
        seekg(0, std::ios::end);
        size_t n = tellg();
        seekg(0, std::ios::beg);
        char *arr = new char[n];
        read(arr, long(n));
//    3917
        std::stringstream ss;
        std::stringstream ss2;
        seekg(6, std::ios::beg);
        unsigned char seventhByte[4];
        read(reinterpret_cast<char *>(&seventhByte), 4);
        for (unsigned char i: seventhByte) {
            ss2 << std::hex << static_cast<unsigned int>(i);
        }
        std::vector<unsigned char> bytes(9);
        for (int i = 0; i < n; ++i) {
            seekg(i, std::ios::beg);
            read(reinterpret_cast<char *>(bytes.data()), 9);
            for (const auto &byte: bytes) {
                ss << std::hex << static_cast<unsigned int>(byte);
            }
            count++;
            if (ss.str() == "2c0000" + ss2.str()) {
                c = count;
                seekg(c + 10, std::ios::beg);
                read(reinterpret_cast<char *>(&lzw), 1);
                lzw++;
                break;
            }
            ss.str("");
        }
        delete[] arr;

        seekg(0, std::ios::end);

        std::streampos startPosition = c + 11;
        seekg(startPosition, std::ios::beg);
        std::streamsize dataSize = fileSize - startPosition;
        std::vector<unsigned char> buffer(dataSize);
        read(reinterpret_cast<char *>(buffer.data()), dataSize);

        pixels = pixels_decrypt(this, buffer);
    }

    template<typename KeyType, typename ValueType>
    static std::map<KeyType, ValueType> create_map(std::map<KeyType, ValueType> &colors, int count_colors) {
        /**
         * Ввод: словарь, количество цветов
         * Создание пустого словаря цветов
         * Вывод: словарь цветов
        **/
        for (int i = 0; i < count_colors; ++i) {
            colors[i].push_back(std::to_string(i));
        }
        colors[count_colors].emplace_back("clear");
        colors[count_colors + 1].emplace_back("end");
        return colors;
    }

    template<typename KeyType, typename ValueType>
    static int getKeyByValue(const std::map<KeyType, ValueType> &map, const ValueType &value) {
        /**
         * Вход: словарь, значение
         * Находит ключ по значению
         * Вывод: ключ
        **/
        for (const auto &pair: map) {
            if (pair.second == value) {
                return pair.first;
            }
        }
        return -1;
    }

    std::vector<std::string> pixels_decrypt(CustomFileStream *file, const std::vector<unsigned char> &arr) {
        /**
         * Ввод: файл, вектор байтов
         * Декодирует байты блока изображения в вектор цветов пикселей
         * Вывод: вектор цветов пикселей
        **/
        int lenBuf = arr[0];
        file->seekg(10, std::ios::beg);
        unsigned char a;
        file->read(reinterpret_cast<char *>(&a), 1);
        count_colors = int(std::pow(2, int(bin_to_dec(dec_to_bin(int(a), 8).substr(1, 3))) + 1));

        std::map<int, std::vector<std::string>> colors;
        colors = create_map(colors, count_colors);

        file->seekg(file->c + 12, std::ios::beg);
        std::string bytes;
        std::string bits;
        std::string bits_last;
        int i = 1;
        int length = 0;
        int llzw = lzw;
        while (true) {
            for (int j = i; j < lenBuf + i; ++j) {
                bytes.insert(0, dec_to_bin(int(arr[j]), 8));
            }
            while (bytes.length() > 0) {
                if (bytes.length() >= llzw) {
                    bits = bytes.substr(bytes.length() - llzw, bytes.length());
                    bytes = bytes.substr(0, bytes.length() - llzw);
                } else {
                    bits = bytes;
                    bytes = "";
                    while (bits.length() != llzw) {
                        bits.insert(0, "0");
                    }
                }
                if (bin_to_dec(bits) == count_colors) {
                    colors.clear();
                    colors = create_map(colors, count_colors);
                    llzw = lzw;
                    bits_last = bits;
                    length = int(colors.size());
                    continue;
                }
                if (bin_to_dec(bits) == count_colors + 1) {
                    return pixels;
                }
                if (bin_to_dec(bits_last) == count_colors) {
                    bits_last = bits;
                    for (int j = 0; j < colors[bin_to_dec(bits)].size(); ++j) {
                        pixels.push_back(colors[bin_to_dec(bits)][j]);
                    }
                    continue;
                }
                if (colors.find(bin_to_dec(bits)) != colors.end()) {
                    colors[length] = colors[bin_to_dec(bits_last)];
                    colors[length].push_back(colors[bin_to_dec(bits)][0]);
                    for (int j = 0; j < colors[bin_to_dec(bits)].size(); ++j) {
                        pixels.push_back(colors[bin_to_dec(bits)][j]);
                    }
                    length++;
                } else {
                    colors[length] = colors[bin_to_dec(bits_last)];
                    colors[length].push_back(colors[bin_to_dec(bits_last)][0]);
                    for (int j = 0; j < colors[bin_to_dec(bits)].size(); ++j) {
                        pixels.push_back(colors[bin_to_dec(bits)][j]);
                    }
                    length++;
                }
                bits_last = bits;
                if (pow(2, llzw) == length) {
                    llzw++;
                }
            }

            i += lenBuf;
            lenBuf = int(arr[i]);
            i++;
            if (lenBuf == 59) {
                break;
            }
        }
        return pixels;
    }

    std::string codes() {
        /**
         * Составляет коды и сразу переводит их в биты
         * Вывод: строка бит
        **/
        int llzw = this->lzw;
        std::string buffer;
        std::string k;
        std::vector<int> codes;
        seekg(10, std::ios::beg);

        std::map<int, std::vector<std::string>> colors;
        colors = create_map(colors, count_colors);
        codes.push_back(count_colors);
        int length;
        int index;

        std::string bits{dec_to_bin(count_colors, llzw)};
        std::string l;
        for (int i = 0; i < pixels.size(); ++i) {
            buffer += pixels[i];
            k = pixels[i + 1];
            std::vector<std::string> key;
            std::string s;
            for (char j: buffer) {
                s = j;
                key.push_back(s);
            }
            key.push_back(k);
            if (getKeyByValue(colors, key) >= 0) {
                if (i == pixels.size() - 2) {
                    if (pow(2, llzw) - 1 == length) llzw++;
                    codes.push_back(getKeyByValue(colors, key));
                    codes.push_back(count_colors + 1);
                    l = dec_to_bin(getKeyByValue(colors, key), llzw);
                    bits.insert(0, l);
                    bits.insert(0, dec_to_bin(count_colors + 1, llzw));
                    int t{int(8 - bits.length() % 8)};
                    if (t == 8) t = 0;
                    if (bits.length() % 8 != 0) {
                        for (int j = 0; j < t; ++j) {
                            bits.insert(0, "0");
                        }
                    }
                    break;
                }
                continue;
            } else {
                length = int(colors.size());
                for (const std::string &j: key) {
                    colors[length].push_back(j);
                }
                key.pop_back();
                index = getKeyByValue(colors, key);
                if (index >= 0) {
                    codes.push_back(index);
                    l = dec_to_bin(index, llzw);
                    bits.insert(0, l);
                }
                if (pow(2, llzw) == length) {
                    llzw++;
                }
                buffer = "";
                if (i == pixels.size() - 2) {
                    codes.push_back(std::stoi(k));
                    l = dec_to_bin(std::stoi(k), llzw);
                    bits.insert(0, l);
                    bits.insert(0, dec_to_bin(count_colors + 1, llzw));
                    int t{int(8 - bits.length() % 8)};
                    if (t == 8) t = 0;
                    if (bits.length() % 8 != 0) {
                        for (int j = 0; j < t; ++j) {
                            bits.insert(0, "0");
                        }
                    }
                    break;
                }
            }
        }

        return bits;
    }

    void pixWrite() {
        /**
         * По вектору pixels формирует байты и записывает их в файл
        **/
        std::string bits = codes();
        std::vector<std::string> bytes;

        for (int i = int(bits.length() - 8); i >= 0; i -= 8) {
            bytes.push_back(bits.substr(i, 8));
        }
        bytes.emplace_back("00111011");
        unsigned char k;
        seekp(c + 11, std::ios::beg);
        k = bytes.size() - 1;
        write(reinterpret_cast<char *>(&k), 1);
        seekp(c + 12, std::ios::beg);
        for (const auto &byte: bytes) {
            k = std::stoi(byte, nullptr, 2);
            write(reinterpret_cast<char *>(&k), 1);
        }

        std::streampos fileSize = tellg();
        std::vector<char> buffer(fileSize);
        seekg(0, std::ios::beg);
        read(buffer.data(), fileSize);
        std::ofstream outputFile(fileName, std::ios::binary);
        outputFile.seekp(0, std::ios::beg);
        outputFile.write(buffer.data(), fileSize);
        outputFile.close();
    }

public:
    int c{};
    int lzw = 0;
    std::string fileName;
    std::vector<std::string> pixels;

    CustomFileStream(const std::string &filename, std::ios_base::openmode mode)
            : std::fstream(filename, mode) {
        init();
        fileName = filename;
    }

    void print() const {
        std::cout << "Высота: " << header.height << std::endl;
        std::cout << "Ширина: " << header.width << std::endl;
        std::cout << "Размер: " << header.weight << std::endl;
        std::cout << "Версия GIF: " << header.version << std::endl;
    }

    void set(uint16_t x1 = 0, uint16_t y1 = 0, uint16_t x2 = 0, uint16_t y2 = 0, int a = 1) {
        /**
         * Ввод: Координаты
         * Обрезает фотографию
        **/
        if (x1 <= 0 or x1 > header.width) {
            std::cout << "x1 должно быть больше 1 и меньше " << header.width << std::endl;
            return;
        } else if (y1 <= 0 or y1 > header.height) {
            std::cout << "y1 должно быть больше 1 и меньше " << header.height << std::endl;
            return;
        } else if (x2 <= 0 or x2 > header.width) {
            std::cout << "x2 должно быть больше 1 и меньше " << header.width << std::endl;
            return;
        } else if (y2 <= 0 or y2 > header.height) {
            std::cout << "y2 должно быть больше 1 и меньше " << header.height << std::endl;
            return;
        }
        if (x1 >= x2) {
            std::cout << "x2 должно быть больше x1" << std::endl;
            return;
        } else if (y1 >= y2) {
            std::cout << "y2 должно быть больше y1" << std::endl;
            return;
        } else if (x2 == 0) {
            x2 = header.width;
        } else if (y2 == 0) {
            y2 = header.height;
        }
        if (y2 != header.height) {
            pixels.erase(pixels.begin(), pixels.begin() + (header.height - y2) * header.width);
        }
        if (y1 != 1) {
            pixels.erase(pixels.end() - (y1 - 1) * header.width, pixels.end());
        }
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                if (j + 1 < x1) {
                    pixels[i * header.width + j] = "-1";
                } else if (j + 1 > x2) {
                    pixels[i * header.width + j] = "-1";
                }
            }
        }
        for (auto it = pixels.begin(); it != pixels.end(); ) {
            if (*it == "-1") {
                it = pixels.erase(it);
            } else {
                ++it;
            }
        }
        uint16_t x = x2 - x1 + 1;
        uint16_t y = y2 - y1 + 1;
        if (x != 0) {
            seekp(6);
            write(reinterpret_cast<const char *>(&x), sizeof(x));
            seekp(c + 5, std::ios::beg);
            write(reinterpret_cast<const char *>(&x), sizeof(x));
            if (a)std::cout << "Новая ширина: " << x << std::endl;

            header.width = x;
        }
        if (y != 0) {
            seekp(8);
            write(reinterpret_cast<const char *>(&y), sizeof(y));
            seekp(c + 7, std::ios::beg);
            write(reinterpret_cast<const char *>(&y), sizeof(y));
            if (a)std::cout << "Новая высота: " << y << std::endl;

            header.height = y;
        }

        pixWrite();
    }

    void flip(const std::string &side) {
        /**
         * Ввод: в какую сторон крутить(r, l)
         * Поворачивает изображение на 90 градусов, если side == r то в право или side == l то в лево
        **/
        if (side != "r" and side != "l") {
            std::cout << "Аргумент должен быть либо 'l' либо 'r'" << std::endl;
            return;
        }
        std::vector<std::string> new_pixels;
        if (side == "r") {
            for (int i = 0; i < header.width; ++i) {
                for (int j = header.height - 1; j >= 0; --j) {
                    new_pixels.push_back(pixels[i + j * header.width]);
                }
            }
        } else if (side == "l") {
            for (int i = header.width - 1; i >= 0; --i) {
                for (int j = 0; j < header.height; ++j) {
                    new_pixels.push_back(pixels[i + j * header.width]);
                }
            }
        } else {
            return;
        }
        pixels = new_pixels;
        seekp(6);
        write(reinterpret_cast<const char *>(&header.height), sizeof(header.height));
        seekp(c + 5, std::ios::beg);
        write(reinterpret_cast<const char *>(&header.height), sizeof(header.height));
        seekp(8);
        write(reinterpret_cast<const char *>(&header.width), sizeof(header.width));
        seekp(c + 7, std::ios::beg);
        write(reinterpret_cast<const char *>(&header.width), sizeof(header.width));
        int tmp = header.width;
        header.width = header.height;
        header.height = tmp;
        pixWrite();
    }

    void join(const std::string &side, const std::string &dop, CustomFileStream *file) {
        /**
         * Ввод: сторона, вверх вниз влево, вправо, файл
         * Склеивает два изображения. Сначала вводится сторона к которой должно быть приклеено изображение,
         * далее в какую сторону это изображение подвинуть если размеры изображений разные
        **/
        if (side == "u" or side == "d") {
            if (dop != "l" and dop != "r") {
                std::cout << "Второй аргумент должен быть либо 'r' либо 'l'" << std::endl;
                return;
            }
        } else if (side == "r" or side == "l") {
            if (dop != "u" and dop != "d") {
                std::cout << "Второй аргумент должен быть либо 'u' либо 'd'" << std::endl;
                return;
            }
        } else {
            std::cout << "Первый аргумент должен быть 'u', 'r', 'd', 'l'" << std::endl;
            return;
        }
        std::vector<std::string> pixelsDop = file->pixels;

        std::vector<unsigned char> buffer(count_colors * 3 + file->count_colors * 3);
        seekg(13, std::ios::beg);
        read(reinterpret_cast<char *>(buffer.data()), count_colors * 3);
        file->seekg(13, std::ios::beg);
        int k = 0;
        for (int i = 0; i < buffer.size(); i += 3) {
            if (int(buffer[i]) == 0 and int(buffer[i + 1]) == 0 and int(buffer[i + 2]) == 0) {
                buffer.clear();
                k = i + 13;
                break;
            }
        }
        if (k == 0) {
            buffer.clear();
            k = count_colors * 3 + 12;
        }
        file->read(reinterpret_cast<char *>(buffer.data()), file->count_colors * 3);

        for (auto &i: pixelsDop) {
            i = std::to_string((k - 13) / 3 + std::stoi(i));
        }

        if (side == "r" or side == "l") {
            if (header.height < file->header.height) {
                if (dop == "u") {
                    for (int i = 0; i < header.width * (file->header.height - header.height); ++i) {
                        pixels.insert(pixels.begin(), std::to_string(count_colors + file->count_colors - 1));
                    }
                } else if (dop == "d") {
                    for (int i = 0; i < header.width * (file->header.height - header.height); ++i) {
                        pixels.emplace_back(std::to_string(count_colors + file->count_colors - 1));
                    }
                }
            } else if (header.height > file->header.height) {
                if (dop == "u") {
                    for (int i = 0; i < file->header.width * (header.height - file->header.height); ++i) {
                        pixelsDop.emplace_back(std::to_string(count_colors + file->count_colors - 1));
                    }
                } else if (dop == "d") {
                    for (int i = 0; i < file->header.width * (header.height - file->header.height); ++i) {
                        pixelsDop.insert(pixelsDop.begin(), std::to_string(count_colors + file->count_colors - 1));
                    }
                }
            }
        } else if (side == "u" or side == "d") {
            if (header.width < file->header.width) {
                if (dop == "r") {
                    for (int i = 0; i < header.height; ++i) {
                        for (int j = 0; j < file->header.width - header.width; ++j) {
                            pixels.insert(pixels.begin() + i * (header.width + file->header.width - header.width) +
                                          header.width, std::to_string(count_colors + file->count_colors - 1));
                        }
                    }
                } else if (dop == "l") {
                    for (int i = 0; i < header.height; ++i) {
                        for (int j = 0; j < file->header.width - header.width; ++j) {
                            pixels.insert(pixels.begin() + i * (header.width + file->header.width - header.width),
                                          std::to_string(count_colors + file->count_colors - 1));
                        }
                    }
                }
            } else if (header.width > file->header.width) {
                if (dop == "r") {
                    for (int i = 0; i < file->header.height; ++i) {
                        for (int j = 0; j < header.width - file->header.width; ++j) {
                            pixelsDop.insert(
                                    pixelsDop.begin() + i * (file->header.width + header.width - file->header.width),
                                    std::to_string(file->count_colors + count_colors - 1));
                        }
                    }
                } else if (dop == "l") {
                    for (int i = 0; i < file->header.height; ++i) {
                        for (int j = 0; j < header.width - file->header.width; ++j) {
                            pixelsDop.insert(
                                    pixelsDop.begin() + i * (file->header.width + header.width - file->header.width) +
                                    file->header.width, std::to_string(file->count_colors + count_colors - 1));
                        }
                    }
                }
            }
        }
        if (side == "u") {
            for (const auto &i: pixelsDop) {
                pixels.insert(pixels.begin(), i);
            }
            header.height += file->header.height;
            header.width = std::max(header.width, file->header.width);
        } else if (side == "d") {
            for (const auto &i: pixelsDop) {
                pixels.insert(pixels.end(), i);
            }
            header.height += file->header.height;
            header.width = std::max(header.width, file->header.width);
        } else if (side == "r") {
            header.height = std::max(header.height, file->header.height);
            for (int i = 0; i < header.height; ++i) {
                for (int j = 0; j < file->header.width; ++j) {
                    pixels.insert(pixels.begin() + i * (header.width + file->header.width) + header.width,
                                  pixelsDop[i * file->header.height + j]);
                }
            }
            header.width += file->header.width;
        } else if (side == "l") {
            header.height = std::max(header.height, file->header.height);
            for (int i = 0; i < header.height; ++i) {
                for (int j = 0; j < file->header.width; ++j) {
                    pixels.insert(pixels.begin() + i * (header.width + file->header.width),
                                  pixelsDop[i * file->header.height + j]);
                }
            }
            header.width += file->header.width;
        }

        seekg(k, std::ios::beg);
        std::vector<char> remainingData((std::istreambuf_iterator<char>(*this)), std::istreambuf_iterator<char>());
        seekp(k, std::ios::beg);
        for (int i = 0; i < file->count_colors * 3; ++i) {
            write(reinterpret_cast<const char *>(&buffer[i]), 1);
        }
        for (char byte: remainingData) {
            put(byte);
        }
        count_colors += file->count_colors;
        c += file->count_colors * 3;
        while (pow(2, lzw - 1) < count_colors) {
            lzw++;
        }
        seekp(10, std::ios::beg);
        read(reinterpret_cast<char *>(&k), 1);
        std::string f = dec_to_bin(k, 8);
        f.replace(1, 3, dec_to_bin(lzw - 2, 3));
        f.replace(5, 7, dec_to_bin(lzw - 2, 3));
        seekp(10, std::ios::beg);
        k = bin_to_dec(f);
        write(reinterpret_cast<char *>(&k), 1);
        seekp(c + 10, std::ios::beg);
        lzw--;
        write(reinterpret_cast<char *>(&lzw), 1);
        lzw++;
        seekp(6);
        write(reinterpret_cast<const char *>(&header.width), sizeof(header.width));
        seekp(c + 5, std::ios::beg);
        write(reinterpret_cast<const char *>(&header.width), sizeof(header.width));
        seekp(8);
        write(reinterpret_cast<const char *>(&header.height), sizeof(header.height));
        seekp(c + 7, std::ios::beg);
        write(reinterpret_cast<const char *>(&header.height), sizeof(header.height));
        pixWrite();
    }

    void negative() {
        /**
         * Создает негатив фотографии
        **/
        seekg(13, std::ios::beg);
        std::vector<char> buffer(3);
        read(buffer.data(), 3);
        int k;
        seekp(13, std::ios::beg);
        for (int j = 1; j < count_colors; ++j) {
            for (char i: buffer) {
                k = 255 - int(i);
                write(reinterpret_cast<const char *>(&k), 1);
            }
            seekg(13 + j * 3, std::ios::beg);
            read(buffer.data(), 3);
            seekp(13 + j * 3, std::ios::beg);
        }
    }

    void overlay_d(int x1, int y1, CustomFileStream *file) {
        /**
         * Ввод: координаты первого изображения, файл
         * Накладывает одно изображение на другое со смешиванием цветов
        **/
        if (file->header.width > header.width) {
            std::cout << "Накладываем изображение должно быть меньше(ширина)" << std::endl;
            return;
        } else if (file->header.height > header.height) {
            std::cout << "Накладываем изображение должно быть меньше(высота)" << std::endl;
            return;
        } else if (x1 - 1 + file->header.width > header.width or y1 - 1 + file->header.height > header.height) {
            std::cout << "Изображение не должно вылазить за рамки другого изображения(измените x и y)" << std::endl;
            return;
        }
        std::vector<unsigned char> colorsStart(count_colors * 3);
        std::vector<unsigned char> colors;
        std::vector<unsigned char> colorsDop(file->count_colors * 3);
        seekg(13, std::ios::beg);
        read(reinterpret_cast<char *>(colorsStart.data()), count_colors * 3);
        file->seekg(13, std::ios::beg);
        file->read(reinterpret_cast<char *>(colorsDop.data()), file->count_colors * 3);


        std::vector<std::string> pixelsDop = file->pixels;
        std::vector<std::string> pixelsNew = pixels;
        int x2 = x1 + file->header.width - 1;
        int y2 = y1 + file->header.height - 1;

        if (y2 != header.height) {
            pixelsNew.erase(pixelsNew.begin(), pixelsNew.begin() + (header.height - y2) * header.width);
        }
        if (y1 != 1) {
            pixelsNew.erase(pixelsNew.end() - (y1 - 1) * header.width, pixelsNew.end());
        }
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                if (j + 1 < x1) {
                    pixelsNew[i * header.width + j] = "-1";
                } else if (j + 1 > x2) {
                    pixelsNew[i * header.width + j] = "-1";
                }
            }
        }
        for (auto it = pixelsNew.begin(); it != pixelsNew.end(); ) {
            if (*it == "-1") {
                it = pixelsNew.erase(it);
            } else {
                ++it;
            }
        }

        for (int i = 0; i < colorsStart.size(); i += 3) {
            if (int(colorsStart[i]) != 0 or int(colorsStart[i + 1]) != 0 or int(colorsStart[i + 2]) != 0) {
                colors.push_back(colorsStart[i]);
                colors.push_back(colorsStart[i + 1]);
                colors.push_back(colorsStart[i + 2]);
            }
        }
        int r, g, b{0};
        bool f = true;
        int k = int(colors.size());
        for (int i = 0; i < pixelsNew.size(); ++i) {
            r = (colors[std::stoi(pixelsNew[i]) * 3] + colorsDop[std::stoi(pixelsDop[i]) * 3] + 1) / 2;
            g = (colors[std::stoi(pixelsNew[i]) * 3 + 1] + colorsDop[std::stoi(pixelsDop[i]) * 3 + 1] + 1) / 2;
            b = (colors[std::stoi(pixelsNew[i]) * 3 + 2] + colorsDop[std::stoi(pixelsDop[i]) * 3 + 2] + 1) / 2;
            for (int j = 0; j < colors.size(); j += 3) {
                if (int(colors[j]) == r and int(colors[j + 1]) == g and int(colors[j + 2]) == b) {
                    f = false;
                    if (j >= k) {
                        pixelsNew[i] = std::to_string(j / 3);
                    }
                }
            }
            if (f) {
                colors.push_back(r);
                colors.push_back(g);
                colors.push_back(b);
                pixelsNew[i] = std::to_string((colors.size() - 1) / 3);
            }
            f = true;
        }

        while (pow(2, lzw - 1) < int(colors.size() / 3)) {
            lzw++;
        }

        while (pow(2, lzw - 1) != int(colors.size() / 3)) {
            colors.push_back(0);
        }

        k = 0;
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                if (j >= x1 - 1 and j <= x2 - 1 and header.height - y2 <= i and header.height - y1 >= i) {
                    pixels[j + i * header.width] = pixelsNew[k];
                    k++;
                }
            }
        }
        k = 13 + count_colors * 3;
        seekg(k, std::ios::beg);
        std::vector<char> remainingData((std::istreambuf_iterator<char>(*this)), std::istreambuf_iterator<char>());
        seekp(13, std::ios::beg);
        for (unsigned char &color: colors) {
            write(reinterpret_cast<const char *>(&color), 1);
        }
        for (char byte: remainingData) {
            put(byte);
        }
        c += int(colors.size()) - count_colors * 3;
        count_colors = int(colors.size()) / 3;
        seekp(10, std::ios::beg);
        read(reinterpret_cast<char *>(&k), 1);
        std::string t = dec_to_bin(k, 8);
        t.replace(1, 3, dec_to_bin(lzw - 2, 3));
        t.replace(5, 7, dec_to_bin(lzw - 2, 3));
        seekp(10, std::ios::beg);
        k = bin_to_dec(t);
        write(reinterpret_cast<char *>(&k), 1);
        seekp(c + 10, std::ios::beg);
        lzw--;
        write(reinterpret_cast<char *>(&lzw), 1);
        lzw++;
        pixWrite();
    }

    void overlay(int x1, int y1, CustomFileStream *file) {
        /**
         * Ввод: координаты первого изображения, файл
         * Накладывает одно изображение на другое
        **/
        if (file->header.width > header.width) {
            std::cout << "Накладываем изображение должно быть меньше(ширина)" << std::endl;
            return;
        } else if (file->header.height > header.height) {
            std::cout << "Накладываем изображение должно быть меньше(высота)" << std::endl;
            return;
        } else if (x1 - 1 + file->header.width > header.width or y1 - 1 + file->header.height > header.height) {
            std::cout << "Изображение не должно вылазить за рамки другого изображения(измените x и y)" << std::endl;
            return;
        }
        x1 -= 1;
        y1 -= 1;
        int x2 = x1 + file->header.width - 1;
        int y2 = y1 + file->header.height - 1;
        std::vector<std::string> pixelsDop = file->pixels;

        std::vector<unsigned char> buffer(count_colors * 3 + file->count_colors * 3);
        seekg(13, std::ios::beg);
        read(reinterpret_cast<char *>(buffer.data()), count_colors * 3);
        file->seekg(13, std::ios::beg);
        int k = 0;
        for (int i = 0; i < buffer.size(); i += 3) {
            if (int(buffer[i]) == 0 and int(buffer[i + 1]) == 0 and int(buffer[i + 2]) == 0) {
                buffer.clear();
                k = i + 13;
                break;
            }
        }
        if (k == 0) {
            buffer.clear();
            k = count_colors * 3 + 12;
        }
        file->read(reinterpret_cast<char *>(buffer.data()), file->count_colors * 3);

        for (auto &i: pixelsDop) {
            i = std::to_string((k - 13) / 3 + std::stoi(i));
        }

        seekg(k, std::ios::beg);
        std::vector<char> remainingData((std::istreambuf_iterator<char>(*this)), std::istreambuf_iterator<char>());
        seekp(k, std::ios::beg);
        for (int i = 0; i < file->count_colors * 3; ++i) {
            write(reinterpret_cast<const char *>(&buffer[i]), 1);
        }
        for (char byte: remainingData) {
            put(byte);
        }
        count_colors += file->count_colors;
        while (pow(2, lzw - 1) < count_colors) {
            lzw++;
        }
        c += file->count_colors * 3;
        seekp(10, std::ios::beg);
        read(reinterpret_cast<char *>(&k), 1);
        std::string f = dec_to_bin(k, 8);
        f.replace(1, 3, dec_to_bin(lzw - 2, 3));
        f.replace(5, 7, dec_to_bin(lzw - 2, 3));
        seekp(10, std::ios::beg);
        k = bin_to_dec(f);
        write(reinterpret_cast<char *>(&k), 1);
        seekp(c + 10, std::ios::beg);
        lzw--;
        write(reinterpret_cast<char *>(&lzw), 1);
        lzw++;
        k = 0;
        for (int i = 0; i < header.height; ++i) {
            for (int j = 0; j < header.width; ++j) {
                if (j >= x1 and j <= x2 and header.height - y2 - 1 <= i and header.height - y1 - 1 >= i) {
                    pixels[j + i * header.width] = pixelsDop[k];
                    k++;
                }
            }
        }
        pixWrite();
    }

};
