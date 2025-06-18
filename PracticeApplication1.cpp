#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

std::string ready(std::string file_name);
bool writey(const std::string filename, const std::string content);

std::string ready(const std::string file_name) {
    std::ifstream fin(file_name);

    if (!fin.is_open()) {
        std::cerr << "������: �� ������� ������� ���� '" << file_name << "'" << std::endl;
        return ""; // ���������� ������ ������ ��� ������
    }

    std::string str;
    if (std::getline(fin, str)) { // ������ ������ ������ � ������� std::getline
        fin.close();
        return str;
    }
    else {
        fin.close();
        std::cerr << "������: ���� ���� ��� ������ ������" << std::endl;
        return "";
    }
}

bool writey(const std::string filename, const std::string content) {
    // �������� ������ ���� � ����� (������� ���������� + ��� �����)
    std::string full_path = filename;  // � ������� ����������

    // ������� � ���������� ����
    std::ofstream out_file(full_path);
    if (!out_file) {
        std::cerr << "������: �� ������� ������� ���� " << full_path << std::endl;
        return false;
    }

    out_file << content;

    if (!out_file) {
        std::cerr << "������ ������ � ���� " << full_path << std::endl;
        return false;
    }

    out_file.close();

    // ������� ������ ���� � ������������ �����
    std::cout << "���� ������� �������: ";
    std::cout << full_path << std::endl;

    return true;
}