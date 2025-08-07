#include "BmpReader.h"
#include <fstream>
#include <iostream>

bool BmpReader::load(const std::string& filename) { // ������� ��� �������� BMP �����
	std::ifstream file(filename, std::ios::binary); // �������� ����� � �������� ������
	if (!file) { // �������� �� �������� �������� �����
        std::cerr << "Cannot open file: " << filename << '\n';
        return false;
    }

	BITMAPFILEHEADER fileHeader; // ��������� ��� ��������� BMP �����
	BITMAPINFOHEADER infoHeader; // ��������� ��� ���������� � BMP �����������

    // ������ ����������
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	// ���������� ���������� � ������
    this->fileHeader = fileHeader; 
    this->infoHeader = infoHeader;

    // �������� ���������
	if (fileHeader.bfType != 0x4D42) { // �������� �� "BM" (0x4D42)
        std::cerr << "Not a BMP file.\n";
        return false;
    }

    // �������� ��������
	if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) { // �������� �� 24-������ � 32-������ BMP
        std::cerr << "Only 24-bit and 32-bit BMP are supported.\n";
        return false;
    }

	width = infoHeader.biWidth; // ������ �����������
    height = abs(infoHeader.biHeight); // �� ����� ���� �������������
	bitCount = infoHeader.biBitCount; // ���������� ��� �� �������

	int rowSize = ((bitCount * width + 31) / 32) * 4; // ������������ ������ �� 4 ����
	int pixelSize = bitCount / 8; // ������ ������� � ������

	pixels.resize(height, std::vector<Pixel>(width)); // ������������� ���������� ������� ��� �������� ��������

	file.seekg(fileHeader.bfOffBits, std::ios::beg); // ������� ��������� �� ������ ������ �����������

	for (int y = height - 1; y >= 0; --y) { // BMP ������ ������ � �������� �������
		for (int x = 0; x < width; ++x) { // ������ ��������
			uint8_t bgr[4] = { 0 }; // ������ ��� �������� BGR(A) ��������
			file.read(reinterpret_cast<char*>(bgr), pixelSize); // ������ ������� �� �����
			pixels[y][x] = { // �������� ������� �� BGR(A) ��������
                bgr[2], // r
                bgr[1], // g
                bgr[0], // b
                (pixelSize == 4) ? bgr[3] : static_cast<uint8_t>(255) // a
            };
        }

		if (bitCount == 24) { // ���� 24-������ BMP, ����� �������� ������������
            int padding = rowSize - (width * 3);
            file.ignore(padding);
        }
    }

	file.close(); // �������� �����
    return true;
}

void BmpReader::display() const { // ������� ��� ����������� ����������� � �������
	for (int y = 0; y < height; ++y) { // �������� �� ������� �����������
		for (int x = 0; x < width; ++x) { // �������� �� �������� �����������
			const Pixel& p = pixels[y][x]; // �������� �������

			if (p.r == 0 && p.g == 0 && p.b == 0) { // ׸���� ����
                std::cout << '#';
            }
			else if (p.r == 255 && p.g == 255 && p.b == 255) { // ����� ����
                std::cout << ' ';
            }
            else {
                std::cout << '?'; // �� ������, ���� � ����� ������ �����
            }
        }
        std::cout << '\n';
    }
}

void BmpReader::drawLine(int x1, int y1, int x2, int y2) { // ������� ��� ��������� ����� �� �����������
	Pixel lineColor = { 0, 0, 0, 255 }; // ׸���� ���� �����

	int dx = abs(x2 - x1); // ������� �� x
	int dy = -abs(y2 - y1); // ������� �� y (�������������, �.�. BMP ������ ������ � �������� �������)
	int sx = x1 < x2 ? 1 : -1; // ����������� �� x
	int sy = y1 < y2 ? 1 : -1; // ����������� �� y
	int err = dx + dy; // ��������� �������� ������

    while (true) {
		if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) { // ��������, ��� ���������� � �������� �����������
			setPixel(x1, y1, lineColor); // ��������� ����� �������
        }

		if (x1 == x2 && y1 == y2) // ���� �������� �������� �����
            break;

		int e2 = 2 * err; // ��������� ������ ��� ���������
		if (e2 >= dy) { // ���� ������ ������ ��� ����� ������� �� y
			err += dy; // ������������ ������
			x1 += sx; // ��������� � ���������� ������� �� x
        }
		if (e2 <= dx) { // ���� ������ ������ ��� ����� ������� �� x
			err += dx; // ������������ ������
			y1 += sy; // ��������� � ���������� ������� �� y
        }
    }
}

void BmpReader::setPixel(int x, int y, const Pixel& color) { // ������� ��� ��������� ����� �������
	if (x >= 0 && x < width && y >= 0 && y < height) { // ��������, ��� ���������� � �������� �����������
		pixels[y][x] = color; // ��������� ����� �������
    }
}

bool BmpReader::save(const std::string& filename) { // ������� ��� ���������� ����������� � BMP ����
	std::ofstream out(filename, std::ios::binary); // �������� ����� ��� ������ � �������� ������
	if (!out) { // �������� �� �������� �������� �����
        std::cerr << "Cannot open file for writing: " << filename << '\n';
        return false;
    }

	out.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader)); // ������ ��������� BMP �����
	out.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader)); // ������ ���������� � BMP �����������

	int rowSize = ((bitCount * width + 31) / 32) * 4; // ������������ ������ �� 4 ����
	int pixelSize = bitCount / 8; // ������ ������� � ������
	std::vector<uint8_t> padding(rowSize - width * pixelSize, 0); // ������������ ������ (���� �����)

	for (int y = height - 1; y >= 0; --y) { // BMP ������ ������ � �������� �������
		for (int x = 0; x < width; ++x) { // �������� �� ��������
			const Pixel& p = pixels[y][x]; // �������� �������

			uint8_t bgr[4] = { p.b, p.g, p.r, p.a }; // BGR(A) �������
			out.write(reinterpret_cast<char*>(bgr), pixelSize); // ������ ������� � ����
        }

		if (bitCount == 24) { // ���� 24-������ BMP, ����� �������� ������������
			out.write(reinterpret_cast<char*>(padding.data()), padding.size()); // ������ ������������
        }
    }

	out.close(); // �������� �����
    return true;
}

