#ifndef __YS_LOGGER_HPP__
#define __YS_LOGGER_HPP__

#include <string>
#include <vector>


namespace ys_render_machine {


// TODO: Possible improvements
// - Timestamping every message
class Logger
{
public:
	enum LogChannel
	{
		kChannelDefault = 0,
		kChannelRenderer,
		kChannelMeshFactory,
		
		kChannelAutoTests,

		kChannelCount,
		kChannelCustom,
		kChannelStdOut
	};

	static void		Log(const std::string& _msg, LogChannel _channel);
	static void		ClearLogFile(LogChannel _channel);
	static void		ClearAll();

	static LogChannel			current_channel;
	static std::string			custom_log_file;
	static const std::string	available_log_files[kChannelCount];

};


}

#define LOG(msg) ys_render_machine::Logger::Log(msg, ys_render_machine::Logger::current_channel)


#endif // __YS_LOGGER_HPP__
