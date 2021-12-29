#pragma once

#include <Arduino.h>
#include <vector>

// Configurationのコマンド実装に要求する関数（抽象クラス）
class ConfigurationCommand
{
public:
    virtual String opcode() = 0; // この設定に入るための文字列(コマンド)
    virtual String help_message() { return ""; }; // helpとタイプした時に表示されるメッセージ
    virtual void execute(std::vector<String> args) = 0; // コマンド入力時の処理を行う関数
};

class ConfigurationMode
{
public:
    static void start();                                   // ConfigurationModeを開始する
    static void addCommand(ConfigurationCommand *command); // コマンドを追加する
    static std::vector<String> split(const String &src_str, const char delim);
    static String trimEnd(const String &input_str, const char trimChar);

protected:
    static bool executeCommand(String src_str); // 文字列を処理する
    static std::vector<ConfigurationCommand *> commands;
};
