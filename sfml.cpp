//sfml.cpp
#include "sfml.h"
#include <SFML/Graphics.hpp>
#include "processData.h"
#include "ForceDirectedGraph.h"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

processData process_data = processData();

SFMLWindow::SFMLWindow(processData &dataProcessor, bool createGraph)
        : dataProcessor(dataProcessor), graph(createGraph ?
                                              ForceDirectedGraph(dataProcessor, window_width, window_height) : ForceDirectedGraph())

{
    sf::Event event;

    forceDirectedGraphRequested = false;

    // Set font
    if (!font.loadFromFile("fonts/futura_medium.ttf")) {
        cerr << "Error: Unable to load font" << endl;
        throw runtime_error("Unable to load font");
    }

    // Set up menu text
    menuText.setFont(font);
    menuText.setCharacterSize(56);
    menuText.setFillColor(sf::Color(225, 235, 230));

    // Set up input text
    inputText.setFont(font);
    inputText.setCharacterSize(56);
    inputText.setFillColor(sf::Color(225, 235, 230));
    inputText.setPosition(30, window_height / 2 + menuText.getCharacterSize() + 20);

    // Set up result text
    resultText.setFont(font);
    resultText.setCharacterSize(32);
    resultText.setFillColor(sf::Color(225, 235, 230));

    // position of result text
    resultText.setPosition(1450, window_height / 2 + 2 * menuText.getCharacterSize() - 900);
}

void SFMLWindow::displayForceDirectedGraph()
{
    if (!forceDirectedGraphRequested) {
        // Request the graph to be displayed
        forceDirectedGraphRequested = true;
    }
    else {
        // Render the graph
        graph.renderGraph("Force-Directed Graph");
    }
}

void SFMLWindow::run() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "TestWindow");
    sf::Event event;

    while (window.isOpen()) {
        window.clear(sf::Color(0, 0, 0));

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered)
                handleTextEntered(event);
            else if (event.type == sf::Event::KeyPressed)
                handleKeyPressed(event);
        }

        // Call the performSearch() function
        performSearch();

        // Reset the displayResult flag to false

        // Pass the user input values to the drawMenu function
        drawMenu(window, topPredatorsInput, depthInput);
        window.display();

        // Check if user has requested force-directed graph
        if (forceDirectedGraphRequested) {
            graph.generateForceDirectedGraph(dataProcessor, window_width, window_height);
            graph.renderGraph("Force-Directed Graph");

            // Reset the flag to false after displaying graph
            forceDirectedGraphRequested = false;

        }
    }
}

void SFMLWindow::handleTextEntered(sf::Event &event) {
    // Handle backspace
    if (event.text.unicode == 8 && !inputString.empty()) {
        inputString.pop_back();
    }
    else if (event.text.unicode > 31 && event.text.unicode < 128 &&
             (selectedOption == 1 || selectedOption == 2) &&
             (event.text.unicode != '1' && event.text.unicode != '2')) {
        inputString.push_back(static_cast<char>(event.text.unicode));

        // Update autocomplete options based on input string
        findAutocompleteOptions();
    }
        // Handle return/enter key
    else if (event.text.unicode == '\r') {
        // input state is 1: input for top predators
        if (inputState == 1) {
            topPredatorsInput = std::stoi(inputString);
            // Clear input string for next input
            inputString.clear();
            // Set input state to 2: input for depth)
            inputState = 2;
        }
            // input state 2: input for depth
        else if (inputState == 2) {
            depthInput = std::stoi(inputString);
            inputString.clear(); // Clear the input string
            inputState = 0; // Reset the input state to 0 (no input for top predators or depth)
            displayResult = false;
        }
    }
}

