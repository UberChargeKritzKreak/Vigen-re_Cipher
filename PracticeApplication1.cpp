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

    // �������
    if (argc == 2)
    {
        std::string argument = argv[1];

        if (argument == "-h" || argument == "help")
        {
            // ����� ������� �������
            std::cout << "����� ������� �������...\n";
            return 0;
        }
    }

    // ���������� ������������ ��� ������ ���������
    if (argc < 2)
    {
        std::cerr << "������: �������� ���������� ����������\n";
        // ����� ������� �������
        std::cout << "����� ������� �������...\n" << std::endl;
        return 1;
    }

    std::string mode = argv[1]; // ����� ������ ���������

    // ���� ������ ����� ���������� ��� �������������
    if (mode == "-sh" || mode == "-rsh")
    {
        if (argc != 6)
        {
            std::cerr << "������: �������� ���������� ����������\n";
            // ����� ������� �������
            std::cout << "����� ������� �������...\n" << std::endl;
            return 1;
        }

        std::string input_file_name = argv[2]; // ��� �����, � ������� ���������� ������������� �����
        std::string key_flag = argv[3]; // ���� �����
        std::string key_value = argv[4]; // �������� �����
        std::string output_file_name = argv[5]; // ��� �����, � ������� ���������� ������������� �����

        // ��������, ��� ����� ����� �������� ����� ������� ���� �����
        if (key_flag != "-k")
        {
            std::cerr << "������: �������� ���� -k ����� ������\n";
            // ����� ������� �������
            std::cout << "����� ������� �������...\n" << std::endl;
            return 1;
        }

        std::cout << "�����: " << (mode == "-sh" ? "����������" : "�������������") << std::endl;
        std::cout << "�������� �������� �����: " << input_file_name << std::endl;
        std::cout << "���� ����������: " << key_value << std::endl;
        std::cout << "�������� ��������� �����: " << output_file_name << std::endl;

        // ����� ������� ����������/�������������
    }
    // ������ ����� ������������
    else if (mode == "-dsh")
    {
        // ��������, ��� ���������� ����������
        if (argc != 4)
        {
            std::cerr << "������: �������� ���������� ����������\n";
            // ����� ������� �������
            std::cout << "����� ������� �������...\n" << std::endl;
            return 1;
        }

        std::string input_file_name = argv[2]; // ��� �����, � ������� ���������� ������������� �����
        std::string output_file_name = argv[3]; // ��� �����, � ������� ���������� ������������� �����

        std::cout << "�����: ������������" << std::endl;
        std::cout << "�������� �������� �����: " << input_file_name << std::endl;
        std::cout << "�������� ��������� �����: " << output_file_name << std::endl;

        // ����� ������� ������������
    }
    else
    {
        std::cerr << "������: ����������� ����� ������ ���������: " << mode << std::endl;
        // ����� ������� �������
        std::cout << "����� ������� �������...\n" << std::endl;
        return 1;
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