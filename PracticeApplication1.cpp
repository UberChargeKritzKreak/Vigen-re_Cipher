#include <iostream>
#include <string>
#include <cctype>

std::string vienere_encrypt(const std::string text, const std::string key)
{
    // Русский алфавит (строчные буквы)
    const std::string alphabet = "абвгдеёжзиклмнопрстуфхцчшщъыьэюя";
    const int n = 33; // Длина алфавита

    // Если в ключе ничего нету, возвращаем неизмененный текст
    if (key.empty()) return text;
}

int main()
{
    setlocale(LC_ALL, "ru");

    std::cout << vienere_encrypt("тукст тукст", "");
}