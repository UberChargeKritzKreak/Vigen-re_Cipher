#include <iostream> // Ââîä/âûâîä
#include <string> // Ðàáîòà ñî ñòîêàìè
#include <vector> // Äèíàìè÷åñêèå ìàññèâû
#include <cmath> // Ìàòåìàòè÷åñêèå ôóíêöèè
#include <cctype> // Ôóíêöèè ðàáîòû ñ ñèìâîëàìè
#include <fstream> // Äëÿ ðàáîòû ñ ôàéëàìè

/*
 * Òàáëèöà ÷àñòîò áóêâ ðóññêîãî àëôàâèòà â ïîðÿäêå "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
 * Çíà÷åíèÿ ïðåäñòàâëÿþò âåðîÿòíîñòè âñòðå÷àåìîñòè áóêâ â ðóññêèõ òåêñòàõ
 * Ñóììà âñåõ âåðîÿòíîñòåé äîëæíà áûòü ïðèáëèçèòåëüíî ðàâíà 1.0
 */
const std::vector<double> RUSSIAN_FREQUENCIES = {
    0.0801, // à
    0.0159, // á
    0.0454, // â
    0.0170, // ã
    0.0017, // ä
    0.0298, // å
    0.0004, // ¸
    0.0094, // æ
    0.0174, // ç
    0.0745, // è
    0.0121, // é
    0.0349, // ê
    0.0440, // ë
    0.0321, // ì
    0.0670, // í
    0.1097, // î
    0.0281, // ï
    0.0473, // ð
    0.0547, // ñ
    0.0626, // ò
    0.0262, // ó
    0.0026, // ô
    0.0097, // õ
    0.0048, // ö
    0.0144, // ÷
    0.0073, // ø
    0.0036, // ù
    0.0004, // ú
    0.0190, // û
    0.0032, // ü
    0.0064, // ý
    0.0020, // þ
    0.0201  // ÿ
};

// Ïðîòîòèïû ôóíêöèé
double calculate_ic(const std::string&); // Âû÷èñëåíèå èíäåêñà ñîâïàäåíèé
std::string vigenere_cipher(const std::string&, const std::string&, bool); // Øèôðîâàíèå/äåøèôðîâêà Âèæåíåðà
std::string break_vigenere(const std::string&); // Âçëîì øèôðà Âèæåíåðà
std::string ready(std::string file_name); // ×òåíèå ôàéëà â ñòðîêó
bool writey(const std::string filename, const std::string content); // Çàïèñü ñòðîêè â ôàéë
void show_help(); // Âûâîä ñïðàâêè

