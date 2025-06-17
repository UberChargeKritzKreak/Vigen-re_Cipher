#include <iostream>
#include <string>
#include <cctype>

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

int main()
{
    setlocale(LC_ALL, "ru");

    std::cout << vigenere_encrypt("Привет Пока!", "нет");
}