#include <iostream> // Ввод/вывод
#include <string> // Работа со строками
#include <vector> // Динамические массивы
#include <cmath> // Математические функции
#include <cctype> // Функции работы с символами
#include <fstream> // Для работы с файлами

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
    0.0017, // д
    0.0298, // е
    0.0004, // ё
    0.0094, // ж
    0.0174, // з
    0.0745, // и
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
    0.0032, // ь
    0.0064, // э
    0.0020, // ю
    0.0201  // я
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
    // Если текст слишком короткий, возвращаем 0
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    std::vector<int> counts(alphabet.size(), 0); // Вектор для подсчета букв
    int total_letters = 0; // Общее количество букв в тексте

    // Подсчитываем количество каждой буквы в тексте
    for (char c : text) {
        // Находим позицию буквы в алфавите
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            counts[pos]++;       // Увеличиваем счетчик для этой буквы
            total_letters++;     // Увеличиваем общий счетчик букв
        }
    }

    // Вычисляем индекс совпадений
    double ic = 0.0;
    for (int count : counts) {
        ic += count * (count - 1);
    }

    // Возвращаем нормализованное значение
    return ic / (total_letters * (total_letters - 1));
}

/*
 * Основная функция для взлома шифра Виженера
 *
 * @param ciphertext - зашифрованный текст
 * @return найденный ключ (в нижнем регистре)
 */
std::string break_vigenere(const std::string& ciphertext) {
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int max_key_length = 30; // Максимальная предполагаемая длина ключа

    /*****************************************************************
     * Этап 1: Определение длины ключа с помощью метода Касиски
     * Анализируем индекс совпадений для разных длин ключа
     * Наиболее вероятная длина ключа даст наибольший средний IC
     *****************************************************************/
    int best_key_length = 1;    // Начальное предположение о длине ключа
    double best_ic = 0.0;       // Лучшее значение IC

    // Перебираем возможные длины ключа от 1 до max_key_length
    for (int key_len = 1; key_len <= max_key_length; key_len++) {
        double sum_ic = 0.0;    // Сумма IC для всех подпоследовательностей
        int valid_sequences = 0; // Количество валидных подпоследовательностей

        // Для каждой позиции в ключе
        for (int i = 0; i < key_len; i++) {
            std::string sequence; // Последовательность символов для i-й позиции ключа

            // Собираем символы, зашифрованные одним символом ключа
            for (int j = i; j < ciphertext.length(); j += key_len) {
                // Приводим символ к нижнему регистру
                char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
                // Если символ есть в алфавите, добавляем его в последовательность
                if (alphabet.find(c) != std::string::npos) {
                    sequence += c;
                }
            }

            // Вычисляем IC только для последовательностей длиной > 1
            if (sequence.length() > 1) {
                sum_ic += calculate_ic(sequence);
                valid_sequences++;
            }
        }

        // Если нашли валидные последовательности, вычисляем средний IC
        if (valid_sequences > 0) {
            double avg_ic = sum_ic / valid_sequences;

            // Выбираем длину ключа с максимальным средним IC
            if (avg_ic > best_ic) {
                best_ic = avg_ic;
                best_key_length = key_len;
            }
        }
    }

    /*****************************************************************
     * Этап 2: Восстановление ключа посимвольно с помощью частотного анализа
     * Для каждой позиции ключа подбираем символ, дающий наиболее "естественное"
     * распределение букв в расшифрованном тексте
     *****************************************************************/
    std::string recovered_key; // Строка для восстановленного ключа

    // Для каждого символа в ключе
    for (int i = 0; i < best_key_length; i++) {
        std::string sequence; // Последовательность для текущего символа ключа

        // Собираем все символы, зашифрованные текущим символом ключа
        for (int j = i; j < ciphertext.length(); j += best_key_length) {
            char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
            if (alphabet.find(c) != std::string::npos) {
                sequence += c;
            }
        }

        // Если не нашли ни одного символа для этой позиции ключа
        if (sequence.empty()) {
            recovered_key += 'а'; // Используем значение по умолчанию
            continue;
        }

        double best_chi_sq = std::numeric_limits<double>::max(); // Минимальное хи-квадрат
        char best_char = 'а'; // Найденный символ ключа

        // Перебираем все возможные символы алфавита как кандидаты на символ ключа
        for (char c : alphabet) {
            std::string decrypted; // Расшифрованная последовательность

            // Пробуем расшифровать последовательность текущим кандидатом
            for (char ch : sequence) {
                // Находим позиции символов в алфавите
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                // Вычисляем позицию в исходном тексте: (pos - key_pos) mod alphabet.size()
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted += alphabet[new_pos];
            }

            // Подсчитываем частоты символов в расшифрованном тексте
            std::vector<int> counts(alphabet.size(), 0);
            int total = 0; // Общее количество символов
            for (char ch : decrypted) {
                size_t pos = alphabet.find(ch);
                if (pos != std::string::npos) {
                    counts[pos]++;
                    total++;
                }
            }

            // Пропускаем, если не нашли ни одного символа
            if (total == 0) continue;

            // Вычисляем статистику хи-квадрат для сравнения с эталонными частотами
            double chi_sq = 0.0;
            for (size_t j = 0; j < alphabet.size(); j++) {
                double expected = RUSSIAN_FREQUENCIES[j] * total; // Ожидаемое количество
                double observed = counts[j];                      // Наблюдаемое количество
                if (expected > 0) { // Избегаем деления на ноль
                    chi_sq += (observed - expected) * (observed - expected) / expected;
                }
            }

            // Выбираем символ с наименьшим хи-квадрат (наиболее близкое распределение)
            if (chi_sq < best_chi_sq) {
                best_chi_sq = chi_sq;
                best_char = c;
            }
        }

        // Добавляем найденный символ в ключ
        recovered_key += best_char;
    }

    // Возвращаем восстановленный ключ
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