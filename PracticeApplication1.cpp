#include <iostream> // Ввод/вывод
#include <string> // Работа со строками
#include <vector> // Динамические массивы
#include <cmath> // Математические функции
#include <cctype> // Функции работы с символами
#include <fstream> // Для работы с файлами
#include <algorithm> // Сортировка частот букв
#include <limits> // Установка максимальной длины ключа
#include <map> // Хранение частот букв


/*
 * Таблица частот букв русского алфавита в порядке "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
 * Значения представляют вероятности встречаемости букв в русских текстах
 * Сумма всех вероятностей должна быть приблизительно равна 1.0
 */
const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, // а
    0.0159, // б
    0.0454, // в
    0.0170, // г
    0.0298, // д
    0.0845, // е
    0.0004, // ё
    0.0094, // ж
    0.0165, // з
    0.0735, // и
    0.0121, // й
    0.0349, // к
    0.0440, // л
    0.0321, // м
    0.0670, // н
    0.1097, // о
    0.0281, // п
    0.0473, // р
    0.0547, // с
    0.0626, // т
    0.0262, // у
    0.0026, // ф
    0.0097, // х
    0.0048, // ц
    0.0144, // ч
    0.0073, // ш
    0.0036, // щ
    0.0004, // ъ
    0.0190, // ы
    0.0174, // ь
    0.0032, // э
    0.0064, // ю
    0.0201  // я
};

// Прототипы функций
double calculate_ic(const std::string&); // Вычисление индекса совпадений
std::string vigenere_cipher(const std::string&, const std::string&, bool); // Шифрование/дешифровка Виженера
std::string break_vigenere(const std::string&); // Взлом шифра Виженера
std::string ready(std::string file_name); // Чтение файла в строку
bool writey(const std::string filename, const std::string content); // Запись строки в файл
void show_help(); // Вывод справки

std::vector<int> find_possible_key_lengths(const std::string&, int max_len);

std::string clean_text(const std::string&);

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

            if (writey(output_file_name, output_string)) std::cout << "Шифрование завершено...\n";
        }
        // Режим расшифрования с ключом
        else if (mode == "-rsh")
        {
            // Чтение -> Расшифровка -> Запись
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, false);

            if (writey(output_file_name, output_string)) std::cout << "Расшифровка завершена...\n";
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

        if (writey(output_file_name, decrypted_string)) std::cout << "Дешифровка завершена...\n";
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

