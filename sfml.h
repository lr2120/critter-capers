// sfml.h
#pragma once
#include "Graph.h"
#include "processData.h"
#include "ForceDirectedGraph.h"
#include <SFML/Graphics.hpp>


class SFMLWindow {
public:
    // Constructor
    SFMLWindow(processData &dataProcessor, bool createGraph = true);
    ForceDirectedGraph graph;

    // Run window loop
    void run();

private:

    // Reference to the data processor
    processData &dataProcessor;

    // Font used for text rendering
    sf::Font font;

    // Text objects for menu, input, and result display
    sf::Text menuText;
    sf::Text inputText;
    sf::Text resultText;

    // Stores the currently selected option
    int selectedOption = 1;

    // Determines whether to display the result or not
    bool displayResult = false;

    // Stores the current user input string
    std::string inputString;

    // Window dimensions
    const int window_width = 2200;
    const int window_height = 1800;

    // Flag to request quitting the application
    bool quitRequested = false;

    // Initialize inputState
    int inputState = 0; // Initialize inputState

    // Used for option 4 in the program as user input defaults.
    int topPredatorsInput = 3; // top number of predators
    int depthInput = 3; // depth



    // Handles text input events
    void handleTextEntered(sf::Event &event);

    // Handles key press events
    void handleKeyPressed(sf::Event &event);

    // Draws the menu and other elements on the window
    void drawMenu(sf::RenderWindow &window, int topPredatorsInput, int depthInput);

    // Stores autocomplete suggestions
    std::vector<std::string> autocompleteOptions;

    // Finds autocomplete options based on the current input string
    void findAutocompleteOptions();

    // Performs search based on the user input
    void performSearch();

    bool forceDirectedGraphRequested = false;

    void displayForceDirectedGraph();

    void drawEdges();

};
