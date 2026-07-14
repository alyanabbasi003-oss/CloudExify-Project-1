
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
using namespace std;


const string BEST_SCORE_FILE = "bestscore.txt";   
const string TOP_SCORES_FILE = "topscores.txt";   
const string STATS_FILE      = "stats.txt";       
const string HISTORY_FILE    = "history.txt";     
const string BEST_TIME_FILE  = "besttime.txt";    
const string COLOR_RESET  = "\033[0m";
const string COLOR_GREEN  = "\033[1;32m";
const string COLOR_RED    = "\033[1;31m";
const string COLOR_YELLOW = "\033[1;33m";
const string COLOR_CYAN   = "\033[1;36m";


struct ScoreEntry {
    string name;
    int attempts;
};


int loadBestScore() {
    ifstream file(BEST_SCORE_FILE);
    int score = 0;
    if (file.is_open()) {
        file >> score;
        file.close();
    }
    return score;
}


void saveBestScore(int score) {
    int current = loadBestScore();
    if (score < current || current == 0) {
        ofstream file(BEST_SCORE_FILE);
        if (file.is_open()) {
            file << score;
            file.close();
            cout << "New best score: " << score << " attempts!" << endl;
        }
    }
}


vector<ScoreEntry> loadTopScores() {
    vector<ScoreEntry> scores;
    ifstream file(TOP_SCORES_FILE);
    if (file.is_open()) {
        string name;
        int attempts;
        while (file >> name >> attempts) {
            scores.push_back({name, attempts});
        }
        file.close();
    }
    return scores;
}

void saveTopScores(const vector<ScoreEntry>& scores) {
    ofstream file(TOP_SCORES_FILE);
    if (file.is_open()) {
        int limit = min((int)scores.size(), 3);
        for (int i = 0; i < limit; i++) {
            file << scores[i].name << " " << scores[i].attempts << "\n";
        }
        file.close();
    }
}


void updateTopScores(const string& playerName, int attempts) {
    vector<ScoreEntry> scores = loadTopScores();
    scores.push_back({playerName, attempts});

    sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.attempts < b.attempts;
    });

    if (scores.size() > 3) {
        scores.resize(3);
    }
    saveTopScores(scores);
}

void printTopScores() {
    vector<ScoreEntry> scores = loadTopScores();
    if (scores.empty()) {
        cout << "No high scores yet. Be the first!" << endl;
        return;
    }
    cout << "\n----- TOP 3 SCORES -----" << endl;
    for (size_t i = 0; i < scores.size(); i++) {
        cout << (i + 1) << ". " << scores[i].name
             << " - " << scores[i].attempts << " attempts" << endl;
    }
    cout << "-------------------------" << endl;
}



void loadStats(int& wins, int& losses) {
    wins = 0;
    losses = 0;
    ifstream file(STATS_FILE);
    if (file.is_open()) {
        file >> wins >> losses;
        file.close();
    }
}

void saveStats(int wins, int losses) {
    ofstream file(STATS_FILE);
    if (file.is_open()) {
        file << wins << " " << losses;
        file.close();
    }
}



void logGameHistory(const string& playerName, int maxNumber, int attempts,
                     int elapsedSeconds, bool won, int points) {
    ofstream file(HISTORY_FILE, ios::app); 
    if (file.is_open()) {
        time_t now = time(0);
        string ts = ctime(&now);
        if (!ts.empty() && ts.back() == '\n') ts.pop_back();

        file << "[" << ts << "] " << playerName
             << " | range 1-" << maxNumber
             << " | " << (won ? "WIN" : "LOSS")
             << " | attempts: " << attempts
             << " | time: " << elapsedSeconds << "s"
             << " | points: " << points << "\n";
        file.close();
    }
}



int loadBestTime() {
    ifstream file(BEST_TIME_FILE);
    int seconds = -1; // -1 means "no time recorded yet"
    if (file.is_open()) {
        file >> seconds;
        file.close();
    }
    return seconds;
}

void saveBestTime(int seconds) {
    int current = loadBestTime();
    if (current == -1 || seconds < current) {
        ofstream file(BEST_TIME_FILE);
        if (file.is_open()) {
            file << seconds;
            file.close();
            cout << COLOR_CYAN << "New fastest time: " << seconds << " seconds!" << COLOR_RESET << endl;
        }
    }
}




