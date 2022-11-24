#pragma once
#include <iostream>
#include <iomanip>
#include <mutex>
#include <ctime>

namespace evo::dbg
{
	class Logger {
	public:
		enum class Severity {
			Info,
			Warning,
			Error
		};

		Logger(std::ostream& stream) : m_stream(stream) { }

		template<typename... T>
		void log(Severity severity, const T&... args) {
			m_mutex.lock();

			tm ltime;
			time_t t = time(NULL);
			localtime_s(&ltime, &t);
			m_stream << std::setw(2) << std::setfill('0') << ltime.tm_hour << ':';
			m_stream << std::setw(2) << std::setfill('0') << ltime.tm_min << ':';
			m_stream << std::setw(2) << std::setfill('0') << ltime.tm_sec << ']';

			switch (severity) {
				case Severity::Warning:
					m_stream << "Warning: ";
					break;
				case Severity::Error:
					m_stream << "ERROR:   ";
					break;
				default:
					m_stream << "         ";
					break;
			}
			(m_stream << ... << args);

			m_mutex.unlock();
		}

	private:
		std::ostream& m_stream;
		std::mutex m_mutex;
	};
}