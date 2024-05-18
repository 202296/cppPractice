#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

class HangmanGame
{
private:
    string word;
    const int maxAttempts = 6;
    int attempts;
    vector<char> guessedLetters;
    ofstream outFile; // File stream for writing guessed letters

public:
    const int getMaxAttempts() const
    {
        return maxAttempts;
    }

    HangmanGame()
    {

        // Open a file for writing guessed letters
        outFile.open("guessed_letters.txt");
        if (!outFile.is_open())
        {
            cerr << "Error: Unable to open file for writing guessed letters." << endl;
            exit(1);
        }

        srand(time(0)); // Seed the random number generator
        word = getRandomWordFromFile();
        attempts = 0;
    }

    ~HangmanGame()
    {
        // Close the file when the game object is destroyed
        outFile.close();
    }

    // Function to read a random word from a file
    string getRandomWordFromFile()
    {
        vector<string> words;       // Store words from file
        ifstream file("words.txt"); // Open file containing words
        if (!file)
        {
            cerr << "Error: Unable to open file." << endl;
            exit(1);
        }

        string word;
        while (file >> word)
        {                          // Read words from file
            words.push_back(word); // Store words in vector
        }

        file.close(); // Close file
        // Choose a random word from the vector
        return words[rand() % words.size()];
    }

    // Function to display current state of the word with blanks for unknown letters
    void displayWord() const
    {
        for (char letter : word)
        {
            if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end())
            {
                cout << letter << " "; // Print the letter if it has been guessed
            }
            else
            {
                cout << "_ "; // Print underscore for unguessed letter
            }
        }
        cout << endl;
    }

    // Function to check if the player has guessed all the letters in the word
    bool isWordGuessed() const
    {
        for (char letter : word)
        {
            if (find(guessedLetters.begin(), guessedLetters.end(), letter) == guessedLetters.end())
            {
                return false; // If any letter is not guessed, return false
            }
        }
        return true; // All letters have been guessed
    }

    // Function to check if the game is over (either won or lost)
    bool isGameOver() const
    {
        return (attempts >= maxAttempts) || isWordGuessed();
    }

    // Function to make a guess
    void makeGuess(char guess)
    {
        guessedLetters.push_back(guess); // Add the guessed letter to the vector
        outFile << guess << " ";         // Write the guessed letter to the file
        // Flush the output buffer to ensure the written data is immediately saved to the file
        outFile.flush();

        if (word.find(guess) == string::npos)
        {
            attempts++; // Increment the number of incorrect attempts if the guess is wrong
        }
    }

    // Function to print game status
    void printStatus() const
    {
        cout << "Attempts left: " << maxAttempts - attempts << endl;
    }

    // Function to print game result
    void printResult() const
    {
        if (isWordGuessed())
        {
            cout << "Congratulations! You guessed the word: " << word << endl;
        }
        else
        {
            cout << "You ran out of attempts. The word was: " << word << endl;
        }
    }
};

int main()
{
    HangmanGame game;

    cout << "Welcome to Hangman!\n";
    cout << "Guess the word by entering one letter at a time.\n";
    cout << "You have " << game.getMaxAttempts() << " attempts to guess the word.\n";

    while (!game.isGameOver())
    {
        cout << "\nWord: ";
        game.displayWord();

        cout << "Enter a letter: ";
        char guess;
        cin >> guess;

        game.makeGuess(guess);

        if (!game.isWordGuessed())
        {
            game.printStatus();
        }
    }

    game.printResult();

    return 0;
}