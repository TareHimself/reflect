#pragma once
#include "reflect/Macro.hpp"
#include "reflect/Reflect.hpp"
#include "reflect/Factory.hpp"
#include "reflect/wrap/Wrap.hpp"


#ifndef _REFLECT_GENERATED_TestMesh
#define _REFLECT_GENERATED_TestMesh
#define _REFLECT_GENERATE_TestMesh \
reflect::factory::ReflectTypeBuilder builder; \
builder.Create<TestMesh>("TestMesh");
#endif


#ifndef _REFLECT_GENERATED_TestGameObject
#define _REFLECT_GENERATED_TestGameObject
#define _REFLECTED_GENERATED_TestGameObject_PROPERTY__mesh REFLECT_WRAP_PROPERTY(TestGameObject,_mesh,Managed<drawing::Mesh>)

#define _REFLECTED_GENERATED_TestGameObject_PROPERTY__meshComponent REFLECT_WRAP_PROPERTY(TestGameObject,_meshComponent,Ref<scene::StaticMeshComponent>)

#define _REFLECTED_GENERATED_TestGameObject_PROPERTY__scriptComp REFLECT_WRAP_PROPERTY(TestGameObject,_scriptComp,Ref<scene::ScriptComponent>)

#define _REFLECTED_GENERATED_TestGameObject_FUNCTION_GetWorldTransform REFLECT_WRAP_FUNCTION_BEGIN(GetWorldTransform) \
{ \
 \
if(result){ \
*result.As<math::Transform>() = instance.As<TestGameObject>()->GetWorldTransform(); \
} \
})

#define _REFLECT_GENERATE_TestGameObject \
reflect::factory::ReflectTypeBuilder builder; \
builder.AddField(_REFLECTED_GENERATED_TestGameObject_PROPERTY__mesh); \
builder.AddField(_REFLECTED_GENERATED_TestGameObject_PROPERTY__meshComponent); \
builder.AddField(_REFLECTED_GENERATED_TestGameObject_PROPERTY__scriptComp); \
builder.AddField(_REFLECTED_GENERATED_TestGameObject_FUNCTION_GetWorldTransform); \
builder.Create<TestGameObject>("TestGameObject");
#endif


