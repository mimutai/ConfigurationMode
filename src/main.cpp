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

void loop()
{
}
