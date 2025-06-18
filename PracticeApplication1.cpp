#include <iostream>
#include <string>
#include <cctype>

std::string vigenere_cipher(const std::string&, const std::string&, bool);

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