#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

const string ClientFile = "Clients.txt";

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0.0;
};

sClient TargetClient;

// Function prototypes
int MenuScreen();
void ReturnToMenuScreen();
void ATMOptions(int Num);
bool IsExisted(string Acc, string PinCode);
void SaveToFile();

/////////////////////////////////////////////////////////////////////////////////

vector<string> SplitString(string Line, string Delim = "/") {
    vector<string> Words;
    short pos;
    string sWord;

    while ((pos = Line.find(Delim)) != std::string::npos) {
        sWord = Line.substr(0, pos);
        if (sWord != "")
            Words.push_back(sWord);
        Line.erase(0, pos + Delim.length());
    }
    if (Line != "")
        Words.push_back(Line);
    return Words;
}

sClient ConvertLineToStruct(string Line) {
    sClient Client;
    vector<string> vsClient = SplitString(Line);

    if (vsClient.size() < 5) {
        Client.AccountNumber = "";
        return Client; // invalid line
    }

    Client.AccountNumber = vsClient[0];
    Client.PinCode = vsClient[1];
    Client.Name = vsClient[2];
    Client.Phone = vsClient[3];
    Client.AccountBalance = stod(vsClient[4]);
    return Client;
}

vector<sClient> UploadFromFile() {
    vector<sClient> vsClient;
    fstream MyFile(ClientFile, ios::in);

    if (MyFile.is_open()) {
        string Line;
        while (getline(MyFile, Line)) {
            sClient Client = ConvertLineToStruct(Line);
            if (Client.AccountNumber != "")
                vsClient.push_back(Client);
        }
        MyFile.close();
    }
    return vsClient;
}

string ConvertStructToLine(sClient Client) {
    string Line = "";
    Line += Client.AccountNumber + "/";
    Line += Client.PinCode + "/";
    Line += Client.Name + "/";
    Line += Client.Phone + "/";
    Line += to_string(Client.AccountBalance);
    return Line;
}

bool IsExisted(string Acc, string PinCode) {
    vector<sClient> vsClient = UploadFromFile();
    for (sClient s : vsClient) {
        if (s.AccountNumber == Acc && s.PinCode == PinCode) {
            TargetClient = s;
            return true;
        }
    }
    return false;
}

void SaveToFile() {
    vector<sClient> vsClient = UploadFromFile();
    fstream MyFile(ClientFile, ios::out | ios::trunc);

    for (sClient& s : vsClient) {
        if (s.AccountNumber == TargetClient.AccountNumber) {
            MyFile << ConvertStructToLine(TargetClient) << endl;
        }
        else {
            MyFile << ConvertStructToLine(s) << endl;
        }
    }
    MyFile.close();
}

/////////////////////////////////////
// Quick Withdraw

void QuickMinus(double Num) {
    if (TargetClient.AccountBalance < Num) {
        cout << "\nThe amount exceeds your balance.\n";
        return;
    }
    TargetClient.AccountBalance -= Num;
}

void QuickWithdraw(int Num) {
    char is;
    cout << "\nAre you sure you want to perform this transaction? (y/n): ";
    cin >> is;
    if (tolower(is) == 'y') {
        QuickMinus(Num);
        SaveToFile();
        cout << "\nDone successfully. New balance: " << TargetClient.AccountBalance << endl;
    }
}

void QuickOption(int num) {
    switch (num) {
    case 1: QuickWithdraw(20); break;
    case 2: QuickWithdraw(50); break;
    case 3: QuickWithdraw(100); break;
    case 4: QuickWithdraw(200); break;
    case 5: QuickWithdraw(400); break;
    case 6: QuickWithdraw(600); break;
    case 7: QuickWithdraw(800); break;
    case 8: QuickWithdraw(1000); break;
    case 9: system("cls"); ReturnToMenuScreen(); break;
    default: cout << "\nInvalid option.\n"; break;
    }
}

void QuickWithdrawMenu() {
    cout << "\n-------------------------------------------------\n";
    cout << "               Quick Withdraw\n";
    cout << "-------------------------------------------------\n";
    cout << "\t[1] 20\t\t[2] 50\n\t[3] 100\t\t[4] 200\n\t[5] 400\t\t[6] 600\n\t[7] 800\t\t[8] 1000\n\t[9] Exit\n";
    cout << "-------------------------------------------------\n";
    cout << "\nYour Balance: " << TargetClient.AccountBalance << endl;

    int choose;
    cout << "Choose amount to withdraw (1-8): ";
    cin >> choose;
    QuickOption(choose);
}

