#include <napi.h>
namespace NodeRawPreview
{
Napi::Object Init(Napi::Env env, Napi::Object exports);
Napi::Promise getPreviewAndMetadata(const Napi::CallbackInfo &info);
} // namespace NodeRawPreview