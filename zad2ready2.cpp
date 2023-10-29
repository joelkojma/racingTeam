// Zadanie 2 Joel Kojma Algorytm Grahama
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

struct point
{
    float x, y;
    float cos;
};

int n; // liczba punktow

float dist(point a, point b) // odleglosc miedzy dwoma punktami
{
    return (sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

float cosinus(point a, point b) // cosinus kata miedzy prosta rownolegla do osi x przechodzaca przez punkt a, i punktem b
{
    return ((b.x - a.x) / (dist(a, b)));
}

void calculateCosinsus(int starting, point *punkty) // policzenie cosinusow dla wszystkich punktow wzgledem punktu startowego
{
    for (int i = 1; i < n; i++)
    {
        punkty[i].cos = cosinus(punkty[starting], punkty[i]);
    }
}

void sortByCosinus(int starting, point *punkty) // posortowanie punktow wzgledem katow
{
    calculateCosinsus(starting, punkty);
    sort(punkty + 1, punkty + n, [](point a, point b)
         { return a.cos > b.cos; });
}

void findMin(point *punkty) // ustawianie elementu o najmniejszym y (a jesli jest ich wiecej to wtedy sposrod tych o najmniejszym x) na zerowy index tablicy punktow
{
    int min_Index = 0;
    for (int i = 1; i < n; i++)
    {
        if (punkty[min_Index].y > punkty[i].y)
            min_Index = i;
        else if ((punkty[min_Index].y == punkty[i].y) && (punkty[min_Index].x > punkty[i].x))
            min_Index = i;
    }
    swap(punkty[0], punkty[min_Index]);
}

string direction(point a, point b, point c) // a - nowy punkt, odcinek b->c 
{
    float value = (c.x - b.x) * (a.y - b.y) - (a.x - b.x) * (c.y - b.y); //wzor iloczyn wektorowy 
    if (value > 0)
        return "left";
    if (value == 0)
        return "straight";
    return "right";
}

void findEdges(int *edges, int &countEdges, point *punkty) // znajdowanie punktow, ktore otaczaja wszystkie inne punkty
{
    edges[0] = 0, edges[1] = 1;
    for (int i = 2; i < n; i++)
    {
        edges[countEdges] = i; // kolejny punkt dodaje do otoczki
        // w petli sprawdzam czy z punktow czasem nie tworzy sie skret w lewo. W takim przypadku przedostatni punkt z tablicy jest usuwany
        while (direction(punkty[edges[countEdges]], punkty[edges[countEdges - 2]], punkty[edges[countEdges - 1]]) != "left" && (countEdges > 1))
        {
            edges[countEdges - 1] = edges[countEdges];
            countEdges--;
        }
        countEdges++;
    }
    //Jeszcze trzeba sprawdzic czy dwa ostatnie punkty(rogi) nie leza na tej samej prostej co punkt startowy
    while (direction(punkty[edges[0]], punkty[edges[countEdges - 2]], punkty[edges[countEdges - 1]]) != "left")
    {
        edges[countEdges - 1] = edges[countEdges];
        countEdges--;
    }
}

int main()
{
    fstream file1;
    string fileName;
    cout << "Podaj nazwe pliku z danymi: ";
    cin >>fileName;
    file1.open(fileName); // Zaczynam pobierac dane
    if (!file1.is_open())
        cerr << "File not opened";
    file1 >> n;
    point punkty[n]; // Points
    for (int i = 0; i < n; i++)
    {
        file1 >> punkty[i].x;
        file1 >> punkty[i].y;
    }
    file1.close(); // koniec pobierania danych

    findMin(punkty); // szukam punktu polozonego najnizej (jesli wiecej to najnizej z lewej)
    int edges[n], countEdges = 2; // tablica do przechowywania indexow punktow na otoczce;
    sortByCosinus(0, punkty);     // punkt startowy ma index 0
    findEdges(edges, countEdges, punkty); //znajduje wszystkie punkty
    
    cout << "[";
    for (int i = 0; i < countEdges - 1; i++)
    {
        cout << "(" << punkty[edges[i]].x << ", " << punkty[edges[i]].y << "), ";
    }
    cout << "(" << punkty[edges[countEdges - 1]].x << ", " << punkty[edges[countEdges - 1]].y << ")]";
    return 0;
}