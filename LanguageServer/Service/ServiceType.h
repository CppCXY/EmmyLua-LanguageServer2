#pragma once

enum class ServiceType
{
	Service = -1,
	DiagnosticService = 0,
	CommandService,
	CodeActionService,

    ConfigService,

	ServiceCount 
};

#define LANGUAGE_SERVICE(ServiceClass)\
    inline static constexpr ServiceType ServiceIndex = ServiceType::ServiceClass
