#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <fstream>

/*
 * ������� ������ ���� �������� �������� � ������� "��������������������������������"
 * �������� ������������ ����������� ������������� ���� � ������� �������
 * ����� ���� ������������ ������ ���� �������������� ����� 1.0
 */
const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, // �
    0.0159, // �
    0.0454, // �
    0.0170, // �
    0.0017, // �
    0.0298, // �
    0.0004, // �
    0.0094, // �
    0.0174, // �
    0.0745, // �
    0.0121, // �
    0.0349, // �
    0.0440, // �
    0.0321, // �
    0.0670, // �
    0.1097, // �
    0.0281, // �
    0.0473, // �
    0.0547, // �
    0.0626, // �
    0.0262, // �
    0.0026, // �
    0.0097, // �
    0.0048, // �
    0.0144, // �
    0.0073, // �
    0.0036, // �
    0.0004, // �
    0.0190, // �
    0.0032, // �
    0.0064, // �
    0.0020, // �
    0.0201  // �
};

double calculate_ic(const std::string&);
std::string vigenere_cipher(const std::string&, const std::string&, bool);
std::string break_vigenere(const std::string&);
std::string ready(std::string file_name);
bool writey(const std::string filename, const std::string content);
void show_help();

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian"); // ����������� ������� ������
    
    // ������������ ��������� � ������� �� Windows-1251
    system("chcp 1251");

    // �������
    if (argc == 2)
    {
        std::string argument = argv[1];

        if (argument == "-h" || argument == "help")
        {
            // ����� ������� �������
            show_help();
            return 0;
        }
    }

    // ���������� ������������ ��� ������ ���������
    if (argc < 2)
    {
        std::cerr << "������: �������� ���������� ����������\n";
        // ����� ������� �������
        show_help();
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
            show_help();
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
            show_help();
            return 1;
        }

        // ����� ������� ����������/�������������
        if (mode == "-sh")
        {
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, true);

            if (writey(output_file_name, output_string) == true) std::cout << "���������� ���������...\n";

            //std::cout << "���������� ���������...\n";
        }
        else if (mode == "-rsh")
        {
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, false);

            if (writey(output_file_name, output_string) == true) std::cout << "������������� ���������...\n";
        }
    }
    // ������ ����� ������������
    else if (mode == "-dsh")
    {
        // ��������, ��� ���������� ����������
        if (argc != 4)
        {
            std::cerr << "������: �������� ���������� ����������\n";
            // ����� ������� �������
            show_help();
            return 1;
        }

        std::string input_file_name = argv[2]; // ��� �����, � ������� ���������� ������������� �����
        std::string output_file_name = argv[3]; // ��� �����, � ������� ���������� ������������� �����

        // ����� ������� ������������

        std::string possible_key = break_vigenere(ready(input_file_name));

        std::string decrypted_string = vigenere_cipher(ready(input_file_name), possible_key, false);

        if (writey(output_file_name, decrypted_string) == true) std::cout << "������������ ���������...\n";
    }
    else
    {
        std::cerr << "������: ����������� ����� ������ ���������: " << mode << std::endl;
        // ����� ������� �������
        show_help();
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

/*
 * ������� ��� ���������� ������� ���������� (Index of Coincidence)
 * IC ������������ ��� ����������� ��������� ����� �����
 *
 * @param text - ������� ������ ��� �������
 * @return �������� ������� ���������� (�� 0.0 �� 1.0)
 */
double calculate_ic(const std::string& text) {
    // ���� ����� ������� ��������, ���������� 0
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "��������������������������������";
    std::vector<int> counts(alphabet.size(), 0); // ������ ��� �������� ����
    int total_letters = 0; // ����� ���������� ���� � ������

    // ������������ ���������� ������ ����� � ������
    for (char c : text) {
        // ������� ������� ����� � ��������
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            counts[pos]++;       // ����������� ������� ��� ���� �����
            total_letters++;     // ����������� ����� ������� ����
        }
    }

    // ��������� ������ ���������� �� �������:
    // IC = ?(n_i*(n_i-1)) / (N*(N-1))
    // ��� n_i - ���������� i-�� �����, N - ����� ���������� ����
    double ic = 0.0;
    for (int count : counts) {
        ic += count * (count - 1);
    }

    // ���������� ��������������� ��������
    return ic / (total_letters * (total_letters - 1));
}

/*
 * �������� ������� ��� ������ ����� ��������
 *
 * @param ciphertext - ������������� �����
 * @return ��������� ���� (� ������ ��������)
 */
