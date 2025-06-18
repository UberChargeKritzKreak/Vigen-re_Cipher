#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

std::string vigenere_encrypt(const std::string&, const std::string&);
std::string ready(std::string file_name);
 
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian"); // ����������� ������� ������
    
    // ������������ ��������� � ������� �� Windows-1251
    system("chcp 1251");
    system("cls");

    // ��������, ��� ���������� ���������� ��� ����������/�������������
    if (argc == 5)
    {
        // ����������� ����������
        std::string input_file_name = argv[1]; // ��� �����, � ������� ���������� ����� ��� ����������/�������������
        std::string output_file_name = argv[2]; // ��� �����, � ������� ���������� �������������/�������������� �����
        std::string key = argv[3]; // ���� ��� ����������/�������������
        std::string mode = argv[4]; // ����� ������ ���������

        // ����� ����������
        // en - ����������
        // de - ������������
        if (!mode.compare("en"))
        {
            // ����� ������� ����������
            std::cout << std::endl << "����� ������� ����������..." << std::endl;
        }
        // ����� �������������
        else if (!mode.compare("de"))
        {
            // ����� ������� �������������
            std::cout << std::endl << "����� ������� �������������..." << std::endl;
        }
        else
        {
            // ����� ������� �������
            std::cout << std::endl << "������ ����������� ����� ������ (en/de)..." << std::endl;
        }
    }
    // ��������, ��� ���������� ���������� ��� ����������
    else if (argc == 3)
    {
        // ����������� ����������
        std::string input_file_name = argv[1]; // ��� �����, � ������� ���������� ������������� �����
        std::string output_file_name = argv[2]; // ��� �����, � ������� ���������� ������������� �����

        // ����� ������� ������������
        std::cout << std::endl << "����� ������� ������������..." << std::endl;
    }
    // ���������� ������������ ��� ��������� ��� ������ ���������
    else
    {
        // ����� ������� �������
        std::cout << std::endl << "����� ������� �������..." << std::endl;
    }

    std::cout << ready("C:\\3\\new");
}

std::string vigenere_encrypt(const std::string& text, const std::string& key) {
    // ������� ������� (33 �����)
    const std::string alphabet = "��������������������������������";
    const int n = alphabet.size();

    // �������� � ���������� �����
    if (key.empty()) return text;

    std::string clean_key;
    for (char c : key) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalpha(uc)) {
            clean_key += std::tolower(uc);
        }
    }
    if (clean_key.empty()) return text;

    // ���������� ������
    std::string result;
    size_t key_index = 0;

    for (char c : text) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalpha(uc)) {
            result += c;
            continue;
        }

        char base_char = std::tolower(uc);

        // ����� ������� ������� � ��������
        size_t text_pos = alphabet.find(base_char);
        if (text_pos == std::string::npos) {
            result += c;
            continue;
        }

        // ��������� ������� �����
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);

        // ����������: (�������_������ + �������_�����) mod 33
        size_t new_pos = (text_pos + key_pos) % n;
        char new_char = alphabet[new_pos];

        result += new_char;
        
    }
    return result;
}

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