#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <chrono>
#include <thread>

using Array2D = std::array<std::array<int, 9>, 9>;

using Array1D = std::array<std::set<int>, 9>;
using Array3DSet = std::array<std::array<std::set<int>, 3>, 3>;
using Array3DArr = std::array<std::array<std::vector<int>, 9>, 9>;
// 1 set por cuadrado
int iterations = 0, nonDFStries = 0;

std::vector<int> checkNum(int i, int j, std::set<int> squareSet, std::set<int> horizontalSet, std::set<int> verticalSet){
    std::set<int> sumOfSets;
    sumOfSets.merge(squareSet);
    sumOfSets.merge(horizontalSet);
    sumOfSets.merge(verticalSet);

    std::vector<int> vectorToReturn;

    for(int i = 1; i<=9; i++)
        if(sumOfSets.find(i) == sumOfSets.end())
            vectorToReturn.push_back(i);
    
    return vectorToReturn;
}

Array3DArr arrayThatContainsPossibleNums;
Array2D preDFSArray;

bool preSolve(Array2D originalArray, Array2D &modifiedArray){
    bool isChanged = false;
    // ver si hay algun numero que sabemos que es 100%
    Array3DSet setThatContainsNumbersThatAppearedInSquare;
    Array1D setThatContainsNumbersThatAppearedInRow, setThatContainsNumbersThatAppearedInColumn;
    arrayThatContainsPossibleNums = {};

    preDFSArray = modifiedArray;

    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(modifiedArray[i][j]!=0){
                setThatContainsNumbersThatAppearedInRow[i].insert(modifiedArray[i][j]);
                setThatContainsNumbersThatAppearedInColumn[j].insert(modifiedArray[i][j]);

                setThatContainsNumbersThatAppearedInSquare[i/3][j/3].insert(modifiedArray[i][j]);
            }
    
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(modifiedArray[i][j]==0){
                arrayThatContainsPossibleNums[i][j] = checkNum(i, j, setThatContainsNumbersThatAppearedInSquare[i/3][j/3],
                                                               setThatContainsNumbersThatAppearedInRow[i],
                                                               setThatContainsNumbersThatAppearedInColumn[j]);
            }
    // ahora ya sabemos que numeros pueden ir en cada celda :smiling_imp:

    // Pre DFS techniques
    std::unordered_map<int, int> seenNumbers;
    for (int initializeMap = 1; initializeMap <= 9; initializeMap++)
        seenNumbers[initializeMap] = 0;

    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(modifiedArray[i][j]==0){
                // si solo hay 1 numero en una celda, que lo ponga
                if(arrayThatContainsPossibleNums[i][j].size() == 1)
                    preDFSArray[i][j] = arrayThatContainsPossibleNums[i][j][0];

                for (int initializeMap = 1; initializeMap <= 9; initializeMap++)
                    seenNumbers[initializeMap] = 0;

                // scanear y ver si solo hay un numero en un cuadrado
                for(int verticalPos = (i/3)*3; verticalPos < (i/3)*3 + 3; verticalPos++)
                    for(int horizontalPos = (j/3)*3; horizontalPos < (j/3)*3 + 3; horizontalPos++)                
                        for(int vectorPos = 0; vectorPos < arrayThatContainsPossibleNums[verticalPos][horizontalPos].size(); vectorPos++){
                            if(seenNumbers.find(arrayThatContainsPossibleNums[verticalPos][horizontalPos][vectorPos]) != seenNumbers.end())
                                seenNumbers[arrayThatContainsPossibleNums[verticalPos][horizontalPos][vectorPos]]++;
                    }
                
                for(int num = 1; num<=9; num++)
                    if(seenNumbers[num] == 1)
                        for(int verticalPos = (i/3)*3; verticalPos < (i/3)*3 + 3; verticalPos++)
                            for(int horizontalPos = (j/3)*3; horizontalPos < (j/3)*3 + 3; horizontalPos++)                
                                if(std::find(arrayThatContainsPossibleNums[verticalPos][horizontalPos].begin(), 
                                             arrayThatContainsPossibleNums[verticalPos][horizontalPos].end(), num) 
                                             != arrayThatContainsPossibleNums[verticalPos][horizontalPos].end())
                                    preDFSArray[verticalPos][horizontalPos] = num;

                // scanear y ver si solo hay un numero posible en la columna en x posicion
                for (int initializeMap = 1; initializeMap <= 9; initializeMap++)
                    seenNumbers[initializeMap] = 0;

                for(int verticalPos = 0; verticalPos < 9; verticalPos++)
                    for(int vectorPos = 0; vectorPos < arrayThatContainsPossibleNums[verticalPos][j].size(); vectorPos++){
                            if(seenNumbers.find(arrayThatContainsPossibleNums[verticalPos][j][vectorPos]) != seenNumbers.end())
                                seenNumbers[arrayThatContainsPossibleNums[verticalPos][j][vectorPos]]++;
                    }
                
                for(int num = 1; num<=9; num++)
                    if(seenNumbers[num] == 1)
                        for(int verticalPos = 0; verticalPos < 9; verticalPos++)
                            if(std::find(arrayThatContainsPossibleNums[verticalPos][j].begin(), 
                                         arrayThatContainsPossibleNums[verticalPos][j].end(), num) 
                                         != arrayThatContainsPossibleNums[verticalPos][j].end())
                                preDFSArray[verticalPos][j] = num;
                
                // scanear y ver si solo hay un numero posible en la fila en x posicion
                for (int initializeMap = 1; initializeMap <= 9; initializeMap++)
                    seenNumbers[initializeMap] = 0;

                for(int horizontalPos = 0; horizontalPos < 9; horizontalPos++)
                    for(int vectorPos = 0; vectorPos < arrayThatContainsPossibleNums[i][horizontalPos].size(); vectorPos++)
                            if(seenNumbers.find(arrayThatContainsPossibleNums[i][horizontalPos][vectorPos]) != seenNumbers.end())
                                seenNumbers[arrayThatContainsPossibleNums[i][horizontalPos][vectorPos]]++;
                
                for(int num = 1; num<=9; num++)
                    if(seenNumbers[num] == 1)
                        for(int horizontalPos = 0; horizontalPos < 9; horizontalPos++)
                            if(std::find(arrayThatContainsPossibleNums[i][horizontalPos].begin(), 
                                         arrayThatContainsPossibleNums[i][horizontalPos].end(), num) 
                                         != arrayThatContainsPossibleNums[i][horizontalPos].end())
                                preDFSArray[i][horizontalPos] = num;
            }

    // comprobar si hay algun spot en el que este el ultimo de los 9 numeros
    for (int initializeMap = 1; initializeMap <= 9; initializeMap++)
        seenNumbers[initializeMap] = 0;

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            for(int vectorPos = 0; vectorPos < arrayThatContainsPossibleNums[i][j].size(); vectorPos++)
                if(seenNumbers.find(arrayThatContainsPossibleNums[i][j][vectorPos]) != seenNumbers.end())
                   seenNumbers[arrayThatContainsPossibleNums[i][j][vectorPos]]++;
                    
    for(int num = 1; num<=9; num++)
        if(seenNumbers[num] == 1)
            for(int i = 0; i < 9; i++)
                for(int j = 0; j < 9; j++)
                    if(std::find(arrayThatContainsPossibleNums[i][j].begin(), 
                                 arrayThatContainsPossibleNums[i][j].end(), num) 
                                 != arrayThatContainsPossibleNums[i][j].end())
                        preDFSArray[i][j] = num;
    
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++)
            if(preDFSArray[i][j]!=modifiedArray[i][j]){
                isChanged = true;
                break;
            }
        if(isChanged)
            break;
    
        }
    
    modifiedArray = preDFSArray;
    nonDFStries++;
    return isChanged;
}

