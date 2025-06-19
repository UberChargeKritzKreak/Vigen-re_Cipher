#include <iostream> // Ввод/вывод
#include <string> // Работа со стоками
#include <vector> // Динамические массивы
#include <cmath> // Математические функции
#include <cctype> // Функции работы с символами
#include <fstream> // Для работы с файлами

const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, 590.01, 0.0454, 0.0170, 0.0017, 0.0298, 0.0004, 0.0094, 0.0174,
    0.0745, 0.0121, 0.0349, 0.0440, 0.0321, 0.0670, 0.1097, 0.0281, 0.0473,
    0.0547, 0.0626, 0.0262, 0.0026, 0.0097, 0.0048, 0.0144, 0.0073, 0.0036,
    0.0004, 0.0190, 0.0032, 0.0064, 0.0020, 0.0201  // "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
};

// Прототипы функций
double calculate_ic(const std::string&); // Вычисление индекса совпадений
std::string vigenere_cipher(const std::string&, const std::string&, bool); // Шифрование/дешифровка Виженера
std::string break_vigenere(const std::string&); // Взлом шифра Виженера
std::string ready(std::string file_name); // Чтение файла в строку
bool writey(const std::string filename, const std::string content); // Запись строки в файл
void show_help(); // Вывод справки

int main(int argc, char* argv[])
{
    // Установка русской локали для корректной работы с кириллицей
    setlocale(LC_ALL, "Russian");

    // Переключение кодировки консоли на Windows-1251 (только для Windows)
    // Это необходимо для правильного отображения русских символов в консоли
    system("chcp 1251");

    // Обработка аргументов командной строки
    if (argc == 2)
    {
        std::string argument = argv[1];
        // Проверка на запрос справки
        if (argument == "-h" || argument == "help")
        {
            show_help();
            return 0; // Корректное завершение программы
        }
    }

    // Проверка минимального количества аргументов
    if (argc < 2)
    {
        std::cerr << "Ошибка: Неверное количество аргументов\n";
        show_help();
        return 1; // Завершение с ошибкой
    }

    // Основной режим работы программы
    std::string mode = argv[1];

    // Режим шифрования (-sh) или расшифрования с ключом (-rsh)
    if (mode == "-sh" || mode == "-rsh")
    {
        // Проверка количества аргументов для режимов с ключом
        if (argc != 6)
        {
            std::cerr << "Ошибка: Неверное количество аргументов\n";
            show_help();
            return 1;
        }

        // Парсинг аргументов
        std::string input_file_name = argv[2];   // Входной файл
        std::string key_flag = argv[3];          // Флаг ключа (должен быть "-k")
        std::string key_value = argv[4];         // Значение ключа
        std::string output_file_name = argv[5];  // Выходной файл

        // Проверка корректности флага ключа
        if (key_flag != "-k")
        {
            std::cerr << "Ошибка: Ожидался флаг -k перед ключом\n";
            show_help();
            return 1;
        }

        // Режим шифрования
        if (mode == "-sh")
        {
            // Чтение -> Шифрование -> Запись
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, true);
            writey(output_file_name, output_string);
            std::cout << "Шифрование завершено...\n";
        }
        // Режим расшифрования с ключом
        else if (mode == "-rsh")
        {
            // Чтение -> Расшифровка -> Запись
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, false);
            writey(output_file_name, output_string);
            std::cout << "Расшифрование завершено...\n";
        }
    }
    // Режим дешифрования без ключа (взлом)
    else if (mode == "-dsh")
    {
        // Проверка количества аргументов для режима взлома
        if (argc != 4)
        {
            std::cerr << "Ошибка: Неверное количество аргументов\n";
            show_help();
            return 1;
        }

        std::string input_file_name = argv[2];   // Зашифрованный файл
        std::string output_file_name = argv[3];  // Файл для расшифрованного текста

        // Процесс взлома:
        // 1. Определение ключа с помощью криптоанализа
        std::string possible_key = break_vigenere(ready(input_file_name));
        // 2. Расшифровка текста найденным ключом
        std::string decrypted_string = vigenere_cipher(ready(input_file_name), possible_key, false);
        // 3. Запись результата
        writey(output_file_name, decrypted_string);

        std::cout << "Дешифрование завершено...\n";
    }
    // Неизвестный режим работы
    else
    {
        std::cerr << "Ошибка: Неизвестный режим работы программы: " << mode << std::endl;
        show_help();
        return 1;
    }

    return 0;
}

