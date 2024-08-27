#include "file_select.h"

// Function to open a native file dialog and return the selected file path
std::string FileSelectDialog() {
    nfdchar_t* outPath = NULL;

    // Define the file filter as a string (e.g., "txt")
    const char* filterList = "txt";

    // Open a file dialog with the filter
    nfdresult_t result = NFD_OpenDialog(filterList, NULL, &outPath);

    if (result == NFD_OKAY) {
        std::string selectedPath(outPath);
        free(outPath);  // Remember to free the memory allocated by NFD
        return selectedPath;
    }
    else if (result == NFD_CANCEL) {
        std::cout << "User pressed cancel." << std::endl;
        return "";
    }
    else {
        std::cerr << "Error: " << NFD_GetError() << std::endl;
        return "";
    }
}


std::vector<int> ReadNumbersFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<int> numbers;

    if (file.is_open()) {
        std::string content;
        std::getline(file, content);

        // Replace commas with spaces
        std::replace(content.begin(), content.end(), ',', ' ');

        // Use a stringstream to parse the numbers
        std::stringstream ss(content);
        int number;
        while (ss >> number) {
            if (number > 0) {
                numbers.push_back(number);
            }
        }

        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }

    return numbers;
}

std::vector<std::string> ReadWordsFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> words;

    if (file.is_open()) {
        std::string content;
        std::getline(file, content);

        // Replace commas with spaces
        std::replace(content.begin(), content.end(), ',', ' ');

        // Use a stringstream to parse the words
        std::stringstream ss(content);
        std::string word;
        while (ss >> word) {
            if (!word.empty()) {
                words.push_back(word);
            }
        }

        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }

    return words;
}