void SFMLWindow::handleKeyPressed(sf::Event &event) {
    if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
        selectedOption = 1;
        inputString = ""; // Resets the input string
        displayResult = false;
        inputState = 0;
        inputText.setFont(font);
        inputText.setCharacterSize(42);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(window_width / 2 - inputText.getGlobalBounds().width / 2,
                              window_height / 2 - inputText.getGlobalBounds().height / 2 + 10);
    }
    else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2) {
        selectedOption = 2;
        inputString = ""; // Resets the input string
        displayResult = false;
        inputState = 0;
        inputText.setFont(font);
        inputText.setCharacterSize(42);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(window_width / 2 - inputText.getGlobalBounds().width / 2,
                              window_height / 2 - inputText.getGlobalBounds().height / 2 + 10);
    }
    else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3) {
        selectedOption = 3;
        inputString = ""; // Resets input string
        displayResult = false;
        inputState = 0;
    }
    else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4) {
        selectedOption = 4;
        inputString = ""; // Resets input string
        displayResult = false;
        inputState = 1;
        inputText.setFont(font);
        inputText.setCharacterSize(42);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(window_width / 2 - inputText.getGlobalBounds().width / 2,
                              window_height / 2 - inputText.getGlobalBounds().height / 2 + 10);
    }
    else if (event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5) {
        selectedOption = 5;
        inputString = "";
        displayResult = false;
        inputState = 0;
        forceDirectedGraphRequested = true; // set flag to true
    }
    else if (event.key.code == sf::Keyboard::Num6 || event.key.code == sf::Keyboard::Numpad6) {
        selectedOption = 6;
        inputString = "";
        displayResult = false;
        inputState = 0;
    }
    else if (event.key.code == sf::Keyboard::Return) {
        // Process the user input and display the search result when the user presses Enter
        if (selectedOption == 1 || selectedOption == 2) {
            displayResult = true;
            performSearch();
        }
        else if (selectedOption == 4) {
            if (inputState == 1) {
                try {
                    depthInput = std::stoi(inputString);
                } catch (...) {
                    // Handle input exception
                    depthInput = 0;
                }
                inputString = "";
                displayResult = false;
                inputState = 2;
                inputText.setFont(font);
                inputText.setCharacterSize(42);
                inputText.setFillColor(sf::Color::White);
                inputText.setPosition(window_width / 2 - inputText.getGlobalBounds().width / 2,
                                      window_height / 2 - inputText.getGlobalBounds().height / 2 + 10);
            }
            else if (inputState == 2) {
                try {
                    topPredatorsInput = std::stoi(inputString);
                } catch (...) {
                    // Handle input exception
                    topPredatorsInput = 0;
                }
                displayResult = true;
                performSearch();
                // Reset inputState after displaying the result
                inputState = 0;
            }
        }
    }
}

void SFMLWindow::drawMenu(sf::RenderWindow& window, int topPredatorsInput, int depthInput)
{
    sf::Event event;

    // If user selects "Quit" option, goodbye message is displyaed for 2 seconds and window closes
    if (selectedOption == 6) {
        menuText.setString("Good Bye!");
        // Goodbye message is positioned in of the window
        menuText.setPosition(window_width / 2 - menuText.getGlobalBounds().width / 2,
                             window_height / 2 - menuText.getGlobalBounds().height / 2 - 250);
        window.draw(menuText);
        window.display();
        sf::sleep(sf::seconds(2));
        window.close();
    }
    else {
        // Display option selection menu with ">>"
        string optionsText = "Input selection:\n\n";
        optionsText += (selectedOption == 1) ? ">> " : "";
        optionsText += "1. Search by taxon name\n\n";
        optionsText += (selectedOption == 2) ? ">> " : "";
        optionsText += "2. Search by common name\n\n";
        optionsText += (selectedOption == 3) ? ">> " : "";
        optionsText += "3. Find predators with max prey variety\n\n";
        optionsText += (selectedOption == 4) ? ">> " : "";
        optionsText += "4. Find predators with max prey variety\n "
                       "and assigned depth\n\n";
        optionsText += (selectedOption == 5) ? ">> " : "";
        optionsText += "5. Display species predatory-prey graph\n\n";
        optionsText += (selectedOption == 6) ? ">> " : "";
        optionsText += "6. Quit";

        // Set the text of the menu and position it at the top of the window
        menuText.setString(optionsText);

        // position of window menu
        menuText.setPosition(200,
                             window_height / 2 - menuText.getGlobalBounds().height / 2 - 350);
        window.draw(menuText);

        // If option 3 is selected and the result has not been displayed yet,
        // find the top predators with max prey variety and display result
        if (selectedOption == 3 && !displayResult) {
            auto topPredators = dataProcessor.findTopPredatorsWithMaxPreyVariety(5);

            // Create a string with the result information for each predator
            string resultStr;
            int rank = 1;
            for (const auto &predator : topPredators) {
                resultStr += "Rank " + to_string(rank++) + ":\n";
                resultStr += "\tPredator TSN: " + to_string(predator.first) + "\n";
                resultStr += "\tTaxon Name: " + predator.second.taxon_name + "\n";
                resultStr += "\tCommon Name: " + predator.second.common_name + "\n";
                resultStr += "\tPrey variety count: " +
                             to_string(dataProcessor.predatorAdjacencyList[predator.first].size()) + "\n\n";
            }
            // Set the text of the result and indicate that it has been displayed
            resultText.setString(resultStr);
            displayResult = true;
        }

        // If option 4 is selected and the result has not been displayed,
        // use default to find top predators with max prey variety and specified depth
        if (selectedOption == 4 && !displayResult) {
            auto predatorsByDepth = dataProcessor.findTopPredatorsWithMaxPreyVarietyWithDepth(5,1);

            // Create a string with the result information for each predator
            string resultStr;
            int rank = 1;
            for (const auto &predator : predatorsByDepth) {
                resultStr += "Rank " + to_string(rank++) + ":\n";
                resultStr += "\tPredator TSN: " + to_string(predator.first) + "\n";
                resultStr += "\tTaxon Name: " + predator.second.taxon_name + "\n";
                resultStr += "\tCommon Name: " + predator.second.common_name + "\n";
                resultStr += "\tPrey variety count: " +
                             to_string(dataProcessor.predatorAdjacencyList[predator.first].size()) +
                             "\n\n";
            }
            // Set the text of the result and indicate that it has been displayed
            resultText.setString(resultStr);
            displayResult = true;
        }
    }

    // If option 1 or 2 is selected, display input box and autocomplete suggestions
    if (selectedOption == 1 || selectedOption == 2)
    {
        // Draw input box
        sf::RectangleShape inputBox(sf::Vector2f(600, 85));
        inputBox.setFillColor(sf::Color(80, 80, 80)); // Dark gray color
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(2);

        // input box position
        inputBox.setPosition(200,window_height / 2 - inputBox.getSize().y / 2 + 220);
        window.draw(inputBox);

        // Draw the input text
        inputText.setString(inputString);
        inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 10);
        window.draw(inputText);

        // Draw autocomplete suggestions if available
        if (!autocompleteOptions.empty()) {
            sf::Text suggestionText;
            suggestionText.setFont(font);
            suggestionText.setCharacterSize(24);
            suggestionText.setFillColor(sf::Color::White);

            float suggestionY = inputBox.getPosition().y + inputBox.getSize().y + 10;
            for (const string &suggestion : autocompleteOptions) {
                suggestionText.setString(suggestion);
                suggestionText.setPosition(inputBox.getPosition().x + 10, suggestionY +10);
                window.draw(suggestionText);
                suggestionY += 30;
            }
        }
    }

    // If a result has been displayed, draw the result text
    if (displayResult) {

        window.draw(resultText);
    }

}

