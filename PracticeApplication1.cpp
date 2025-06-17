#include <iostream>
#include <string>
#include <cctype>

std::string vienere_encrypt(const std::string text, const std::string key)
{
    // Русский алфавит (строчные буквы)
    const std::string alphabet = "абвгдеёжзиклмнопрстуфхцчшщъыьэюя ";
    const int n = 33; // Длина алфавита

    // Если в ключе ничего нету, возвращаем неизмененный текст
    if (key.empty()) return text;

    std::string clean_key; // Все символы в нижнем регистре
    
    // Цикл проходит по каждому символу key и оставляет только буквы (в clean_key)
    for (char c : key) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalpha(uc)) {
            clean_key += std::tolower(uc);
        }
    }
    if (clean_key.empty()) return text;

    // Шифрование текста
    std::string result;
    unsigned int key_index = 0;

    for (char c : text) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalpha(uc)) {
            result += c;
            continue;
        }

        // Определение регистра и базовой буквы
        bool is_upper = std::isupper(uc);
        char base_char = std::tolower(uc);

        // Поиск позиции символа в алфавите
        size_t text_pos = alphabet.find(base_char);
        if (text_pos == std::string::npos) { // npos означает не найденную позицию
            result += c;
            continue;
        }

        // Получение символа ключа
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);

        // Шифрование: (позиция_текста + позиция_ключа) % 33
        unsigned int new_pos = (text_pos + key_pos) % n;
        char new_char = alphabet[new_pos];

        // Восстановление регистра
        if (is_upper) {
            result += static_cast<char>(std::toupper(new_char));
        }
        else {
            result += new_char;
        }
        key_index++;
    }
    return result;
}

int main()
{
    setlocale(LC_ALL, "ru");

    std::cout << vienere_encrypt("Привет Пока!", "нет");
}