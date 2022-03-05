#define BOOST_TEST_MODULE DictionaryTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
namespace tt = boost::test_tools;
namespace bd = boost::unit_test::data;

#include "gulachek/dictionary.hpp"
#include <gulachek/gtree/encoding/signed.hpp>
#include <gulachek/gtree/encoding/string.hpp>
#include <gulachek/gtree/encoding/map.hpp>
#include <gulachek/gtree/encoding/vector.hpp>

#include <map>
#include <vector>
#include <sstream>

namespace gt = gulachek::gtree;

using dict = gulachek::dictionary;

BOOST_AUTO_TEST_CASE(EncodesToMapOfValues)
{
	dict d;
	d.assign("foo", 0x006f6f66);
	d.assign("bar", std::string{"bar"});

	std::stringstream ss;
	auto err = gt::write(ss, d);
	BOOST_REQUIRE(!err);

	std::map<std::string, std::string> map;
	ss.seekg(0, std::ios::beg);
	gt::read(ss, &map);

	BOOST_TEST(map["foo"] == "foo");
	BOOST_TEST(map["bar"] == "bar");
}

BOOST_AUTO_TEST_CASE(AssignAnLvalueRef)
{
	int foo = 3;

	dict d;
	d.assign("foo", foo);

	int out = 0;
	d.read("foo", &out);

	BOOST_TEST(out == 3);
}

BOOST_AUTO_TEST_CASE(DecodesFromMapOfValues)
{
	std::map<std::string, std::string> map;
	map["foo"] = "foo";
	map["bar"] = "bar";

	std::stringstream ss;
	gt::write(ss, map);

	dict d;
	ss.seekg(0, std::ios::beg);
	auto err = gt::read(ss, &d);

	BOOST_REQUIRE(!err);

	int foo; std::string bar;
	BOOST_TEST(!d.read("foo", &foo));
	BOOST_TEST(foo == 0x006f6f66);

	BOOST_TEST(!d.read("bar", &bar));
	BOOST_TEST(bar == "bar");
}

BOOST_AUTO_TEST_CASE(ReadAlreadyConstructed)
{
	dict d;
	d.assign<int>("hello", 3);

	int out;
	BOOST_TEST(!d.read("hello", &out));
	BOOST_TEST(out == 3);
}

BOOST_AUTO_TEST_CASE(HasProperSize)
{
	dict d;
	BOOST_TEST(d.size() == 0);

	d.assign<int>("hello", 3);
	BOOST_TEST(d.size() == 1);

	d.assign<int>("goodbye", 4);
	BOOST_TEST(d.size() == 2);
}
