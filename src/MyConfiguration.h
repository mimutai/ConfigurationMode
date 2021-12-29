#include <ConfigurationMode.h>

// [設定コマンドの実装例]
//   ConfigurationCommandを継承して任意のクラス名(CFG_setting01など)を設定する
//   コマンドを複数作成する場合にはその数だけクラスを作成する
//   文字列を返す関数(opcode, help_message)、コマンド入力時の処理を行う関数(execute)を実装する
class CFG_setting01 : public ConfigurationCommand
{
public:
    String opcode() { return "setting01"; };                    // この設定に入るための任意の文字列
    String help_message() { return "setting01_help_message"; }; // helpとタイプした時に表示される任意のメッセージ
    void execute(std::vector<String> args)
    {
        const int arg_size = args.size();
        // 引数が存在しない場合
        if (arg_size < 1)
        {
            // 現在の設定値を出力して終了
            Serial.println("[CFG INFO] " + opcode() + "= ");
            // Serial.println(getHoge()); // 現在の設定値を出力する
            return;
        }

        // 設定処理
        Serial.println("[CFG UPDATE] " + opcode() + "= " + args[0]); // 設定する値を出力
        // setHoge(args[0]); // 値を設定する
    }
};

class CFG_setting02 : public ConfigurationCommand
{
public:
    String opcode() { return "setting02"; };
    String help_message() { return "setting02_help_message"; };
    void execute(std::vector<String> args)
    {
        const int arg_size = args.size();
        if (arg_size < 1)
        {
            Serial.println("[CFG INFO] " + opcode() + "= ");
            // Serial.println(getHoge());
            return;
        }

        // 設定処理
        Serial.println("[CFG UPDATE] " + opcode() + "= " + args[0]);
        // setHoge(args[0]);
    }
};


// [コマンドの登録処理] 
class MyConfiguration
{
public:
    static void setup()
    {
        //作成したコマンドを登録する
        ConfigurationMode::addCommand(new CFG_setting01());
        ConfigurationMode::addCommand(new CFG_setting02());
    }
};