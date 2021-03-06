#include "StdAfx.h"
#include "MonoTester.h"

#include "MonoScriptSystem.h"

#include "MonoClass.h"
#include "MonoAssembly.h"
#include "MonoObject.h"
#include "MonoArray.h"

CMonoTester::CMonoTester()
	: m_refs(0)
{
	IMonoScriptSystem *pScriptSystem = gEnv->pMonoScriptSystem;

	REGISTER_METHOD(TestScriptBind);
}

void CMonoTester::CommenceTesting()
{
	IMonoScriptSystem *pScriptSystem = gEnv->pMonoScriptSystem;

	pScriptSystem->RegisterScriptBind(this);

	IMonoAssembly *pCryBraryAssembly = gEnv->pMonoScriptSystem->GetCryBraryAssembly();
	TestStaticMethods(pCryBraryAssembly);
	TestInstantiatedMethods(pCryBraryAssembly);
}

void CMonoTester::TestStaticMethods(IMonoAssembly *pCryBrary)
{
	IMonoClass *pTesterStatic = pCryBrary->GetCustomClass("Tester", "CryEngine.Utils");
	if(!pTesterStatic)
	{
		CryLogAlways("Mono feature test failed! Failed to obtain static tester class.");

		return;
	}

	if(pTesterStatic->CallMethod("StaticMethodNoParameters", NULL, true)->Unbox<float>()!=13.37f)
	{
		CryLogAlways("Mono feature test failed! Static method result was invalid.");

		return;
	}

	pTesterStatic->CallMethod("StaticMethodWithParameters", GetTestParameters(), true);

	SAFE_DELETE(pTesterStatic);
}

void CMonoTester::TestInstantiatedMethods(IMonoAssembly *pCryBrary)
{
	IMonoClass *pTesterClassStandardConstructor = pCryBrary->InstantiateClass("CryEngine.Utils", "Tester");
	if(!pTesterClassStandardConstructor)
	{
		CryLogAlways("Mono feature test failed! Failed to obtain tester class.");

		return;
	}

	SAFE_DELETE(pTesterClassStandardConstructor);

	IMonoClass *pTesterClassComplexConstructor = pCryBrary->InstantiateClass("CryEngine.Utils", "Tester", GetTestParameters());
	if(!pTesterClassComplexConstructor)
	{
		CryLogAlways("Mono feature test failed! Failed to instantiate tester class.");

		return;
	}

	if(pTesterClassComplexConstructor->CallMethod("MethodNoParameters")->Unbox<float>()!=13.37f)
	{
		CryLogAlways("Mono feature test failed! Method results were invalid.");

		return;
	}

	pTesterClassComplexConstructor->CallMethod("MethodWithParameters", GetTestParameters());

	SAFE_DELETE(pTesterClassComplexConstructor);
}

void CMonoTester::TestScriptBind(mono::string testString, int testInt, mono::array testArray)
{
	CMonoArray *pTestArray = new CMonoArray(testArray);

	if(!strcmp(ToCryString(testString), "TestStringContents") && testInt == 1337 && pTestArray->GetSize()==2)
	{
		const char *unboxedString = pTestArray->GetItemString(0);
		int unboxedInt = pTestArray->GetItemUnboxed<int>(1);

		if(/*!strcmp(unboxedString, "TestStringContents") &&*/ unboxedInt != 1337)
		{
			CryLogAlways("Mono feature test failed! Scriptbind result was invalid.");

			return;
		}
	}
	else
	{
		CryLogAlways("Mono feature test failed! Scriptbind parameters were invalid.");

		return;
	}
}

IMonoArray *CMonoTester::GetTestParameters()
{
	const char *testString = "TestStringContents";
	int testInt = 1337;

	IMonoArray *pParameters = CreateMonoArray(3);
	pParameters->Insert(testString);
	pParameters->Insert(testInt);

	IMonoArray *pSubArray = CreateMonoArray(2);
	pSubArray->Insert(testString);
	pSubArray->Insert(testInt);
	pParameters->Insert(pSubArray);

	return pParameters;
}

void CMonoTester::Benchmark()
{
	/*
	// Lua
	IScriptTable *pBenchmarkScript = NULL;

	clock_t start = clock();
	if(gEnv->pScriptSystem->ExecuteFile("D:\\Dev\\INK\\ProjectInk\\Game\\Scripts\\Tests\\Benchmark.lua", true, false, pBenchmarkScript));
	long luaExecutionTime = (clock() - start) / CLOCKS_PER_SEC * 1000;

	//start = clock();

	long monoExecutionTime = 0;//(clock() - start) / CLOCKS_PER_SEC * 1000;

	CryLogAlways("Benchmark done; Lua executed the script in %f ms and C# executed the script in %f ms", luaExecutionTime, monoExecutionTime);*/
}