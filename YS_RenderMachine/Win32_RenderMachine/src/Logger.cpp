#include "Logger.hpp"

#include <iostream>
#include <fstream>


namespace ys_render_machine {


Logger::LogChannel	Logger::current_channel = Logger::kChannelDefault;

std::string			Logger::custom_log_file = "output.yslog";

const std::string	Logger::available_log_files[] = {
	"output.yslog",
	"renderer.yslog",
	"mesh_factory.yslog"
};


void
Logger::Log(const std::string& _msg, LogChannel _channel)
{
	if (_channel != kChannelCount)
	{
		std::fstream output_stream;
		if (_channel != kChannelCustom)
		{
			output_stream.open(available_log_files[_channel],
							   std::ios_base::out | std::ios_base::app);
		}
		else
		{
			output_stream.open(custom_log_file,
							   std::ios_base::out | std::ios_base::app);
		}

		output_stream << _msg << std::endl;
		output_stream.close();
	}
	else if (_channel == kChannelStdOut)
	{
		std::cout << _msg << std::endl;
	}
}


void
Logger::ClearLogFile(LogChannel _channel)
{
	if (_channel != kChannelCount && _channel != kChannelStdOut)
	{
		std::fstream output_stream;
		if (_channel != kChannelCustom)
		{
			output_stream.open(available_log_files[_channel],
							   std::ios_base::out);
		}
		else
		{
			output_stream.open(custom_log_file,
							   std::ios_base::out);
		}

		output_stream.clear();
		output_stream.close();
	}
}


void
Logger::ClearAll()
{
	for (unsigned int i = 0; i < kChannelCount; ++i)
	{
		ClearLogFile(LogChannel(i));
	}
}


}