int main(int argc, char* argv[])
{
    // Óñòàíîâêà ðóññêîé ëîêàëè äëÿ êîððåêòíîé ðàáîòû ñ êèðèëëèöåé
    setlocale(LC_ALL, "Russian");

    // Ïåðåêëþ÷åíèå êîäèðîâêè êîíñîëè íà Windows-1251 (òîëüêî äëÿ Windows)
    // Ýòî íåîáõîäèìî äëÿ ïðàâèëüíîãî îòîáðàæåíèÿ ðóññêèõ ñèìâîëîâ â êîíñîëè
    system("chcp 1251");

    // Îáðàáîòêà àðãóìåíòîâ êîìàíäíîé ñòðîêè
    if (argc == 2)
    {
        std::string argument = argv[1];
        // Ïðîâåðêà íà çàïðîñ ñïðàâêè
        if (argument == "-h" || argument == "help")
        {
            show_help();
            return 0; // Êîððåêòíîå çàâåðøåíèå ïðîãðàììû
        }
    }

    // Ïðîâåðêà ìèíèìàëüíîãî êîëè÷åñòâà àðãóìåíòîâ
    if (argc < 2)
    {
        std::cerr << "Îøèáêà: Íåâåðíîå êîëè÷åñòâî àðãóìåíòîâ\n";
        show_help();
        return 1; // Çàâåðøåíèå ñ îøèáêîé
    }

    // Îñíîâíîé ðåæèì ðàáîòû ïðîãðàììû
    std::string mode = argv[1];

    // Ðåæèì øèôðîâàíèÿ (-sh) èëè ðàñøèôðîâàíèÿ ñ êëþ÷îì (-rsh)
    if (mode == "-sh" || mode == "-rsh")
    {
        // Ïðîâåðêà êîëè÷åñòâà àðãóìåíòîâ äëÿ ðåæèìîâ ñ êëþ÷îì
        if (argc != 6)
        {
            std::cerr << "Îøèáêà: Íåâåðíîå êîëè÷åñòâî àðãóìåíòîâ\n";
            show_help();
            return 1;
        }

        // Ïàðñèíã àðãóìåíòîâ
        std::string input_file_name = argv[2];   // Âõîäíîé ôàéë
        std::string key_flag = argv[3];          // Ôëàã êëþ÷à (äîëæåí áûòü "-k")
        std::string key_value = argv[4];         // Çíà÷åíèå êëþ÷à
        std::string output_file_name = argv[5];  // Âûõîäíîé ôàéë

        // Ïðîâåðêà êîððåêòíîñòè ôëàãà êëþ÷à
        if (key_flag != "-k")
        {
            std::cerr << "Îøèáêà: Îæèäàëñÿ ôëàã -k ïåðåä êëþ÷îì\n";
            show_help();
            return 1;
        }

        // Ðåæèì øèôðîâàíèÿ
        if (mode == "-sh")
        {
            // ×òåíèå -> Øèôðîâàíèå -> Çàïèñü
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, true);

            if (writey(output_file_name, output_string) == true) std::cout << "���������� ���������...\n";
        }
        // Ðåæèì ðàñøèôðîâàíèÿ ñ êëþ÷îì
        else if (mode == "-rsh")
        {
            // ×òåíèå -> Ðàñøèôðîâêà -> Çàïèñü
            std::string output_string = vigenere_cipher(ready(input_file_name), key_value, false);

            if (writey(output_file_name, output_string) == true) std::cout << "������������� ���������...\n";
        }
    }
    // Ðåæèì äåøèôðîâàíèÿ áåç êëþ÷à (âçëîì)
    else if (mode == "-dsh")
    {
        // Ïðîâåðêà êîëè÷åñòâà àðãóìåíòîâ äëÿ ðåæèìà âçëîìà
        if (argc != 4)
        {
            std::cerr << "Îøèáêà: Íåâåðíîå êîëè÷åñòâî àðãóìåíòîâ\n";
            show_help();
            return 1;
        }

        std::string input_file_name = argv[2];   // Çàøèôðîâàííûé ôàéë
        std::string output_file_name = argv[3];  // Ôàéë äëÿ ðàñøèôðîâàííîãî òåêñòà

        // Ïðîöåññ âçëîìà:
        // 1. Îïðåäåëåíèå êëþ÷à ñ ïîìîùüþ êðèïòîàíàëèçà
        std::string possible_key = break_vigenere(ready(input_file_name));
        // 2. Ðàñøèôðîâêà òåêñòà íàéäåííûì êëþ÷îì
        std::string decrypted_string = vigenere_cipher(ready(input_file_name), possible_key, false);

        if (writey(output_file_name, decrypted_string) == true) std::cout << "������������ ���������...\n";
    }
    // Íåèçâåñòíûé ðåæèì ðàáîòû
    else
    {
        std::cerr << "Îøèáêà: Íåèçâåñòíûé ðåæèì ðàáîòû ïðîãðàììû: " << mode << std::endl;
        show_help();
        return 1;
    }
    return 0;
}

// Ôóíêöèÿ øèôðîâàíèÿ/ðàñøèôðîâàíèÿ òåêñòà
std::string vigenere_cipher(const std::string& text, const std::string& key, bool encrypt = true)
{
    // Ðóññêèé àëôàâèò (33 áóêâû)
    const std::string alphabet = "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
    const int n = alphabet.size();

    // Ïîäãîòîâêà êëþ÷à (òîëüêî ñòðî÷íûå áóêâû àëôàâèòà)
    std::string clean_key;
    for (char c : key) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        if (alphabet.find(lower_c) != std::string::npos) {
            clean_key += lower_c;
        }
    }
    if (clean_key.empty()) return text;

    // Îáðàáîòêà òåêñòà
    std::string result;
    size_t key_index = 0;

    for (char c : text) {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        size_t text_pos = alphabet.find(lower_c);
        
        if (text_pos == std::string::npos) {
            result += lower_c;  // Íå-áóêâû ñîõðàíÿåì êàê åñòü
            continue;
        }

        // Ïîëó÷àåì ñèìâîë êëþ÷à
        char key_char = clean_key[key_index % clean_key.size()];
        size_t key_pos = alphabet.find(key_char);
        key_index++;

        // Âû÷èñëÿåì íîâóþ ïîçèöèþ
        size_t new_pos;
        if (encrypt) {
            new_pos = (text_pos + key_pos) % n;  // Øèôðîâàíèå
        }
        else {
            new_pos = (text_pos - key_pos + n) % n;  // Ðàñøèôðîâêà
        }

        result += alphabet[new_pos];
    }

    return result;
 }

