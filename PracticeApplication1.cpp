#include <iostream>
#include <string>

// Режим работы программы
enum Mode
{
    ENCRYPT = 1,
    DECRYPT = 2
};

// Определение прототипов функций
bool isNumber(const std::string&); 
Mode getModeFromUser();

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian"); // Подключение русской локали
    
    // Переключение кодировки в консоли на Windows-1251
    system("chcp 1251");
    system("cls");

    // Проверка, что аргументов достаточно для шифрования/расшифрования
    if (argc == 4)
    {
        // Определение переменных
        std::string input_file_name = argv[1]; // Имя файла, в котором содержится текст для шифрования/расшифрования
        std::string output_file_name = argv[2]; // Имя файла, в который поместится зашифрованный/расшифрованный текст
        std::string key = argv[3]; // Ключ для шифрования/расшифрования

        // Режим работы программы
        Mode mode = getModeFromUser();

        // Режим шифрования
        if (mode == Mode::ENCRYPT)
        {
            // Вызов функции шифрования
            std::cout << std::endl << "Вызов функции шифрования..." << std::endl;
        }
        // Режим расшифрования
        else if (mode == Mode::DECRYPT)
        {
            // Вызов функции расшифрования
            std::cout << std::endl << "Вызов функции расшифрования..." << std::endl;
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

// Проверка, что строку можно преобразовать в число
bool isNumber(const std::string& s)
{
    // Проверка, что строка пустая
    if (s.empty())
        return false;

    // Проверка, что все символы являются цифрами
    for (int i = 0; i < s.length(); i++)
        if (!isdigit(s[i]))
            return false;

    return true;
}

// Получение режима работы программы от пользователя
Mode getModeFromUser()
{
    std::string input;
    int mode;

    do {
        std::cout << "Выберите режим работы:\n"
            << "1 - шифрование\n"
            << "2 - расшифрование\n"
            << "Введите режим работы: ";
        std::cin >> input;

        if (!isNumber(input)) {
            std::cout << "Ошибка: введите число\n";
            continue;
        }

        mode = std::stoi(input);
        if (mode < 1 || mode > 2) {
            std::cout << "Ошибка: число должно быть 1 или 2\n";
        }
    } while (mode < 1 || mode > 2);

    return static_cast<Mode>(mode);
}