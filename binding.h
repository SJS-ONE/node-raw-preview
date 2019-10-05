#include <napi.h>
namespace functionexample
{
Napi::Object Init(Napi::Env env, Napi::Object exports);
Napi::Promise base64(const Napi::CallbackInfo &info);
} // namespace functionexample