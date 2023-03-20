#pragma once

enum class ServiceType
{
	Service = -1,
	DiagnosticService = 0,
	CommandService,
	CodeActionService,

    ConfigService,
    VirtualFileSystem,

	ServiceCount 
};

#define LANGUAGE_SERVICE(ServiceClass)\
    inline static constexpr ServiceType ServiceIndex = ServiceType::ServiceClass