// Функция для определения возможных длин ключа на основе анализа индекса совпадений (IC)
// Принимает:
// - ciphertext: зашифрованный текст для анализа
// - max_len: максимальная предполагаемая длина ключа (по умолчанию 30)
// Возвращает вектор из 3 наиболее вероятных длин ключа, отсортированных по убыванию вероятности
std::vector<int> find_possible_key_lengths(const std::string& ciphertext, int max_len = 30) {
    // Вектор для хранения пар (длина ключа, средний IC для этой длины)
    std::vector<std::pair<int, double>> key_ics;

    // Перебираем все возможные длины ключа от 1 до max_len
    for (int len = 1; len <= max_len; len++) {
        double avg_ic = 0.0;  // Средний IC для текущей длины ключа
        int sequences = 0;    // Количество последовательностей для этой длины

        // Для каждого возможного смещения в пределах длины ключа
        for (int i = 0; i < len; i++) {
            std::string sequence;  // Последовательность символов, взятых с шагом len

            // Собираем последовательность символов через интервал, равный предполагаемой длине ключа
            for (size_t j = i; j < ciphertext.size(); j += len) {
                sequence += ciphertext[j];
            }

            // Вычисляем IC только если последовательность достаточно длинная
            if (sequence.size() > 1) {
                avg_ic += calculate_ic(sequence);  // Суммируем IC для всех последовательностей
                sequences++;                      // Увеличиваем счетчик последовательностей
            }
        }

        // Если нашли хотя бы одну последовательность для этой длины ключа
        if (sequences > 0) {
            avg_ic /= sequences;  // Вычисляем средний IC
            key_ics.emplace_back(len, avg_ic);  // Сохраняем длину ключа и средний IC
        }
    }

    // Сортируем результаты по убыванию IC (более высокий IC означает более вероятную длину ключа)
    std::sort(key_ics.begin(), key_ics.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // Возвращаем 3 наиболее вероятные длины ключа
    std::vector<int> result;
    for (int i = 0; i < std::min(3, static_cast<int>(key_ics.size())); i++) {
        result.push_back(key_ics[i].first);
    }

    return result;
}

// Функция для очистки текста, оставляющая только буквы русского алфавита в нижнем регистре
// Принимает:
// - text: исходный текст для обработки
// Возвращает строку, содержащую только буквы русского алфавита в нижнем регистре
std::string clean_text(const std::string& text) {
    // Строка, содержащая все допустимые символы (русский алфавит)
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    // Результирующая строка (очищенный текст)
    std::string result;

    // Перебираем все символы входного текста
    for (char c : text) {
        // Приводим символ к нижнему регистру
        // Используем static_cast<unsigned char> для корректной работы с не-ASCII символами
        char lower_c = std::tolower(static_cast<unsigned char>(c));

        // Проверяем, содержится ли символ в русском алфавите
        if (alphabet.find(lower_c) != std::string::npos) {
            // Если символ допустимый, добавляем его в результат
            result += lower_c;
        }
    }

    // Возвращаем очищенный текст
    return result;
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

/*
 * Функция для вычисления индекса совпадений (Index of Coincidence)
 * IC используется для определения вероятной длины ключа
 *
 * @param text - входная строка для анализа
 * @return значение индекса совпадений (от 0.0 до 1.0)
 */
double calculate_ic(const std::string& text) {
    if (text.length() < 2) return 0.0;

    std::map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }

    double total = text.length();
    double sum = 0.0;
    for (const auto& pair : freq) {
    char ch = pair.first;
    int count = pair.second;
    sum += count * (count - 1);
}

    return sum / (total * (total - 1));
}

/*
 * Основная функция для взлома шифра Виженера
 *
 * @param ciphertext - зашифрованный текст
 * @return найденный ключ (в нижнем регистре)
 */
std::string break_vigenere(const std::string& ciphertext) {
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    std::string clean_cipher = clean_text(ciphertext);

    if (clean_cipher.empty()) return "";

    // Получаем несколько возможных длин ключа
    auto possible_lengths = find_possible_key_lengths(clean_cipher);

    std::string best_key;
    double best_score = std::numeric_limits<double>::max();

    for (int key_len : possible_lengths) {
        std::string key;

        for (int i = 0; i < key_len; i++) {
            std::string sequence;
            for (size_t j = i; j < clean_cipher.size(); j += key_len) {
                sequence += clean_cipher[j];
            }

            char best_char = 'а';
            double best_chi = std::numeric_limits<double>::max();

            for (char c : alphabet) {
                std::string decrypted;
                for (char ch : sequence) {
                    size_t pos = alphabet.find(ch);
                    size_t key_pos = alphabet.find(c);
                    size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                    decrypted += alphabet[new_pos];
                }

                std::vector<int> counts(alphabet.size(), 0);
                for (char ch : decrypted) {
                    size_t pos = alphabet.find(ch);
                    if (pos != std::string::npos) {
                        counts[pos]++;
                    }
                }

                double chi = 0.0;
                for (size_t j = 0; j < alphabet.size(); j++) {
                    double expected = RUSSIAN_FREQUENCIES[j] * decrypted.size();
                    double observed = counts[j];
                    if (expected > 0.5) { // Игнорируем очень редкие буквы
                        chi += std::pow(observed - expected, 2) / expected;
                    }
                }

                if (chi < best_chi) {
                    best_chi = chi;
                    best_char = c;
                }
            }

            key += best_char;
        }

        // Оцениваем качество ключа
        std::string decrypted = vigenere_cipher(clean_cipher, key, false);
        double score = 0.0;
        for (size_t i = 0; i < alphabet.size(); i++) {
            double expected = RUSSIAN_FREQUENCIES[i];
            double observed = std::count(decrypted.begin(), decrypted.end(), alphabet[i]) / (double)decrypted.size();
            score += std::pow(observed - expected, 2);
        }

        if (score < best_score) {
            best_score = score;
            best_key = key;
        }
    }

    return best_key;
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
    if (content.empty()) {
        std::cerr << "Ошибка: попытка записи пустой строки" << std::endl;
        return false;
    }

    // Проверка доступности файла для записи
    std::ofstream test(filename);
    if (!test) {
        std::cerr << "Ошибка: невозможно создать файл " << filename << std::endl;
        return false;
    }
    test.close();

    // Запись данных
    std::ofstream out_file(filename, std::ios::binary);
    out_file.write(content.data(), content.size());

    if (!out_file) {
        std::cerr << "Ошибка записи в файл " << filename << std::endl;
        return false;
    }

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

