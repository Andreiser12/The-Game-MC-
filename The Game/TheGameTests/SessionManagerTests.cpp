#include "CppUnitTest.h"

import SessionManager; 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(TheGameTests)
	{
	public:
		TEST_METHOD(Login_Successful)
		{
			auto& sessionManager = SessionManager::Instance();
			Assert::IsTrue(sessionManager.Login(100));
			Assert::IsTrue(sessionManager.IsLoggedIn(100));
			sessionManager.Logout(100);
		}
		TEST_METHOD(Double_Login_Fails)
		{
			auto& sessionManager = SessionManager::Instance();
			Assert::IsTrue(sessionManager.Login(101));
			Assert::IsFalse(sessionManager.Login(101));
			sessionManager.Logout(101);
		}
	};
}
