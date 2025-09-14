#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Logger.hpp"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

#include <vector>
#include <string>



class cxmfAssimpLogStream final : public Assimp::LogStream
{
private:
	using Super = Assimp::LogStream;

private:
	std::string& m_Output;

public:
	cxmfAssimpLogStream() = delete;

	explicit cxmfAssimpLogStream(std::string& out);

	~cxmfAssimpLogStream() = default;

public:
	void write(const char* message) override;
};



class cxmfAssimpLogger final : public Assimp::Logger
{
private:
	using Super = Assimp::Logger;

	using stream_storage_t = std::vector<std::pair<Assimp::LogStream*, unsigned int>>;

private:
	stream_storage_t m_Streams;

private:
	void _process_log(const char* message, ErrorSeverity errType);

private:
	void OnDebug(const char* message) override;
	void OnVerboseDebug(const char* message) override;
	void OnInfo(const char* message) override;
	void OnWarn(const char* message) override;
	void OnError(const char* message) override;

public:
	explicit cxmfAssimpLogger(LogSeverity severity);
	cxmfAssimpLogger();

	~cxmfAssimpLogger();

public:
	bool attachStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;
	bool detachStream(Assimp::LogStream* pStream, unsigned int severity = Debugging | Err | Warn | Info) override;
};



class cxmfScopeLogger final
{
private:
	cxmfAssimpLogger m_Logger;
	cxmfAssimpLogStream m_Stream;
	unsigned int m_Severity;

public:
	cxmfScopeLogger() = delete;

	explicit cxmfScopeLogger(std::string& outStream,				   //
							 Assimp::Logger::LogSeverity logSeverity,  //
							 unsigned int errorSeverity);

	cxmfScopeLogger(cxmfScopeLogger&&) = delete;
	cxmfScopeLogger& operator=(cxmfScopeLogger&&) = delete;
	cxmfScopeLogger(const cxmfScopeLogger&) = delete;
	cxmfScopeLogger& operator=(const cxmfScopeLogger&) = delete;

	~cxmfScopeLogger();
};
