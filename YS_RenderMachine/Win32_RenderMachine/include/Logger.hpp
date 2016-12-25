#ifndef __YS_LOGGER_HPP__
#define __YS_LOGGER_HPP__

#include <string>
#include <vector>


namespace ys_render_machine {


// TODO: Possible improvements
// - Timestamping every message

/// This is the central logging facility.
/// It can write to files and standard outputs on request.
/// It has several channels and can be extended with additionnal ones.
/// A channel definition requires an enum entry and a file name.
/// A custom channel is provided, the file can be set manually at any time.
class Logger
{
public:
	enum LogChannel
	{
		kChannelDefault = 0,
		kChannelRenderer,
		kChannelMeshFactory,
		
		kChannelAutoTests,

		/// Any new channel should be added before this line.
		kChannelCount,
		kChannelCustom, 
		kChannelStdOut
	};

	/// Log opens the file associated with the given channel, writes inside
	/// then close it.
	/// This is not very efficient, logs could be accumulated and flushed
	/// periodically.
	static void		Log(const std::string& _msg, LogChannel _channel);
	/// Clears the log file associated with the given channel.
	/// If no file is associated with the channel nothing happens.
	static void		ClearLogFile(LogChannel _channel);
	/// Clears every file associated with the Logger.
	static void		ClearAll();

	/// Support variable for the shortcut macro.
	static LogChannel			current_channel;
	/// Path to the custom log file. it can be changed whenever needed.
	static std::string			custom_log_file;


	static const std::string	available_log_files[kChannelCount];
};


}

#define LOG(msg) ys_render_machine::Logger::Log(msg, ys_render_machine::Logger::current_channel)


#endif // __YS_LOGGER_HPP__
