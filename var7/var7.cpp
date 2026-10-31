#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

int getSumSquareDigits(int number)
{
    int temp = number;
    int sum = 0;

    while (temp > 0)
    {
        int digit = temp % 10;
        sum += digit * digit;
        temp /= 10;
    }

    return sum;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    int startRange = 0;
    int endRange = 0;

    if (argc >= 3)
    {
        startRange = stoi(argv[1]);
        endRange = stoi(argv[2]);
    }
    else
    {
        cout << "Введите начальное число диапазона: ";
        cin >> startRange;
        cout << "Введите конечное число диапазона: ";
        cin >> endRange;
    }

    if (startRange > endRange)
    {
        int temp = startRange;
        startRange = endRange;
        endRange = temp;
    }

    cout << "Диапазон: от " << startRange << " до " << endRange << endl;

    int maxNumber = startRange;
    int maxSumSquares = -1;

    for (int i = startRange; i <= endRange; i++)
    {
        int currentSum = getSumSquareDigits(i);
        cout << "Число: " << i << ", сумма квадратов цифр: " << currentSum << endl;

        if (currentSum > maxSumSquares)
        {
            maxSumSquares = currentSum;
            maxNumber = i;
        }
    }

    cout << "Число с максимальной суммой квадратов цифр: " << maxNumber << endl;
    cout << "Сумма квадратов: " << maxSumSquares << endl;

    _getch();

    return 0;
}