int calculatePoints(int maxNumber, int attempts, int elapsedSeconds, int maxAttempts) {
    
    int basePoints = maxNumber; 

    
    int attemptsSaved = maxAttempts - attempts;
    int attemptBonus = attemptsSaved * 10;

   
    int timeBonus = max(0, 60 - elapsedSeconds);

    int total = basePoints + attemptBonus + timeBonus;
    return max(total, 10); 
}



void printAchievements(int attempts, int elapsedSeconds) {
    vector<string> badges;
    if (attempts <= 3) badges.push_back("Sharpshooter (won in 3 attempts or fewer)");
    if (elapsedSeconds <= 10) badges.push_back("Speed Demon (won in under 10 seconds)");
    if (attempts == 1) badges.push_back("First Try Legend (guessed correctly immediately)");

    if (!badges.empty()) {
        cout << COLOR_YELLOW << "\n*** Achievements unlocked! ***" << COLOR_RESET << endl;
        for (const string& badge : badges) {
            cout << "  - " << badge << endl;
        }
    }
}


int getRange() {
    int choice;
    cout << "\nChoose difficulty:" << endl;
    cout << "1. Easy   (1 - 50)" << endl;
    cout << "2. Medium (1 - 100)" << endl;
    cout << "3. Hard   (1 - 200)" << endl;
    cout << "4. Custom (you pick the range)" << endl;
    cout << "Enter choice (1-4): ";
    cin >> choice;

   
    while (cin.fail() || choice < 1 || choice > 4) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid choice. Enter 1, 2, 3, or 4: ";
        cin >> choice;
    }

    if (choice == 1) return 50;
    if (choice == 3) return 200;

    if (choice == 4) {
        int customMax;
        cout << "Enter the maximum number for your custom range (2-1000): ";
        cin >> customMax;
        while (cin.fail() || customMax < 2 || customMax > 1000) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a number between 2 and 1000: ";
            cin >> customMax;
        }
        return customMax;
    }

    return 100; 
}


int getMaxAttempts(int maxNumber) {
    if (maxNumber == 50)  return 7;   // Easy
    if (maxNumber == 100) return 10;  // Medium
    if (maxNumber == 200) return 12;  // Hard
    
    int estimated = (int)(log2(maxNumber)) + 3;
    return max(5, min(estimated, 20));
}


struct GameResult {
    bool won;
    int attempts;
    int elapsedSeconds;
    int points; // 0 if lost
};


void printWarmerColderHint(int guess, int secret, int previousDistance, bool hasPrevious) {
    int distance = abs(guess - secret);
    if (hasPrevious) {
        if (distance < previousDistance) {
            cout << COLOR_CYAN << "(Getting WARMER)" << COLOR_RESET << endl;
        } else if (distance > previousDistance) {
            cout << COLOR_CYAN << "(Getting COLDER)" << COLOR_RESET << endl;
        } else {
            cout << "(Same distance as last guess)" << endl;
        }
    }
}


void giveHint(int secret, int low, int high, int hintsUsedSoFar) {
    if (hintsUsedSoFar % 2 == 0) {
        cout << COLOR_YELLOW << "HINT: The number is "
             << (secret % 2 == 0 ? "EVEN" : "ODD") << "." << COLOR_RESET << endl;
    } else {
        int mid = (low + high) / 2;
        if (secret <= mid) {
            cout << COLOR_YELLOW << "HINT: The number is between " << low
                 << " and " << mid << "." << COLOR_RESET << endl;
        } else {
            cout << COLOR_YELLOW << "HINT: The number is between " << (mid + 1)
                 << " and " << high << "." << COLOR_RESET << endl;
        }
    }
}


