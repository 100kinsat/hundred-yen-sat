# Recorder

## 概要

`Storage`を介して1つのファイルへCSV行や任意データを追記する簡易レコーダー。

## API

```cpp
#include <hundred_yen_sat/storage/Recorder.h>

Storage sd;
Recorder recorder(sd);

void setup() {
    if (sd.begin(3000)) {
        recorder.begin("sensor.csv", "millis,light");
    }
}

void loop() {
    char line[48];
    snprintf(line, sizeof(line), "%lu,%u", millis(), lightValue);
    recorder.writeLine(line);
}
```

- `begin(path, header = nullptr)` 記録先を設定する。`path`に該当するファイル
  が存在しない場合だけ、`header`を最初の1行として書き込む。
- `writeLine(line)` 文字列と改行を追記する。
- `write(data, size)` 改行を付けずに任意のバイト列を追記する。

## 設計メモ

- ヘッダ行の書き込みを「ファイルが存在しない場合だけ」に限定しているのは、
  再起動やリセットを挟んでも同じファイルへ追記し続けられるようにするため。
  毎回`begin()`を呼んでもヘッダが重複しない。
- `Recorder`自身はファイルI/Oの詳細（fsync・パス解決など）を持たず、すべて
  `Storage`に委譲する。複数の`Recorder`が同じ`Storage`を共有しても、
  mount確認やパス解決のロジックは1箇所にまとまる。
