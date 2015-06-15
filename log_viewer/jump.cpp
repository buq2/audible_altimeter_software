#include "jump.hh"
#include <iostream>
#include <fstream>

Jump::Jump()
{

}

void Jump::AddPoint(const std::string &name, const double time, const double value)
{
    auto &series = data_[name];
    series.push_back(std::make_pair(time,value));
}

std::vector<std::string> Jump::GetDataNames()
{
    std::vector<std::string> names;
    for(auto &c : data_) {
        names.push_back(c.first);
    }
    return names;
}

Jump::time_series Jump::GetData(const std::string &name)
{
    auto it = data_.find(name);
    if (it == data_.end()) {
        return time_series();
    }

    return it->second;
}

void Jump::SaveToCsv(const std::string out_fname)
{
    std::ofstream file(out_fname.c_str());
    for(auto graph : data_) {
        const std::string name = graph.first;
        for (auto point : graph.second) {
            file << name << '\t' << point.first << '\t' << point.second << std::endl;
        }
    }
    file.close();
}
