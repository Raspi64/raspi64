
#include "TextEditorLanguageExtension.hpp"


const TextEditor::LanguageDefinition& LanguageDefinitionExtension::Basic()
{
    static bool inited = false;
    static TextEditor::LanguageDefinition langDef;
    if (!inited)
    {
        static const char* const keywords[] = {
                "AND", "CALL", "CLASS", "DEF", "DIM", "DO", "ELSE", "ELSEIF", "END", "ENDCLASS", "ENDDEF", "ENDIF", "EXIT",
                "FALSE", "FOR", "GOSUB", "GOTO", "IF", "IMPORT", "IN", "IS", "LET", "NEW", "NEXT", "NIL", "NOT", "OR",
                "RETURN", "STEP", "THEN", "TO", "TRUE", "UNTIL", "VAR", "WEND", "WHILE"
        };

        for (auto& k : keywords)
            langDef.mKeywords.insert(k);

        static const char* const identifiers[] = {
                "ABS", "ACOS", "ASC", "ASIN", "ATAN", "BACK", "CEIL", "CHR", "CLEAR", "CLONE", "COS", "DICT", "EXIST", "EXP",
                "FIX", "FLOOR", "GET", "INDEX_OF", "INPUT", "INSERT", "ITERATOR", "LAMBDA", "LEFT", "LEN", "LIST", "LOG",
                "MEM", "MID", "MOD", "MOVE_NEXT", "POP", "PRINT", "PUSH", "REFLECT", "REMOVE", "RIGHT", "RND", "ROUND", "SET",
                "SGN", "SIN", "SORT", "SQR", "SRND", "STR", "TAN", "TO_ARRAY", "TYPE", "VAL"
        };

        for (auto& k : identifiers)
        {
            TextEditor::Identifier id;
            id.mDeclaration = "Built-in function";
            langDef.mIdentifiers.insert(std::make_pair(std::string(k), id));
        }

        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("L?\\\"(\\\\.|[^\\\"])*\\\"", TextEditor::PaletteIndex::String));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("\\\'[^\\\']*\\\'", TextEditor::PaletteIndex::String));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", TextEditor::PaletteIndex::Number));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[+-]?[0-9]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", TextEditor::PaletteIndex::Identifier));
        langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", TextEditor::PaletteIndex::Punctuation));

        langDef.mCommentStart = "REM";
        langDef.mCommentEnd = "";
        langDef.mSingleLineComment = "REM";

        langDef.mCaseSensitive = false;
        langDef.mAutoIndentation = false;

        langDef.mName = "Basic";

        inited = true;

    }
    return langDef;
}
