#include <iostream>
#include <windows.h>
#include <string>
#include <stack>
#include <fstream>

using namespace std;

stack <int> Stack;
stack <string> stack_string;
string otveti[100000];
const char oper[3] = { '+','-','*' };
int position_question[100];
int  otvet = 0, kk = 0;
ifstream open;
ofstream close;

//формирование новой части строки
string new_part(string String, int start, int finish) {
    string new_String = "";

    for (int i = 0; i < finish; i++)
        if (start + i < String.size()) {
            new_String += String[start + i];
        }
        else break;
    return new_String;
}

//высчитывание результата в строке
int Calculate(string String) {
    int i = 0, j = 0, sborka = 0, number_1 = 0, number_2 = 0, kol = 0;

    while (i < String.size()) {
        j = 0;
        //если число собираем его, а затем кладем в стэк
        while (String[i + j] >= '0' && String[i + j] <= '9') j++;
        if (j > 0) {
            sborka = stoi(String.substr(i, j));
            Stack.push(sborka);
            i += j;
            //sborka = 0;
        }
        //если не число, то либо это пробел и мы ничего не делаем, либо операция, которую мы выполняем с 2 числами из стэка
        else
        {
            if (String[i] != ' ') {
                number_1 = Stack.top();
                Stack.pop();
                number_2 = Stack.top();
                Stack.pop();
            }
            switch (String[i]) {
            case ' ': break;
            case '+': Stack.push(number_1 + number_2); break;
            case '-': Stack.push(-number_1 + number_2); break;
            case '*': Stack.push(number_1 * number_2); break;
            }
            i++;
        }

    }
    //достаем получившийся результат из стэка, стэк обнуляем и возвращаем результат
    int rez = Stack.top();
    Stack.pop();
    return rez;
}

//перевод из постфиксной в инфиксную запись
string perevod(string String) {
    int i = 0, j = 0, sborka = 0;
    string new_String, part_1, part_2, part;
    while (i < String.size()) {
        j = 0;
        while (String[i + j] >= '0' && String[i + j] <= '9') j++;
        if (j > 0) {
            part = String.substr(i, j);
            stack_string.push(part);
            i += j;
        }
        else {
            if (String[i] != ' ') {
                part_1 = stack_string.top();
                stack_string.pop();
                part_2 = stack_string.top();
                stack_string.pop();
            }
            switch (String[i]) {
            case ' ': break;
            case '+': stack_string.push(part_2 + '+' + part_1); break;
            case '-': stack_string.push(part_2 + '-' + part_1); break;
            case '*': stack_string.push(part_2 + '*' + part_1); break;
            }
            i++;
        }
    }
    new_String = stack_string.top();
    stack_string.pop();
    return new_String;
}

