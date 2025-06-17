#include <iostream>
#include <string>
#include <cctype>

std::string vigenere_cipher(const std::string&, const std::string&, bool);

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
}

// ������� ����������/������������� ������
std::string vigenere_cipher(const std::string& text, const std::string& key, bool encrypt = true)
{
    // ������� ������� (33 �����)
    const std::string alphabet = "��������������������������������";
    const int n = alphabet.size();

    // ���������� ����� (������ �������� ����� ��������)
    std::string clean_key;
    for (char c : key) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        if (alphabet.find(lower_c) != std::string::npos) {
            clean_key += lower_c;
        }
    }
    if (clean_key.empty()) return text;

    // ��������� ������
    std::string result;
    size_t key_index = 0;

    for (char c : text) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        size_t text_pos = alphabet.find(lower_c);

        if (text_pos == std::string::npos) {
            result += lower_c;  // ��-����� ��������� ��� ����
            continue;
        }

        // �������� ������ �����
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);
        key_index++;

        // ��������� ����� �������
        size_t new_pos;
        if (encrypt) {
            new_pos = (text_pos + key_pos) % n;  // ����������
        }
        else {
            new_pos = (text_pos - key_pos + n) % n;  // �����������
        }

        result += alphabet[new_pos];
    }

    return result;
}