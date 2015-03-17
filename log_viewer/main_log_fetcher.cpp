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

class LogFetcher
{
 public:
    struct DataStore {
        DataStore(std::vector<char> d)
            :
              data(d),
              pos(0)
        {
        }
        std::vector<char> data;
        size_t pos;
    };

    LogFetcher(const std::string &com_port)
        :
          device_string_(com_port),
          serial_(io_service_),
          serial_open_(false)
    {
    }

    bool Open()
    {
        if (serial_open_) {
            return true;
        }

        try {
            serial_.open(device_string_);
        } catch(...) {
            std::cerr << "Failed to open serial port" << std::endl;
            return false;
        }

        serial_.set_option(boost::asio::serial_port_base::baud_rate(115200));
        serial_.set_option(boost::asio::serial_port_base::character_size(8));
        serial_.set_option(boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none));
        serial_.set_option(boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none));
        serial_.set_option(boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one));

        serial_open_ = true;

        FlushInput();

        return true;
    }

    void FlushInput()
    {
        std::vector<char> buf = GetRawSerialData();
    }

    std::vector<char> GetRawSerialData()
    {
        size_t bytes = BytesWaiting();
        std::vector<char> readbuf(bytes);

        if (0 != bytes) {
            bytes = serial_.read_some(boost::asio::buffer(&(readbuf[0]), bytes));
        }

        readbuf.resize(bytes);
        return readbuf;
    }

    void WriteData(const void *ptr, const size_t bytes)
    {
        boost::asio::write(serial_, boost::asio::buffer(ptr,bytes));
    }

    size_t BytesWaiting()
    {
        boost::asio::serial_port::native_handle_type handle = serial_.native_handle();

        size_t nbytes = 0;
        #if defined(BOOST_WINDOWS)
            // Uhh, ohh. Don't know how to read available bytes in Windows.
            // I thought that you could use the following:
            //   DWORD junk;
            //   DeviceIoControl(handle,
            //           FIONREAD,
            //           NULL,0,
            //           &nbytes, sizeof(nbytes),
            //           &junk,
            //           NULL
            //           );
            // But apparently this does not work. We can also lie and say that
            // there is always ~100kb of data.
            nbytes = 1024*100;
        #else
            ioctl(handle, FIONREAD, (char*)&nbytes);
        #endif

        return nbytes;
    }

    size_t GetNumberOfJumps()
    {
        char command[] = "num_jumps\r";
        WriteData((void*)command, strlen(command));

        auto data = GetResponse();

        if (data.size() == 0) {
            return 0;
        }

        std::string str(&data[0],data.size());
        boost::trim(str);

        return boost::lexical_cast<size_t>(str);
    }

    std::vector<char> GetRawJumpData(const size_t jump_idx)
    {
        std::string command = boost::str(boost::format("get_jump\r%d\r") % jump_idx);
        WriteData((void*)command.c_str(), strlen(command.c_str()));
        return GetResponse();
    }

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

    void PrintJumpData(const size_t jump_idx)
    {
        std::vector<char> data = GetRawJumpData(jump_idx);
        if (data.size() == 0) {
            return;
        }

        DataStore d(data);
        JumpData_Header header;
        if (!CastData(d,&header)) {
            std::cerr << "Not enough data" << std::endl;
            return;
        }
        if (header.magic_number != JumpData_Header().magic_number) {
            std::cerr << "Incorrect header magic number" << std::endl;
            return;
        }

        while(true) {
            // Get struct type enum
            char c;
            if (!CastData(d,&c)) {
                std::cerr << "Not enough data for enum" << std::endl;
                return;
            }

            const JumpData_StructEnum en = (JumpData_StructEnum)c;
            switch(en) {
            case JUMPDATA_STRUCT_ENUM_JUMP_INFO:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_JUMP_INFO" << std::endl;

                JumpData_Date_Info s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for jump info" << std::endl;
                    return;
                }
                break;
            }
            case JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA" << std::endl;
                JumpData_BeginSensorData s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for begin sensor data" << std::endl;
                    return;
                }
                std::cout << "Time since last update: " << s.time_since_last_sensor_data_seconds << std::endl;
                break;
            }
            case JUMPDATA_STRUCT_ENUM_ALTITUDE:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_ALTITUDE" << std::endl;
                JumpData_Altitude s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for altitude" << std::endl;
                    return;
                }
                std::cout << "Altitude: " << (int)s.altitude_sensor_id << ": " << s.altitude_meters << std::endl;
                break;
            }
            case JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE" << std::endl;
                JumpData_AltitudeRate s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for altitude rate" << std::endl;
                    return;
                }
                std::cout << "Altitude rate: " << s.altitude_rate_m_per_s << std::endl;
                break;
            }
            case JUMPDATA_STRUCT_ENUM_SECONDS_TIC:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_SECONDS_TIC" << std::endl;
                JumpData_SecondsTic s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for tic" << std::endl;
                    return;
                }
                std::cout << "Tic: " << s.second << std::endl;
                break;
            }
            case JUMPDATA_STRUCT_ENUM_DATA_PADDING:
            {
                std::cout << "JUMPDATA_STRUCT_ENUM_DATA_PADDING" << std::endl;
                JumpData_Padding s;
                if (!CastData(d,&s)) {
                    std::cerr << "Not enough data for jump data padding" << std::endl;
                    return;
                }

                // Skip bytes
                d.pos += s.padding_bytes;
                break;
            }
            default:
                std::cerr << "Unknown jump enum: " << (int)en << std::endl;
                return;
            }
        }
    }

    std::vector<char> GetResponse()
    {
        std::vector<char> data;

        bool first_bytes_received = false;
        const size_t max_waits_for_first_data = 30;
        const size_t max_waits_after_data = 3;
        size_t waits_after_data = 0;
        size_t waits_for_first_data = 0;
        while(true) {
            std::vector<char> new_data = GetRawSerialData();

            data.insert(data.end(), new_data.begin(), new_data.end());

            if (new_data.size() == 0) {
                // No data
                if (first_bytes_received) {
                    if (waits_after_data > max_waits_after_data) {
                        // No more data
                        return data;
                    }

                    // Wait a little for new data
                    boost::this_thread::sleep(boost::posix_time::millisec(10));
                    ++waits_after_data;
                } else {
                    // We haven't received any data
                    if (waits_for_first_data > max_waits_for_first_data) {
                        // Did not get response
                        std::cerr << "Failed to get response" << std::endl;
                        return data;
                    }

                    boost::this_thread::sleep(boost::posix_time::millisec(100));
                    ++waits_for_first_data;
                }
            } else {
                // Received some data
                first_bytes_received = true;
                waits_after_data = false;
            }
        }
    }
 private:
    // Serial device string ("COM1", "/dev/ttyACM0")
    std::string device_string_;

    // IO-service which was used to create the serial object
    boost::asio::io_service io_service_;

    // Serial device object
    boost::asio::serial_port serial_;

    bool serial_open_;
}; //class LogFetcher

int main(int argc, char *argv[])
{
    LogFetcher log("/dev/ttyACM0");
    if (!log.Open()) {
        std::cerr << "Failed to open serial port" << std::endl;
        return -1;
    }
    const size_t num_jumps = log.GetNumberOfJumps();
    if (num_jumps > 0) {
        log.PrintJumpData(num_jumps-1);
    }
    return 0;
}
