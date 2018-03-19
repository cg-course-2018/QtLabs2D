#include "stdafx.h"
#include "CppUnitTest.h"

#include <libmath/Color4f.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	constexpr float TOLERANCE = 0.000001f;
}

namespace libmath_tests
{
	TEST_CLASS(test_Color4f)
	{
	public:
		TEST_METHOD(Color4f_Default_construction)
		{
			Color4f c;
			Assert::AreEqual(c.r, 0.f, TOLERANCE);
			Assert::AreEqual(c.g, 0.f, TOLERANCE);
			Assert::AreEqual(c.b, 0.f, TOLERANCE);
			Assert::AreEqual(c.a, 1.f, TOLERANCE);
		}

		TEST_METHOD(Color4f_Custom_construction)
		{
			Color4f c(0.1f, 0.2f, 0.3f, 0.4f);
			Assert::AreEqual(c.r, 0.1f, TOLERANCE);
			Assert::AreEqual(c.g, 0.2f, TOLERANCE);
			Assert::AreEqual(c.b, 0.3f, TOLERANCE);
			Assert::AreEqual(c.a, 0.4f, TOLERANCE);
		}
	};
}