GameResult playGame(int maxNumber) {
    int secret = (rand() % maxNumber) + 1;
    int guess;
    int attempts = 0;
    int maxAttempts = getMaxAttempts(maxNumber);
    int previousDistance = 0;
    bool hasPrevious = false;

   
    const int MAX_HINTS = 2;
    int hintsUsed = 0;

 
    time_t startTime = time(0);

    cout << "\nI picked a number between 1 and " << maxNumber << "." << endl;
    cout << "Try to guess it! You have " << maxAttempts << " attempts." << endl;
    cout << "(Type h instead of a number for a hint - " << MAX_HINTS << " available)" << endl;

    while (true) {
        cout << "\nAttempt " << (attempts + 1) << "/" << maxAttempts
             << " - Your guess: ";

        string input;
        cin >> input;

       
        if (input == "h" || input == "H") {
            if (hintsUsed < MAX_HINTS) {
                giveHint(secret, 1, maxNumber, hintsUsed);
                hintsUsed++;
            } else {
                cout << COLOR_RED << "No hints left!" << COLOR_RESET << endl;
            }
            continue; // hints don't cost an attempt
        }

        
        stringstream ss(input);
        if (!(ss >> guess)) {
            cout << "Please enter a valid number (or 'h' for a hint)!" << endl;
            continue;
        }

        if (guess < 1 || guess > maxNumber) {
            cout << "Please guess between 1 and " << maxNumber << "!" << endl;
            continue; 
        }

        attempts++;

        if (guess < secret) {
            cout << COLOR_RED << "Too LOW! Try higher." << COLOR_RESET << endl;
            printWarmerColderHint(guess, secret, previousDistance, hasPrevious);
            previousDistance = abs(guess - secret);
            hasPrevious = true;
        } else if (guess > secret) {
            cout << COLOR_RED << "Too HIGH! Try lower." << COLOR_RESET << endl;
            printWarmerColderHint(guess, secret, previousDistance, hasPrevious);
            previousDistance = abs(guess - secret);
            hasPrevious = true;
        } else {
           
            int elapsed = (int)difftime(time(0), startTime);
            cout << COLOR_GREEN << "\nCORRECT! The number was " << secret << "!" << COLOR_RESET << endl;
            cout << "You got it in " << attempts << " attempts and " << elapsed << " seconds!" << endl;

            int points = calculatePoints(maxNumber, attempts, elapsed, maxAttempts);
            cout << COLOR_YELLOW << "Points earned: " << points << COLOR_RESET << endl;

            printAchievements(attempts, elapsed);

            return {true, attempts, elapsed, points};
        }

        if (attempts >= maxAttempts) {
            int elapsed = (int)difftime(time(0), startTime);
            cout << COLOR_RED << "\nOut of attempts! The number was " << secret << "." << COLOR_RESET << endl;
            return {false, attempts, elapsed, 0};
        }
    }
}



int main() {
    srand((unsigned int)time(0)); 

    cout << COLOR_CYAN << "================================" << endl;
    cout << "  NUMBER GUESS GAME " << endl;
    cout << "================================" << COLOR_RESET << endl;
    cout << "Best score: " << loadBestScore() << " attempts" << endl;

    int bestTime = loadBestTime();
    if (bestTime != -1) {
        cout << "Best time: " << bestTime << " seconds" << endl;
    }

    
    string playerName;
    cout << "Enter your name: ";
    cin >> playerName;

    int wins, losses;
    loadStats(wins, losses);
    cout << "Session stats so far - Wins: " << wins << ", Losses: " << losses << endl;

    printTopScores();

    int totalPoints = 0;
    char playAgain;
    do {
        int maxNum = getRange();
        GameResult result = playGame(maxNum);

        if (result.won) {
            wins++;
            totalPoints += result.points;
            saveBestScore(result.attempts);
            saveBestTime(result.elapsedSeconds);
            updateTopScores(playerName, result.attempts);
        } else {
            losses++;
            cout << "Better luck next time, " << playerName << "!" << endl;
        }

        logGameHistory(playerName, maxNum, result.attempts, result.elapsedSeconds,
                        result.won, result.points);

        saveStats(wins, losses);

        cout << "\nCurrent stats - Wins: " << wins << ", Losses: " << losses
             << " | Total points this session: " << totalPoints << endl;
        printTopScores();

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThanks for playing, " << playerName << "! You scored "
         << totalPoints << " points this session. Goodbye!" << endl;
    return 0;
}
