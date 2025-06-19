#include <iostream> // ����/�����
#include <string> // ������ �� �������
#include <vector> // ������������ �������
#include <cmath> // �������������� �������
#include <cctype> // ������� ������ � ���������
#include <fstream> // ��� ������ � �������

const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, 590.01, 0.0454, 0.0170, 0.0017, 0.0298, 0.0004, 0.0094, 0.0174,
    0.0745, 0.0121, 0.0349, 0.0440, 0.0321, 0.0670, 0.1097, 0.0281, 0.0473,
    0.0547, 0.0626, 0.0262, 0.0026, 0.0097, 0.0048, 0.0144, 0.0073, 0.0036,
    0.0004, 0.0190, 0.0032, 0.0064, 0.0020, 0.0201  // "��������������������������������"
};

// ��������� �������
double calculate_ic(const std::string&); // ���������� ������� ����������
std::string vigenere_cipher(const std::string&, const std::string&, bool); // ����������/���������� ��������
std::string break_vigenere(const std::string&); // ����� ����� ��������
std::string ready(std::string file_name); // ������ ����� � ������
bool writey(const std::string filename, const std::string content); // ������ ������ � ����
void show_help(); // ����� �������

int main(int argc, char* argv[])
{
    // ��������� ������� ������ ��� ���������� ������ � ����������
    setlocale(LC_ALL, "Russian");

    // ������������ ��������� ������� �� Windows-1251 (������ ��� Windows)
    // ��� ���������� ��� ����������� ����������� ������� �������� � �������
    system("chcp 1251");

    // ��������� ���������� ��������� ������
    if (argc == 2)
    {
        std::string argument = argv[1];
        // �������� �� ������ �������
        if (argument == "-h" || argument == "help")
        {
            show_help();
            return 0; // ���������� ���������� ���������
        }
    }

    // �������� ������������ ���������� ����������
    if (argc < 2)
    {
        std::cerr << "������: �������� ���������� ����������\n";
        show_help();
        return 1; // ���������� � �������
    }

    // �������� ����� ������ ���������
    std::string mode = argv[1];

    // ����� ���������� (-sh) ��� ������������� � ������ (-rsh)
    if (mode == "-sh" || mode == "-rsh")
    {
        // �������� ���������� ���������� ��� ������� � ������
        if (argc != 6)
        {
            std::cerr << "������: �������� ���������� ����������\n";
            show_help();
            return 1;
        }

        // ������� ����������
        std::string input_file_name = argv[2];   // ������� ����
        std::string key_flag = argv[3];          // ���� ����� (������ ���� "-k")
        std::string key_value = argv[4];         // �������� �����
        std::string output_file_name = argv[5];  // �������� ����

        // �������� ������������ ����� �����
        if (key_flag != "-k")
        {
            std::cerr << "������: �������� ���� -k ����� ������\n";
            show_help();
            return 1;
        }

        // ����� ����������
        if (mode == "-sh")
        {
            // ������ -> ���������� -> ������
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, true);
            writey(output_file_name, output_string);
            std::cout << "���������� ���������...\n";
        }
        // ����� ������������� � ������
        else if (mode == "-rsh")
        {
            // ������ -> ����������� -> ������
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, false);
            writey(output_file_name, output_string);
            std::cout << "������������� ���������...\n";
        }
    }
    // ����� ������������ ��� ����� (�����)
    else if (mode == "-dsh")
    {
        // �������� ���������� ���������� ��� ������ ������
        if (argc != 4)
        {
            std::cerr << "������: �������� ���������� ����������\n";
            show_help();
            return 1;
        }

        std::string input_file_name = argv[2];   // ������������� ����
        std::string output_file_name = argv[3];  // ���� ��� ��������������� ������

        // ������� ������:
        // 1. ����������� ����� � ������� �������������
        std::string possible_key = break_vigenere(ready(input_file_name));
        // 2. ����������� ������ ��������� ������
        std::string decrypted_string = vigenere_cipher(ready(input_file_name), possible_key, false);
        // 3. ������ ����������
        writey(output_file_name, decrypted_string);

        std::cout << "������������ ���������...\n";
    }
    // ����������� ����� ������
    else
    {
        std::cerr << "������: ����������� ����� ������ ���������: " << mode << std::endl;
        show_help();
        return 1;
    }

    return 0;
}

//������ ����� ����� � ������ (�������� �����)
std::string ready(const std::string file_name) {
    // �������� ����� � �������� ������
    std::ifstream fin(file_name, std::ios::binary);

    if (!fin.is_open()) {
        std::cerr << "������: �� ������� ������� ���� '" << file_name << "'\n";
        return "";
    }

    // ����������� ������� �����
    fin.seekg(0, std::ios::end); // seekg ������������� ������� ������ � �������� ������
    const auto size = fin.tellg(); // tellg ���������� ������� ������� ������ � �����
    if (size <= 0) {
        fin.close();
        return "";
    }

    // ������ ����� �����
    fin.seekg(0, std::ios::beg);
    std::string str(size, '\0');
    fin.read(&str[0], size);
    fin.close();

    return str;
}

// ������ ������ � ���� (�������� �����)
bool writey(const std::string filename, const std::string content) {
    // �������� ����� � �������� ������
    std::ofstream out_file(filename, std::ios::binary);
    if (!out_file) {
        std::cerr << "������: �� ������� ������� ���� " << filename << std::endl;
        return false;
    }

    // ������ �����������
    out_file.write(content.data(), content.size()); // data() ��������� �������� � ���������� ������ ��� � C-������� ��� ��������� �������
    if (!out_file) {
        std::cerr << "������ ������ � ���� " << filename << std::endl;
        return false;
    }

    out_file.close();
    std::cout << "���� ������� �������: " << filename << std::endl;
    return true;
}

// ����� ������� �� ������������� ���������
void show_help() {
    std::cout << "��������� ��� ����������/������������� ������\n\n";
    std::cout << "�������������:\n";
    std::cout << "  program.exe -sh input.txt -k key output.enc   # ����������\n";
    std::cout << "  program.exe -rsh input.enc -k key output.txt # ������������� � ������\n";
    std::cout << "  program.exe -dsh input.enc output.txt        # ������������ (�����)\n";
    std::cout << "  program.exe -h                               # �������\n";
    std::cout << "  program.exe help                             # �������\n\n";
    std::cout << "������ ������:\n";
    std::cout << "  -sh    ���������� ������\n";
    std::cout << "  -rsh   ������������� ������ � ��������� ������\n";
    std::cout << "  -dsh   ������������ ������ ��� ����� (�������������� ������ �����)\n";
}