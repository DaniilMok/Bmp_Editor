#include <iostream>
#include "BmpReader.h"

int main() { 
    setlocale(LC_ALL, "Russian");
	// ������ ����� �������� BMP ����� � �������� �� �������
	std::string inputFile, outputFile; // ���������� ��� �������� ���� �������� � ��������� ������
	std::cout << ">> Enter input BMP file name: "; // ������ ����� �������� �����
	std::cin >> inputFile; 
	if (inputFile.empty()) { // �������� �� ������� ����� �������� �����
		std::cerr << "Input file name cannot be empty\n";
		return 1; 
	}

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
	if (outputFile.empty()) { // �������� �� ������� ����� ��������� �����
        std::cerr << "Output file name cannot be empty\n";
        return 1;
	}
	if (!image.save(outputFile)) { // ���� ����������� �� ������� ���������, ����� �������� ��������� �� ������
        std::cerr << "Failed to save image\n";
        return 1;
    }

    return 0;
}
