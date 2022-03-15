#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "init_data.h"

using namespace std;


void readData()
{
    ifstream fin ("/home/serafim/Рабочий стол/comp_alg/lab_1/initData.txt");
    if (fin.is_open()) {
        for (int k = 0; k < dataSize; ++k) {
            for (int i = 0; i < dataSize; ++i) {
                for (int j = 0; j < dataSize; ++j) {
                    fin >> initData[i][j][k];
                }
            }
        }
        fin.close();
    }
}


void printInitialData()
{
    for (int k = 0; k < dataSize; ++k)
    {
        cout << "z = " << k << endl;
        for (int i = 0; i < dataSize; ++i)
        {
            for (int j = 0; j < dataSize; ++j)
            {
                cout << initData[i][j][k] << ' ';
            }
            cout << endl;
        }
    }
}


vector <vector <double> > getTableOfSepDiff(vector <int> argInds, vector <double> data)
{
    int size = data.size();
    vector <vector <double> > tableOfSepDiff;
    tableOfSepDiff.push_back(data);

    for (int i = 1; i < size; ++i)
    {
        vector <double> curLine, prevLine = tableOfSepDiff[i - 1];
        for (int j = 0; j < prevLine.size() - 1; ++j)
        {
            double xDiff = 1.0;
            curLine.push_back((prevLine[j + 1] - prevLine[j]) / xDiff);
        }
        tableOfSepDiff.push_back(curLine);
    }
    return tableOfSepDiff;
}


double countRes(vector <int> nearest, vector <vector<double>> tableOfSepDiff, double arg)
{
    double res = 0;
    for (int i = 0; i < tableOfSepDiff.size(); ++i)
    {
        double cur_res = tableOfSepDiff[i][0];
        for (int j = 0; j < i; ++j)
        {
            cur_res *= arg - args[nearest[j]];
        }
        res += cur_res;
    }
    return res;
}


vector <int> getNearest(double arg, int power)
{
    power++;
    vector <int> indexesOfArgs;
    if (power > dataSize)
        power = dataSize;

    int centerIndex = lower_bound(args.begin(), args.end(), arg) - args.begin();
    int begIndex = max(0, centerIndex - power / 2);
    int endIndex = min(dataSize, centerIndex + power / 2 + power % 2);


    if (endIndex - begIndex != power)
    {
        int dif = power - endIndex + begIndex;
        begIndex = max(0, begIndex - dif);
        endIndex = min(dataSize, endIndex + dif);
    }

    for (int i = begIndex; i < endIndex; ++i)
        indexesOfArgs.push_back(i);

    return indexesOfArgs;
}


double getValueOfOne(double x, int y, int z, int nx)
{
    vector <int> nearest = getNearest(x, nx);
    vector <double> data;
    for (int i = 0; i < nearest.size(); ++i)
    {
        data.push_back(initData[nearest[i]][y][z]);
    }
    vector <vector <double>> tableOfSepDiff = getTableOfSepDiff(nearest, data);
    double res = countRes(nearest, tableOfSepDiff, x);
    return res;
}


double getValueOfTwo(double x, double y, int z, int nx, int ny)
{
    vector <int> nearest = getNearest(y, ny);
    vector <double> data;
    for (int i = 0; i < nearest.size(); ++i) {
        data.push_back(getValueOfOne(x, args[nearest[i]], z, nx));
    }
    vector <vector <double>> tableOfSepDiff = getTableOfSepDiff(nearest, data);
    return countRes(nearest, tableOfSepDiff, y);
}


double getValueOfThree(double x, double y, double z, int nx, int ny, int nz)
{
    vector <int> nearest = getNearest(z, nz);
    vector <double> data;
    for (int i = 0; i < nearest.size(); ++i) {
        data.push_back(getValueOfTwo(x, y, args[nearest[i]], nx, ny));
    }
    vector <vector <double>> tableOfSepDiff = getTableOfSepDiff(nearest, data);
    return countRes(nearest, tableOfSepDiff, z);
}


int main()
{
    readData();
    printInitialData();

    int nx = 3, ny = 3, nz = 3;
    double x, y, z;

    cout << endl << "Введите точку, в которой нужно найти значение (x, y, z)" << endl;
    cin >> x >> y >> z;
    cout << getValueOfThree(x, y, z, nx, ny, nz) << endl;

    return 0;
}
