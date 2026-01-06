#include "CppUnitTest.h"

import SessionManager; 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(SessionManagerTests)
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
		TEST_METHOD(Logout_Successful)
		{
			auto& sessionManager = SessionManager::Instance();
			sessionManager.Login(200);
			Assert::IsTrue(sessionManager.Logout(200));
			Assert::IsFalse(sessionManager.IsLoggedIn(200));
		}
		TEST_METHOD(Logout_NonExistingUser_Fails)
		{
			auto& sessionManager = SessionManager::Instance();
			Assert::IsFalse(sessionManager.IsLoggedIn(999));
			Assert::IsFalse(sessionManager.Logout(999));
		}

	};
}