/*
 * Ôóíêöèÿ äëÿ âû÷èñëåíèÿ èíäåêñà ñîâïàäåíèé (Index of Coincidence)
 * IC èñïîëüçóåòñÿ äëÿ îïðåäåëåíèÿ âåðîÿòíîé äëèíû êëþ÷à
 *
 * @param text - âõîäíàÿ ñòðîêà äëÿ àíàëèçà
 * @return çíà÷åíèå èíäåêñà ñîâïàäåíèé (îò 0.0 äî 1.0)
 */
double calculate_ic(const std::string& text) {
    // Åñëè òåêñò ñëèøêîì êîðîòêèé, âîçâðàùàåì 0
    if (text.length() < 2) return 0.0;

    const std::string alphabet = "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
    std::vector<int> counts(alphabet.size(), 0); // Âåêòîð äëÿ ïîäñ÷åòà áóêâ
    int total_letters = 0; // Îáùåå êîëè÷åñòâî áóêâ â òåêñòå

    // Ïîäñ÷èòûâàåì êîëè÷åñòâî êàæäîé áóêâû â òåêñòå
    for (char c : text) {
        // Íàõîäèì ïîçèöèþ áóêâû â àëôàâèòå
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos) {
            counts[pos]++;       // Óâåëè÷èâàåì ñ÷åò÷èê äëÿ ýòîé áóêâû
            total_letters++;     // Óâåëè÷èâàåì îáùèé ñ÷åò÷èê áóêâ
        }
    }

    // Âû÷èñëÿåì èíäåêñ ñîâïàäåíèé ïî ôîðìóëå:
    // IC = ?(n_i*(n_i-1)) / (N*(N-1))
    // ãäå n_i - êîëè÷åñòâî i-îé áóêâû, N - îáùåå êîëè÷åñòâî áóêâ
    double ic = 0.0;
    for (int count : counts) {
        ic += count * (count - 1);
    }

    // Âîçâðàùàåì íîðìàëèçîâàííîå çíà÷åíèå
    return ic / (total_letters * (total_letters - 1));
}

/*
 * Îñíîâíàÿ ôóíêöèÿ äëÿ âçëîìà øèôðà Âèæåíåðà
 *
 * @param ciphertext - çàøèôðîâàííûé òåêñò
 * @return íàéäåííûé êëþ÷ (â íèæíåì ðåãèñòðå)
 */
