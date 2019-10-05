#include "binding.h"
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>
#include "base64/base64.h"
#include <sstream>
#include <string>

Exiv2::PreviewProperties getBiggestPreview(Exiv2::PreviewPropertiesList list)
{
    uint32_t curMaxSize = 0;
    uint32_t curMaxKey = 0;

    for (std::vector<Exiv2::PreviewProperties>::size_type i = 0; i != list.size(); i++)
    {
        if (list[i].size_ > curMaxSize)
        {
            curMaxSize = list[i].size_;
            curMaxKey = i;
        }
    }
    return list[curMaxKey];
}

std::string test(std::string path)
{

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path);
    image->readMetadata();

    Exiv2::PreviewManager loader(*image);
    Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();
    Exiv2::PreviewImage preview = loader.getPreviewImage(getBiggestPreview(list));

    std::string name = "preview" + preview.extension();
    std::stringstream strStream;

    strStream << "width:" << preview.width() << ";"
              << "heigth:" << preview.height() << ";"
              << "mime:" << preview.mimeType() << ";"
              << "base64:" << base64_encode(preview.pData(), preview.size()) << std::endl;

    return strStream.str();
}

Napi::Promise functionexample::base64(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    auto deferred = Napi::Promise::Deferred::New(env);

    Napi::String first = info[0].As<Napi::String>();

    std::string returnValue = test(first.Utf8Value());

    deferred.Resolve(Napi::String::New(env, returnValue));

    return deferred.Promise();
}

Napi::Object functionexample::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("base64", Napi::Function::New(env, functionexample::base64));

    return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return functionexample::Init(env, exports);
}

NODE_API_MODULE(testaddon, InitAll)
