//
// Created by Alex on 15.11.20.
//

#include <cstdio>
#include <imgui.h>

#include <EditorWindow.hpp>
#include <EditorWindowHandler.hpp>
#include <gui/config.hpp>
#include <fstream>


EditorWindow::EditorWindow() : Window() {
    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    ImVec4 clear_col = ImColor(114, 144, 154);

    ///////////////////////////////////////////////////////////////////////
    // TEXT EDITOR SAMPLE
    auto lang = TextEditor::LanguageDefinition::Lua();

    // set your own known preprocessor symbols...
    static const char *ppnames[] = {"NULL", "PM_REMOVE",
                                    "ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI", "D3D11_SDK_VERSION", "assert"};
    // ... and their corresponding values
    static const char *ppvalues[] = {
            "#define NULL ((void*)0)",
            "#define PM_REMOVE (0x0001)",
            "Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ",
            "enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
            "enum D3D_FEATURE_LEVEL",
            "enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
            "#define WINAPI __stdcall",
            "#define D3D11_SDK_VERSION (7)",
            " #define assert(expression) (void)(                                                  \n"
            "    (!!(expression)) ||                                                              \n"
            "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
            " )"
    };

    for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i) {
        TextEditor::Identifier id;
        id.mDeclaration = ppvalues[i];
        lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
    }

    // set your own identifiers
    static const char *identifiers[] = {
            "HWND", "HRESULT", "LPRESULT", "D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC", "MSG", "LRESULT", "WPARAM", "LPARAM", "UINT", "LPVOID",
            "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
            "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
            "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor"};
    static const char *idecls[] =
            {
                    "typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
                    "typedef tagMSG MSG\n * Message structure", "typedef LONG_PTR LRESULT", "WPARAM", "LPARAM", "UINT", "LPVOID",
                    "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
                    "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
                    "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor"};
    for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i) {
        TextEditor::Identifier id;
        id.mDeclaration = std::string(idecls[i]);
        lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
    }
    editor.SetLanguageDefinition(lang);
    //editor.SetPalette(TextEditor::GetLightPalette());

    // error markers
    TextEditor::ErrorMarkers markers;
    markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    markers.insert(std::make_pair<int, std::string>(11, "drawer_wrong.lua:11: attempt to call a nil value (global 'sletp')"));
   editor.SetErrorMarkers(markers);

    // "breakpoint" markers
    TextEditor::Breakpoints bpts;
    bpts.insert(18);
    bpts.insert(19);
    bpts.insert(20);
    editor.SetBreakpoints(bpts);
}

bool EditorWindow::open_file(const std::string &path) {
    openFilePath = path;

    std::ifstream stream(path);
    if (stream.good()) {
        std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        editor.SetText(str);
        return true;
    }
    return false;
}

void EditorWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(60, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(700, 950), ImGuiCond_None);

    auto cpos = editor.GetCursorPosition();
    ImGui::Begin(WIN_TITLE_EDITOR, nullptr, FLAGS_EDITOR);
    after_imgui_begin(WIN_TITLE_EDITOR);

    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Datei")) {
            if (ImGui::MenuItem("Speichern")) {
                auto textToSave = editor.GetText();
                /// save text....
                printf("Speichert: \"%s\n\"", textToSave.c_str());
            }
                ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
  
    ImGui::Text(
            "%6d/%-6d %6d Zeilen  | %s | %s | %s | %s",
            cpos.mLine + 1,
            cpos.mColumn + 1,
            editor.GetTotalLines(),
            editor.IsOverwrite() ? "Ovr" : "Ins",
            editor.CanUndo() ? "*" : " ",
            editor.GetLanguageDefinition().mName.c_str(),
            openFilePath.c_str()
    );
    ImGui::SetItemDefaultFocus();

    if (ImGui::IsWindowFocused())
        ImGui::SetNextWindowFocus();
    editor.Render("TextEditor");

    ImGui::End();
}

std::string EditorWindow::get_text() {
    return editor.GetText();
}