std::string break_vigenere(const std::string& ciphertext) {
    const std::string alphabet = "àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
    const int max_key_length = 30; // Ìàêñèìàëüíàÿ ïðåäïîëàãàåìàÿ äëèíà êëþ÷à

    /*****************************************************************
     * Ýòàï 1: Îïðåäåëåíèå äëèíû êëþ÷à ñ ïîìîùüþ ìåòîäà Êàñèñêè
     * Àíàëèçèðóåì èíäåêñ ñîâïàäåíèé äëÿ ðàçíûõ äëèí êëþ÷à
     * Íàèáîëåå âåðîÿòíàÿ äëèíà êëþ÷à äàñò íàèáîëüøèé ñðåäíèé IC
     *****************************************************************/
    int best_key_length = 1;    // Íà÷àëüíîå ïðåäïîëîæåíèå î äëèíå êëþ÷à
    double best_ic = 0.0;       // Ëó÷øåå çíà÷åíèå IC

    // Ïåðåáèðàåì âîçìîæíûå äëèíû êëþ÷à îò 1 äî max_key_length
    for (int key_len = 1; key_len <= max_key_length; key_len++) {
        double sum_ic = 0.0;    // Ñóììà IC äëÿ âñåõ ïîäïîñëåäîâàòåëüíîñòåé
        int valid_sequences = 0; // Êîëè÷åñòâî âàëèäíûõ ïîäïîñëåäîâàòåëüíîñòåé

        // Äëÿ êàæäîé ïîçèöèè â êëþ÷å
        for (int i = 0; i < key_len; i++) {
            std::string sequence; // Ïîñëåäîâàòåëüíîñòü ñèìâîëîâ äëÿ i-é ïîçèöèè êëþ÷à

            // Ñîáèðàåì ñèìâîëû, çàøèôðîâàííûå îäíèì ñèìâîëîì êëþ÷à
            for (int j = i; j < ciphertext.length(); j += key_len) {
                // Ïðèâîäèì ñèìâîë ê íèæíåìó ðåãèñòðó
                char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
                // Åñëè ñèìâîë åñòü â àëôàâèòå, äîáàâëÿåì åãî â ïîñëåäîâàòåëüíîñòü
                if (alphabet.find(c) != std::string::npos) {
                    sequence += c;
                }
            }

            // Âû÷èñëÿåì IC òîëüêî äëÿ ïîñëåäîâàòåëüíîñòåé äëèíîé > 1
            if (sequence.length() > 1) {
                sum_ic += calculate_ic(sequence);
                valid_sequences++;
            }
        }

        // Åñëè íàøëè âàëèäíûå ïîñëåäîâàòåëüíîñòè, âû÷èñëÿåì ñðåäíèé IC
        if (valid_sequences > 0) {
            double avg_ic = sum_ic / valid_sequences;

            // Âûáèðàåì äëèíó êëþ÷à ñ ìàêñèìàëüíûì ñðåäíèì IC
            if (avg_ic > best_ic) {
                best_ic = avg_ic;
                best_key_length = key_len;
            }
        }
    }

    /*****************************************************************
     * Ýòàï 2: Âîññòàíîâëåíèå êëþ÷à ïîñèìâîëüíî ñ ïîìîùüþ ÷àñòîòíîãî àíàëèçà
     * Äëÿ êàæäîé ïîçèöèè êëþ÷à ïîäáèðàåì ñèìâîë, äàþùèé íàèáîëåå "åñòåñòâåííîå"
     * ðàñïðåäåëåíèå áóêâ â ðàñøèôðîâàííîì òåêñòå
     *****************************************************************/
    std::string recovered_key; // Ñòðîêà äëÿ âîññòàíîâëåííîãî êëþ÷à

    // Äëÿ êàæäîãî ñèìâîëà â êëþ÷å
    for (int i = 0; i < best_key_length; i++) {
        std::string sequence; // Ïîñëåäîâàòåëüíîñòü äëÿ òåêóùåãî ñèìâîëà êëþ÷à

        // Ñîáèðàåì âñå ñèìâîëû, çàøèôðîâàííûå òåêóùèì ñèìâîëîì êëþ÷à
        for (int j = i; j < ciphertext.length(); j += best_key_length) {
            char c = std::tolower(static_cast<unsigned char>(ciphertext[j]));
            if (alphabet.find(c) != std::string::npos) {
                sequence += c;
            }
        }

        // Åñëè íå íàøëè íè îäíîãî ñèìâîëà äëÿ ýòîé ïîçèöèè êëþ÷à
        if (sequence.empty()) {
            recovered_key += 'à'; // Èñïîëüçóåì çíà÷åíèå ïî óìîë÷àíèþ
            continue;
        }

        double best_chi_sq = std::numeric_limits<double>::max(); // Ìèíèìàëüíîå õè-êâàäðàò
        char best_char = 'à'; // Íàéäåííûé ñèìâîë êëþ÷à

        // Ïåðåáèðàåì âñå âîçìîæíûå ñèìâîëû àëôàâèòà êàê êàíäèäàòû íà ñèìâîë êëþ÷à
        for (char c : alphabet) {
            std::string decrypted; // Ðàñøèôðîâàííàÿ ïîñëåäîâàòåëüíîñòü

            // Ïðîáóåì ðàñøèôðîâàòü ïîñëåäîâàòåëüíîñòü òåêóùèì êàíäèäàòîì
            for (char ch : sequence) {
                // Íàõîäèì ïîçèöèè ñèìâîëîâ â àëôàâèòå
                size_t pos = alphabet.find(ch);
                size_t key_pos = alphabet.find(c);
                // Âû÷èñëÿåì ïîçèöèþ â èñõîäíîì òåêñòå: (pos - key_pos) mod alphabet.size()
                size_t new_pos = (pos - key_pos + alphabet.size()) % alphabet.size();
                decrypted += alphabet[new_pos];
            }

            // Ïîäñ÷èòûâàåì ÷àñòîòû ñèìâîëîâ â ðàñøèôðîâàííîì òåêñòå
            std::vector<int> counts(alphabet.size(), 0);
            int total = 0; // Îáùåå êîëè÷åñòâî ñèìâîëîâ
            for (char ch : decrypted) {
                size_t pos = alphabet.find(ch);
                if (pos != std::string::npos) {
                    counts[pos]++;
                    total++;
                }
            }

            // Ïðîïóñêàåì, åñëè íå íàøëè íè îäíîãî ñèìâîëà
            if (total == 0) continue;

            // Âû÷èñëÿåì ñòàòèñòèêó õè-êâàäðàò äëÿ ñðàâíåíèÿ ñ ýòàëîííûìè ÷àñòîòàìè
            double chi_sq = 0.0;
            for (size_t j = 0; j < alphabet.size(); j++) {
                double expected = RUSSIAN_FREQUENCIES[j] * total; // Îæèäàåìîå êîëè÷åñòâî
                double observed = counts[j];                      // Íàáëþäàåìîå êîëè÷åñòâî
                if (expected > 0) { // Èçáåãàåì äåëåíèÿ íà íîëü
                    chi_sq += (observed - expected) * (observed - expected) / expected;
                }
            }

            // Âûáèðàåì ñèìâîë ñ íàèìåíüøèì õè-êâàäðàò (íàèáîëåå áëèçêîå ðàñïðåäåëåíèå)
            if (chi_sq < best_chi_sq) {
                best_chi_sq = chi_sq;
                best_char = c;
            }
        }

        // Äîáàâëÿåì íàéäåííûé ñèìâîë â êëþ÷
        recovered_key += best_char;
    }

    // Âîçâðàùàåì âîññòàíîâëåííûé êëþ÷
    return recovered_key;
}

