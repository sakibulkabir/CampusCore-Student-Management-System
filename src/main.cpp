#include "ConsoleUI.h"
#include <iostream>

int main() {
    StudentManager manager("data/students.db");
    std::string error;
    if (!manager.load(error)) {
        std::cerr << "Startup error: " << error << "\n";
        return 1;
    }
    ConsoleUI app(manager);
    app.run();
    return 0;
}
