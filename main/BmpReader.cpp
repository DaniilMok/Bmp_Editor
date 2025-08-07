#include "BmpReader.h"
#include <fstream>
#include <iostream>

bool BmpReader::load(const std::string& filename) { // Функция для загрузки BMP файла
	std::ifstream file(filename, std::ios::binary); // Открытие файла в бинарном режиме
	if (!file) { // Проверка на успешное открытие файла
        std::cerr << "Cannot open file: " << filename << '\n';
        return false;
    }

	BITMAPFILEHEADER fileHeader; // Структура для заголовка BMP файла
	BITMAPINFOHEADER infoHeader; // Структура для информации о BMP изображении

    // Чтение заголовков
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	// Сохранение заголовков в классе
    this->fileHeader = fileHeader; 
    this->infoHeader = infoHeader;

    // Проверка сигнатуры
	if (fileHeader.bfType != 0x4D42) { // Проверка на "BM" (0x4D42)
        std::cerr << "Not a BMP file.\n";
        return false;
    }

    // Проверка битности
	if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) { // Проверка на 24-битный и 32-битный BMP
        std::cerr << "Only 24-bit and 32-bit BMP are supported.\n";
        return false;
    }

	width = infoHeader.biWidth; // Ширина изображения
    height = abs(infoHeader.biHeight); // Не может быть отрицательной
	bitCount = infoHeader.biBitCount; // Количество бит на пиксель

	int rowSize = ((bitCount * width + 31) / 32) * 4; // Выравнивание строки до 4 байт
	int pixelSize = bitCount / 8; // Размер пикселя в байтах

	pixels.resize(height, std::vector<Pixel>(width)); // Инициализация двумерного вектора для хранения пикселей

	file.seekg(fileHeader.bfOffBits, std::ios::beg); // Позиция указателя на начало данных изображения

	for (int y = height - 1; y >= 0; --y) { // BMP хранит строки в обратном порядке
		for (int x = 0; x < width; ++x) { // Чтение пикселей
			uint8_t bgr[4] = { 0 }; // Массив для хранения BGR(A) значений
			file.read(reinterpret_cast<char*>(bgr), pixelSize); // Чтение пикселя из файла
			pixels[y][x] = { // Создание пикселя из BGR(A) значений
                bgr[2], // r
                bgr[1], // g
                bgr[0], // b
                (pixelSize == 4) ? bgr[3] : static_cast<uint8_t>(255) // a
            };
        }

		if (bitCount == 24) { // Если 24-битный BMP, нужно добавить выравнивание
            int padding = rowSize - (width * 3);
            file.ignore(padding);
        }
    }

	file.close(); // Закрытие файла
    return true;
}

void BmpReader::display() const { // Функция для отображения изображения в консоли
	for (int y = 0; y < height; ++y) { // Проходим по строкам изображения
		for (int x = 0; x < width; ++x) { // Проходим по столбцам изображения
			const Pixel& p = pixels[y][x]; // Получаем пиксель

			if (p.r == 0 && p.g == 0 && p.b == 0) { // Чёрный цвет
                std::cout << '#';
            }
			else if (p.r == 255 && p.g == 255 && p.b == 255) { // Белый цвет
                std::cout << ' ';
            }
            else {
                std::cout << '?'; // На случай, если в файле другие цвета
            }
        }
        std::cout << '\n';
    }
}

void BmpReader::drawLine(int x1, int y1, int x2, int y2) { // Функция для рисования линии на изображении
	Pixel lineColor = { 0, 0, 0, 255 }; // Чёрный цвет линии

	int dx = abs(x2 - x1); // Разница по x
	int dy = -abs(y2 - y1); // Разница по y (отрицательная, т.к. BMP хранит строки в обратном порядке)
	int sx = x1 < x2 ? 1 : -1; // Направление по x
	int sy = y1 < y2 ? 1 : -1; // Направление по y
	int err = dx + dy; // Начальное значение ошибки

    while (true) {
		if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) { // Проверка, что координаты в пределах изображения
			setPixel(x1, y1, lineColor); // Установка цвета пикселя
        }

		if (x1 == x2 && y1 == y2) // Если достигли конечной точки
            break;

		int e2 = 2 * err; // Удвоенная ошибка для сравнения
		if (e2 >= dy) { // Если ошибка больше или равна разнице по y
			err += dy; // Корректируем ошибку
			x1 += sx; // Переходим к следующему пикселю по x
        }
		if (e2 <= dx) { // Если ошибка меньше или равна разнице по x
			err += dx; // Корректируем ошибку
			y1 += sy; // Переходим к следующему пикселю по y
        }
    }
}

void BmpReader::setPixel(int x, int y, const Pixel& color) { // Функция для установки цвета пикселя
	if (x >= 0 && x < width && y >= 0 && y < height) { // Проверка, что координаты в пределах изображения
		pixels[y][x] = color; // Установка цвета пикселя
    }
}

bool BmpReader::save(const std::string& filename) { // Функция для сохранения изображения в BMP файл
	std::ofstream out(filename, std::ios::binary); // Открытие файла для записи в бинарном режиме
	if (!out) { // Проверка на успешное открытие файла
        std::cerr << "Cannot open file for writing: " << filename << '\n';
        return false;
    }

	out.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader)); // Запись заголовка BMP файла
	out.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader)); // Запись информации о BMP изображении

	int rowSize = ((bitCount * width + 31) / 32) * 4; // Выравнивание строки до 4 байт
	int pixelSize = bitCount / 8; // Размер пикселя в байтах
	std::vector<uint8_t> padding(rowSize - width * pixelSize, 0); // Выравнивание строки (если нужно)

	for (int y = height - 1; y >= 0; --y) { // BMP хранит строки в обратном порядке
		for (int x = 0; x < width; ++x) { // Проходим по пикселям
			const Pixel& p = pixels[y][x]; // Получаем пиксель

			uint8_t bgr[4] = { p.b, p.g, p.r, p.a }; // BGR(A) порядок
			out.write(reinterpret_cast<char*>(bgr), pixelSize); // Запись пикселя в файл
        }

		if (bitCount == 24) { // Если 24-битный BMP, нужно добавить выравнивание
			out.write(reinterpret_cast<char*>(padding.data()), padding.size()); // Запись выравнивания
        }
    }

	out.close(); // Закрытие файла
    return true;
}

