#define BOOST_TEST_MODULE DictionaryTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
namespace tt = boost::test_tools;
namespace bd = boost::unit_test::data;

#include "gulachek/dictionary.hpp"
//#include <boost/fusion/adapted/struct/adapt_struct.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
#include <gulachek/gtree/encoding/signed.hpp>

#include <map>

namespace gt = gulachek::gtree;

using dict = gulachek::dictionary;

BOOST_AUTO_TEST_CASE(EncodesToMapOfTrees)
{
	dict d;
	d.assign("foo", 123);
	d.assign("bar", std::string{"bar"});

	gt::mutable_tree tr;
	gt::encode(d, tr);

	std::map<std::string, gt::mutable_tree> map;
	gt::decode(tr, map);

	int foo;
	gt::decode(map["foo"], foo);
	BOOST_TEST(foo == 123);

	std::string bar;
	gt::decode(map["bar"], bar);
	BOOST_TEST(bar == "bar");
}

BOOST_AUTO_TEST_CASE(DecodesFromMapOfTrees)
{
	std::map<std::string, gt::mutable_tree> map;
	gt::encode(123, map["foo"]);
	gt::encode(std::string{"bar"}, map["bar"]);

	gt::mutable_tree tr;
	gt::encode(map, tr);

	dict d;
	gt::decode(tr, d);

	int foo; std::string bar;
	BOOST_TEST(!d.read("foo", foo));
	BOOST_TEST(foo == 123);

	BOOST_TEST(!d.read("bar", bar));
	BOOST_TEST(bar == "bar");
}

BOOST_AUTO_TEST_CASE(ReadAlreadyConstructed)
{
	dict d;
	d.assign<int>("hello", 3);

	int out;
	BOOST_TEST(!d.read("hello", out));
	BOOST_TEST(out == 3);
}
