# JSON パーサー

[ENGLISH](/README.md)

## Description

JSON パーサーです。C++11～C++20をサポートしています。

+ JSON 仕様に沿ったデータ構造クラスに、パース(parse)と出力(stringify)機能を付加したものです。
+ 1つのヘッダーファイルで動作します。外部ライブラリには依存していません。
+ JSON5 の一部仕様を実装しています。(オプションで無効に出来ます)
  + コメント付きJSONをパース可能です。
  + 末尾のカンマ(最後のカンマ)を許可します。
+ JSON Pointer を実装しています。
+ 初期化子リストでの構築が可能です。
+ 参照や編集等で例外は発生しない設計です。( at() 関数を除く)
  + 範囲外の読み込みはデフォルト値が取得され、書き込みの場合は要素を作成します。
+ javascript と違い、数値は浮動小数点数(double)と整数(std::intmax_t)に区別しています。
+ 入出力は std::string(UTF-8), std::u8string をサポートしています。
  + パース処理のストリーム入力には非対応です。


## Requirement

c++11～C++20 環境で動作します。動作に必要なものは特にありません。

以下の環境で動作確認しています。
+ linux g++
+ windows VisualStudio 2015,2017,2019,2022,2026

## Usage

Json.h をインクルードすることで使用可です。

## example

```c++

#include "Json.h"

try {
    using Json = rlib::Json;
    const Json j = Json::parse(                         // JSON 文字列から構築
        u8R"({                  // allows comments (JSON5)
            "n" : -123.456e+2,
            "list":[
                32,
                "ABC",          // allows Trailing comma (JSON5)
            ],
            "b": true,
            "c": null
        })");
    double d0 = j["n"].get<double>();                   // -123.456e+2 を取得
    double da = j.at("n").get<double>();                // at() で参照する記述です。（範囲外の場合に例外が発生します）
    double d1 = j["e"].get<double>();                   // 0.0 を取得 (存在しない位置を指定したのでデフォルト値が取れる)
    std::intmax_t n1 = j["n"].get<std::intmax_t>();     // -12346 を取得 (double値を四捨五入した整数値が取れます)
    std::string s0 = j["list"][1].get<std::string>();   // "ABC" を取得
    std::string sa = j.at(Json::Pointer("/list/1")).get<std::string>(); // JSON Pointerで指定する記述です。
    std::string s1 = j["ary"][9].get<std::string>();    // 空文字を取得 (存在しない位置を指定したのでデフォルト値が取れる)
    Json list = j["list"];                              // "list"以下をコピー(複製)
    list[10]["add"] = 123;                              // [10]の位置に {"add":123} を 追加 ( 配列[2～9]の位置は null で埋められる)
    bool compare = list == j["list"];                   // 比較です。false が返ります。
    std::string json = list.stringify();                // JSON 文字列を取得
    list[10].erase("add");                              // [10]の位置の連想配列の要素({"add":123})を削除
    list.erase(9);                                      // [9]の位置の要素(null)を削除
    const Json j1 = Json::Map{                          // 初期化子リストでの構築です
        {"a", 123},                                     //  {   "a": 123,
        {"b", Json::Array{456, "ABC", 0.5}},            //      "b": [456, "ABC", 0.5],
        {"c", Json::Map{                                //      "c": {
            {"d", true},                                //          "d": true,
            {"e", nullptr},                             //          "e": null
        }},                                             //      }
    };                                                  //  }
    std::map<Json, int> map{ {j,0},{j1,1} };            // std::map, set などのキーにすることが可能です
    const Json& c = j1.at("f");                         // at() で参照すると範囲外の場合に例外が発生します
} catch (rlib::Json::ParseException& e) {               // パース 失敗
    std::cerr << e.what() << std::endl;
} catch (std::out_of_range& e) {                        // 範囲外参照
    std::cerr << e.what() << std::endl;
}
```


## Feature and Limitations

- - Json_test.cpp はテストコードです。テストコードには BOOST.TEST を使っていますのでご注意ください。

## Licence

[LICENSE](/LICENSE)

