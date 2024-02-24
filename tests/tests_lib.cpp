#define BOOST_TEST_MODULE test_version

#include "Bulk.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
    BOOST_CHECK(version() > 0);
}

BOOST_AUTO_TEST_CASE(test1)
{
    bool  res;
    int sz = 3;
    Bulk b(sz);

    streambuf* standartCoutBuf = cout.rdbuf();
    streambuf* standartCinBuf = cin.rdbuf();

    {
        ostringstream out;
        cout.rdbuf(out.rdbuf());
        istringstream in;
        cin.rdbuf(in.rdbuf());
        in.str("a b c");
        b.processLoop();
        res = (out.str() == "bulk: a, b, c\n");
	BOOST_CHECK( res );
    }

    {
        ostringstream out;
        cout.rdbuf(out.rdbuf());
        istringstream in;
        cin.rdbuf(in.rdbuf());
        in.str("a b { c d e }");
        b.processLoop();
        res = (out.str() == "bulk: a, b\nbulk: c, d, e\n");
	BOOST_CHECK( res );
    }

    {
        ostringstream out;
        cout.rdbuf(out.rdbuf());
        istringstream in;
        cin.rdbuf(in.rdbuf());
        in.str("a b { c d e { f g } h } i j k");
        b.processLoop();
        res = (out.str() == "bulk: a, b\nbulk: c, d, e, f, g, h\nbulk: i, j, k\n");
	BOOST_CHECK( res );
    }

    {
        ostringstream out;
        cout.rdbuf(out.rdbuf());
        istringstream in;
        cin.rdbuf(in.rdbuf());
        in.str("a b c d e ");
        b.processLoop();
        res = (out.str() == "bulk: a, b, c\nbulk: d, e\n");
	BOOST_CHECK( res );
    }

    {
        ostringstream out;
        cout.rdbuf(out.rdbuf());
        istringstream in;
        cin.rdbuf(in.rdbuf());
        in.str("{ a b } ");
        b.processLoop();
        res = (out.str() == "");
	BOOST_CHECK( res );
    }

    cout.rdbuf(standartCoutBuf);
    cin.rdbuf(standartCinBuf);
	

}


BOOST_AUTO_TEST_SUITE_END()
