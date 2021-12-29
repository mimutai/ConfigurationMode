#include "ConfigurationMode.h"

#include <Arduino.h>
#include <FreeRTOS.h>
#include <vector>

std::vector<ConfigurationCommand *> ConfigurationMode::commands;

// ConfigurationModeを開始する
void ConfigurationMode::start()
{
    bool _do_exit = false; // 設定モードを終了するかのフラグ
    Serial.println("[CFG INFO] ConfigurationMode");
    Serial.println("[CFG INFO] Type 'help' for a list of commands.");

    // 設定ループ
    do
    {
        if (Serial.available())
        {            
            String str = Serial.readString(); // シリアルから文字列を受け取る
            _do_exit = executeCommand(str);   // 受け取った文字列（命令）を実行する
        }
    } while (!_do_exit);

    Serial.println("[CFG INFO] Exit");
}

// コマンドを追加する
void ConfigurationMode::addCommand(ConfigurationCommand *command)
{
    ConfigurationMode::commands.push_back(command);
}

// 受信した文字列を処理する
bool ConfigurationMode::executeCommand(String input_str)
{
    Serial.println((String) "[CFG INFO] input_str: " + input_str); // 入力をそのまま出力する

    // 末尾の不要な文字列を消去する
    input_str = trimEnd(input_str, '\n');
    input_str = trimEnd(input_str, ' ');

    std::vector<String> input_str_vec = split(input_str, ' '); // 文字列を空白行で区切る

    if (input_str_vec.size() < 1) // 要素が何もない場合
        return false;

    // 命令コードの実行
    String opcode = input_str_vec[0];           // 命令コード（1つめの空白より前）
    input_str_vec.erase(input_str_vec.begin()); // 命令コードの要素を削除して引数要素のみにする
    std::vector<String> args = input_str_vec;   // 引数ベクトル

    const int command_size = commands.size(); // 登録済みコマンドの数

    if (opcode == "exit" || opcode == "quit") // configurationModeから出る命令
    {
        return true; // Exitフラグ
    }

    if (opcode == "help") // コマンド一覧を表示する
    {
        Serial.println("[CFG INFO] command list");
        for (int idx = 0; idx < command_size; idx++)
        {
            // 1コマンドずつ表示する
            Serial.print("  > ");
            Serial.print(commands[idx]->opcode());
            Serial.print("\t... ");
            Serial.println(commands[idx]->help_message());
        }
        return false;
    }

    // opcodeのコマンドが存在していたら実行する
    bool was_cmd_executed = false; //コマンドを実行したフラグ
    for (int idx = 0; idx < command_size; idx++)
    {
        if (opcode == commands[idx]->opcode())
        {
            commands[idx]->execute(args); //コマンドを実行する
            was_cmd_executed = true;      //コマンドを実行した
            break;
        }
    }

    if (!was_cmd_executed) // 無効なコマンドの場合
    {
        Serial.println("[CFG ERROR] Command not found: " + opcode);
    }

    return false;
}

// 指定文字区切りの文字列ベクトルを返す
// input_str: 元の文字列
// delim: 区切り文字(各要素にはこの文字は含まれない)
std::vector<String> ConfigurationMode::split(const String &input_str, const char delim)
{
    std::vector<String> vec_str;
    int len = input_str.length();
    for (int idx = 0, sp_idx; idx < len; idx = sp_idx + 1)
    {
        sp_idx = input_str.indexOf(delim, idx);
        if (sp_idx == -1)
            sp_idx = len;

        vec_str.push_back(input_str.substring(idx, sp_idx));
    }
    return vec_str;
}

// 末尾の特定文字を除く文字列を返す
// input_str: 元の文字列
// trimChar: 末尾から削除する文字
String ConfigurationMode::trimEnd(const String &input_str, const char trimChar)
{
    int end_idx = input_str.length() - 1;
    while (end_idx >= 0)
    {
        if (input_str[end_idx] != trimChar)
            break;
        else
            end_idx--;
    }
    return input_str.substring(0, end_idx + 1);
}