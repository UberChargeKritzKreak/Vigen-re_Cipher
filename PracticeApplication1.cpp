#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

std::string ready(std::string file_name);
bool writey(const std::string filename, const std::string content);

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

bool writey(const std::string filename, const std::string content) {
    // Получаем полный путь к файлу (текущая директория + имя файла)
    std::string full_path = filename;  // В текущей директории

    // Создаем и записываем файл
    std::ofstream out_file(full_path);
    if (!out_file) {
        std::cerr << "Ошибка: не удалось создать файл " << full_path << std::endl;
        return false;
    }

    out_file << content;

    if (!out_file) {
        std::cerr << "Ошибка записи в файл " << full_path << std::endl;
        return false;
    }

    out_file.close();

    // Выводим полный путь к сохраненному файлу
    std::cout << "Файл успешно записан: ";
    std::cout << full_path << std::endl;

    return true;
}