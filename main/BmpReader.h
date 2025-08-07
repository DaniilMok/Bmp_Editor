#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <cstdint>

struct Pixel { // Структура для хранения цвета пикселя
    uint8_t r, g, b, a;
};

class BmpReader { // Класс для работы с BMP изображениями
public: 
	bool load(const std::string& filename); // Функция для загрузки BMP файла
	void display() const; // Функция для отображения изображения в консоли
	void drawLine(int x1, int y1, int x2, int y2); // Функция для рисования линии на изображении
	bool save(const std::string& filename); // Функция для сохранения изображения в BMP файл

	int getWidth() const { return width; } // Функция для получения ширины изображения
	int getHeight() const { return height; } // Функция для получения высоты изображения

private:
    int width = 0;
    int height = 0;
    int bitCount = 0;
    std::vector<std::vector<Pixel>> pixels;

	void setPixel(int x, int y, const Pixel& color); // Функция для установки цвета пикселя

	// Структуры для заголовков BMP файла
    BITMAPFILEHEADER fileHeader; 
    BITMAPINFOHEADER infoHeader;
};
