# Storage

## 概要

Arduinoの`SD`APIを使わず、`/mnt/sd0`にマウントされたSDカードをPOSIX API
（`open`/`read`/`write`/`close`）で直接操作するクラス。

## API

```cpp
#include <hundred_yen_sat/storage/Storage.h>

Storage sd;  // デフォルトroot: /mnt/sd0

void setup() {
    if (!sd.begin(3000)) {
        Serial.println("SD is not mounted at /mnt/sd0");
        return;
    }
    sd.makeDirectory("logs");
    sd.write("logs/config.txt", "hello", 5);
    sd.append("logs/sensor.csv", "1,2,3\n", 6);
}

void loop() {
    char buffer[64];
    size_t bytesRead = sd.read("logs/config.txt", buffer, sizeof(buffer));
}
```

- `begin(timeoutMs = 5000)` `root`がディレクトリとして現れるまで待つ。
  SDの初期化やmount自体は行わない。
- `mounted()` `root`が現在ディレクトリとして存在するかを確認する。
- `exists(path)` / `makeDirectory(path)` `root`からの相対パスに対する
  存在確認と1階層のディレクトリ作成（再帰作成はしない）。
- `append(path, data, size)` / `write(path, data, size)` 末尾追記、または
  切り詰めて書き込む。いずれも書き込み後に`fsync`してから閉じる。
- `read(path, data, capacity)` 最大`capacity`byte読み、実際に読んだbyte数
  を返す。

## 設計メモ

- Arduinoの`SD`APIを使わない最大の理由は、SDが未認識の状態でAPIを呼ぶと
  処理がハングする致命的な経路があるため。`begin()`は`stat()`による
  mount確認だけを行い、SDの初期化やmount処理そのものには関与しない設計に
  している。
- 書き込み系（`append`/`write`）は呼び出しごとに`mounted()`を再確認する。
  SDが運用中に抜かれた場合でも`open()`へ進まないようにするための安全策。
- `write()`は要求サイズ未満で正常復帰することがあるため、要求量に達するまで
  繰り返し呼ぶループにしている。一度の`write()`呼び出しで完了したと仮定
  しない。
- 書き込み成功時は`close()`前に`fsync()`する。電源断やリセットの可能性が
  高いミッション機体での運用を前提に、書き込み内容を媒体へ確実に反映する
  ことを優先している。
- パス引数は常に`root`からの相対パスとし、mount先のパスを`Storage`の内部に
  閉じ込めている。呼び出し側が絶対パスやmountポイントを意識する必要はない。