void DFS(Array2D &postDFSArray){
    int i = 0, j = 0, numSeen, candidateIndex[9][9] = {};
    bool isInvalidNum;

    while(i < 9){
        while(j < 9){
            if(preDFSArray[i][j] == 0){
                isInvalidNum = true;
                while(isInvalidNum){
                    if(candidateIndex[i][j] >= arrayThatContainsPossibleNums[i][j].size()){
                        isInvalidNum = true;
                        break;
                    }
                    numSeen = 0;
                    postDFSArray[i][j] = arrayThatContainsPossibleNums[i][j][candidateIndex[i][j]];
                    // vertical check
                    for(int verticalCheck = 0; verticalCheck < 9; verticalCheck++)
                        if(postDFSArray[i][j] == postDFSArray[verticalCheck][j] && i!=verticalCheck)
                            numSeen++;
                    // horizontal check
                    for(int horizontalCheck = 0; horizontalCheck < 9; horizontalCheck++)
                        if(postDFSArray[i][j] == postDFSArray[i][horizontalCheck] && j!=horizontalCheck)
                            numSeen++;
                    // square check
                    for(int squareVerticalCheck = (i/3)*3; squareVerticalCheck < (i/3)*3+3; squareVerticalCheck++)
                        for(int squareHorizontalCheck = (j/3)*3; squareHorizontalCheck < (j/3)*3+3; squareHorizontalCheck++)
                            if(postDFSArray[i][j] == postDFSArray[squareVerticalCheck][squareHorizontalCheck] && !(j==squareHorizontalCheck && i== squareVerticalCheck))
                                numSeen++;
                    if(numSeen == 0)
                        isInvalidNum = false;
                    else
                        candidateIndex[i][j]++;
                    iterations++;
                }
                if(isInvalidNum){
                    postDFSArray[i][j] = 0;
                    candidateIndex[i][j] = 0;
                    do{
                        if (j>0)
                            j--;
                        else if ( i>0){
                            j=8;
                            i--;
                        }
                    }while(preDFSArray[i][j]!=0);
                    candidateIndex[i][j]++;
                }
                else
                    j++;
            }
            else
                j++;
        }
        j=0;
        i++;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "WINDOWS?? NAH LINUX");

    sf::Vector2u windowSize = window.getSize();

    // LINES
    float xPosBigLines = windowSize.x / 3.0f; // 1/3rd of the screen
    float yPosBigLines = windowSize.y / 3.0f; // 1/3rd of the screen

    float xPosSmallLines = windowSize.x / 9.0f; // 1/9th of the screen
    float yPosSmallLines = windowSize.y / 9.0f; // 1/9th of the screen

    float xTextStart = windowSize.x / 25.0f;
    float yTextStart = windowSize.y / 50.0f; 
    
    sf::RectangleShape vertBigRectangle({4.0f, 1000.0f});
    vertBigRectangle.setFillColor(sf::Color::Black);
    sf::RectangleShape horiBigRectangle({1000.0f, 4.0f});
    horiBigRectangle.setFillColor(sf::Color::Black);

    sf::RectangleShape vertSmallRectangle({1.0f, 1000.0f});
    vertSmallRectangle.setFillColor(sf::Color::Black);
    sf::RectangleShape horiSmallRectangle({1000.0f, 1.0f});
    horiSmallRectangle.setFillColor(sf::Color::Black);

    // TEXT
    sf::Font textFont("ARIAL.TTF");
    sf::Text text(textFont);
    text.setString("Default");
    text.setCharacterSize(48);

    // ARRAY
    Array2D sudokuNums = {{
    {{0,0,0, 0,0,0, 0,0,0}},
    {{0,0,0, 0,0,4, 5,0,2}},
    {{0,2,0, 0,5,0, 1,0,7}},

    {{0,0,9, 0,0,0, 4,0,0}},
    {{8,0,0, 0,0,0, 0,9,1}},
    {{4,0,0, 6,0,0, 0,3,0}},

    {{0,0,8, 0,6,0, 0,0,0}},
    {{0,0,1, 7,9,0, 3,0,0}},
    {{0,0,0, 0,1,8, 0,5,0}}}};

    Array2D solvedNums = sudokuNums;
    bool wasChanged = true;

    auto start = std::chrono::high_resolution_clock::now();
    while(wasChanged) 
        wasChanged = preSolve(sudokuNums, solvedNums); 
    DFS(solvedNums);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Non DFS took " << elapsed.count() << " seconds\n";
    std::cout << "Total non DFS attempts: " << nonDFStries-1 << "\n";
    std::cout << "Total DFS iterations: " << iterations << " iterations\n";

    while (window.isOpen())
    {
        text.setFillColor(sf::Color::Black);

        vertBigRectangle.setPosition({xPosBigLines,0.0f});
        horiBigRectangle.setPosition({0.0f,yPosBigLines});

        vertSmallRectangle.setPosition({xPosSmallLines,0.0f});
        horiSmallRectangle.setPosition({0.0f,yPosSmallLines});

        text.setPosition({xTextStart,yTextStart});

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        // just drawing
        for(int i=0;i<9;i++){ // loop for small lines
            window.draw(vertSmallRectangle);
            vertSmallRectangle.move({xPosSmallLines, 0.0f});

            window.draw(horiSmallRectangle);
            horiSmallRectangle.move({0.0f, yPosSmallLines});
        }
        for(int i=0;i<3;i++){ // loop for big lines
            window.draw(vertBigRectangle);
            vertBigRectangle.move({xPosBigLines, 0.0f});

            window.draw(horiBigRectangle);
            horiBigRectangle.move({0.0f, yPosBigLines});
        }
        for(int i=0;i<9;i++){ // loop for numbers
            for(int j=0;j<9;j++){ // loop for numbers
                if (solvedNums[i][j] == 0)
                    text.setFillColor(sf::Color::White);
                else if(sudokuNums[i][j] != 0)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Blue);
                text.setString(std::to_string(solvedNums[i][j]));
                window.draw(text);
                text.move({xPosSmallLines, 0.0f});
            }
            text.setPosition({xTextStart,yTextStart});
            text.move({0.0f, yPosSmallLines*(i+1)});
        }

        window.display();
    }
}