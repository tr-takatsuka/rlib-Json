#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

#include "Json.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4566)
#endif

template <class CharT> std::basic_string<CharT> StringFormat(const boost::basic_format<CharT>& format) {
	return format.str();
}
template <class CharT, class Head, class... Tail> std::basic_string<CharT> StringFormat(boost::basic_format<CharT>& format, Head& head, Tail&&... tail) {
#if 0
	try {
		using HeadT = std::remove_const_t<Head>;
		if constexpr (std::is_same_v<HeadT, ATL::CStringA> || std::is_same_v<HeadT, ATL::CStringW>) {
			return StringFormat<CharT>(format % std::basic_string<CharT>(head.GetString()), tail...);
		}
	} catch (...) {
	}
#endif
	return StringFormat<CharT>(format % head, tail...);
}

#if 0
template <std::size_t I, class CharT, class... Head, class... Tail> std::basic_string<CharT> StringFormat(boost::basic_format<CharT>& format, const std::tuple<Head...>& head, Tail&&... tail)
{
	if constexpr (I < sizeof...(Head)) {
		return StringFormat<I + 1, CharT>(format % std::get<I>(head), head, tail...);
	}
	return StringFormat<CharT>(format, tail...);;
}

template <class CharT, class... Head, class... Tail> std::basic_string<CharT> StringFormat(boost::basic_format<CharT>& format, const std::tuple<Head...>& head, Tail&&... tail) {
	return StringFormat<0, CharT>(format, head, tail...);
}
#endif

template <class CharT, class... Args> std::basic_string<CharT> StringFormat(const CharT* lpszFormat, Args&&... args) {
	boost::basic_format<CharT> format;
	format.exceptions(boost::io::no_error_bits);  // 例外を発生させない
	format.parse(lpszFormat);
	return StringFormat<CharT>(format, args...);
}
template <class CharT, class... Args> std::basic_string<CharT> StringFormat(const std::basic_string<CharT>& s, Args&&... args) {
	return StringFormat<CharT>(s.c_str(), args...);
}


