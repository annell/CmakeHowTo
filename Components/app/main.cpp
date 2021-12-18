#include <iostream>
#include "Functions.h"
#include "Yaml.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Special number: " << SpecialNumber() << std::endl;
    std::cout << "Pi: " << Pi() << std::endl;
    Yaml yaml;
    yaml.name = "yammlarn";
    yaml.node["data"] = "something";
    auto node = yaml.encode();
    std::cout << node << std::endl;
    Bammel bammel;
    bammel.name = "bammlarn";

    return 0;
}
