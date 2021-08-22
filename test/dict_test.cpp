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

namespace gt = gulachek::gtree;

using dict = gulachek::dictionary;

BOOST_AUTO_TEST_CASE(EncodesToMapOfValues)
{
	dict d;
	d.assign("foo", 0x006f6f66);
	d.assign("bar", std::string{"bar"});

	gt::mutable_tree tr;
	gt::encode(d, tr);

	std::map<std::string, std::string> map;
	gt::decode(tr, map);

	BOOST_TEST(map["foo"] == "foo");
	BOOST_TEST(map["bar"] == "bar");
}

BOOST_AUTO_TEST_CASE(DecodesFromMapOfValues)
{
	std::map<std::string, std::string> map;
	map["foo"] = "foo";
	map["bar"] = "bar";

	gt::mutable_tree tr;
	gt::encode(map, tr);

	dict d;
	gt::decode(tr, d);

	int foo; std::string bar;
	BOOST_TEST(!d.read("foo", foo));
	BOOST_TEST(foo == 0x006f6f66);

	BOOST_TEST(!d.read("bar", bar));
	BOOST_TEST(bar == "bar");
}

BOOST_AUTO_TEST_CASE(EncodesToMapOfContainers)
{
	using vec = std::vector<int>;
	vec foo = {1, 2, 3};
	vec bar = {4, 5, 6};

	dict d;
	d.assign("foo", foo);
	d.assign("bar", bar);

	gt::mutable_tree tr;
	gt::encode(d, tr);

	std::map<std::string, vec> map;
	gt::decode(tr, map);

	BOOST_TEST(map["foo"] == foo, tt::per_element());
	BOOST_TEST(map["bar"] == bar, tt::per_element());
}

BOOST_AUTO_TEST_CASE(DecodesFromMapOfContainers)
{
	using vec = std::vector<int>;
	vec foo = {1, 2, 3};
	vec bar = {4, 5, 6};

	std::map<std::string, vec> map;
	map["foo"] = foo;
	map["bar"] = bar;

	gt::mutable_tree tr;
	gt::encode(map, tr);

	dict d;
	gt::decode(tr, d);

	vec f, b;
	BOOST_TEST(!d.read("foo", f));
	BOOST_TEST(f == foo, tt::per_element());

	BOOST_TEST(!d.read("bar", b));
	BOOST_TEST(b == bar, tt::per_element());
}

BOOST_AUTO_TEST_CASE(ReadAlreadyConstructed)
{
	dict d;
	d.assign<int>("hello", 3);

	int out;
	BOOST_TEST(!d.read("hello", out));
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