/////////////////////////////////////
// Normal Withdraw

void NormalMinus(double Num) {
    if (TargetClient.AccountBalance < Num) {
        cout << "\nYou exceeded your balance.\n";
        return;
    }
    TargetClient.AccountBalance -= Num;
}

void NormalWithdraw() {
    char is;
    double num;

    while (true) {
        cout << "\nEnter an amount (multiple of 5): ";
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        if (fmod(num, 5) == 0) {
            cout << "Confirm transaction (y/n): ";
            cin >> is;
            if (tolower(is) == 'y') {
                NormalMinus(num);
                SaveToFile();
                cout << "Done successfully. New balance: " << TargetClient.AccountBalance << endl;
            }
            break;
        }
        else {
            cout << "Amount must be multiple of 5.\n";
        }
    }
}

void NormalWithdrawMenu() {
    cout << "\n========================================================\n";
    cout << "                Normal Withdraw Screen\n";
    cout << "========================================================\n";
    NormalWithdraw();
}

/////////////////////////////////////
// Deposit

void AddMoney(double Num) {
    TargetClient.AccountBalance += Num;
}

void Deposit() {
    double Num;
    char is;

    cout << "\n========================================================\n";
    cout << "                  Deposit Screen\n";
    cout << "========================================================\n";
    cout << "\nEnter amount to deposit: ";
    cin >> Num;
    cout << "Confirm deposit (y/n): ";
    cin >> is;
    if (tolower(is) == 'y') {
        AddMoney(Num);
        SaveToFile();
        cout << "\nDone successfully. New balance: " << TargetClient.AccountBalance << endl;
    }
}

/////////////////////////////////////
// Check Balance

void CheckBalanceScreen() {
    cout << "\n========================================================\n";
    cout << "               Check Balance Screen\n";
    cout << "========================================================\n";
    cout << "\nYour Balance: " << TargetClient.AccountBalance << endl;
}

/////////////////////////////////////

void ReturnToMenuScreen() {
    cout << "\n\nPress any key to go back to main menu...";
    system("pause>0");
    system("cls");
    ATMOptions(MenuScreen());
}

void AskLogin() {
    string Acc, PinCode;
    do {
        system("cls");
        cout << "\n-------------------------------------------------\n";
        cout << "                    Login Screen\n";
        cout << "-------------------------------------------------\n";
        cout << "Enter Account Number: ";
        cin >> Acc;
        cout << "Enter Pin Code: ";
        cin >> PinCode;

        if (!IsExisted(Acc, PinCode)) {
            cout << "\nInvalid Account Number or Pin Code!\n";
            system("pause>0");
        }
    } while (!IsExisted(Acc, PinCode));

    system("cls");
    ATMOptions(MenuScreen());
}

int MenuScreen() {
    cout << "\n-------------------------------------------------\n";
    cout << "               ATM Main Menu Screen\n";
    cout << "-------------------------------------------------\n";
    cout << "[1] Quick Withdraw\n";
    cout << "[2] Normal Withdraw\n";
    cout << "[3] Deposit\n";
    cout << "[4] Check Balance\n";
    cout << "[5] Logout\n";
    cout << "-------------------------------------------------\n";
    int Num;
    cout << "Choose option [1-5]: ";
    cin >> Num;
    return Num;
}

void ATMOptions(int Num) {
    system("cls");
    switch (Num) {
    case 1: QuickWithdrawMenu(); ReturnToMenuScreen(); break;
    case 2: NormalWithdrawMenu(); ReturnToMenuScreen(); break;
    case 3: Deposit(); ReturnToMenuScreen(); break;
    case 4: CheckBalanceScreen(); ReturnToMenuScreen(); break;
    case 5: cout << system("pause>0"); AskLogin(); return;
    default: cout << "Invalid choice.\n"; ReturnToMenuScreen(); break;
    }
}

void LoginScreen() {
    AskLogin();
}

int main() {
    LoginScreen();
    return 0;
}
