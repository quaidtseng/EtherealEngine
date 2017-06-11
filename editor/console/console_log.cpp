#include "console_log.h"


void console_log::_sink_it(const logging::details::log_msg& msg)
{
	_entries.push_back({ msg.formatted.c_str(), msg.level });
	if (_entries.size() > _max_size)
		_entries.pop_front();
	flush();
	++_pending;
}

void console_log::flush()
{

}

console_log::entries_t console_log::get_items()
{
	entries_t itemsCopy;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		itemsCopy = _entries;
	}
	return itemsCopy;
}

void console_log::clearLog()
{
	_entries = entries_t();
	_pending = 0;
}

const std::array<float, 4>& console_log::get_level_colorization(logging::level::level_enum level)
{
	static std::map<logging::level::level_enum, std::array<float, 4>> colorization_mappings
	{
		{ logging::level::trace,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::debug,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::info,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::notice,{ 1.0f, 5.0f, 0.0f, 1.0f } },
		{ logging::level::warn,{ 1.0f, 0.494f, 0.0f, 1.0f } },
		{ logging::level::err,{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ logging::level::critical,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::alert,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::emerg,{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ logging::level::off,{ 1.0f, 1.0f, 1.0f, 1.0f } },
	};

	return colorization_mappings[level];
}