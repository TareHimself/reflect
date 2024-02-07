#pragma once
#include "reflect/Macro.hpp"
#include "reflect/Reflect.hpp"
#include "reflect/Factory.hpp"
#include "reflect/wrap/Wrap.hpp"


#ifndef _REFLECT_GENERATED_LiveAudio
#define _REFLECT_GENERATED_LiveAudio
#define _REFLECTED_GENERATED_LiveAudio_FUNCTION_Play REFLECT_WRAP_FUNCTION_BEGIN(Play) \
{ \
 \
if(result){ \
*result.As<bool>() = instance.As<LiveAudio>()->Play(); \
} \
})

#define _REFLECTED_GENERATED_LiveAudio_FUNCTION_Pause REFLECT_WRAP_FUNCTION_BEGIN(Pause) \
{ \
 \
if(result){ \
*result.As<bool>() = instance.As<LiveAudio>()->Pause(); \
} \
})

#define _REFLECTED_GENERATED_LiveAudio_FUNCTION_Seek REFLECT_WRAP_FUNCTION_BEGIN(Seek) \
{ \
auto arg_0 = args[0].As<double>(); \
 \
instance.As<LiveAudio>()->Seek(*arg_0); \
})

#define _REFLECTED_GENERATED_LiveAudio_FUNCTION_GetPosition REFLECT_WRAP_FUNCTION_BEGIN(GetPosition) \
{ \
 \
if(result){ \
*result.As<double>() = instance.As<LiveAudio>()->GetPosition(); \
} \
})

#define _REFLECTED_GENERATED_LiveAudio_FUNCTION_GetLength REFLECT_WRAP_FUNCTION_BEGIN(GetLength) \
{ \
 \
if(result){ \
*result.As<size_t>() = instance.As<LiveAudio>()->GetLength(); \
} \
})

#define _REFLECT_GENERATE_LiveAudio \
reflect::factory::ReflectTypeBuilder builder; \
builder.AddField(_REFLECTED_GENERATED_LiveAudio_FUNCTION_Play); \
builder.AddField(_REFLECTED_GENERATED_LiveAudio_FUNCTION_Pause); \
builder.AddField(_REFLECTED_GENERATED_LiveAudio_FUNCTION_Seek); \
builder.AddField(_REFLECTED_GENERATED_LiveAudio_FUNCTION_GetPosition); \
builder.AddField(_REFLECTED_GENERATED_LiveAudio_FUNCTION_GetLength); \
builder.Create<LiveAudio>("LiveAudio");
#endif


