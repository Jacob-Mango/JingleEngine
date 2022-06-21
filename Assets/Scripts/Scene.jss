class Scene
{
	@Native
	Camera GetCamera();
}

struct TestArray extends Array
{
	@Native
	void Insert(Object instance);
}

class TestClassA
{
	@Property
	string value0;
}

class SomeTestClass
{
	@Property
	int value0;

	@Property
	string value1;

	@Property
	string value2;

	@ArrayProperty("TestClassA")
	TestArray value3;
}
