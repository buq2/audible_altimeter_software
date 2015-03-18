#include "log_fetcher.hh"


LogFetcher::DataStore::DataStore(std::vector<char> d)
    :
      data(d),
      pos(0)
{
}


LogFetcher::LogFetcher(const std::string &com_port)
    :
      device_string_(com_port),
      serial_(io_service_),
      serial_open_(false)
{
}

bool LogFetcher::Open()
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

void LogFetcher::FlushInput()
{
    std::vector<char> buf = GetRawSerialData();
}

std::vector<char> LogFetcher::GetRawSerialData()
{
    size_t bytes = BytesWaiting();
    std::vector<char> readbuf(bytes);

    if (0 != bytes) {
        bytes = serial_.read_some(boost::asio::buffer(&(readbuf[0]), bytes));
    }

    readbuf.resize(bytes);
    return readbuf;
}

void LogFetcher::WriteData(const void *ptr, const size_t bytes)
{
    boost::asio::write(serial_, boost::asio::buffer(ptr,bytes));
}

size_t LogFetcher::BytesWaiting()
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

size_t LogFetcher::GetNumberOfJumps()
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

std::vector<char> LogFetcher::GetRawJumpData(const size_t jump_idx)
{
    std::string command = boost::str(boost::format("get_jump\r%d\r") % jump_idx);
    WriteData((void*)command.c_str(), strlen(command.c_str()));
    return GetResponse();
}

Jump LogFetcher::GetJump(const size_t jump_idx)
{
    Jump jump;

    std::vector<char> data = GetRawJumpData(jump_idx);
    if (data.size() == 0) {
        return jump;
    }

    DataStore d(data);
    JumpData_Header header;
    if (!CastData(d,&header)) {
        std::cerr << "Not enough data" << std::endl;
        return jump;
    }
    if (header.magic_number != JumpData_Header().magic_number) {
        std::cerr << "Incorrect header magic number" << std::endl;
        return jump;
    }

    double time = 0;
    while(true) {
        // Get struct type enum
        char c;
        if (!CastData(d,&c)) {
            std::cerr << "Not enough data for enum" << std::endl;
            return jump;
        }

        const JumpData_StructEnum en = (JumpData_StructEnum)c;
        switch(en) {
        case JUMPDATA_STRUCT_ENUM_JUMP_INFO:
        {
            JumpData_Date_Info s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for jump info" << std::endl;
                return jump;
            }
            break;
        }
        case JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA:
        {
            JumpData_BeginSensorData s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for begin sensor data" << std::endl;
                return jump;
            }
            time += s.time_since_last_sensor_data_seconds;
            break;
        }
        case JUMPDATA_STRUCT_ENUM_ALTITUDE:
        {
            JumpData_Altitude s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for altitude" << std::endl;
                return jump;
            }
            jump.AddPoint(boost::str(boost::format("altitude_%d") % (int)s.altitude_sensor_id), time, s.altitude_meters);
            break;
        }
        case JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE:
        {
            JumpData_AltitudeRate s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for altitude rate" << std::endl;
                return jump;
            }
            jump.AddPoint("altitude_rate", time, s.altitude_rate_m_per_s);
            break;
        }
        case JUMPDATA_STRUCT_ENUM_SECONDS_TIC:
        {
            std::cout << "JUMPDATA_STRUCT_ENUM_SECONDS_TIC" << std::endl;
            JumpData_SecondsTic s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for tic" << std::endl;
                return jump;
            }
            std::cout << "Tic: " << s.second << std::endl;
            break;
        }
        case JUMPDATA_STRUCT_ENUM_DATA_PADDING:
        {
            JumpData_Padding s;
            if (!CastData(d,&s)) {
                std::cerr << "Not enough data for jump data padding" << std::endl;
                return jump;
            }

            // Skip bytes
            d.pos += s.padding_bytes;

            // After padding we have a header
            if (!CastData(d,&header)) {
                std::cerr << "Not enough data for header" << std::endl;
                return jump;
            }
            if (header.magic_number != JumpData_Header().magic_number) {
                std::cerr << "Incorrect header magic number" << std::endl;
                return jump;
            }

            break;
        }
        default:
            std::cerr << "Unknown jump enum: " << (int)en << std::endl;
            return jump;
        }
    }
}

std::vector<char> LogFetcher::GetResponse()
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
