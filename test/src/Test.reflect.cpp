#include "Test.hpp"

std::shared_ptr<reflect::wrap::Reflected> TestClass::Reflected = [](){
reflect::factory::TypeBuilder builder;
{
  auto field = _REFLECTED_GENERATED_TestClass_PROPERTY_num;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestClass_PROPERTY_health;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestClass_FUNCTION_HelloWorld;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestClass_FUNCTION_WithArgs;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestClass_FUNCTION_Construct;
  builder.AddField(field);
}
auto created = builder.Create<TestClass>("TestClass");
return created;
}();


std::shared_ptr<reflect::wrap::Reflected> TestClass::GetReflected() const
{
return reflect::factory::find<TestClass>();
}
std::shared_ptr<reflect::wrap::Reflected> TestStruct::Reflected = [](){
reflect::factory::TypeBuilder builder;
{
  auto field = _REFLECTED_GENERATED_TestStruct_PROPERTY_num;
  field->AddTag("TestTag");
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestStruct_PROPERTY_num2;
  field->AddTag("Two");
  field->AddTag("Tags");
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestStruct_PROPERTY_num5;
  field->AddTag("ATagFoMe");
  field->AddTag("AndForYou");
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestStruct_FUNCTION_Foo;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc2;
  builder.AddField(field);
}
{
  auto field = _REFLECTED_GENERATED_TestStruct_FUNCTION_TestFunc3;
  builder.AddField(field);
}
auto created = builder.Create<TestStruct>("TestStruct");
created->AddTag("VisibleInCaptures");
return created;
}();


std::shared_ptr<reflect::wrap::Reflected> TestStruct::GetReflected() const
{
return reflect::factory::find<TestStruct>();
}
