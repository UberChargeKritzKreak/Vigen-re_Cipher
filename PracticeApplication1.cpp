#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

std::string vigenere_encrypt(const std::string&, const std::string&);
std::string ready(std::string file_name);
 
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

    std::cout << ready("C:\\3\\new");
}

std::string vigenere_encrypt(const std::string& text, const std::string& key) {
    // Русский алфавит (33 буквы)
    const std::string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int n = alphabet.size();

    // Проверка и подготовка ключа
    if (key.empty()) return text;

    std::string clean_key;
    for (char c : key) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalpha(uc)) {
            clean_key += std::tolower(uc);
        }
    }
    if (clean_key.empty()) return text;

    // Шифрование текста
    std::string result;
    size_t key_index = 0;

    for (char c : text) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalpha(uc)) {
            result += c;
            continue;
        }

        char base_char = std::tolower(uc);

        // Поиск позиции символа в алфавите
        size_t text_pos = alphabet.find(base_char);
        if (text_pos == std::string::npos) {
            result += c;
            continue;
        }

        // Получение символа ключа
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);

        // Шифрование: (позиция_текста + позиция_ключа) mod 33
        size_t new_pos = (text_pos + key_pos) % n;
        char new_char = alphabet[new_pos];

        result += new_char;
        
    }
    return result;
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