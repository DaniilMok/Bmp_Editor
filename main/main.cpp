#include <iostream>
#include "BmpReader.h"

void checkForEmptyInput(const std::string& file) {
	if (file.empty()) { // �������� �� ������� ����� �����
		std::cerr << "File name cannot be empty\n";
		exit(1);
	}
}

int main() { 
    setlocale(LC_ALL, "Russian");
	// ������ ����� �������� BMP ����� � �������� �� �������
	std::string inputFile, outputFile; // ���������� ��� �������� ���� �������� � ��������� ������
	std::cout << ">> Enter input BMP file name: "; // ������ ����� �������� �����
	std::cin >> inputFile;
	checkForEmptyInput(inputFile); // �������� �� ������� �����

	BmpReader image; // �������� ������� ��� ������ � BMP �������������
	if (!image.load(inputFile)) { // ���� ����������� �� ������� ���������, ����� �������� ��������� �� ������
        std::cerr << "Failed to load image\n";
        return 1;
    }

    std::cout << "Original image:\n"; 
	image.display(); // ����������� ��������� �����������

	image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1); // ��������� ����� �� �������� ������ �� ������� ������� ����
	image.drawLine(0, image.getHeight() - 1, image.getWidth() - 1, 0); // ��������� ����� �� ������� ������ �� �������� ������� ����

    std::cout << "\nImage with cross:\n";
	image.display(); // ����������� ������� � ������������ �������

    std::cout << ">> Enter output BMP file name: ";
    std::cin >> outputFile;
	checkForEmptyInput(outputFile); // �������� �� ������� �����
	// ���������� ����������� ����������� � ����� BMP ����
	if (!image.save(outputFile)) { // ���� ����������� �� ������� ���������, ����� �������� ��������� �� ������
        std::cerr << "Failed to save image\n";
        return 1;
    }

    return 0;
}
