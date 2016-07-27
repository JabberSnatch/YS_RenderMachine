#ifndef __YS_LOGGER_HPP__
#define __YS_LOGGER_HPP__

#include <string>


namespace ys_render_machine {


class Logger
{
public:
	static void					LogMessage(const std::string& msg);
	static void					ClearLogFile();
	static const std::string	default_log_file;
	static std::string			log_file;
};


}

#define LOG(msg) ys_render_machine::Logger::LogMessage(msg)


#endif // __YS_LOGGER_HPP__
