#include <bits/stdc++.h>
#include "PD.hpp"

//#define M_PI 3.14159265358979323846

using namespace std;

const double r = 5;
const double  k = 10;
const long double caseSize = r/sqrt(2);

int height = 20;
int width = 20;

const int nbLines = height / caseSize + 1;
const int nbColumns = width / caseSize + 1; 

StPoisonDisk PD = {r, k};

vector<Coord> PDPoint::generate(int k){

    vector<Coord> children;

    for (int i = 0; i < k; i++){
        long double factor = rand()/(double)(RAND_MAX)*PD.radius+PD.radius;
        long double angle = rand()/(double)RAND_MAX *2*M_PI;
        children.push_back( Coord(p.x + factor * cos(angle), p.y + factor * sin(angle) ) );
    }

    return children;
}

double distance(PDPoint a, PDPoint b){
    return ( (a.p.x-b.p.x)*(a.p.x-b.p.x) + (a.p.y-b.p.y)*(a.p.y-b.p.y) );
}


void StPoisonDisk::init(){
    //Initialising the grid 
    PD.grid = vector< vector<int> >(nbLines, vector<int>(nbColumns, -1) );

    //Creating the first point
    double x =  rand()/(double)(RAND_MAX) * width;
    double y = rand()/(double)(RAND_MAX) * height;
    PDPoint firstPDPoint( Coord(x, y) );

    int gridPlaceX = firstPDPoint.p.x / caseSize;
    int gridPlaceY = firstPDPoint.p.y / caseSize;

    grid[gridPlaceY][gridPlaceX] = 0; //Affecting the index of the point index 

    PD.points.push_back(firstPDPoint); //Adding the point in the points list
    PD.active.push_back(0); //Adding its index to the active list

}

bool StPoisonDisk::isFree(PDPoint p, int gridSampleLin, int gridSampleCol){

    bool valide = true;

    for (int l = max(gridSampleLin-2, 0); l <= min(gridSampleLin+2, nbLines-1); l++){
        for (int c = max(gridSampleCol-2, 0); c <= min(gridSampleCol+2, nbColumns-1); c++){
            if (points[ grid[l][c] ].p.x != -1){
                if ( distance( p, points[ grid[l][c] ] ) <= r*r ){
                    valide = false;

                }
            }
        }
    }

    return valide;
}

void StPoisonDisk::compute(){

    while (!PD.active.empty()){
        int indice = PD.active[ rand() % active.size() ]; //Choosing an index of a random point in the active list
        PDPoint origine = PD.points[indice]; //Extracting this point from the point using its index
        
        bool ajout = false;

        vector<Coord> Samples = origine.generate(k); //Generating k children

        for (auto i : Samples){
            PDPoint p(i);

            if (i.x < 0 || i.x > width || i.y < 0 || i.y > height) continue;

            int gridSampleLin = i.y / caseSize;
            int gridSampleCol = i.x / caseSize;

            if ( isFree(p, gridSampleLin, gridSampleCol) ){

                //Adding the index of the child in the grid and in the active list
                grid[gridSampleLin][gridSampleCol] = PD.points.size();
                PD.active.push_back( grid[gridSampleLin][gridSampleCol] ); 

                PD.points.push_back( p ); //Adding the child to the points list
                ajout = true;

            }
        }
        if (!ajout){
         PD.active.erase( PD.active.begin() + indice );
        }
    }
}



int main(){
    PD.init();
    PD.compute();
}


