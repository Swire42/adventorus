#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846
#define show cout << __LINE__ << endl;

using namespace std;

struct Point{
    double x; double y;
};

double distance(Point a, Point b){
    return ( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) );
}

int main(){
    int r = 10;
    int k = 1000;

    double caseSize = r/sqrt(2);

    int height = 100;
    int width = 100;

    int nbLines = height / caseSize +1;
    int nbColumns = width / caseSize +1;

    vector <Point> activeList;

    vector<vector<Point> > grid(nbLines, vector<Point>(nbColumns, {-1, -1}));

    /*for (int i = 0; i < nbLines; i++){
     grid.push_back(vector<Point>());
        for (int a = 0; a < nbColumns; a++){
            grid[i][a] = {-1, -1};
        }
    }*/

    Point firstPoint;
    firstPoint.x = rand()/(double)(RAND_MAX) * width;
    firstPoint.y = rand()/(double)(RAND_MAX) * height;

    int gridPlaceX = firstPoint.x / caseSize;
    int gridPlaceY = firstPoint.y / caseSize;

    grid[gridPlaceY][gridPlaceX] = firstPoint;

    activeList.push_back(firstPoint);


    while (!activeList.empty()){
        int indice = rand() % activeList.size();
        Point origine = activeList[indice];
        bool ajout = false;


        for (int i = 0; i < k; i++){
            Point Sample;
            long double factor = rand()/(double)(RAND_MAX)*r+r;
            long double angle = rand()/(long double)RAND_MAX *2*M_PI;
            Sample.x = origine.x + factor * cos(angle);
            Sample.y = origine.y + factor * sin(angle);

            if (Sample.x < 0 || Sample.x > width || Sample.y < 0 || Sample.y > height) continue;

            int gridSampleLin = Sample.y / caseSize;
            int gridSampleCol = Sample.x / caseSize;

            bool valide = true;

            for (int l = max(gridSampleLin-2, 0); l <= min(gridSampleLin+2, nbLines-1); l++){
                for (int c = max(gridSampleCol-2, 0); c <= min(gridSampleCol+2, nbColumns-1); c++){
                    if (grid[l][c].x != -1){
                        if ( distance(Sample, grid[l][c]) <= r*r ){
                            valide = false;

                        }
                    }
                }
            }
            if (valide){
                grid[gridSampleLin][gridSampleCol] = Sample;
                activeList.push_back(Sample);
                ajout = true;
                break;
            }
        }

        if (!ajout){
            activeList.erase( activeList.begin() + indice );
        }

    }

    cout << nbLines*nbColumns << endl;
    for (int i = 0; i < nbLines; i++){
        for (int a = 0; a < nbColumns; a++){
                cout << grid[i][a].x << ' ' << grid[i][a].y << "\n";
        }
    }
}
