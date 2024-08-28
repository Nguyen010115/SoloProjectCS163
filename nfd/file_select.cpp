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
        std::string line;

        // Read the file line by line
        while (std::getline(file, line)) {
            // Replace commas with spaces (if necessary)
            std::replace(line.begin(), line.end(), ',', ' ');

            // Use a stringstream to parse the numbers in the current line
            std::stringstream ss(line);
            int number;
            while (ss >> number) {
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
        std::string line;
        std::string content;

        // Read the entire file, line by line
        while (std::getline(file, line)) {
            content += line + " "; // Add a space after each line to separate words
        }

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