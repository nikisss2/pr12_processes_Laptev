#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>

using namespace std;

struct ProcessData
{
    string name;
    PROCESS_INFORMATION pi;
    bool isAlive;
};

const int MAX_PROCESSES = 30;
ProcessData procList[MAX_PROCESSES];
int procCount = 0;

void updateProcessStatus()
{
    for (int i = 0; i < procCount; i++)
    {
        if (procList[i].isAlive)
        {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(procList[i].pi.hProcess, &exitCode))
            {
                if (exitCode != STILL_ACTIVE)
                {
                    procList[i].isAlive = false;
                }
            }
        }
    }
}

bool runNewProcess(string name, string cmdLine)
{
    if (procCount >= MAX_PROCESSES)
    {
        cout << "Достигнут лимит процессов" << endl;
        return false;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    ZeroMemory(&piApp, sizeof(PROCESS_INFORMATION));

    char cmd[512];
    strcpy_s(cmd, sizeof(cmd), cmdLine.c_str());

    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
    {
        cout << "Процесс не получилось создать" << endl;
        return false;
    }

    procList[procCount].name = name;
    procList[procCount].pi = piApp;
    procList[procCount].isAlive = true;
    procCount++;

    cout << "Процесс успешно создан. ID: " << piApp.dwProcessId << endl;
    return true;
}

void showProcesses()
{
    updateProcessStatus();

    if (procCount == 0)
    {
        cout << "Список пуст" << endl;
        return;
    }

    for (int i = 0; i < procCount; i++)
    {
        cout << i + 1 << ". Программа: " << procList[i].name << ", ID: " << procList[i].pi.dwProcessId << ", Состояние: ";

        if (procList[i].isAlive)
        {
            cout << "Работает" << endl;
        }
        else
        {
            cout << "Завершен" << endl;
        }
    }
}

void showActiveCount()
{
    updateProcessStatus();

    int activeCount = 0;
    for (int i = 0; i < procCount; i++)
    {
        if (procList[i].isAlive)
        {
            activeCount++;
        }
    }

    cout << "Количество работающих процессов: " << activeCount << endl;
}

void stopProcess()
{
    updateProcessStatus();
    showProcesses();

    if (procCount == 0)
    {
        return;
    }

    cout << "Введите номер процесса для завершения: ";
    int num = 0;
    cin >> num;

    if (num < 1 || num > procCount)
    {
        cout << "Некорректный номер" << endl;
        return;
    }

    int index = num - 1;
    if (!procList[index].isAlive)
    {
        cout << "Этот процесс уже завершен" << endl;
        return;
    }

    if (TerminateProcess(procList[index].pi.hProcess, 0))
    {
        procList[index].isAlive = false;
        cout << "Процесс успешно завершен" << endl;
    }
    else
    {
        cout << "Не удалось завершить процесс" << endl;
    }
}

void createProcessMenu()
{
    cout << "Выберите программу для запуска:" << endl;
    cout << "1. taskmgr.exe (Диспетчер задач)" << endl;
    cout << "2. ping.exe (Проверка сетевого узла)" << endl;
    cout << "3. Вариант 7 (Сумма квадратов цифр)" << endl;
    cout << "Выбор: ";

    int appChoice = 0;
    cin >> appChoice;

    if (appChoice == 1)
    {
        runNewProcess("taskmgr.exe", "taskmgr.exe");
    }
    else if (appChoice == 2)
    {
        cout << "Выберите режим запуска ping: " << endl;
        cout << "1. 4 запроса: ping.exe " << endl;
        cout << "2. Заданное число N запросов: ping.exe -n " << endl;
        cout << "3. Непрерывно: ping.exe -t " << endl;
        cout << "Выбор: ";

        int pingMode = 0;
        cin >> pingMode;

        string ip;
        cout << "Введите ip: ";
        cin >> ip;

        string command = "";

        if (pingMode == 1)
        {
            command = "ping.exe " + ip;
        }
        else if (pingMode == 2)
        {
            int n = 4;
            cout << "Введите количество запросов N: ";
            cin >> n;
            command = "ping.exe -n " + to_string(n) + " " + ip;
        }
        else if (pingMode == 3)
        {
            command = "ping.exe -t " + ip;
        }
        else
        {
            cout << "Некорректный режим" << endl;
            return;
        }

        runNewProcess("ping.exe", command);
    }
    else if (appChoice == 3)
    {
        string pathProc = "\\\\Mac\\Home\\Desktop\\Программирование\\СП_Гариев\\pr12_processes_Laptev\\ARM64\\Debug\\var7.exe";

        int startVal = 0;
        int endVal = 0;

        cout << "Введите начальное число диапазона: ";
        cin >> startVal;
        cout << "Введите конечное число диапазона: ";
        cin >> endVal;

        string command = "\"" + pathProc + "\" " + to_string(startVal) + " " + to_string(endVal);
        runNewProcess("var7.exe", command);
    }
    else
    {
        cout << "Неверный выбор" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    while (true)
    {
        cout << "\n1. Создать процесс" << endl;
        cout << "2. Завершить процесс" << endl;
        cout << "3. Список всех процессов" << endl;
        cout << "4. Количество работающих процессов" << endl;
        cout << "5. Выход" << endl;
        cout << "Выберите действие: ";

        int mainChoice = 0;
        cin >> mainChoice;

        if (mainChoice == 1)
        {
            createProcessMenu();
        }
        else if (mainChoice == 2)
        {
            stopProcess();
        }
        else if (mainChoice == 3)
        {
            showProcesses();
        }
        else if (mainChoice == 4)
        {
            showActiveCount();
        }
        else if (mainChoice == 5)
        {
            break;
        }
        else
        {
            cout << "Неверный пункт меню" << endl;
        }
    }

    for (int i = 0; i < procCount; i++)
    {
        CloseHandle(procList[i].pi.hProcess);
        CloseHandle(procList[i].pi.hThread);
    }

    return 0;
}