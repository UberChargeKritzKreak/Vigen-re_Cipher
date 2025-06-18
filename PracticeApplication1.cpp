#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <fstream>

// ��������� ������� ���� �������� ����� (� ������� ��������)
const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, 0.0159, 0.0454, 0.0170, 0.0017, 0.0298, 0.0004, 0.0094, 0.0174,
    0.0745, 0.0121, 0.0349, 0.0440, 0.0321, 0.0670, 0.1097, 0.0281, 0.0473,
    0.0547, 0.0626, 0.0262, 0.0026, 0.0097, 0.0048, 0.0144, 0.0073, 0.0036,
    0.0004, 0.0190, 0.0032, 0.0064, 0.0020, 0.0201  // "��������������������������������"
};

double calculate_ic(const std::string&);
std::string vigenere_cipher(const std::string&, const std::string&, bool);
std::string break_vigenere(const std::string&);
std::string ready(std::string file_name);
bool writey(const std::string filename, const std::string content);

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

// ������� ��� ���������� ������� ����������
double calculate_ic(const std::string& text) {
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "��������������������������������";
    std::vector<int> counts(alphabet.size(), 0);
    int total_letters = 0;

    for (char c : text) {
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            counts[pos]++;
            total_letters++;
        }
    }

    double ic = 0.0;
    for (int count : counts) {
        ic += count * (count - 1);
    }

    return ic / (total_letters * (total_letters - 1));
}

// ������� ��� ������ ����� ��������
std::string break_vigenere(const std::string& ciphertext)
{
    const std::string alphabet = "��������������������������������";
    const int max_key_length = 30;
    int best_key_length = 1;
    double best_ic = 0.0;

    // ��� 1: ����������� ����� �����
    for (int key_len = 1; key_len <= max_key_length; key_len++)
    {
        double sum_ic = 0.0;
        int sequences = 0;

        for (int i = 0; i < key_len; i++)
        {
            std::string sequence;
            for (int j = i; j < ciphertext.length(); j += key_len)
            {
                char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
                if (alphabet.find(c) != std::string::npos) 
                    sequence += c;
            }

            if (sequence.length() > 1)
            {
                double seq_ic = calculate_ic(sequence);
                sum_ic += seq_ic;
                sequences++;
            }
        }

        if (sequences > 0)
        {
            double avg_ic = sum_ic / sequences;
            if (avg_ic > best_ic)
            {
                best_ic = avg_ic;
                best_key_length = key_len;
            }
        }
    }

    // ��� 2: ����������� �������� �����
    std::string recovered_key;
    for (int i = 0; i < best_key_length; i++)
    {
        std::string sequence;
        for (int j = i; j < ciphertext.length(); j += best_key_length)
        {
            char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
            if (alphabet.find(c) != std::string::npos)
                sequence += c;
        }

        double best_chi_sq = 1e9;
        char best_char = '�';

        for (char c : alphabet)
        {
            // ������� ������������ ������������������ � ������� �������� �����
            std::string decrypted_seq;
            for (char ch : sequence)
            {
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted_seq += alphabet[new_pos];
            }

            // ��������� ������������� ������
            std::vector<int> freq_counts(alphabet.size(), 0);
            int total_chars = 0;

            for (char ch : decrypted_seq) 
            {
                size_t pos = alphabet.find(ch);
                if (pos != std::string::npos) 
                {
                    freq_counts[pos]++;
                    total_chars++;
                }
            }

            // ��������� ���������� ��-�������
            double chi_sq = 0.0;
            if (total_chars > 0)
            {
                for (size_t j = 0; j < alphabet.size(); j++)
                {
                    double expected = RUSSIAN_FREQUENCIES[j] * total_chars;
                    double observed = freq_counts[j];
                    double diff = observed - expected;
                    chi_sq += (diff * diff) / expected;
                }

                if (chi_sq < best_chi_sq)
                {
                    best_chi_sq = chi_sq;
                    best_char = c;
                }
            }
        }
        recovered_key += best_char;
    }

    return recovered_key;
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