/*std::string vigenere_cipher(const std::string& text, const std::string& key, bool encrypt)
{
    // Русский алфавит (33 буквы с ё)
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int n = alphabet.size(); // Размер алфавита

    // Очистка ключа: удаляем все символы, не входящие в алфавит
    std::string clean_key;
    for (char c : key) {
        char lower_c = std::tolower(static_cast<unsigned char>(c)); // Приведение к нижнему регистру
        if (alphabet.find(lower_c) != std::string::npos) {
            clean_key += lower_c;
        }
    }

    // Если в ключе не осталось допустимых символов
    if (clean_key.empty()) return text;

    std::string result; // Результирующая строка
    size_t key_index = 0; // Индекс текущего символа ключа

    // Обработка каждого символа входного текста
    for (char c : text) {
        // Приведение символа к нижнему регистру
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        size_t text_pos = alphabet.find(lower_c); // Позиция символа в алфавите

        // Если символ не из алфавита, сохраняем без изменений
        if (text_pos == std::string::npos) {
            result += lower_c;
            continue;
        }

        // Получение текущего символа ключа (циклически)
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);
        key_index++; // Переход к следующему символу ключа

        // Вычисление новой позиции в алфавите
        size_t new_pos;
        if (encrypt) {
            // Шифрование: (текст + ключ) % n
            new_pos = (text_pos + key_pos) % n;
        }
        else {
            // Расшифровка: (текст - ключ + n) % n
            new_pos = (text_pos - key_pos + n) % n;
        }

        // Добавление преобразованного символа
        result += alphabet[new_pos];
    }

    return result;
}*/


// Вычисление индекса совпадений (Index of Coincidence)
double calculate_ic(const std::string& text) {
    // Для текста короче 2 символов IC не вычисляется
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    std::vector<int> counts(alphabet.size(), 0); // Счетчики частот букв
    int total_letters = 0; // Общее количество букв в тексте

    // Подсчет частот символов
    for (char c : text) {
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            counts[pos]++;
            total_letters++;
        }
    }

    // Вычисление IC по формуле: sum(n_i*(n_i-1))/(N*(N-1))
    double ic = 0.0;
    for (int count : counts) {
        ic += count * (count - 1);
    }

    return ic / (total_letters * (total_letters - 1));
}

 //Функция взлома шифра Виженера (нахождение ключа)
