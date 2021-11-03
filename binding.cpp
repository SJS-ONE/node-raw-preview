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

napi_value getExifDataFromImage(Napi::Env env, Exiv2::Image::AutoPtr image)
{

    Exiv2::ExifData &exifData = image->exifData();

    napi_value exifDataObject = Napi::Object::New(env);
    napi_value unknownMetaDataArray = Napi::Array::New(env);

    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
    {
        const char *tn = i->typeName();

        if (!tn)
        {
            continue;
        }

        std::string searchString = "Exif.Sony2.0x";
        if ((i->key()).find(searchString) != std::string::npos)
        {
            continue;
        }

        if (i->count())
        {
            auto key = Napi::String::New(env, i->key());
            auto value = Napi::String::New(env, i->getValue()->toString());
            napi_set_property(env, exifDataObject, key, value);
        }
    }
    return exifDataObject;
}

napi_value getPreviewForPath(Napi::Env env, std::string path)
{

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path);
    image->readMetadata();

    Exiv2::PreviewManager loader(*image);
    Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();
    Exiv2::PreviewImage preview = loader.getPreviewImage(getBiggestPreview(list));

    std::string name = "preview" + preview.extension();
    std::stringstream strStream;

    napi_value previewImageObject = Napi::Object::New(env);

    napi_set_property(env, previewImageObject, Napi::String::New(env, "width"), Napi::String::New(env, std::to_string(preview.width())));
    napi_set_property(env, previewImageObject, Napi::String::New(env, "height"), Napi::String::New(env, std::to_string(preview.height())));
    napi_set_property(env, previewImageObject, Napi::String::New(env, "mime"), Napi::String::New(env, preview.mimeType()));
    napi_set_property(env, previewImageObject, Napi::String::New(env, "base64"), Napi::String::New(env, base64_encode(preview.pData(), preview.size())));

    napi_value exifDataObject = getExifDataFromImage(env, image);

    napi_value previewObject = Napi::Object::New(env);
    napi_set_property(env, previewObject, Napi::String::New(env, "image"), previewImageObject);
    napi_set_property(env, previewObject, Napi::String::New(env, "exifData"), exifDataObject);

    return previewObject;
}

Napi::Promise NodeRawPreview::getPreviewAndMetadata(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::String first = info[0].As<Napi::String>();
    napi_value returnValue = getPreviewForPath(env, first.Utf8Value());

    auto deferred = Napi::Promise::Deferred::New(env);
    deferred.Resolve(returnValue);
    return deferred.Promise();
}

Napi::Object NodeRawPreview::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getPreviewAndMetadata", Napi::Function::New(env, NodeRawPreview::getPreviewAndMetadata));

    return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return NodeRawPreview::Init(env, exports);
}

NODE_API_MODULE(testaddon, InitAll)
