#ifndef AUDIBLEA_LOG_FETCHER_HH
#define AUDIBLEA_LOG_FETCHER_HH

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_array.hpp>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "jump_data.hh"
#include "jump.hh"

class LogFetcher
{
 public:
    struct DataStore {
        DataStore(std::vector<char> d);
        std::vector<char> data;
        size_t pos;
    };

    LogFetcher(const std::string &com_port);

    bool Open();

    void FlushInput();

    std::vector<char> GetRawSerialData();

    void WriteData(const void *ptr, const size_t bytes);

    size_t BytesWaiting();

    size_t GetNumberOfJumps();

    std::vector<char> GetRawJumpData(const size_t jump_idx);

    template<class T>
    static bool CastData(DataStore &d, T *out)
    {
        if (d.pos + sizeof(T) > d.data.size()) {
            // Not enough data
            return false;
        }

        // Cast
        memcpy(out,&d.data[d.pos], sizeof(T));
        d.pos += sizeof(T);

        return true;
    }

    Jump GetJump(const size_t jump_idx);

    std::vector<char> GetResponse();
 private:
    // Serial device string ("COM1", "/dev/ttyACM0")
    std::string device_string_;

    // IO-service which was used to create the serial object
    boost::asio::io_service io_service_;

    // Serial device object
    boost::asio::serial_port serial_;

    bool serial_open_;
}; //class LogFetcher

#endif //ifndef AUDIBLEA_LOG_FETCHER_HH