// A helper function to convert a string to lowercase
string toLowerCase(const string &str) {
    string lowerCaseStr = str;
    transform(lowerCaseStr.begin(), lowerCaseStr.end(), lowerCaseStr.begin(), ::tolower);
    return lowerCaseStr;
}

void SFMLWindow::performSearch() {
    // Check if the input string is empty
    if ((selectedOption == 1 || selectedOption == 2 || selectedOption == 5) && inputString.empty()) {
        resultText.setString("No input provided");
        return;
    }

    // Convert input string lower case
    string inputStringLowerCase = toLowerCase(inputString);

    // Iterate through the speciesInfoMap to find the matching taxon or common name
    for (const auto &entry : dataProcessor.speciesInfoMap) {
        string taxonNameLowerCase = toLowerCase(entry.second.taxon_name);
        string commonNameLowerCase = toLowerCase(entry.second.common_name);

        if (selectedOption == 1 && taxonNameLowerCase == inputStringLowerCase) {
            resultText.setString("Taxon Name: " + entry.second.taxon_name + "\nCommon Name: " + entry.second.common_name);
            break;
        }
        else if (selectedOption == 2 && commonNameLowerCase == inputStringLowerCase) {
            resultText.setString("Taxon Name: " + entry.second.taxon_name + "\nCommon Name: " + entry.second.common_name);
            break;
        }
    }

    // If no matching is found, display "No results" message
    if (resultText.getString().isEmpty()) {
        resultText.setString("No results found for \"" + inputString + "\"");
    }

    // If selectedOption is 5, set resultText to display species information

    if (selectedOption == 5) {
        // Find the SpeciesInfo object for "Felis catus"
        int felisCatusId = 849752;
        auto it = dataProcessor.speciesInfoMap.find(felisCatusId);
        if (it != dataProcessor.speciesInfoMap.end()) {
            processData::SpeciesInfo speciesInfo = it->second;
            std::string outputString =
                    "Taxon Name: " + speciesInfo.taxon_name + "\nCommon Name: " + speciesInfo.common_name;
            resultText.setString(outputString);
            forceDirectedGraphRequested = true;
        }
        // Retrieve the SpeciesInfo object by its index
        processData::SpeciesInfo speciesInfo = dataProcessor.speciesInfoMap[felisCatusId];

        // Construct the output string manually by concatenating the fields
//        std::string outputString = "Taxon Name: " + speciesInfo.taxon_name + "\nCommon Name: " + speciesInfo.common_name;
        std::string outputString = "Taxon Name: " + speciesInfo.taxon_name;

        // Set the resultText object to display the constructed output string
        resultText.setString(outputString);

        // Set the flag to display the force-directed graph
        forceDirectedGraphRequested = true;
    }
}

void SFMLWindow::findAutocompleteOptions() {
    autocompleteOptions.clear();
    string inputStringLowerCase = toLowerCase(inputString);

    for (const auto &entry : dataProcessor.speciesInfoMap) {
        if (selectedOption == 1) {
            string taxonNameLowerCase = toLowerCase(entry.second.taxon_name);
            if (taxonNameLowerCase.find(inputStringLowerCase) == 0) {
                autocompleteOptions.push_back(entry.second.taxon_name);
            }
        } else if (selectedOption == 2) {
            string commonNameLowerCase = toLowerCase(entry.second.common_name);
            if (commonNameLowerCase.find(inputStringLowerCase) == 0) {
                autocompleteOptions.push_back(entry.second.common_name);
            }
        }
    }
}



