#include "AssimpDefs.hpp"



cxmfAssimpLogStream::cxmfAssimpLogStream(std::string& out)
	: Super(), m_Output(out)
{}

void cxmfAssimpLogStream::write(const char* message)
{
	m_Output.append(message);
	if (!m_Output.ends_with('\n'))	//
		m_Output.push_back('\n');
}





cxmfAssimpLogger::cxmfAssimpLogger(LogSeverity severity)
	: Super(severity), m_Streams()
{}

cxmfAssimpLogger::cxmfAssimpLogger()
	: cxmfAssimpLogger(LogSeverity::NORMAL)
{}

cxmfAssimpLogger::~cxmfAssimpLogger()
{
	for (const auto& pair : m_Streams)
	{
		delete pair.first;
	}
}

void cxmfAssimpLogger::_process_log(const char* message, ErrorSeverity errType)
{
	if (!message) return;

	for (const auto& pair : m_Streams)
	{
		if (pair.second & errType)
		{
			pair.first->write(message);
		}
	}
}

void cxmfAssimpLogger::OnDebug(const char* message)
{
	if (Super::m_Severity != LogSeverity::NORMAL)
	{
		_process_log(message, ErrorSeverity::Debugging);
	}
}

void cxmfAssimpLogger::OnVerboseDebug(const char* message)
{
	if (Super::m_Severity == LogSeverity::VERBOSE)
	{
		_process_log(message, ErrorSeverity::Debugging);
	}
}

void cxmfAssimpLogger::OnInfo(const char* message)
{
	if (Super::m_Severity == LogSeverity::VERBOSE)
	{
		_process_log(message, ErrorSeverity::Info);
	}
}

void cxmfAssimpLogger::OnWarn(const char* message)
{
	if (Super::m_Severity != LogSeverity::DEBUGGING)
	{
		_process_log(message, ErrorSeverity::Warn);
	}
}

void cxmfAssimpLogger::OnError(const char* message)
{
	if (Super::m_Severity != LogSeverity::DEBUGGING)
	{
		_process_log(message, ErrorSeverity::Err);
	}
}

bool cxmfAssimpLogger::attachStream(Assimp::LogStream* pStream, unsigned int severity)
{
	if (!pStream || severity == 0) return false;

	for (auto& pair : m_Streams)
	{
		if (pair.first == pStream)
		{
			pair.second |= severity;
			return true;
		}
	}
	m_Streams.emplace_back(pStream, severity);
	return true;
}

bool cxmfAssimpLogger::detachStream(Assimp::LogStream* pStream, unsigned int severity)
{
	if (!pStream || severity == 0) return false;

	auto _It = m_Streams.begin();
	const auto _End = m_Streams.end();
	while (_It != _End)
	{
		if (_It->first == pStream)
		{
			_It->second &= ~severity;
			if (_It->second == 0)
			{
				m_Streams.erase(_It);
				return true;
			}
			break;
		}
		++_It;
	}
	return false;
}





cxmfScopeLogger::cxmfScopeLogger(std::string& outStream,				   //
								 Assimp::Logger::LogSeverity logSeverity,  //
								 unsigned int errorSeverity)
	: m_Logger(logSeverity), m_Stream(outStream), m_Severity(errorSeverity)
{
	m_Logger.attachStream(&m_Stream, m_Severity);
	Assimp::DefaultLogger::set(&m_Logger);
}

cxmfScopeLogger::~cxmfScopeLogger()
{
	Assimp::DefaultLogger::set(nullptr);
	m_Logger.detachStream(&m_Stream, m_Severity);
}
