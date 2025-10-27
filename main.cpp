#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <tuple>

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
};

std::string strip(const std::string& str) {
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
        if (user_message == "Д") {
            return true;
        }
        if (user_message == "Н") {
            return false;
        }
        std::cout << "Неправильный ввод: \"Д\" - Да, \"Н\" - Нет\n";
    }
}

void print_state(GameState &state) {
    std::cout << "Мой повелитель, соизволь поведать тебе:\n";
    std::cout << "\tв году "<< state.round << " твоего высочайшего правления\n";
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
        std::cout << "\tМы собрали " << state.harvest << " бушелей пшеницы, по " << state.fertility << " бушеля с акра\n";
    }
    if (state.rats) {
        std::cout << "\tКрысы истребили " << state.rats << " бушелей пшеницы, оставив " << state.wheat << " бушеля в амбарах;\n";
    } else {
        std::cout << "\tВ абмарах " << state.wheat << " бушеля пшеницы\n";
    }
    std::cout << "\tГород сейчас занимает " << state.area << " акров;\n";
    std::cout << "\t1 акр земли стоит сейчас " << state.area_price << " бушель.\n";
}

int ask_int() {
    return 0;
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
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::cout << "Добро пожаловать в игру Хамурапи!\n" << std::endl;
    std::ifstream save_file("saves.txt");
    if (save_file.is_open()) {
        std::cout << "Хотите ли вы продолжить предыдущую игру? Д/Н\n";
        bool load_bool = ask_bool();
    }
    GameState state;
    while (state.round != 11) {
        print_state(state);

    }
    return 0;
}
