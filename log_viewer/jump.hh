#ifndef AUDIBLEA_JUMP_HH
#define AUDIBLEA_JUMP_HH

#include <vector>
#include <map>
#include <string>

class Jump
{
 public:
    typedef std::pair<double,double> point;
    typedef std::vector<point> time_series;

    Jump();

    void AddPoint(const std::string &name,
                  const double time,
                  const double value);
    std::vector<std::string> GetDataNames();
    time_series GetData(const std::string &name);
 private:


    std::map<std::string, time_series> data_;
}; //class Jump

#endif //ifndef AUDIBLEA_JUMP_HH