//проверка правильности выбранной комбинации, так как скобки не ставятся
int proverka(string String) {
    int l = 0, p = 0, number_1 = 0, number_2 = 0, kol_oper = 0;
    string men;

    for (int i = 0; i < String.size(); i++)
        if (String[i] == '+' || String[i] == '-' || String[i] == '*') kol_oper++;

    for (int i = 1; i < String.size(); i++)
        if (String[i] == '*') {
            do {
                p++;
            } while (String[i + p + 1] >= '0' && String[i + p + 1] <= '9');
            do {
                l++;
            } while (i - l - 1 >= 0 && String[i - l - 1] >= '0' && String[i - l - 1] <= '9');
            number_1 = stoi(String.substr(i + 1, p));
            number_2 = stoi(String.substr(i - l, l));
            men = to_string(number_1 * number_2);
            String.erase(i - l, l + p + 1);
            String.insert(i - l, men, 0, men.size());
            kol_oper--;
            l = 0;
            p = 0;
            i = 0;
        }

    for (int i = 1; i < String.size(); i++) {
        if (kol_oper == 0) break;
        //if (String[i - 1] == '-') break;
        if (String[i] == '+') {
            do {
                p++;
            } while (String[i + p + 1] >= '0' && String[i + p + 1] <= '9');
            do {
                l++;
            } while (i - l - 1 >= 0 && String[i - l - 1] >= '0' && String[i - l - 1] <= '9');
            //cout << i - l - 1 << endl;
            if (i - l - 1 == 0)
                if (String[i - l - 1] != '-') {
                    number_1 = stoi(String.substr(i + 1, p));
                    number_2 = stoi(String.substr(i - l, l));
                    men = to_string(number_2 + number_1);
                    String.erase(i - l, l + p + 1);
                    String.insert(i - l, men, 0, men.size());
                }
                else {
                    number_1 = stoi(String.substr(i + 1, p));
                    number_2 = stoi(String.substr(i - l, l));
                    if (number_1 > number_2) men = to_string(number_1 - number_2);
                    else men = '-' + to_string(number_2 - number_1);
                    //men = '-' + to_string(number_2 - number_1);
                    String.erase(i - l - 1, l + p + 2);
                    String.insert(i - l - 1, men, 0, men.size());
                }
            else {
                number_1 = stoi(String.substr(i + 1, p));
                number_2 = stoi(String.substr(i - l, l));
                men = to_string(number_2 + number_1);
                String.erase(i - l, l + p + 1);
                String.insert(i - l, men, 0, men.size());
            }
            kol_oper--;
            l = 0;
            p = 0;
            i = 0;
        }
        else if (String[i] == '-') {
            do {
                p++;
            } while (String[i + p + 1] >= '0' && String[i + p + 1] <= '9');
            do {
                l++;
            } while (i - l - 1 >= 0 && String[i - l - 1] >= '0' && String[i - l - 1] <= '9');
            //cout << i - l - 1 << endl;
            if (i - l - 1 == 0)
                if (String[i - l - 1] != '-') {
                    number_1 = stoi(String.substr(i + 1, p));
                    number_2 = stoi(String.substr(i - l, l));
                    men = to_string(number_2 - number_1);
                    String.erase(i - l, l + p + 1);
                    String.insert(i - l, men, 0, men.size());
                }
                else {
                    number_1 = stoi(String.substr(i + 1, p));
                    number_2 = stoi(String.substr(i - l, l));
                    men = '-' + to_string(number_2 + number_1);
                    String.erase(i - l - 1, l + p + 2);
                    String.insert(i - l - 1, men, 0, men.size());
                }
            else {
                number_1 = stoi(String.substr(i + 1, p));
                number_2 = stoi(String.substr(i - l, l));
                men = to_string(number_2 - number_1);
                String.erase(i - l, l + p + 1);
                String.insert(i - l, men, 0, men.size());
            }
            kol_oper--;
            l = 0;
            p = 0;
            i = 0;
        }
        //cout << String << "|||" << endl;
    }
    //сout << kol_oper << "      ";
    return stoi(String);
}
//замена знаков вопроса на операции
void zamena(string String, int kol, int result) {
    string new_String, n_string;
    int j = 0, mesto_znaka = 0, perehod = 0, first_question = 0;
    int znak[30] = { 0 };

    first_question = String.find('?', 0);

    if (first_question < 1 || kol < 1) return;

    for (int i = 0; i < String.size();i++)
        if (String[i] == '?') j++;

    //Переребор всез возможных операций вместо последнего знака вопроса
    if (j == 1) {
        for (int i = 0; i < kol; i++) {
            znak[i] = 0;
        }
        do {
            new_String = "";
            for (int i = 0; i < kol; i++)
                new_String += oper[znak[i]];
            n_string = String.substr(0, String.find('?', 0)) + new_String;
            //cout << n_string << "           " << Calculate(n_string) << endl;
            if (Calculate(n_string) == result) {
                if (proverka(perevod(n_string)) == result) {
                    for (int i = 0; i < otvet; i++)
                        if (otveti[i] == perevod(n_string)) break;
                        else kk++;
                    //if (otvet == 30) exit(0);
                    if (kk == otvet) { otveti[otvet] = perevod(n_string); /*cout << otveti[otvet] << ' ' << kk << endl; */close << perevod(n_string) << " = " << result << endl; otvet++; }
                    kk = 0;
                }
                //cout << n_string << "                " << perevod(n_string) << "                   " << proverka(perevod(n_string)) << endl;
            }
            perehod = 1;
            for (int i = kol - 1; i >= 0; i--) {
                znak[i] += perehod;
                if (znak[i] > 2) {
                    perehod = 1;
                    znak[i] = 0;

                }
                else perehod = 0;
            }
        } while (perehod != 1);
    }
    else {
        first_question = String.find('?', 0);
        for (int i = 0; i < kol; i++) {
            for (int o = 0; o <= i; o++)
                znak[o] = 0;
            do {
                new_String = "";
                for (int o = 0; o <= i; o++)
                    new_String += oper[znak[o]];
                //cout << new_String << endl;
                new_String = String.substr(0, first_question) + new_String + String.substr(first_question + 1, String.size() - first_question - 1);
                zamena(new_String, kol - i, result);

                perehod = 1;
                for (int o = i; o >= 0; o--) {
                    znak[o] += perehod;
                    if (znak[o] > 2) {
                        perehod = 1;
                        znak[o] = 0;
                    }
                    else perehod = 0;
                }
            } while (perehod != 1);
        }
        new_String = String.substr(0, first_question) + ' ' + String.substr(first_question + 1, String.size() - first_question - 1);
        zamena(new_String, kol + 1, result);
    }

}

//формирование всех возможных вариаций знаков и чисел
void perebor(string String, string Right, int result) {
    string l, r;

    //запуск функции для проверки всех возможных вариантов данной комбинации
    zamena(String + ' ' + Right, 1, result);

    //формирование нового варианта
    for (int i = 1; i < String.size(); i++) {
        l = new_part(String, 0, i);
        r = new_part(String, i, String.size() - i);
        perebor(l, r + " ? " + Right, result);
    }
}
int main()
{
    string number_string, result_string;
    int result;

    open.open("input.txt");
    close.open("output.txt");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //cout << "Введите шестизначное число :";
    getline(open, number_string);
    //Проверка на некорректность ввода
    if (number_string.size() != 6) {
        close << "Некорректный ввод!!! Сказали же - 6 символов!";
        return 0;
    }
    else
        for (int i = 0; i < number_string.size();i++)
            if (number_string[i] < '0' || number_string[i] > '9') {
                close << "Некорректный ввод!!! Сказали же - число!";
                return 0;
            }
    //close << endl << "Введите значение суммы :";
    getline(open, result_string);
    for (int i = 0; i < result_string.size();i++)
        if (result_string[i] < '0' || result_string[i] > '9') {
            close << "Некорректный ввод!!! Сказали же - число!";
            return 0;
        }

    result = stoi(result_string);

    perebor(number_string, "", result);

    if (otvet == 0) close << "Прошу прощения, но кажется ответов на данный пример не существует!";

    return 0;
}