BOOST_AUTO_TEST_CASE(json_test)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);	// ログレベル

	{// version 表示
		const auto v = rlib::Json::version();
		BOOST_TEST_MESSAGE(StringFormat("rlib::Json version %d.%d.%d",
			// v));
			std::get<0>(v), std::get<1>(v), std::get<2>(v)));
	}

	// example
	try {
		using Json = rlib::Json;
		const Json j = Json::parse(							// JSON 文字列から構築
			u8R"({					// allows comments (JSON5)
				"n" : -123.456e+2,
				"list":[
					32,
					"ABC",			// allows Trailing comma (JSON5)
				],
				"b": true,
				"c": null
			})");
		double d0 = j["n"].get<double>();					// -123.456e+2 を取得
		double da = j.at("n").get<double>();				// at() で参照する記述です。（範囲外の場合に例外が発生します）
		double d1 = j["e"].get<double>();					// 0.0 を取得 (存在しない位置を指定したのでデフォルト値が取れる)
		std::intmax_t n1 = j["n"].get<std::intmax_t>();		// -12346 を取得 (double値を四捨五入した整数値が取れます)
		std::string s0 = j["list"][1].get<std::string>();	// "ABC" を取得
		std::string sa = j.at(Json::Pointer("/list/1")).get<std::string>();	// JSON Pointerで指定する記述です。
		std::string s1 = j["ary"][9].get<std::string>();	// 空文字を取得 (存在しない位置を指定したのでデフォルト値が取れる)
		Json list = j["list"];								// "list"以下をコピー(複製)
		list[10]["add"] = 123;								// [10]の位置に {"add":123} を 追加 ( 配列[2～9]の位置は null で埋められる)
		bool compare = list == j["list"];					// 比較です。false が返ります。
		std::string json = list.stringify();				// JSON 文字列を取得
		list[10].erase("add");								// [10]の位置の連想配列の要素({"add":123})を削除
		list.erase(9);										// [9]の位置の要素(null)を削除
		const Json j1 = Json::Map{							// 初期化子リストでの構築です
			{"a", 123},										//	{	"a": 123,
			{"b", Json::Array{456, "ABC", 0.5}},			//		"b": [456, "ABC", 0.5],
			{"c", Json::Map{								//		"c": {
				{"d", true},								//			"d": true,
				{"e", nullptr},								//			"e": null
			}},												//		}
		};													//	}
		std::map<Json, int> map{ {j,0},{j1,1} };			// std::map, set などのキーにすることが可能です
		const Json& c = j1.at("f");							// at() で参照すると範囲外の場合に例外が発生します
	} catch (rlib::Json::ParseException& e) {				// パース 失敗
		std::cerr << e.what() << std::endl;
	} catch (std::out_of_range& e) {						// 範囲外参照
		std::cerr << e.what() << std::endl;
	}



	try {
		using Json = rlib::Json;

		{// set type
			BOOST_TEST_MESSAGE("set type");
			std::string str0("a");
			const std::string str1("a");
			bool b0 = true;
			const bool b1 = false;
			float f0 = 0;
			const float f1 = 1;
			double d0 = 0;
			const double d1 = 1;
			char c0 = 0;
			const char c1 = 1;
			unsigned char uc0 = 0;
			const unsigned char uc1 = 1;
			int i0 = 0;
			const int i1 = 1;
			unsigned int ui0 = 0;
			const unsigned int ui1 = 1;
			short s0 = 0;
			const short s1 = 1;
			unsigned short us0 = 0;
			const unsigned short us1 = 1;
			long long ll0 = 0;
			const long long ll1 = 1;
			unsigned long long  ull0 = 0;
			const unsigned long long  ull1 = 1;
			Json j = Json::Map{
				{"null", Json::Array{nullptr}},
				{"bool", Json::Array{true, false, b0, b1}},
				{"string", Json::Array{"a", str0, str1}},
				{"string2", {"a","b"}},
				{"float", Json::Array{0.0, 1.0f, -123.456e+2, f0, f1, d0, d1}},
				{"int", Json::Array{
					0, -1, 123,
					c0,c1,uc0,uc1,
					i0,i1,ui0,ui1,
					s0,s1,us0,us1,
					ll0,ll1,ull0,ull1,
				}},
				{"int2", {1,2,3,4}},		// std::initializer_list
			};
			for (auto& i : j["null"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::Null));
			for (auto& i : j["bool"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::Bool));
			for (auto& i : j["string"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::String));
			for (auto& i : j["string2"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::String));
			for (auto& i : j["float"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::Float));
			for (auto& i : j["int"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::Int));
			for (auto& i : j["int2"].get<Json::Array>()) BOOST_CHECK(i.isType(Json::Type::Int));
		}

		{// singles
			BOOST_TEST_MESSAGE("singles");
			BOOST_CHECK(Json::parse(R"(null)").isType(Json::Type::Null));
			BOOST_CHECK(Json::parse(R"(true)").get<bool>() == true);
			BOOST_CHECK(Json::parse(R"(false)").get<bool>() == false);
			BOOST_CHECK(Json::parse(R"("abc")").get<std::string>() == "abc");
			BOOST_CHECK(Json::parse(R"(123)").get<int>() == 123);
			BOOST_CHECK_CLOSE(Json::parse(u8R"(456.789)").get<double>(), 456.789, 0.001);
			BOOST_CHECK(Json::parse(R"(" ")").get<std::string>() == " ");
			BOOST_CHECK(Json::parse(R"("	")").get<std::string>() == "\t");		// エスケープなしのタブ。とりあえず許可する。
			BOOST_CHECK_THROW(Json::parse("\"\\\""), Json::ParseException);			// "\"
			BOOST_CHECK_THROW(Json::parse(R"("\a")"), Json::ParseException);		// 未定義のエスケープ文字
		}

		{// key name
			BOOST_TEST_MESSAGE("key name");
			const auto j = Json::parse(u8R"({"!\"#$%&'()=~|{}`_?>< +-@;:[]\\,./" : "!\"#$%&'()=~|{}`_?>< +-@;:[]\\,./"})");
			const auto s = u8R"(!"#$%&'()=~|{}`_?>< +-@;:[]\,./)";
			BOOST_CHECK(j[s].get<std::string>() == s);
			BOOST_CHECK(j == Json::parse(j.stringify()));
		}

		{// etc
			BOOST_TEST_MESSAGE("etc");
			const auto j = Json::parse(
				u8R"([						// 配列
						123,				// 数字
						"abc",				// 文字列
						null,				// null
						true,				// true
						false,				// false
						{					/* オブジェクト */
							"a":			/*
											 * 複数行コメント
											 */
								456.789,
							"b": 555,
							"":"empty",		// 空文字キー
							" ":"space",	// 空白文字キー
							"\"":"dq",
							"\\":"bslash",
							"/":"slash",
							"\b":"bs",		// バックスペース
							"\f":"ff",		// 改ページ
							"\n":"cr",		// CR
							"\r":"lf",		// LF
							"\t":"tab",		// タブ
							"~":"tilde",
						}
					]						// 閉じる
				)");
			BOOST_CHECK(j[0].get<int>() == 123);
			BOOST_CHECK(j[1].get<std::string>() == "abc");
			BOOST_CHECK(j[2].isType(Json::Type::Null));
			BOOST_CHECK(j[3].get<bool>() == true);
			BOOST_CHECK(j[4].get<bool>() == false);
			BOOST_CHECK_CLOSE(j[5]["a"].get<double>(), 456.789, 0.001);
			BOOST_CHECK(j[5][""].get<std::string>() == "empty");
			BOOST_CHECK(j[5][" "].get<std::string>() == "space");
			BOOST_CHECK(j[5]["\""].get<std::string>() == "dq");
			BOOST_CHECK(j[5]["\\"].get<std::string>() == "bslash");
			BOOST_CHECK(j[5]["/"].get<std::string>() == "slash");
			BOOST_CHECK(j[5]["\b"].get<std::string>() == "bs");
			BOOST_CHECK(j[5]["\f"].get<std::string>() == "ff");
			BOOST_CHECK(j[5]["\n"].get<std::string>() == "cr");
			BOOST_CHECK(j[5]["\r"].get<std::string>() == "lf");
			BOOST_CHECK(j[5]["\t"].get<std::string>() == "tab");
			BOOST_CHECK(j[5]["~"].get<std::string>() == "tilde");
			// JSON Pointer
			BOOST_CHECK(j.at(Json::Pointer("")) == j);
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/b)")).get<int>() == 555);
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/)")).get<std::string>() == "empty");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/ )")).get<std::string>() == "space");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\")")).get<std::string>() == "dq");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\\)")).get<std::string>() == "bslash");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/~1)")).get<std::string>() == "slash");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\b)")).get<std::string>() == "bs");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\f)")).get<std::string>() == "ff");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\n)")).get<std::string>() == "cr");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\r)")).get<std::string>() == "lf");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/\t)")).get<std::string>() == "tab");
			BOOST_CHECK(j.at(Json::Pointer(R"(/5/~0)")).get<std::string>() == "tilde");
			BOOST_CHECK_THROW(j.at(Json::Pointer("/")), std::out_of_range);
			BOOST_CHECK_THROW(j.at(Json::Pointer("/5/b/")), std::out_of_range);
			BOOST_CHECK_THROW(j.at(Json::Pointer("/10/b/")), std::out_of_range);
			BOOST_CHECK_THROW(j.at(Json::Pointer("e/10/b/")), std::out_of_range);
			BOOST_CHECK_THROW(j.at(Json::Pointer("0/10/b/")), std::out_of_range);
			BOOST_CHECK_THROW(j.at(Json::Pointer(R"(/5/~)")), std::out_of_range);
			// stringify
			BOOST_CHECK(j == Json::parse(j.stringify()));
			BOOST_CHECK(Json::parse(j.stringify()) == Json::parse(j.stringify(Json::Stringify::standard)));
		}

		{// etc2
			BOOST_TEST_MESSAGE("etc2");
			const auto j = Json::parse(
				u8R"({
						"123": true			// 数字だけの文字列
					}
				)");
			BOOST_CHECK(j["123"].get<bool>());
			BOOST_CHECK(j.at(Json::Pointer(R"(/123)")).get<bool>());
			// stringify
			BOOST_CHECK(j == Json::parse(j.stringify()));
		}


		{// JSON Pointer
			BOOST_TEST_MESSAGE("JSON Pointer");
			const auto j = Json::parse(u8R"(
				{
					"foo": ["bar", "baz"] ,
					"" : 0,
					"a/b" : 1,
					"c%d" : 2,
					"e^f" : 3,
					"g|h" : 4,
					"i\\j" : 5,
					"k\"l" : 6,
					" " : 7,
					"m~n" : 8
				})");
			BOOST_CHECK(j.at(Json::Pointer(R"(/foo)")) == Json::parse(u8R"(["bar", "baz"])"));
			BOOST_CHECK(j.at(Json::Pointer(R"(/foo/0)")).get<std::string>() == "bar");
			BOOST_CHECK(j.at(Json::Pointer(R"(/)")).get<int>() == 0);
			BOOST_CHECK(j.at(Json::Pointer(R"(/a~1b)")).get<int>() == 1);
			BOOST_CHECK(j.at(Json::Pointer(R"(/c%d)")).get<int>() == 2);
			BOOST_CHECK(j.at(Json::Pointer(R"(/e^f)")).get<int>() == 3);
			BOOST_CHECK(j.at(Json::Pointer(R"(/g|h)")).get<int>() == 4);
			BOOST_CHECK(j.at(Json::Pointer(R"(/i\\j)")).get<int>() == 5);
			BOOST_CHECK(j.at(Json::Pointer(R"(/k\"l)")).get<int>() == 6);
			BOOST_CHECK(j.at(Json::Pointer(R"(/ )")).get<int>() == 7);
			BOOST_CHECK(j.at(Json::Pointer(R"(/m~0n)")).get<int>() == 8);
			BOOST_CHECK(j == Json::parse(j.stringify()));
		}

		BOOST_TEST_MESSAGE("float");
		for (auto i : { "%8.5e","%lf" }) {
			std::vector<double> l = {
				-111.1, 0.0, 1.23456789, 123.456789, -1.23456789, -123.456789,
				(std::numeric_limits<double>::max)(),
				// (std::numeric_limits<double>::min)(),	← "2.22507e-308" となり stod() で例外が発生するので
			};
			for (auto n : l) {
				const auto s = StringFormat(StringFormat("%s", i), n);
				const auto json = Json::parse(s);
				auto r0 = json.get<double>();
				if (r0 > -0.01 && r0 < 0.01) {
					BOOST_CHECK_SMALL(n - r0, 0.001);
				} else {
					BOOST_CHECK_CLOSE(n, r0, 0.001);
				}
				auto r1 = json.get<intmax_t>();
				BOOST_CHECK(std::llroundl(n) == r1);
			}
		}

		{// int
			BOOST_TEST_MESSAGE("int");
			std::vector<std::intmax_t> l = {
				0, 1, 123, -1, -123, 0x7fffffffffffffff, 0xfffffffffffffff,
				(std::numeric_limits<std::intmax_t>::max)(),  (std::numeric_limits<std::intmax_t>::min)()
			};
			for (auto n : l) {
				const auto s = StringFormat(u8R"(
						{"a" : %ld}
					)", n);
				auto json = Json::parse(s);
				auto r0 = json["a"].get<double>();
				BOOST_CHECK_CLOSE(static_cast<double>(n), r0, 0.001);
				auto r1 = json["a"].get<intmax_t>();
				BOOST_CHECK(n == r1);
			}
		}

		{// 大きい整数(doubleで処理する)
			const auto j = Json::parse(u8R"(123456789123456789123456789)");
			BOOST_CHECK_CLOSE(j.get<double>(), 123456789123456789123456789.0, 0.001);
		}

		{// parse error カンマ漏れ
			BOOST_TEST_MESSAGE("missing comma");
			BOOST_CHECK_THROW(Json::parse(
				u8R"({
					"a" : 1
					"b" : 2
				)")
				, Json::ParseException);
		}

		{// trailing comma 末尾のカンマ
			BOOST_TEST_MESSAGE("trailing comma");
			std::string s =
				u8R"([
					{
						"a":1,
						"b":2,
					},
					[
						3,
						4,
					],
				])";
			const auto j = Json::parse(s);
			BOOST_CHECK(j[0]["b"].get<int>() == 2);
			BOOST_CHECK(j[0].get<Json::Map>().size() == 2);
			BOOST_CHECK(j[1][1].get<int>() == 4);
			BOOST_CHECK(j[1].get<Json::Array>().size() == 2);
			BOOST_CHECK(j.get<Json::Array>().size() == 2);
			BOOST_CHECK_THROW(Json::parse(s,
				[] {
					Json::ParseOptions opt;
					opt.trailingComma = false;
					return opt;
				}()), Json::ParseException);
		}

		{// サロゲートペア
			BOOST_TEST_MESSAGE("surrogate pair");
			const auto j = Json::parse(u8R"(
					["\ud83c\udf59","かんきせん\u306f\"換気扇\""]
				)");
			BOOST_CHECK(j[0].get<std::string>() == u8R"(🍙)");
			BOOST_CHECK(j[1].get<std::string>() == u8R"(かんきせんは"換気扇")");
		}

		{// stringify
			BOOST_TEST_MESSAGE("stringify");
			const auto j = Json::parse(u8R"(
					[
						123,"abc",null,true,false,
						{
							"a" : 456.789,
							"\ud83c\udf59" : "かんきせん\u306f\"換気扇\""
						}
					]
				)");
			const auto json = j.stringify();
			const auto j2 = Json::parse(json,
				[] {
					Json::ParseOptions opt;
					opt.comment = false;
					opt.trailingComma = false;
					return opt;
				}());
			BOOST_CHECK(j == j2);
		}
	} catch (std::exception& e) {				// パース 失敗
		std::cerr << e.what() << std::endl;
		BOOST_FAIL(e.what());
	} catch (...) {
		BOOST_FAIL("unknown");
	}


}

