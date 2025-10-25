#include <iostream>
#include <fstream>

struct GameState {
    int round;
    int population;
    int eaten_by_rats;
    int wheat;
    int area;
    int price;
};

bool ask_bool() {
    std::string user_message;
    while (std::cin >> user_message) {
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

int main() {
    std::cout << "Добро пожаловать в игру Хамурапи" << std::endl;
    std::ifstream save_file("saves.txt");
    //if (save_file.is_open()) {
        std::cout << "Хотите ли вы продолжить предыдущую игру? Д/Н\n";
        bool load_bool = ask_bool();
    //}
    return 0;
}
