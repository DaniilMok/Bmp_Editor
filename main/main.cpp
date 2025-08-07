#include <iostream>
#include "BmpReader.h"

void checkForEmptyInput(const std::string& file) {
	if (file.empty()) { // Проверка на пустоту имени файла
		std::cerr << "File name cannot be empty\n";
		exit(1);
	}
}

int main() { 
    setlocale(LC_ALL, "Russian");
	// Запрос имени входного BMP файла и проверка на пустоту
	std::string inputFile, outputFile; // Переменные для хранения имен входного и выходного файлов
	std::cout << ">> Enter input BMP file name: "; // Запрос имени входного файла
	std::cin >> inputFile;
	checkForEmptyInput(inputFile); // Проверка на пустоту ввода

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
	checkForEmptyInput(outputFile); // Проверка на пустоту ввода
	// Сохранение измененного изображения в новый BMP файл
	if (!image.save(outputFile)) { // Если изображение не удалось сохранить, будет выведено сообщение об ошибке
        std::cerr << "Failed to save image\n";
        return 1;
    }

    return 0;
}
