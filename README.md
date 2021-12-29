# ConfigurationMode
シリアル通信によってプログラム変数を設定するモードを提供する ESP32プログラム向けのライブラリです。

## 導入方法
PlatformIOプロジェクト向けの導入方法を解説します。  
  
リポジトリのlibフォルダにあるConfigurationModeフォルダを、PlatformIOプロジェクトのlibフォルダ以下に配置します。  
リポジトリのMyConfiguration.hのようにコマンドを定義します。  
``` cpp
#include <ConfigurationMode.h>
// ConfigurationCommandを継承したクラスCFG_setting01(クラス名は任意)
class CFG_setting01 : public ConfigurationCommand
{
public:
    String opcode() { return "setting01"; };                    // この設定に入るための任意の文字列
    String help_message() { return "setting01_help_message"; }; // helpとタイプした時に表示される任意のメッセージ
    void execute(std::vector<String> args)
    {
        // ここに設定処理を記述します。
        // argsにはコマンド以後のスペースで区切られた文字列リストが入ります。
        // setting01 abc 123 の時, ["abc", "123"]
    }
};
```
  
リポジトリのMyConfiguration.hのようにコマンド登録処理を実装します。  
作成したクラスのインスタンスをaddCommandで登録します。  
``` cpp
// [コマンドの登録処理] 
class MyConfiguration
{
public:
    static void setup()
    {
        //作成したコマンドを登録する
        ConfigurationMode::addCommand(new CFG_setting01()); 
    }
};
```
  
main処理で登録処理、ConfigurationModeに入る処理を呼び出します。  
ConfigurationModeに入る前にシリアルポートをオープンしましょう。  
``` cpp

#include <Arduino.h>

#include <ConfigurationMode.h>
#include "MyConfiguration.h"

void setup()
{
  Serial.begin(115200); // シリアルポートをオープン

  MyConfiguration::setup(); // 作成したコマンドを登録する処理
  ConfigurationMode::start(); // ConfigurationModeに入る

  Serial.end();
}
```

## 使用例
### ESP32起動時にボタンが押されていた場合にConfigurationModeに入る  
``` cpp
void setup()
{
    // _button_pin が押されている場合
    if(digitalRead(_button_pin) == LOW)
    {
        Serial.begin(115200); // シリアルポートをオープン
        MyConfiguration::setup(); // 作成したコマンドを登録する処理
        ConfigurationMode::start(); // ConfigurationModeに入る
        Serial.end();  
    }
}
```
  
### IPアドレスをラインタイムで設定するコマンド  
``` cpp
#include <ConfigurationMode.h>
#include "WiFiControl.h" // これは独自のライブラリです

class CFG_ip : public ConfigurationCommand
{
public:
    String opcode() { return "ip"; };
    String help_message() { return "ipアドレスを設定する処理です. 入力例> ip 192.168.1.1"; };
    void execute(std::vector<String> args)
    {
        int arg_size = args.size();
        //引数が存在しない場合終了
        if (arg_size < 1)
        {
            Serial.println("[CFG INFO] " + opcode() + "= " + WiFiControl::getLocalIP());
            return;
        }

        // 設定処理
        // 入力例： ip 192.168.1.1
        std::vector<String> ip_octets = ConfigurationMode::split(args[0], '.');
        if (ip_octets.size() != 4)
        {
            // IPアドレスが3つのコロンで区切られていない
            Serial.println("[CFG ERROR] Invalid format.");
            return;
        }

        uint8_t first_octet = ip_octets[0].toInt();  // 192
        uint8_t second_octet = ip_octets[1].toInt(); // 168
        uint8_t third_octet = ip_octets[2].toInt();  // 1
        uint8_t fourth_octet = ip_octets[3].toInt(); // 1

        WiFiControl::storeLocalIP(first_octet, second_octet, third_octet, fourth_octet);
        Serial.println("[CFG UPDATE] " + opcode() + "= " + WiFiControl::getLocalIP());
    }
};
```
