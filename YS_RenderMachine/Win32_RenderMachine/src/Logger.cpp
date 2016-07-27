#include "Logger.hpp"

#include <iostream>
#include <fstream>


namespace ys_render_machine {


const std::string Logger::default_log_file = "output.yslog";
std::string	Logger::log_file = Logger::default_log_file;

void
Logger::LogMessage(const std::string& msg)
{
	if (log_file != "")
	{
		std::fstream output_stream(log_file, std::ios_base::out | std::ios_base::app);
		output_stream << msg << std::endl;
		output_stream.close();
	}
	else
	{
		std::cout << msg << std::endl;
	}
}


void
Logger::ClearLogFile()
{
	if (log_file != "")
	{
		std::fstream log_file_stream(log_file, std::ios_base::out);
		log_file_stream.clear();
		log_file_stream.close();
	}
}


}

