/*

//Завдання 3, Створіть систему управління персональними фінансами. 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

class ExpenseCategory {
public:
    string name;

    ExpenseCategory(string _name) : name(_name) {}
};

class Expense {
public:
    string description;
    double amount;
    ExpenseCategory category;
    time_t timestamp;

    Expense(string _description, double _amount, ExpenseCategory _category, time_t _timestamp) : description(_description), amount(_amount), category(_category), timestamp(_timestamp) {}
};

class Card {
public:
    string name;
    double balance;

    Card(string _name) : name(_name), balance(0) {}

    void deposit(double amount) {
        balance += amount;
    }

    void spend(double amount) {
        balance -= amount;
    }
};

class FinanceManager {
private:
    vector<Card> cards;
    vector<ExpenseCategory> categories;
    vector<Expense> expenses;

public:
    void addCard(Card card) {
        cards.push_back(card);
    }

    void addCategory(ExpenseCategory category) {
        categories.push_back(category);
    }

    void depositToCard(string cardName, double amount) {
        for (Card& card : cards) {
            if (card.name == cardName) {
                card.deposit(amount);
                // Додавання поповнення карт в список витрат
                Expense deposit("Deposit", amount, ExpenseCategory(cardName), time(nullptr));
                expenses.push_back(deposit);
                cout << "Deposited " << amount << " to " << cardName << " successfully." << endl;
                return;
            }
        }
        cout << "Card not found." << endl;
    }

    void addExpense(string description, double amount, string categoryName, time_t timestamp) {
        ExpenseCategory category(categoryName);
        for (ExpenseCategory& cat : categories) {
            if (cat.name == categoryName) {
                category = cat;
                break;
            }
        }
        Expense expense(description, amount, category, timestamp);
        expenses.push_back(expense);
        cout << "Expense added successfully." << endl;
    }

    void showCardBalance(string cardName) {
        for (const Card& card : cards) {
            if (card.name == cardName) {
                cout << "Balance of " << cardName << ": " << card.balance << endl;
                return;
            }
        }
        cout << "Card not found." << endl;
    }

    void showExpenses() {
        cout << "Expenses:" << endl;
        for (const Expense& expense : expenses) {
            cout << "Description: " << expense.description << ", Amount: " << expense.amount << ", Category: " << expense.category.name << endl;
        }
    }

    void showTopExpensesWeekly() {
        time_t currentTime = time(nullptr);
        vector<Expense> weeklyExpenses;

        for (Expense& expense : expenses) {
            if (difftime(currentTime, expense.timestamp) < 7 * 86400) {
                weeklyExpenses.push_back(expense);
            }
        }

        sort(weeklyExpenses.begin(), weeklyExpenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount > b.amount;
            });

        cout << "Top Expenses for the Week:" << endl;
        int count = 0;
        for (const Expense& expense : weeklyExpenses) {
            cout << "Description: " << expense.description << ", Amount: " << expense.amount << ", Category: " << expense.category.name << endl;
            if (count++ >= 3) {
                break;
            }
        }
    }

    void showTopExpensesMonthly() {
        time_t currentTime = time(nullptr);
        vector<Expense> monthlyExpenses;

        for (Expense& expense : expenses) {
            if (difftime(currentTime, expense.timestamp) < 30 * 86400) { 
                monthlyExpenses.push_back(expense);
            }
        }

        sort(monthlyExpenses.begin(), monthlyExpenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount > b.amount;
            });

        cout << "Top Expenses for the Month:" << endl;
        int count = 0;
        for (const Expense& expense : monthlyExpenses) {
            cout << "Description: " << expense.description << ", Amount: " << expense.amount << ", Category: " << expense.category.name << endl;
            if (count++ >= 3) {
                break;
            }
        }
    }

    void showAllCards() {
        cout << "All Cards:" << endl;
        for (const Card& card : cards) {
            cout << card.name << endl;
        }
    }

    void saveReportsAndRatingsToFile() {
        ofstream outFile("reports_and_ratings.txt");
        if (outFile.is_open()) {
            generateTopExpensesWeeklyToFile(outFile);
            generateTopExpensesMonthlyToFile(outFile);
            generateDailyReportToFile(outFile);
            outFile.close();
            cout << "Reports and ratings saved to reports_and_ratings.txt" << endl;
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void generateTopExpensesWeeklyToFile(ofstream& outFile) {
        time_t currentTime = time(nullptr);
        vector<Expense> weeklyExpenses;

        for (Expense& expense : expenses) {
            if (difftime(currentTime, expense.timestamp) < 7 * 86400) { 
                weeklyExpenses.push_back(expense);
            }
        }

        sort(weeklyExpenses.begin(), weeklyExpenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount > b.amount;
            });

        outFile << "Top Expenses for the Week:" << endl;
        int count = 0;
        for (const Expense& expense : weeklyExpenses) {
            outFile << "Description: " << expense.description << ", Amount: " << expense.amount << ", Category: " << expense.category.name << endl;
            if (++count >= 3) {
                break;
            }
        }
    }

    void generateTopExpensesMonthlyToFile(ofstream& outFile) {
        time_t currentTime = time(nullptr);
        vector<Expense> monthlyExpenses;

        for (Expense& expense : expenses) {
            if (difftime(currentTime, expense.timestamp) < 30 * 86400) {
                monthlyExpenses.push_back(expense);
            }
        }

        sort(monthlyExpenses.begin(), monthlyExpenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount > b.amount;
            });

        outFile << "Top Expenses for the Month:" << endl;
        int count = 0;
        for (const Expense& expense : monthlyExpenses) {
            outFile << "Description: " << expense.description << ", Amount: " << expense.amount << ", Category: " << expense.category.name << endl;
            if (++count >= 3) {
                break;
            }
        }
    }

    void generateDailyReportToFile(ofstream& outFile) {
        map<string, double> dailyExpenses;
        time_t currentTime = time(nullptr);
        struct tm timeInfo;
        localtime_s(&timeInfo, &currentTime);

        for (Expense& expense : expenses) {
            struct tm expenseTimeInfo;
            localtime_s(&expenseTimeInfo, &expense.timestamp);
            if (difftime(currentTime, expense.timestamp) < 86400 && timeInfo.tm_mday == expenseTimeInfo.tm_mday) {
                dailyExpenses[expense.category.name] += expense.amount;
            }
        }

        outFile << "Daily Expenses Report:" << endl;
        for (auto& pair : dailyExpenses) {
            outFile << pair.first << ": " << pair.second << endl;
        }
    }

    void showCardDeposits(string cardName) {
        cout << "Deposits for " << cardName << ":" << endl;
        for (const Expense& expense : expenses) {
            if (expense.amount > 0 && expense.description == "Deposit" && expense.category.name == cardName) {
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &expense.timestamp);
                cout << "Amount: " << expense.amount << ", Timestamp: " << buffer;
            }
        }
    }
};

int main() {
    FinanceManager manager;

    manager.addCard(Card("Debit Card"));
    manager.addCard(Card("Credit Card"));

    manager.addCategory(ExpenseCategory("Food"));
    manager.addCategory(ExpenseCategory("Entertainment"));
    manager.addCategory(ExpenseCategory("Transport"));

    manager.depositToCard("Debit Card", 1000);

    time_t now = time(nullptr);
    manager.addExpense("Dinner", 50, "Food", now - 2 * 86400);
    manager.addExpense("Movie tickets", 25, "Entertainment", now - 5 * 86400);
    manager.addExpense("Groceries", 100, "Food", now - 6 * 86400);
    manager.addExpense("Bus", 2, "Transport", now - 1 * 86400);
    manager.addExpense("Dinner", 50, "Food", now - 10 * 86400);
    manager.addExpense("Coffe", 40, "Food", now - 21 * 86400);
    manager.addExpense("Bought a bed", 500, "Entertainment", now - 3 * 86400);
    manager.addExpense("Gaming club", 2467, "Entertainment", now - 7 * 86400);
    manager.addExpense("Lessons of Croatian", 750, "Entertainment", now - 20 * 86400);
    manager.addExpense("Donut", 15, "Food", now - 4 * 86400);

    int choice;
    do {
        cout << "Choose an option:" << endl;
        cout << "1) Show card balance" << endl;
        cout << "2) Show expenses" << endl;
        cout << "3) Show card deposits" << endl;
        cout << "4) Show top three expenses for the week" << endl;
        cout << "5) Show top three expenses for the month" << endl;
        cout << "6) Show all cards" << endl;
        cout << "7) Save reports and ratings to file" << endl;
        cout << "0) Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1: {
            string cardName;
            cout << "Enter card name: ";
            cin >> cardName;
            manager.showCardBalance(cardName);
            break;
        }
        case 2:
            manager.showExpenses();
            break;
        case 3: {
            string cardName;
            cout << "Enter card name: ";
            cin >> cardName;
            manager.showCardDeposits(cardName);
            break;
        }
        case 4:
            manager.showTopExpensesWeekly();
            break;
        case 5:
            manager.showTopExpensesMonthly();
            break;
        case 6:
            manager.showAllCards();
            break;
        case 7:
            manager.saveReportsAndRatingsToFile();
            break;
        case 0:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please choose again." << endl;
        }
    } while (choice != 0);

    return 0;
}*/


