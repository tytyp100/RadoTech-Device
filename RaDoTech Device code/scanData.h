#ifndef SCANDATA_H
#define SCANDATA_H

#include <vector>
#include<QDebug>
using namespace std;

class ScanData
{
public:
    ScanData();
    ScanData(vector<float>);
    ~ScanData();

private:
    vector<float> computedData;
};
#endif