std::string break_vigenere(const std::string& ciphertext)
{
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int max_key_length = 30; // Максимальная предполагаемая длина ключа
    int best_key_length = 1;       // Найденная длина ключа
    double best_ic = 0.0;          // Лучшее значение IC

    // Шаг 1: Определение длины ключа с помощью IC
    for (int key_len = 1; key_len <= max_key_length; key_len++)
    {
        double sum_ic = 0.0;
        int sequences = 0; // Количество подпоследовательностей

        // Для каждой позиции в ключе
        for (int i = 0; i < key_len; i++)
        {
            std::string sequence; // Последовательность символов

            // Формирование последовательности для i-го символа ключа
            for (int j = i; j < ciphertext.length(); j += key_len)
            {
                char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
                // Фильтрация символов не из алфавита
                if (alphabet.find(c) != std::string::npos)
                    sequence += c;
            }

            // Вычисление IC для последовательности
            if (sequence.length() > 1)
            {
                sum_ic += calculate_ic(sequence);
                sequences++;
            }
        }

        // Вычисление среднего IC
        if (sequences > 0)
        {
            double avg_ic = sum_ic / sequences;
            // Выбор длины ключа с максимальным IC
            if (avg_ic > best_ic)
            {
                best_ic = avg_ic;
                best_key_length = key_len;
            }
        }
    }

    // Шаг 2: Определение символов ключа
    std::string recovered_key; // Восстановленный ключ

    // Для каждого символа ключа
    for (int i = 0; i < best_key_length; i++)
    {
        std::string sequence; // Последовательность для текущего символа

        // Сбор символов, зашифрованных одним символом ключа
        for (int j = i; j < ciphertext.length(); j += best_key_length)
        {
            char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
            if (alphabet.find(c) != std::string::npos)
                sequence += c;
        }

        double best_chi_sq = 1e9; // Лучшее значение хи-квадрат
        char best_char = 'а';     // Найденный символ ключа

        // Перебор всех возможных символов ключа
        for (char c : alphabet)
        {
            // Расшифровка последовательности текущим кандидатом
            std::string decrypted_seq;
            for (char ch : sequence)
            {
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted_seq += alphabet[new_pos];
            }

            // Подсчет частот символов в расшифрованной последовательности
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

            // Вычисление критерия хи-квадрат
            double chi_sq = 0.0;
            if (total_chars > 0)
            {
                for (size_t j = 0; j < alphabet.size(); j++)
                {
                    double expected = RUSSIAN_FREQUENCIES[j] * total_chars;
                    double observed = freq_counts[j];
                    // Избегаем деления на ноль
                    if (expected > 1e-9) {
                        chi_sq += (observed - expected) * (observed - expected) / expected;
                    }
                }

                // Выбор символа с минимальным хи-квадрат
                if (chi_sq < best_chi_sq)
                {
                    best_chi_sq = chi_sq;
                    best_char = c;
                }
            }
        }

        // Добавление найденного символа в ключ
        recovered_key += best_char;
    }

    return recovered_key;
}

//Чтение всего файла в строку (бинарный режим)
std::string ready(const std::string file_name) {
    // Открытие файла в бинарном режиме
    std::ifstream fin(file_name, std::ios::binary);

    if (!fin.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл '" << file_name << "'\n";
        return "";
    }

    // Определение размера файла
    fin.seekg(0, std::ios::end); // seekg устанавливает позицию чтения в файловом потоке
    const auto size = fin.tellg(); // tellg возвращает текущую позицию чтения в файле
    if (size <= 0) {
        fin.close();
        return "";
    }

    // Чтение всего файла
    fin.seekg(0, std::ios::beg);
    std::string str(size, '\0');
    fin.read(&str[0], size);
    fin.close();

    return str;
}

// Запись строки в файл (бинарный режим)
bool writey(const std::string filename, const std::string content) {
    // Открытие файла в бинарном режиме
    std::ofstream out_file(filename, std::ios::binary);
    if (!out_file) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return false;
    }

    // Запись содержимого
    out_file.write(content.data(), content.size()); // data() позволяет работать с содержимым строки как с C-строкой или бинарными данными
    if (!out_file) {
        std::cerr << "Ошибка записи в файл " << filename << std::endl;
        return false;
    }

    out_file.close();
    std::cout << "Файл успешно записан: " << filename << std::endl;
    return true;
}

// Вывод справки по использованию программы
void show_help() {
    std::cout << "Программа для шифрования/расшифрования файлов\n\n";
    std::cout << "Использование:\n";
    std::cout << "  program.exe -sh input.txt -k key output.enc   # Шифрование\n";
    std::cout << "  program.exe -rsh input.enc -k key output.txt # Расшифрование с ключом\n";
    std::cout << "  program.exe -dsh input.enc output.txt        # Дешифрование (взлом)\n";
    std::cout << "  program.exe -h                               # Справка\n";
    std::cout << "  program.exe help                             # Справка\n\n";
    std::cout << "Режимы работы:\n";
    std::cout << "  -sh    Шифрование текста\n";
    std::cout << "  -rsh   Расшифрование текста с известным ключом\n";
    std::cout << "  -dsh   Дешифрование текста без ключа (автоматический подбор ключа)\n";
}