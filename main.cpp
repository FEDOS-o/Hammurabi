#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <tuple>
#include <map>
#ifdef _WIN32
#include <windows.h>
#endif

static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

struct GameState {
    int round = 1;
    int population = 100;
    int starved = 0;
    int arrived = 0;
    bool plague = false;
    int rats = 0;
    int wheat = 2800;
    int harvest = 0;
    int fertility = 0;
    int area = 1000;
    int area_price = 17 + static_cast<int>(rng() % 10);
    bool loose = false;
    int total_starved = 0;


    std::string serialize() const {
        std::ostringstream oss;
        oss << "round=" << round << "\n"
            << "population=" << population << "\n"
            << "starved=" << starved << "\n"
            << "arrived=" << arrived << "\n"
            << "plague=" << (plague ? "1" : "0") << "\n"
            << "rats=" << rats << "\n"
            << "wheat=" << wheat << "\n"
            << "harvest=" << harvest << "\n"
            << "fertility=" << fertility << "\n"
            << "area=" << area << "\n"
            << "area_price=" << area_price << "\n"
            << "loose=" << (loose ? "1" : "0") << "\n"
            << "total_starved=" << total_starved;
        return oss.str();
    }

    // Десериализация из строки
    bool deserialize(const std::string& data) {
        try {
            std::map<std::string, std::string> key_values;
            std::istringstream iss(data);
            std::string line;

            // Парсим строки формата "ключ=значение"
            while (std::getline(iss, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    key_values[key] = value;
                }
            }

            // Загружаем значения
            if (key_values.count("round")) round = std::stoi(key_values["round"]);
            if (key_values.count("population")) population = std::stoi(key_values["population"]);
            if (key_values.count("starved")) starved = std::stoi(key_values["starved"]);
            if (key_values.count("arrived")) arrived = std::stoi(key_values["arrived"]);
            if (key_values.count("plague")) plague = (key_values["plague"] == "1");
            if (key_values.count("rats")) rats = std::stoi(key_values["rats"]);
            if (key_values.count("wheat")) wheat = std::stoi(key_values["wheat"]);
            if (key_values.count("harvest")) harvest = std::stoi(key_values["harvest"]);
            if (key_values.count("fertility")) fertility = std::stoi(key_values["fertility"]);
            if (key_values.count("area")) area = std::stoi(key_values["area"]);
            if (key_values.count("area_price")) area_price = std::stoi(key_values["area_price"]);
            if (key_values.count("loose")) loose = (key_values["loose"] == "1");
            if (key_values.count("total_starved")) total_starved = std::stoi(key_values["total_starved"]);

            return true;
        } catch (const std::exception& e) {
            std::cout << "Ошибка при загрузке сохранения: " << e.what() << std::endl;
            return false;
        }
    }
};

struct GameRules {
    static constexpr int consume = 20;
    static constexpr int cultivate = 10;
    static constexpr int seed = 2;
};

bool ask_bool();

bool save_game(const GameState& state, const std::string& filename = "saves.txt") {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
            return false;
        }

        std::string data = state.serialize();
        file << data;
        file.close();

        std::cout << "Игра успешно сохранена в файл " << filename << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cout << "Ошибка при сохранении игры: " << e.what() << std::endl;
        return false;
    }
}

bool load_game(GameState& state, const std::string& filename = "saves.txt") {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Файл сохранения " << filename << " не найден." << std::endl;
            return false;
        }
        std::cout << "Хотите ли вы продолжить предыдущую игру? Y/N\n";
        if (!ask_bool()) {
            return false;
        }

        std::ostringstream file_content;
        file_content << file.rdbuf();
        file.close();

        bool success = state.deserialize(file_content.str());
        if (success) {
            std::cout << "Игра успешно загружена из файла " << filename << std::endl;
        } else {
            std::cout << "Не удалось загрузить игру из файла " << filename << std::endl;
        }
        return success;

    } catch (const std::exception& e) {
        std::cout << "Ошибка при загрузке игры: " << e.what() << std::endl;
        std::cout << "Файл сохранения может быть поврежден." << std::endl;
        return false;
    }
}

