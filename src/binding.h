#include <napi.h>
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>
#include "../base64/base64.h"
#include <sstream>
#include <string>
#include "./utils.h"

namespace NodeRawPreview
{
    Napi::Object Init(Napi::Env env, Napi::Object exports);
    Napi::Promise getPreviewAndMetadata(const Napi::CallbackInfo &info);
} // namespace NodeRawPreview