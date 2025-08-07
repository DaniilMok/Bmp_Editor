#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <cstdint>

struct Pixel { // ��������� ��� �������� ����� �������
    uint8_t r, g, b, a;
};

class BmpReader { // ����� ��� ������ � BMP �������������
public: 
	bool load(const std::string& filename); // ������� ��� �������� BMP �����
	void display() const; // ������� ��� ����������� ����������� � �������
	void drawLine(int x1, int y1, int x2, int y2); // ������� ��� ��������� ����� �� �����������
	bool save(const std::string& filename); // ������� ��� ���������� ����������� � BMP ����

	int getWidth() const { return width; } // ������� ��� ��������� ������ �����������
	int getHeight() const { return height; } // ������� ��� ��������� ������ �����������

private:
    int width = 0;
    int height = 0;
    int bitCount = 0;
    std::vector<std::vector<Pixel>> pixels;

	void setPixel(int x, int y, const Pixel& color); // ������� ��� ��������� ����� �������

	// ��������� ��� ���������� BMP �����
    BITMAPFILEHEADER fileHeader; 
    BITMAPINFOHEADER infoHeader;
};
