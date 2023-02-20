// Markus Buchholz, 2023

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <random>
#include <numeric>

//--------------------------------------------------------------------------------

float xmin = -5.0;
float xmax = 5.0;
float ymin = -5.0;
float ymax = 5.0;

int EVOLUTIOINS = 100;
int ANT = 20;
int LION = 20;
//--------------------------------------------------------------------------------

struct Pos
{

    float x;
    float y;
};

//--------------------------------------------------------------------------------

float generateRandom()
{

    std::random_device engine;
    std::mt19937 gen(engine());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(gen);
}

//--------------------------------------------------------------------------------

float valueGenerator(float low, float high)
{

    return low + generateRandom() * (high - low);
}

//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < ANT; ii++)
    {

        pos.push_back({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {

        funcValue.push_back(ii.x * ii.y);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(Pos pos)
{

    return pos.x * pos.y;
}

//--------------------------------------------------------------------------------

bool compareMin(std::pair<Pos, float> a, std::pair<Pos, float> b)
{

    return a.second < b.second;
}

//-------------------------------------------------------------------------------

std::pair<Pos, float> findBestValue(std::vector<Pos> pos, std::vector<float> func)
{
    std::vector<std::pair<Pos, float>> temp;

    for (int ii = 0; ii < func.size(); ii++)
    {

        temp.push_back(std::pair<Pos, float>(pos[ii], func[ii]));
    }

    std::sort(temp.begin(), temp.end(), compareMin);

    return temp[0];
}
//--------------------------------------------------------------------------------

std::tuple<std::vector<Pos>, std::vector<float>> sortFittness(std::vector<Pos> pos, std::vector<float> func)
{
    std::vector<Pos> sortedPos;
    std::vector<float> sortedValue;
    std::vector<std::pair<Pos, float>> sorted;

    for (int ii = 0; ii < func.size(); ii++)
    {

        sorted.push_back(std::pair<Pos, float>(pos[ii], func[ii]));
    }

    std::sort(sorted.begin(), sorted.end(), compareMin);

    for (auto &ii : sorted)
    {
        sortedPos.push_back(ii.first);
        sortedValue.push_back(ii.second);
    }

    return std::make_tuple(sortedPos, sortedValue);
}
//--------------------------------------------------------------------------------

int chooseLion()
{
    std::random_device engine;
    std::mt19937 gen(engine());
    std::uniform_int_distribution<int> distribution(0, LION);
    return distribution(gen);
}

//-------------------------------------------------------------------------------

std::vector<Pos> randomWalk(Pos ant, int iter)
{

    std::vector<Pos> newPosAnt;

    float I = 0;

    if (iter >= EVOLUTIOINS * 0.95)
    {

        I = 1 + std::pow(10, 6) * (float)iter / EVOLUTIOINS;
    }

    else if (iter >= EVOLUTIOINS * 0.9)
    {

        I = 1 + std::pow(10, 5) * (float)iter / EVOLUTIOINS;
    }

    else if (iter >= EVOLUTIOINS * 0.75)
    {

        I = 1 + std::pow(10, 4) * (float)iter / EVOLUTIOINS;
    }

    else if (iter >= EVOLUTIOINS * 0.5)
    {

        I = 1 + std::pow(10, 3) * (float)iter / EVOLUTIOINS;
    }

    else
    {

        I = 1 + std::pow(10, 2) * (float)iter / EVOLUTIOINS;
    }

    float lbx = (float)xmin / I;
    float ubx = (float)xmax / I;
    float lby = (float)ymin / I;
    float uby = (float)ymax / I;


    if (generateRandom() < 0.5)
    {
        lbx = lbx + ant.x;
        lby = lby + ant.y;
    }
    else
    {
        lbx = -lbx + ant.x;
        lby = -lby + ant.y;
    }

    if (generateRandom() >= 0.5)
    {
        ubx = ubx + ant.x;
        uby = uby + ant.y;
    }
    else
    {
        ubx = -ubx + ant.x;
        uby = -uby + ant.y;
    }

    std::vector<int> xi(EVOLUTIOINS + 1, 0);
    std::vector<int> xx;
    std::vector<int> xxs;
    std::vector<int> yi(EVOLUTIOINS + 1, 0);
    std::vector<int> yy;
    std::vector<int> yys;

    for (int ii = 1; ii < EVOLUTIOINS + 1; ii++)
    {

        if (generateRandom() > 0.5)
        {
            xi[ii] = 1;
        }
        else
        {
            xi[ii] = -1;
        }
    }

    for (int ii = 1; ii < EVOLUTIOINS + 1; ii++)
    {

        if (generateRandom() > 0.5)
        {
            yi[ii] = 1;
        }
        else
        {
            yi[ii] = -1;
        }
    }

    for (int ii = 1; ii < xi.size(); ii++)
    {

        xx.push_back(std::accumulate(xi.begin(), xi.begin()+ii, 0));
        yy.push_back(std::accumulate(yi.begin(), yi.begin()+ii, 0));
    }

    xxs = xx;
    yys = yy;

    sort(xxs.begin(), xxs.end());
    sort(yys.begin(), yys.end());

    int ax = xxs[0];
    int bx = xxs[xxs.size() - 1];

    int ay = yys[0];
    int by = yys[yys.size() - 1];

    float cx = lbx;
    float dx = ubx;

    float cy = lby;
    float dy = uby;

    for (int ii = 0; ii < xx.size(); ii++)
    {

        Pos newPos;

        newPos.x = ((xx[ii] - ax) * (dx - cx)) / (bx - ax) + cx;
        newPos.y = ((yy[ii] - ay) * (dy - cy)) / (by - ay) + cy;


        newPosAnt.push_back(newPos);
    }



    return newPosAnt;
}

//--------------------------------------------------------------------------------

Pos positionUpdateCheck(Pos actualPos)
{

    Pos Pnew = actualPos;

    if (Pnew.x < xmin)
    {
        Pnew.x = xmin;
    }

    if (Pnew.x > xmax)
    {
        Pnew.x = xmax;
    }

    if (Pnew.y < ymin)
    {
        Pnew.y = ymin;
    }

    if (Pnew.y > ymax)
    {
        Pnew.y = ymax;
    }

    return Pnew;
}
//--------------------------------------------------------------------------------

void runALO()
{

    std::vector<std::tuple<int,float>> convergence;

    std::vector<Pos> currPosAnt = initPosXY();
    std::vector<Pos> currPosLion = initPosXY();

    std::vector<float> antFittness = function(currPosAnt);
    std::vector<float> lionFittness = function(currPosLion);

    auto sortedAnt = sortFittness(currPosAnt, antFittness);
    std::vector<Pos> sortedAntPos = std::get<0>(sortedAnt);
    std::vector<float> sortedAntFit = std::get<1>(sortedAnt);

    auto sortedLion = sortFittness(currPosLion, lionFittness);
    std::vector<Pos> sortedLionPos = std::get<0>(sortedLion);
    std::vector<float> sortedLionFit = std::get<1>(sortedLion);

    auto eliteLion = findBestValue(sortedLionPos, sortedLionFit);
    Pos eliteLionPos = eliteLion.first;
    float eliteLionFit = eliteLion.second;

    for (auto jj = 0; jj < EVOLUTIOINS; jj++)
    {

        for (int ii = 0; ii < ANT; ii++)
        {
            // roulette index
            int lion = chooseLion();
            std::vector<Pos> randomWalkPosRA = randomWalk(sortedLionPos[lion], jj);
            // elite antlion
            std::vector<Pos> randomWalkPosRE = randomWalk(eliteLionPos, jj);

            Pos newAntPos;
            newAntPos.x = (randomWalkPosRA[jj].x + randomWalkPosRE[jj].x) * 0.5;
            newAntPos.y = (randomWalkPosRA[jj].y + randomWalkPosRE[jj].y) * 0.5;
            newAntPos = positionUpdateCheck(newAntPos);

            float new_antFittness = func(newAntPos);
            sortedAntPos[ii] = newAntPos;
            sortedAntFit[ii] = new_antFittness;
        }
        sortedAnt = sortFittness(sortedAntPos, sortedAntFit);
        sortedAntPos = std::get<0>(sortedAnt);
        sortedAntFit = std::get<1>(sortedAnt);

        sortedLion = sortFittness(sortedLionPos, sortedLionFit);
        sortedLionPos = std::get<0>(sortedLion);
        sortedLionFit = std::get<1>(sortedLion);

        std::vector<Pos> doublePopulation(sortedAntPos);
        doublePopulation.insert(doublePopulation.end(),  sortedLionPos.begin(), sortedLionPos.end());
        std::vector<float> doubleFittness = function(doublePopulation);
        
        auto sortedDouble = sortFittness(doublePopulation, doubleFittness);
        auto sortedDoublePos = std::get<0>(sortedDouble);
        auto sortedDoubleFit = std::get<1>(sortedDouble);


        for (int ij = 0; ij < LION; ij++ ){

            sortedLionPos[ij] = sortedDoublePos[ij];
            sortedLionFit[ij] = sortedDoubleFit[ij];

        }

        if (sortedLionFit[0] <= eliteLionFit){

            eliteLionFit = sortedLionFit[0];
            eliteLionPos = sortedLionPos[0];

        }

        sortedLionFit[0] = eliteLionFit;
        sortedLionPos[0] = eliteLionPos;


        convergence.push_back(std::make_tuple(jj, eliteLionFit));

    }

    for (auto &ii : sortedLionPos){
        std::cout << ii.x << " : " << ii.y <<"\n";
    }
}

int main()
{
    runALO();
}