std::string strip(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

bool ask_bool() {
    std::string user_message;
    while (std::getline(std::cin, user_message)) {
        std::cerr << '"' << user_message << '"' << "\n";
        user_message = strip(user_message);
        std::cerr << '"' << user_message << '"' << "\n";
        if (user_message == "Y") {
            return true;
        }
        if (user_message == "N") {
            return false;
        }
        std::cout << "Неправильный ввод: \"Y\" - Да, \"N\" - Нет\n";
    }
}

void print_state(GameState &state) {
    std::cout << "Мой повелитель, соизволь поведать тебе:\n";
    std::cout << "\tв году " << state.round << " твоего высочайшего правления\n";
    if (state.starved) {
        std::cout << '\t' << state.starved << " человек умерли с голоду;\n";
    }
    if (state.arrived) {
        std::cout << '\t' << state.arrived << " человек прибыли в наш великий город;\n";
    }
    if (state.plague) {
        std::cout << "\tЧума уничтожила половину населения;\n";
    }
    std::cout << "\tНаселение города сейчас составляет " << state.population << " человек;\n";
    if (state.harvest) {
        std::cout << "\tМы собрали " << state.harvest << " бушелей пшеницы, по " << state.fertility <<
                " бушеля с акра\n";
    }
    if (state.rats) {
        std::cout << "\tКрысы истребили " << state.rats << " бушелей пшеницы, оставив " << state.wheat <<
                " бушеля в амбарах;\n";
    } else {
        std::cout << "\tВ абмарах " << state.wheat << " бушеля пшеницы\n";
    }
    std::cout << "\tГород сейчас занимает " << state.area << " акров;\n";
    std::cout << "\t1 акр земли стоит сейчас " << state.area_price << " бушель.\n";
}

int ask_int() {
    std::string user_message;
    while (std::getline(std::cin, user_message)) {
        //std::cerr << '"' << user_message << '"' << "\n";
        user_message = strip(user_message);
        //std::cerr << '"' << user_message << '"' << "\n";
        try {
            return std::stoi(user_message);
        } catch (const std::exception &e) {
            std::cout << "Неправильный ввод: введите целое число" << std::endl;
        }
    }
}

std::tuple<int, int, int, int> ask_round(GameState &state) {
    std::cout << "Что пожелаешь, повелитель?\n";
    std::cout << "Сколько акров земли повелеваешь купить?" << std::endl;
    int area_to_buy = ask_int();
    int area_to_sell = 0;
    if (area_to_buy == 0) {
        std::cout << "Сколько акров земли повелеваешь продать?" << std::endl;
        area_to_sell = ask_int();
    }
    std::cout << "Сколько бушелей пшеницы повелеваешь съесть?" << std::endl;
    int wheat_to_eat = ask_int();
    std::cout << "Сколько акров земли повелеваешь засеять?" << std::endl;
    int area_to_seed = ask_int();
    return {area_to_buy, area_to_sell, wheat_to_eat, area_to_seed};
}

bool validate_user_input(GameState &state, std::tuple<int, int, int, int> input) {
    auto [area_to_buy, area_to_sell, wheat_to_eat, area_to_seed] = input;
    if ((long long) area_to_buy * state.area_price + wheat_to_eat + (area_to_seed + GameRules::seed - 1) /
        GameRules::seed > state.wheat + area_to_sell * state.area_price) {
        return false;
    }
    if (area_to_seed > state.area + area_to_buy - area_to_sell) {
        return false;
    }
    if (state.population * GameRules::cultivate < area_to_seed) {
        return false;
    }
    return true;
}

void calculate_turn(GameState &state, std::tuple<int, int, int, int> input) {
    auto [area_to_buy, area_to_sell, wheat_to_eat, area_to_seed] = input;
    state.round++;
    int get_from_area = (rng() % 6 + 1);
    int harvest = get_from_area * area_to_seed;
    state.wheat -= area_to_buy * state.area_price;
    state.area += area_to_buy;
    state.wheat += area_to_sell * state.area_price;
    state.area -= area_to_sell;
    state.wheat -= wheat_to_eat;
    state.wheat -= (area_to_seed + GameRules::seed - 1) / GameRules::seed;
    state.wheat += harvest;
    state.rats = rng() % 8 * state.wheat / 100;
    state.wheat -= state.rats;
    state.starved = std::max(state.population - wheat_to_eat / GameRules::consume, 0);
    state.total_starved += state.starved;
    if (state.population * 45 / 100 < state.starved) {
        state.loose = true;
    }
    state.population -= state.starved;
    state.arrived = state.starved / 2 + (5 - get_from_area) * state.wheat / 600 + 1;
    state.arrived = std::max(state.arrived, 0);
    state.arrived = std::min(state.arrived, 50);
    state.population += state.arrived;
    if (rng() % 100 < 15) {
        state.plague = true;
        state.population /= 2;
    } else {
        state.plague = false;
    }
    state.area_price = 17 + static_cast<int>(rng() % 10);
}


void end_of_game(GameState &state) {
    int P = state.total_starved * 100 / state.population;
    int L = state.area / state.population;
    if (P > 33 && L < 7) {
        std::cout <<
                "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании"
                << std::endl;
    } else if (P > 10 && L < 9) {
        std::cout <<
                "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем"
                << std::endl;
    } else if (P > 3 && L < 10) {
        std::cout <<
                "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова"
                << std::endl;
    } else {
        std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше"
                << std::endl;
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    std::cout << "Добро пожаловать в игру Хамурапи!\n" << std::endl;
    std::ifstream save_file("saves.txt");
    GameState state;
    load_game(state);
    while (state.round != 11) {
        print_state(state);
        std::tuple<int, int, int, int> player_turn = ask_round(state);
        while (!validate_user_input(state, player_turn)) {
            std::cout << "О, повелитель, пощади нас! У нас только " << state.population << " человек, " << state.wheat
                    << " бушелей пшеницы и " << state.area <<
                    " акров земли!" << std::endl;
            player_turn = ask_round(state);
        }
        calculate_turn(state, player_turn);

        if (state.loose) {
            std::cout << "От голода умерло больше 45% населения. В городе поднялся бунт! Ваше правление окончено" <<
                    std::endl;
            return 0;
        }
        save_game(state);
    }
    end_of_game(state);
    return 0;
}
