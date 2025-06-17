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

    // Проверка, что аргументов достаточно для шифрования/расшифрования
    if (argc == 5)
    {
        // Определение переменных
        std::string input_file_name = argv[1]; // Имя файла, в котором содержится текст для шифрования/расшифрования
        std::string output_file_name = argv[2]; // Имя файла, в который поместится зашифрованный/расшифрованный текст
        std::string key = argv[3]; // Ключ для шифрования/расшифрования
        std::string mode = argv[4]; // Режим работы программы

        // Режим шифрования
        // en - шифрование
        // de - дешифрование
        if (!mode.compare("en"))
        {
            // Вызов функции шифрования
            std::cout << std::endl << "Вызов функции шифрования..." << std::endl;
        }
        // Режим расшифрования
        else if (!mode.compare("de"))
        {
            // Вызов функции расшифрования
            std::cout << std::endl << "Вызов функции расшифрования..." << std::endl;
        }
        else
        {
            // Вызов функции справки
            std::cout << std::endl << "Выбран некорретный режим работы (en/de)..." << std::endl;
        }
    }
    // Проверка, что аргументов достаточно для дешифрации
    else if (argc == 3)
    {
        // Определение переменных
        std::string input_file_name = argv[1]; // Имя файла, в котором содержится зашифрованный текст
        std::string output_file_name = argv[2]; // Имя файла, в который поместится дешифрованный текст

        // Вызов функции дешифрования
        std::cout << std::endl << "Вызов функции дешифрования..." << std::endl;
    }
    // Аргументов недостаточно или избыточно для работы программы
    else
    {
        // Вызов функции справки
        std::cout << std::endl << "Вызов функции справки..." << std::endl;
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