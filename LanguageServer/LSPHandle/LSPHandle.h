#pragma once

#include "Lib/LSP/LSP.h"
#include <functional>
#include <map>
#include <memory>

class LanguageServer;

template<class T>
std::shared_ptr<T> MakeRequestObject(nlohmann::json json) {
    auto object = std::make_shared<T>();
    object->Deserialize(json);

    return object;
}

class LSPHandle {
public:
    using MessageHandle = std::function<std::shared_ptr<lsp::Serializable>(nlohmann::json)>;

    LSPHandle(LanguageServer *server);

    ~LSPHandle();

    bool Initialize();

    std::shared_ptr<lsp::Serializable> Dispatch(std::string_view method,
                                                nlohmann::json params);

    void RefreshDiagnostic();

private:
    template<class ParamType, class ReturnType>
    void JsonProtocol(std::string_view method, std::shared_ptr<ReturnType> (LSPHandle::*handle)(std::shared_ptr<ParamType>)) {
        _handles[std::string(method)] = [this, handle](auto jsonParams) {
            return (this->*handle)(MakeRequestObject<ParamType>(jsonParams));
        };
    }

    template<class ParamType>
    void JsonNotification(std::string_view method, void (LSPHandle::*handle)(std::shared_ptr<ParamType>)) {
        _handles[std::string(method)] = [this, handle](auto jsonParams) {
            (this->*handle)(MakeRequestObject<ParamType>(jsonParams));
            return nullptr;
        };
    }

    std::shared_ptr<lsp::InitializeResult> OnInitialize(std::shared_ptr<lsp::InitializeParams> params);

    std::shared_ptr<lsp::Serializable> OnInitialized(std::shared_ptr<lsp::Serializable> params);

    void OnDidChange(std::shared_ptr<lsp::DidChangeTextDocumentParams> params);

    void OnDidOpen(std::shared_ptr<lsp::DidOpenTextDocumentParams> params);

    void OnClose(std::shared_ptr<lsp::DidCloseTextDocumentParams> params);

    std::shared_ptr<lsp::CodeActionResult> OnCodeAction(std::shared_ptr<lsp::CodeActionParams> params);

    std::shared_ptr<lsp::SyntaxTreeViewResponse> OnRequestSyntaxTreeNodes(std::shared_ptr<lsp::SyntaxTreeViewParams> params);

    void OnExecuteCommand(std::shared_ptr<lsp::ExecuteCommandParams> params);

    //	std::shared_ptr<lsp::Serializable> OnDidChangeWatchedFiles(std::shared_ptr<lsp::DidChangeWatchedFilesParams> param);
    //
    //	std::shared_ptr<lsp::CompletionList> OnCompletion(std::shared_ptr<lsp::CompletionParams> param);

    void OnWorkspaceDidChangeConfiguration(std::shared_ptr<lsp::DidChangeConfigurationParams> params);

    std::shared_ptr<lsp::DocumentDiagnosticReport> OnTextDocumentDiagnostic(std::shared_ptr<lsp::DocumentDiagnosticParams> params);

    std::shared_ptr<lsp::WorkspaceDiagnosticReport> OnWorkspaceDiagnostic(std::shared_ptr<lsp::WorkspaceDiagnosticParams> params);

    std::map<std::string, MessageHandle, std::less<>> _handles;

    LanguageServer *_server;
};
