#include "stdafx.h"
#include "CppUnitTest.h"

#include <libmath/RectF.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	constexpr float TOLERANCE = 0.000001f;
}

namespace libmath_tests
{
TEST_CLASS(test_RectF)
{
public:
	TEST_METHOD(Default_construction)
	{
		RectF r;
		Assert::AreEqual(r.origin.x, 0.f, TOLERANCE);
		Assert::AreEqual(r.origin.y, 0.f, TOLERANCE);
		Assert::AreEqual(r.size.x, 0.f, TOLERANCE);
		Assert::AreEqual(r.size.y, 0.f, TOLERANCE);
	}

	TEST_METHOD(Custom_construction)
	{
		RectF r(1, 2, 3, 4);
		Assert::AreEqual(r.origin.x, 1.f, TOLERANCE);
		Assert::AreEqual(r.origin.y, 2.f, TOLERANCE);
		Assert::AreEqual(r.size.x, 3.f, TOLERANCE);
		Assert::AreEqual(r.size.y, 4.f, TOLERANCE);
	}

	TEST_METHOD(Construct_from_vectors)
	{
		RectF r(Vector2f(1, 2), Vector2f(3, 4));
		Assert::AreEqual(r.origin.x, 1.f, TOLERANCE);
		Assert::AreEqual(r.origin.y, 2.f, TOLERANCE);
		Assert::AreEqual(r.size.x, 3.f, TOLERANCE);
		Assert::AreEqual(r.size.y, 4.f, TOLERANCE);
	}

	TEST_METHOD(Edges)
	{
		RectF r(Vector2f(5, 6), Vector2f(7, 8));

		Assert::AreEqual(r.leftEdge(), 5.f, TOLERANCE);
		Assert::AreEqual(r.rightEdge(), 12.f, TOLERANCE);
		Assert::AreEqual(r.topEdge(), 6.f, TOLERANCE);
		Assert::AreEqual(r.bottomEdge(), 14.f, TOLERANCE);
	}
};
}