std::string break_vigenere(const std::string& ciphertext) {
    const std::string alphabet = "��������������������������������";
    const int max_key_length = 30; // ������������ �������������� ����� �����

    /*****************************************************************
     * ���� 1: ����������� ����� ����� � ������� ������ �������
     * ����������� ������ ���������� ��� ������ ���� �����
     * �������� ��������� ����� ����� ���� ���������� ������� IC
     *****************************************************************/
    int best_key_length = 1;    // ��������� ������������� � ����� �����
    double best_ic = 0.0;       // ������ �������� IC

    // ���������� ��������� ����� ����� �� 1 �� max_key_length
    for (int key_len = 1; key_len <= max_key_length; key_len++) {
        double sum_ic = 0.0;    // ����� IC ��� ���� ����������������������
        int valid_sequences = 0; // ���������� �������� ����������������������

        // ��� ������ ������� � �����
        for (int i = 0; i < key_len; i++) {
            std::string sequence; // ������������������ �������� ��� i-� ������� �����

            // �������� �������, ������������� ����� �������� �����
            for (int j = i; j < ciphertext.length(); j += key_len) {
                // �������� ������ � ������� ��������
                char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
                // ���� ������ ���� � ��������, ��������� ��� � ������������������
                if (alphabet.find(c) != std::string::npos) {
                    sequence += c;
                }
            }

            // ��������� IC ������ ��� ������������������� ������ > 1
            if (sequence.length() > 1) {
                sum_ic += calculate_ic(sequence);
                valid_sequences++;
            }
        }

        // ���� ����� �������� ������������������, ��������� ������� IC
        if (valid_sequences > 0) {
            double avg_ic = sum_ic / valid_sequences;

            // �������� ����� ����� � ������������ ������� IC
            if (avg_ic > best_ic) {
                best_ic = avg_ic;
                best_key_length = key_len;
            }
        }
    }

    /*****************************************************************
     * ���� 2: �������������� ����� ����������� � ������� ���������� �������
     * ��� ������ ������� ����� ��������� ������, ������ �������� "������������"
     * ������������� ���� � �������������� ������
     *****************************************************************/
    std::string recovered_key; // ������ ��� ���������������� �����

    // ��� ������� ������� � �����
    for (int i = 0; i < best_key_length; i++) {
        std::string sequence; // ������������������ ��� �������� ������� �����

        // �������� ��� �������, ������������� ������� �������� �����
        for (int j = i; j < ciphertext.length(); j += best_key_length) {
            char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
            if (alphabet.find(c) != std::string::npos) {
                sequence += c;
            }
        }

        // ���� �� ����� �� ������ ������� ��� ���� ������� �����
        if (sequence.empty()) {
            recovered_key += '�'; // ���������� �������� �� ���������
            continue;
        }

        double best_chi_sq = std::numeric_limits<double>::max(); // ����������� ��-�������
        char best_char = '�'; // ��������� ������ �����

        // ���������� ��� ��������� ������� �������� ��� ��������� �� ������ �����
        for (char c : alphabet) {
            std::string decrypted; // �������������� ������������������

            // ������� ������������ ������������������ ������� ����������
            for (char ch : sequence) {
                // ������� ������� �������� � ��������
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                // ��������� ������� � �������� ������: (pos - key_pos) mod alphabet.size()
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted += alphabet[new_pos];
            }

            // ������������ ������� �������� � �������������� ������
            std::vector<int> counts(alphabet.size(), 0);
            int total = 0; // ����� ���������� ��������
            for (char ch : decrypted) {
                size_t pos = alphabet.find(ch);
                if (pos != std::string::npos) {
                    counts[pos]++;
                    total++;
                }
            }

            // ����������, ���� �� ����� �� ������ �������
            if (total == 0) continue;

            // ��������� ���������� ��-������� ��� ��������� � ���������� ���������
            double chi_sq = 0.0;
            for (size_t j = 0; j < alphabet.size(); j++) {
                double expected = RUSSIAN_FREQUENCIES[j] * total; // ��������� ����������
                double observed = counts[j];                      // ����������� ����������
                if (expected > 0) { // �������� ������� �� ����
                    chi_sq += (observed - expected) * (observed - expected) / expected;
                }
            }

            // �������� ������ � ���������� ��-������� (�������� ������� �������������)
            if (chi_sq < best_chi_sq) {
                best_chi_sq = chi_sq;
                best_char = c;
            }
        }

        // ��������� ��������� ������ � ����
        recovered_key += best_char;
    }

    // ���������� ��������������� ����
    return recovered_key;
}

std::string ready(const std::string file_name) {
    // ��������� ���� � �������� ������ ��� ������� ���������� ��������
    std::ifstream fin(file_name, std::ios::binary);

    if (!fin.is_open()) {
        std::cerr << "������: �� ������� ������� ���� '" << file_name << "'\n";
        return "";
    }

    // ���������� ��������� � ����� �����
    fin.seekg(0, std::ios::end);
    // ��������� ������ �����
    const auto size = fin.tellg();

    if (size <= 0) { // ���� ���� ��� ������
        fin.close();
        return ""; // ���������� ������ ������
    }

    // ���������� ��������� � ������
    fin.seekg(0, std::ios::beg);

    // ������� ������ ������� �������
    std::string str(size, '\0');
    // ������ ���� ���� � ������
    fin.read(&str[0], size);
    fin.close();

    return str;
}

bool writey(const std::string filename, const std::string content) {
    if (content.empty()) {
        std::cerr << "������: ������� �������� ������ ������" << std::endl;
        return false;
    }

    // ��������� ����������� �������� ����� ��� ������������ ��������
    std::ofstream test(filename);
    if (!test) {
        std::cerr << "������: ���������� ������� ���� " << filename << std::endl;
        return false;
    }
    test.close();

    // �������� ������
    std::ofstream out_file(filename, std::ios::binary);
    out_file.write(content.data(), content.size());

    if (!out_file) {
        std::cerr << "������ ������ � ���� " << filename << std::endl;
        return false;
    }

    std::cout << "���� ������� �������: " << filename << std::endl;
    return true;
}

void show_help() {
    std::cout << "��������� ��� ����������/������������� ������\n\n";
    std::cout << "�������������:\n";
    std::cout << "  program.exe -sh input.txt -k key output.enc\n";
    std::cout << "  program.exe -rsh input.enc -k key output.txt\n";
    std::cout << "  program.exe -dsh input.enc output.txt\n";
    std::cout << "  program.exe -h\n";
    std::cout << "  program.exe help\n\n";
    std::cout << "������ ������:\n";
    std::cout << "  -sh    ����������\n";
    std::cout << "  -rsh   �������������\n";
    std::cout << "  -dsh   ������������ (��� �����)\n";
}
