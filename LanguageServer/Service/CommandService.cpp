#include "CommandService.h"
#include "Util/Url.h"
#include "LanguageServer.h"
#include "ConfigService.h"

CommandService::CommandService(LanguageServer *owner)
        : Service(owner) {
}

void CommandService::Start() {
}

bool CommandService::Initialize() {
//    CommandProtocol("emmylua.reformat.me", &CommandService::Reformat);
//    CommandProtocol("emmylua.spell.correct", &CommandService::SpellCorrect);
    return true;
}

std::vector<std::string> CommandService::GetCommands() {
    std::vector<std::string> results;

    for (auto it: _handles) {
        results.push_back(it.first);
    }

    return results;
}

std::string CommandService::GetCommand(Command command) {
//    switch (command) {
//        case Command::Reformat:
//            return "emmylua.reformat.me";
//        case Command::SpellCorrect:
//            return "emmylua.spell.correct";
//        case Command::SpellAddDict:
//            return "emmylua.spell.addDict";
//    }

    return "";
}

void CommandService::Dispatch(std::string_view command, std::shared_ptr<lsp::ExecuteCommandParams> params) {
    std::string cmd(command);
    auto it = _handles.find(cmd);
    if (it != _handles.end()) {
        it->second(params);
    }
}

