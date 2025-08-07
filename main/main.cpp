#include <iostream>
#include "BmpReader.h"

int main() { 
    setlocale(LC_ALL, "Russian");
	// Запрос имени входного BMP файла и проверка на пустоту
	std::string inputFile, outputFile; // Переменные для хранения имен входного и выходного файлов
	std::cout << ">> Enter input BMP file name: "; // Запрос имени входного файла
	std::cin >> inputFile; 
	if (inputFile.empty()) { // Проверка на пустоту имени входного файла
		std::cerr << "Input file name cannot be empty\n";
		return 1; 
	}

	BmpReader image; // Создание объекта для работы с BMP изображениями
	if (!image.load(inputFile)) { // Если изображение не удалось загрузить, будет выведено сообщение об ошибке
        std::cerr << "Failed to load image\n";
        return 1;
    }

    std::cout << "Original image:\n"; 
	image.display(); // Отображение исходного изображения

	image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1); // Рисование линии от верхнего левого до нижнего правого угла
	image.drawLine(0, image.getHeight() - 1, image.getWidth() - 1, 0); // Рисование линии от нижнего левого до верхнего правого угла

    std::cout << "\nImage with cross:\n";
	image.display(); // Отображение картинк с нарисованным крестом

    std::cout << ">> Enter output BMP file name: ";
    std::cin >> outputFile;
	if (outputFile.empty()) { // Проверка на пустоту имени выходного файла
        std::cerr << "Output file name cannot be empty\n";
        return 1;
	}
	if (!image.save(outputFile)) { // Если изображение не удалось сохранить, будет выведено сообщение об ошибке
        std::cerr << "Failed to save image\n";
        return 1;
    }

    return 0;
}
