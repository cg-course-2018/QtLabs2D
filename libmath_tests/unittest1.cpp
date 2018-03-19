#include "stdafx.h"
#include "CppUnitTest.h"

#include <libmath/Vector2f.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	constexpr float TOLERANCE = 0.000001f;
}

namespace libmath_tests
{		
TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Default_construction)
		{
			Vector2f v1;
			Assert::AreEqual(v1.x, 0.0f, TOLERANCE);
			Assert::AreEqual(v1.y, 0.0f, TOLERANCE);
		}

		TEST_METHOD(Custom_construction)
		{
			Vector2f v1(2.5f, -5.4f);
			Assert::AreEqual(v1.x, 2.5f, TOLERANCE);
			Assert::AreEqual(v1.y, -5.4f, TOLERANCE);
		}

		TEST_METHOD(Uniform_construction)
		{
			Vector2f v1{ 3.5f, -12.4f };
			Assert::AreEqual(v1.x, 3.5f, TOLERANCE);
			Assert::AreEqual(v1.y, -12.4f, TOLERANCE);

			Vector2f v2{ };
			Assert::AreEqual(v2.x, 0.f, TOLERANCE);
			Assert::AreEqual(v2.y, 0.f, TOLERANCE);

			Vector2f v3 = { 6.6f, 7.7f };
			Assert::AreEqual(v3.x, 6.6f, TOLERANCE);
			Assert::AreEqual(v3.y, 7.7f, TOLERANCE);
		}

		TEST_METHOD(Assignment_construction)
		{
			Vector2f v1(2.5f, -5.4f);
			Vector2f v2 = v1;

			Assert::AreEqual(v2.x, 2.5f, TOLERANCE);
			Assert::AreEqual(v2.y, -5.4f, TOLERANCE);
			// change v2
			v2.x = 5;
			v2.y = 6;
			// check v1 is the same
			Assert::AreEqual(v1.x, 2.5f, TOLERANCE);
			Assert::AreEqual(v1.y, -5.4f, TOLERANCE);
		}

		TEST_METHOD(Copy_construction)
		{
			Vector2f v1(2.5f, -5.4f);
			Vector2f v2(v1);

			Assert::AreEqual(v2.x, 2.5f, TOLERANCE);
			Assert::AreEqual(v2.y, -5.4f, TOLERANCE);
			
			Vector2f v3{ v1 };
			// check v1 is the same
			Assert::AreEqual(v3.x, 2.5f, TOLERANCE);
			Assert::AreEqual(v3.y, -5.4f, TOLERANCE);
		}

		TEST_METHOD(Length)
		{
			Vector2f v1{ 2.f, 0.f };
			Assert::AreEqual(v1.length(), 2.0f, TOLERANCE);
			
			Vector2f v2{ 3, -4 };
			Assert::AreEqual(v2.length(), 5, TOLERANCE);
		}

		TEST_METHOD(Operator_plus)
		{
			Vector2f v1{ 2.f, 0.f };
			Vector2f v2{ 3, -4 };

			Vector2f v3 = v1 + v2;

			Assert::AreEqual(v3.x, 5.0f, TOLERANCE);
			Assert::AreEqual(v3.y, -4.0f, TOLERANCE);

			v3 += Vector2f(-1, -1);
			Assert::AreEqual(v3.x, 4.0f, TOLERANCE);
			Assert::AreEqual(v3.y, -5.0f, TOLERANCE);
		}

		TEST_METHOD(Operator_minus)
		{
			Vector2f v1{ 2.f, 0.f };
			Vector2f v2{ 3, -4 };

			Vector2f v3 = v1 - v2;

			Assert::AreEqual(v3.x, -1.0f, TOLERANCE);
			Assert::AreEqual(v3.y, 4.0f, TOLERANCE);

			v3 -= Vector2f(2, 1);
			Assert::AreEqual(v3.x, -3.0f, TOLERANCE);
			Assert::AreEqual(v3.y, 3.0f, TOLERANCE);

			Vector2f v3_bck = v3;
			Vector2f v4 = -v3;
			Assert::AreEqual(v4.x, 3.0f, TOLERANCE);
			Assert::AreEqual(v4.y, -3.0f, TOLERANCE);
			
			Assert::AreEqual(v3_bck.x, v3.x, TOLERANCE);
			Assert::AreEqual(v3_bck.y, v3.y, TOLERANCE);
		}

		TEST_METHOD(Operator_compare)
		{
			Vector2f v1{ 2.f, 0.f };
			Vector2f v2{ 3, -4 };
			Vector2f v3 = v1;

			Assert::IsFalse(v1 == v2);
			Assert::IsTrue(v1 == v3);
			
			Assert::IsTrue(v1 != v2);
			Assert::IsFalse(v1 != v3);
		}

		TEST_METHOD(Operator_multiply)
		{
			Vector2f v1{ 2.f, 1.f };
			Vector2f v2 = v1 * 3.5;

			Assert::AreEqual(v2.x, 7.f, TOLERANCE);
			Assert::AreEqual(v2.y, 3.5f, TOLERANCE);

			Vector2f v3 = -2 * v2;
			
			Assert::AreEqual(v3.x, -14.f, TOLERANCE);
			Assert::AreEqual(v3.y, -7.0f, TOLERANCE);

			v3 *= 1.5;

			Assert::AreEqual(v3.x, -21.f, TOLERANCE);
			Assert::AreEqual(v3.y, -10.5f, TOLERANCE);
		}

		TEST_METHOD(Operator_divide)
		{
			Vector2f v1{ 6.f, 3.f };
			Vector2f v2 = v1 / 1.5;

			Assert::AreEqual(v2.x, 4.f, TOLERANCE);
			Assert::AreEqual(v2.y, 2.f, TOLERANCE);

			v2 /= -2;

			Assert::AreEqual(v2.x, -2.f, TOLERANCE);
			Assert::AreEqual(v2.y, -1.f, TOLERANCE);
		}

		TEST_METHOD(Dot_product)
		{
			Vector2f v1{ 2, 0 };
			Vector2f v2{ 0, 4 };
			Vector2f v3{ -2, 0 };

			float f1 = dot(v1, v2);
			Assert::AreEqual(f1, 0.f, TOLERANCE);
			
			float f2 = dot(v1, v3);
			Assert::AreEqual(f2, -4.f, TOLERANCE);

			Assert::AreEqual(dot(Vector2f{}, Vector2f{ 5,6 }), 0.f, TOLERANCE);
			Assert::AreEqual(dot(Vector2f{2,-3}, Vector2f{ 5,6 }), -8.f, TOLERANCE);
		}

		TEST_METHOD(Normalize_test)
		{
			Vector2f v1n{ 2, 0 };
			Vector2f v2n{ 0, -4 };
			Vector2f v3n{ -2, -2 };

			v1n.normalize();
			Assert::AreEqual(v1n.x, 1.f, TOLERANCE);
			Assert::AreEqual(v1n.y, 0.f, TOLERANCE);

			v2n.normalize();
			Assert::AreEqual(v2n.x, 0.f, TOLERANCE);
			Assert::AreEqual(v2n.y, -1.f, TOLERANCE);

			v3n.normalize();
			Assert::AreEqual(v3n.x, -sqrtf(2) / 2.f, TOLERANCE);
			Assert::AreEqual(v3n.y, -sqrtf(2) / 2.f, TOLERANCE);
		}

		TEST_METHOD(Normed_test)
		{
			Vector2f v1{ 2, 0 };
			Vector2f v2{ 0, -4 };
			Vector2f v3{ -2, -2 };

			Vector2f v1n = norm(v1);
			Assert::AreEqual(v1n.x, 1.f, TOLERANCE);
			Assert::AreEqual(v1n.y, 0.f, TOLERANCE);

			Vector2f v2n = norm(v2);
			Assert::AreEqual(v2n.x, 0.f, TOLERANCE);
			Assert::AreEqual(v2n.y, -1.f, TOLERANCE);

			Vector2f v3n = norm(v3);
			Assert::AreEqual(v3n.x, -sqrtf(2) / 2.f, TOLERANCE);
			Assert::AreEqual(v3n.y, -sqrtf(2) / 2.f, TOLERANCE);
		}

		TEST_METHOD(Perp_test)
		{
			Vector2f v1{ 2, 0 };
			Vector2f v2{ -2, -4.5 };

			Vector2f v1p = perp(v1);
			Assert::AreEqual(dot(v1, v1p), 0.f, TOLERANCE);

			Vector2f v2p = perp(v2);
			Assert::AreEqual(dot(v2, v2p), 0.f, TOLERANCE);
		}

		TEST_METHOD(Distance_test)
		{
			Vector2f v1{ 3, 0 };
			Vector2f v2{ 0, 4 };

			Assert::AreEqual(dist(v1, v2), 5.f, TOLERANCE);
		}

		TEST_METHOD(Projection_test)
		{
			Vector2f v1{ 3, 4 };
			Vector2f v2{ 2.2f, 0 };
			Vector2f v3{ 0, -1.5 };

			float v1_2 = v1.projectOn(v2);
			Assert::AreEqual(v1_2, 3.f, TOLERANCE);

			float v1_3 = v1.projectOn(v3);
			Assert::AreEqual(v1_3, -4.f, TOLERANCE);

			float v2_3 = v2.projectOn(v3);
			Assert::AreEqual(v2_3, 0.f, TOLERANCE);
		}
	};
}