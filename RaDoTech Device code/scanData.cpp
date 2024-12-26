#include "scanData.h"

ScanData::ScanData(){
    vector<float> computedData;
}

ScanData::ScanData(vector<float> data){
    for (unsigned int i = 0; i < data.size(); ++i){
        computedData.push_back(data[i]);
    }
}

ScanData::~ScanData(){}

