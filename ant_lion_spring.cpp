// Markus Buchholz, 2023

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <random>
#include <numeric>

//--------------------------------------------------------------------------------

float x1min = 0.05;
float x1max = 2.0;
float x2min = 0.25;
float x2max = 1.3;
float x3min = 2.0;
float x3max = 15.0;

int EVOLUTIOINS = 50;
int ANT = 500;
int LION = 500;
float R = 0.045;
//--------------------------------------------------------------------------------

struct Pos
{

    float x1;
    float x2;
    float x3;
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

        pos.push_back({valueGenerator(x1min, x1max), valueGenerator(x2min, x2max), valueGenerator(x3min, x3max)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {
        float p1{0}, p2{0}, p3{0}, p4{0};
        auto f1 = 1 - (float)(std::pow(ii.x2, 2) * ii.x3) / (float)(71785.0 * std::pow(ii.x1, 4));

        auto f2 = (float)(4.0 * std::pow(ii.x2, 2) - ii.x1 * ii.x2) / (float)(12556.0 * (ii.x2 * std::pow(ii.x1, 3) - std::pow(ii.x1, 4))) + 1.0 / (float)(5108.0 * std::pow(ii.x1, 2));

        auto f3 = 1.0 - (float)(140.45 * ii.x1) / (float)(std::pow(ii.x2, 2) * ii.x3);

        auto f4 = ((float)(ii.x1 + ii.x2) / (float)1.5) - 1.0;

        if (f1 > 0)
        {
            p1 = std::pow(f1, 2);
        }
        if (f2 > 0)
        {
            p2 = std::pow(f2, 2);
        }
        if (f3 > 0)
        {
            p3 = std::pow(f3, 2);
        }

        if (f4 > 0)
        {
            p4 = std::pow(f4, 2);
        }

        auto f = (ii.x3 + 2) * ii.x2 * std::pow(ii.x1, 2) + R * p1 + R * p2 + R * p3 + R * p4;

        funcValue.push_back(f);
    }

    return funcValue;
}


//--------------------------------------------------------------------------------

float func(Pos pos)
{
    float p1{0}, p2{0}, p3{0}, p4{0};
    auto f1 = 1 - (float)(std::pow(pos.x2, 2) * pos.x3) / (float)(71785.0 * std::pow(pos.x1, 4));

    auto f2 = (float)(4.0 * std::pow(pos.x2, 2) - pos.x1 * pos.x2) / (float)(12556.0 * (pos.x2 * std::pow(pos.x1, 3) - std::pow(pos.x1, 4))) + 1.0 / (float)(5108.0 * std::pow(pos.x1, 2));

    auto f3 = 1.0 - (float)(140.45 * pos.x1) / (float)(std::pow(pos.x2, 2) * pos.x3);

    auto f4 = ((float)(pos.x1 + pos.x2) / (float)1.5) - 1.0;

    if (f1 > 0)
    {
        p1 = std::pow(f1, 2);
    }
    if (f2 > 0)
    {
        p2 = std::pow(f2, 2);
    }
    if (f3 > 0)
    {
        p3 = std::pow(f3, 2);
    }

    if (f4 > 0)
    {
        p4 = std::pow(f4, 2);
    }

    return (pos.x3 + 2) * pos.x2 * std::pow(pos.x1, 2) + R * p1 + R * p2 + R * p3 + R * p4;

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

    float lbx1 = (float)x1min / I;
    float ubx1 = (float)x1max / I;

    float lbx2 = (float)x2min / I;
    float ubx2 = (float)x2max / I;

    float lbx3 = (float)x3min / I;
    float ubx3 = (float)x3max / I;


    if (generateRandom() < 0.5)
    {
        lbx1 = lbx1 + ant.x1;
        lbx2 = lbx2 + ant.x2;
        lbx3 = lbx3 + ant.x3;
    }
    else
    {
        lbx1 = -lbx1 + ant.x1;
        lbx2 = -lbx2 + ant.x2;
        lbx3 = -lbx3 + ant.x3;
    }

    if (generateRandom() >= 0.5)
    {
        ubx1 = ubx1 + ant.x1;
        ubx2 = ubx2 + ant.x2;
        ubx3 = ubx3 + ant.x3;
    }
    else
    {
        ubx1 = -ubx1 + ant.x1;
        ubx2 = -ubx2 + ant.x2;
        ubx3 = -ubx3 + ant.x3;
    }

    std::vector<int> xi1(EVOLUTIOINS + 1, 0);
    std::vector<int> xx1;
    std::vector<int> xxs1;

    std::vector<int> xi2(EVOLUTIOINS + 1, 0);
    std::vector<int> xx2;
    std::vector<int> xxs2;

    std::vector<int> xi3(EVOLUTIOINS + 1, 0);
    std::vector<int> xx3;
    std::vector<int> xxs3;


    for (int ii = 1; ii < EVOLUTIOINS + 1; ii++)
    {

        if (generateRandom() > 0.5)
        {
            xi1[ii] = 1;
        }
        else
        {
            xi1[ii] = -1;
        }
    }

    for (int ii = 1; ii < EVOLUTIOINS + 1; ii++)
    {

        if (generateRandom() > 0.5)
        {
            xi2[ii] = 1;
        }
        else
        {
            xi2[ii] = -1;
        }
    }

    for (int ii = 1; ii < EVOLUTIOINS + 1; ii++)
    {

        if (generateRandom() > 0.5)
        {
            xi3[ii] = 1;
        }
        else
        {
            xi3[ii] = -1;
        }
    }


    for (int ii = 1; ii < xi1.size(); ii++)
    {

        xx1.push_back(std::accumulate(xi1.begin(), xi1.begin() + ii, 0));
        xx2.push_back(std::accumulate(xi2.begin(), xi2.begin() + ii, 0));
        xx3.push_back(std::accumulate(xi3.begin(), xi3.begin() + ii, 0));
    }

    xxs1 = xx1;
    xxs2 = xx2;
    xxs3 = xx3;

    sort(xxs1.begin(), xxs1.end());
    sort(xxs2.begin(), xxs2.end());
    sort(xxs3.begin(), xxs3.end());

    int ax1 = xxs1[0];
    int bx1 = xxs1[xxs1.size() - 1];
    float cx1 = lbx1;
    float dx1 = ubx1;

    int ax2 = xxs2[0];
    int bx2 = xxs2[xxs2.size() - 1];
    float cx2 = lbx2;
    float dx2 = ubx2;

    int ax3 = xxs3[0];
    int bx3 = xxs3[xxs3.size() - 1];
    float cx3 = lbx3;
    float dx3 = ubx3;


    for (int ii = 0; ii < xx1.size(); ii++)
    {

        Pos newPos;

        newPos.x1 = ((xx1[ii] - ax1) * (dx1 - cx1)) / (bx1 - ax1) + cx1;
        newPos.x2 = ((xx2[ii] - ax2) * (dx2 - cx2)) / (bx2 - ax2) + cx2;
        newPos.x3 = ((xx3[ii] - ax3) * (dx3 - cx3)) / (bx3 - ax3) + cx3;

        newPosAnt.push_back(newPos);
    }

    return newPosAnt;
}

//--------------------------------------------------------------------------------

Pos positionUpdateCheck(Pos actualPos)
{

    Pos Pnew = actualPos;

    if (Pnew.x1 < x1min)
    {
        Pnew.x1 = x1min;
    }

    if (Pnew.x1 > x1max)
    {
        Pnew.x1 = x1max;
    }

    if (Pnew.x2 < x2min)
    {
        Pnew.x2 = x2min;
    }

    if (Pnew.x2 > x2max)
    {
        Pnew.x2 = x2max;
    }

    if (Pnew.x3 < x3min)
    {
        Pnew.x3 = x3min;
    }

    if (Pnew.x3 > x3max)
    {
        Pnew.x3 = x3max;
    }


    return Pnew;
}
//--------------------------------------------------------------------------------

void runALO()
{

    std::vector<std::tuple<int, float>> convergence;

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
            newAntPos.x1 = (randomWalkPosRA[jj].x1 + randomWalkPosRE[jj].x1) * 0.5;
            newAntPos.x2 = (randomWalkPosRA[jj].x2 + randomWalkPosRE[jj].x2) * 0.5;
            newAntPos.x3 = (randomWalkPosRA[jj].x3 + randomWalkPosRE[jj].x3) * 0.5;

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
        doublePopulation.insert(doublePopulation.end(), sortedLionPos.begin(), sortedLionPos.end());
        std::vector<float> doubleFittness = function(doublePopulation);

        auto sortedDouble = sortFittness(doublePopulation, doubleFittness);
        auto sortedDoublePos = std::get<0>(sortedDouble);
        auto sortedDoubleFit = std::get<1>(sortedDouble);

        for (int ij = 0; ij < LION; ij++)
        {

            sortedLionPos[ij] = sortedDoublePos[ij];
            sortedLionFit[ij] = sortedDoubleFit[ij];
        }

        if (sortedLionFit[0] <= eliteLionFit)
        {

            eliteLionFit = sortedLionFit[0];
            eliteLionPos = sortedLionPos[0];
        }

        sortedLionFit[0] = eliteLionFit;
        sortedLionPos[0] = eliteLionPos;

        convergence.push_back(std::make_tuple(jj, eliteLionFit));
    }

    // for (auto &ii : sortedLionPos)
    // {
    //     std::cout << ii.x1 << " : " << ii.x2 << " : " << ii.x3 << "\n";
    // }
    std::cout << "--- Optimization problem: tension/compression spring ---\n";
    std::cout<< "min weight= " << eliteLionFit<< "\n";   
    std::cout<< "values:   d= " << eliteLionPos.x1 << " D= " <<eliteLionPos.x2 << " N= " << eliteLionPos.x3<< "\n";   


}

int main()
{
    runALO();
}