#include "CppUnitTest.h"
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
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
		TEST_METHOD(IsOnline_ImmediatelyAfterLogin)
		{
			auto& sessionManager = SessionManager::Instance();
			sessionManager.Login(400);
			Assert::IsTrue(sessionManager.IsOnline(400, 5s));
			sessionManager.Logout(400);
		}
		TEST_METHOD(IsOnline_AfterTimeout)
		{
			auto& sessionManager = SessionManager::Instance();
			sessionManager.Login(401);
			std::this_thread::sleep_for(50ms);
			Assert::IsFalse(sessionManager.IsOnline(401, 1ms));
			sessionManager.Logout(401);
		}
	};
}
