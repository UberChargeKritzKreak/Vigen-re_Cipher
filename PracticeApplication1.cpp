#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <fstream>

// Эталонные частоты букв русского языка (в порядке алфавита)
const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, 0.0159, 0.0454, 0.0170, 0.0017, 0.0298, 0.0004, 0.0094, 0.0174,
    0.0745, 0.0121, 0.0349, 0.0440, 0.0321, 0.0670, 0.1097, 0.0281, 0.0473,
    0.0547, 0.0626, 0.0262, 0.0026, 0.0097, 0.0048, 0.0144, 0.0073, 0.0036,
    0.0004, 0.0190, 0.0032, 0.0064, 0.0020, 0.0201  // "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
};

double calculate_ic(const std::string&);
std::string vigenere_cipher(const std::string&, const std::string&, bool);
std::string break_vigenere(const std::string&);
std::string ready(std::string file_name);
bool writey(const std::string filename, const std::string content);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian"); // Подключение русской локали
    
    // Переключение кодировки в консоли на Windows-1251
    system("chcp 1251");
    system("cls");

    // Справка
    if (argc == 2)
    {
        std::string argument = argv[1];

        if (argument == "-h" || argument == "help")
        {
            // Вызов функции справки
            std::cout << "Вызов функции справки...\n";
            return 0;
        }
    }

    // Аргументов недостаточно для работы программы
    if (argc < 2)
    {
        std::cerr << "Ошибка: Неверное количество аргументов\n";
        // Вызов функции справки
        std::cout << "Вызов функции справки...\n" << std::endl;
        return 1;
    }

    std::string mode = argv[1]; // Режим работы программы

    // Если выбран режим шифрования или расшифрования
    if (mode == "-sh" || mode == "-rsh")
    {
        if (argc != 6)
        {
            std::cerr << "Ошибка: Неверное количество аргументов\n";
            // Вызов функции справки
            std::cout << "Вызов функции справки...\n" << std::endl;
            return 1;
        }

        std::string input_file_name = argv[2]; // Имя файла, в котором содержится зашифрованный текст
        std::string key_flag = argv[3]; // Флаг ключа
        std::string key_value = argv[4]; // Значение ключа
        std::string output_file_name = argv[5]; // Имя файла, в который поместится дешифрованный текст

        // Проверка, что после имени входного файла следует флаг ключа
        if (key_flag != "-k")
        {
            std::cerr << "Ошибка: Ожидался флаг -k перед ключом\n";
            // Вызов функции справки
            std::cout << "Вызов функции справки...\n" << std::endl;
            return 1;
        }

        std::cout << "Режим: " << (mode == "-sh" ? "Шифрование" : "Расшифрование") << std::endl;
        std::cout << "Название входного файла: " << input_file_name << std::endl;
        std::cout << "Ключ шифрования: " << key_value << std::endl;
        std::cout << "Название выходного файла: " << output_file_name << std::endl;

        // Вызов функций шифрования/расшифрования
    }
    // Выбран режим дешифрования
    else if (mode == "-dsh")
    {
        // Проверка, что достаточно аргументов
        if (argc != 4)
        {
            std::cerr << "Ошибка: Неверное количество аргументов\n";
            // Вызов функции справки
            std::cout << "Вызов функции справки...\n" << std::endl;
            return 1;
        }

        std::string input_file_name = argv[2]; // Имя файла, в котором содержится зашифрованный текст
        std::string output_file_name = argv[3]; // Имя файла, в который поместится дешифрованный текст

        std::cout << "Режим: Дешифрование" << std::endl;
        std::cout << "Название входного файла: " << input_file_name << std::endl;
        std::cout << "Название выходного файла: " << output_file_name << std::endl;

        // Вызов функции дешифрования
    }
    else
    {
        std::cerr << "Ошибка: Неизвестный режим работы программы: " << mode << std::endl;
        // Вызов функции справки
        std::cout << "Вызов функции справки...\n" << std::endl;
        return 1;
    }
}

// Функция шифрования/расшифрования текста
std::string vigenere_cipher(const std::string& text, const std::string& key, bool encrypt = true)
{
    // Русский алфавит (33 буквы)
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int n = alphabet.size();

    // Подготовка ключа (только строчные буквы алфавита)
    std::string clean_key;
    for (char c : key) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        if (alphabet.find(lower_c) != std::string::npos) {
            clean_key += lower_c;
        }
    }
    if (clean_key.empty()) return text;

    // Обработка текста
    std::string result;
    size_t key_index = 0;

    for (char c : text) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        size_t text_pos = alphabet.find(lower_c);
        
        if (text_pos == std::string::npos) {
            result += lower_c;  // Не-буквы сохраняем как есть
            continue;
        }

        // Получаем символ ключа
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);
        key_index++;

        // Вычисляем новую позицию
        size_t new_pos;
        if (encrypt) {
            new_pos = (text_pos + key_pos) % n;  // Шифрование
        }
        else {
            new_pos = (text_pos - key_pos + n) % n;  // Расшифровка
        }

        result += alphabet[new_pos];
    }

    return result;
 }

// Функция для вычисления индекса совпадений
double calculate_ic(const std::string& text) {
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
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

// Функция для взлома шифра Виженера
std::string break_vigenere(const std::string& ciphertext)
{
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int max_key_length = 30;
    int best_key_length = 1;
    double best_ic = 0.0;

    // Шаг 1: Определение длины ключа
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

    // Шаг 2: Определение символов ключа
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
        char best_char = 'а';

        for (char c : alphabet)
        {
            // Пробуем расшифровать последовательность с текущим символом ключа
            std::string decrypted_seq;
            for (char ch : sequence)
            {
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted_seq += alphabet[new_pos];
            }

            // Вычисляем распределение частот
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

            // Вычисляем статистику хи-квадрат
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
        std::cerr << "Ошибка: не удалось открыть файл '" << file_name << "'" << std::endl;
        return ""; // Возвращаем пустую строку при ошибке
    }

    std::string str;
    if (std::getline(fin, str)) { // Чтение первой строки с помощью std::getline
        fin.close();
        return str;
    }
    else {
        fin.close();
        std::cerr << "Ошибка: файл пуст или ошибка чтения" << std::endl;
        return "";
    }
}

bool writey(const std::string filename, const std::string content) {
    // Получаем полный путь к файлу (текущая директория + имя файла)
    std::string full_path = filename;  // В текущей директории

    // Создаем и записываем файл
    std::ofstream out_file(full_path);
    if (!out_file) {
        std::cerr << "Ошибка: не удалось создать файл " << full_path << std::endl;
        return false;
    }

    out_file << content;

    if (!out_file) {
        std::cerr << "Ошибка записи в файл " << full_path << std::endl;
        return false;
    }

    out_file.close();

    // Выводим полный путь к сохраненному файлу
    std::cout << "Файл успешно записан: ";
    std::cout << full_path << std::endl;

    return true;
}