/*

//Завдання 4, Шибениця

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

string chooseWord(const vector<string>& words) {
    srand(time(0));
    return words[rand() % words.size()];
}

string displayWord(const string& word, const vector<char>& guessedLetters) {
    string displayedWord;
    for (char letter : word) {
        if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            displayedWord += letter;
        }
        else {
            displayedWord += '_';
        }
        displayedWord += ' ';
    }
    return displayedWord;
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    ifstream inputFile("words.txt");
    if (!inputFile.is_open()) {
        cerr << "Помилка вiдкриття файлу зi словами." << endl;
        return 1;
    }

    vector<string> words;
    string word;
    while (inputFile >> word) {
        words.push_back(word);
    }
    inputFile.close();

    string chosenWord = chooseWord(words);
    const int MAX_TRIES = 6;
    int tries = 0;
    vector<char> guessedLetters;

    cout << "Ласкаво просимо до гри \"Шибениця\"!" << endl;

    while (tries < MAX_TRIES) {
        cout << "Слово: " << displayWord(chosenWord, guessedLetters) << endl;
        cout << "Введiть букву: ";
        char guess;
        cin >> guess;

        if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            cout << "Ви вже вводили цю букву. Спробуйте ще раз." << endl;
            continue;
        }

        guessedLetters.push_back(guess);

        if (chosenWord.find(guess) != string::npos) {
            cout << "Вiтаю! Ця буква є в словi." << endl;
            if (displayWord(chosenWord, guessedLetters).find('_') == string::npos) {
                cout << "Ви виграли! Загадане слово: " << chosenWord << endl;
                break;
            }
        }
        else {
            cout << "На жаль, цiєї букви немає в словi." << endl;
            tries++;
            cout << "Спроб залишилося: " << MAX_TRIES - tries << endl;
        }
    }

    if (tries == MAX_TRIES) {
        cout << "Ви програли. Загадане слово було: " << chosenWord << endl;
    }

    return 0;
}*/