//×òåíèå âñåãî ôàéëà â ñòðîêó (áèíàðíûé ðåæèì)
std::string ready(const std::string file_name) {
    // Îòêðûòèå ôàéëà â áèíàðíîì ðåæèìå
    std::ifstream fin(file_name, std::ios::binary);

    if (!fin.is_open()) {
        std::cerr << "Îøèáêà: íå óäàëîñü îòêðûòü ôàéë '" << file_name << "'\n";
        return "";
    }

    // Îïðåäåëåíèå ðàçìåðà ôàéëà
    fin.seekg(0, std::ios::end); // seekg óñòàíàâëèâàåò ïîçèöèþ ÷òåíèÿ â ôàéëîâîì ïîòîêå
    const auto size = fin.tellg(); // tellg âîçâðàùàåò òåêóùóþ ïîçèöèþ ÷òåíèÿ â ôàéëå
    if (size <= 0) {
        fin.close();
        return "";
    }

    // ×òåíèå âñåãî ôàéëà
    fin.seekg(0, std::ios::beg);
    std::string str(size, '\0');
    fin.read(&str[0], size);
    fin.close();

    return str;
}

// Çàïèñü ñòðîêè â ôàéë (áèíàðíûé ðåæèì)
bool writey(const std::string filename, const std::string content) {
    if (content.empty()) {
        std::cerr << "������: ������� �������� ������ ������" << std::endl;
        return false;
    }

    // ��������� ����������� �������� ����� ��� ������������ ��������
    std::ofstream test(filename);
    if (!test) {
        std::cerr << "������: ���������� ������� ���� " << filename << std::endl;
        return false;
    }
    test.close();

    // �������� ������
    std::ofstream out_file(filename, std::ios::binary);
    out_file.write(content.data(), content.size());

    if (!out_file) {
        std::cerr << "Îøèáêà çàïèñè â ôàéë " << filename << std::endl;
        return false;
    }

    std::cout << "���� ������� �������: " << filename << std::endl;
    return true;
}

// Âûâîä ñïðàâêè ïî èñïîëüçîâàíèþ ïðîãðàììû
void show_help() {
    std::cout << "Ïðîãðàììà äëÿ øèôðîâàíèÿ/ðàñøèôðîâàíèÿ ôàéëîâ\n\n";
    std::cout << "Èñïîëüçîâàíèå:\n";
    std::cout << "  program.exe -sh input.txt -k key output.enc   # Øèôðîâàíèå\n";
    std::cout << "  program.exe -rsh input.enc -k key output.txt # Ðàñøèôðîâàíèå ñ êëþ÷îì\n";
    std::cout << "  program.exe -dsh input.enc output.txt        # Äåøèôðîâàíèå (âçëîì)\n";
    std::cout << "  program.exe -h                               # Ñïðàâêà\n";
    std::cout << "  program.exe help                             # Ñïðàâêà\n\n";
    std::cout << "Ðåæèìû ðàáîòû:\n";
    std::cout << "  -sh    Øèôðîâàíèå òåêñòà\n";
    std::cout << "  -rsh   Ðàñøèôðîâàíèå òåêñòà ñ èçâåñòíûì êëþ÷îì\n";
    std::cout << "  -dsh   Äåøèôðîâàíèå òåêñòà áåç êëþ÷à (àâòîìàòè÷åñêèé ïîäáîð êëþ÷à)\n";
}