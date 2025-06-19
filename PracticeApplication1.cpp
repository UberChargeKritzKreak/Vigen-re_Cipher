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