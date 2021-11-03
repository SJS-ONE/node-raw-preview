#include "binding.h"

Napi::Object getExifDataFromImage(Napi::Env env, Exiv2::Image::AutoPtr image, bool ignoreMetadataWithoutName)
{
    Exiv2::ExifData &exifData = image->exifData();

    Napi::Object exifDataObject = Napi::Object::New(env);
    Napi::Array unknownMetaDataArray = Napi::Array::New(env);

    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
    {
        std::string searchString = "Exif.Sony2.0x";
        if (!i->typeName() || (i->key()).find(searchString) != std::string::npos || i->count() == 0)
        {
            continue;
        }

        auto value = Napi::String::New(env, i->getValue()->toString());
        auto keyPathList = splitString(i->key().erase(0, 5), '.');

        auto targetObject = exifDataObject;

        for (const auto &keyPath : keyPathList)
        {
            if (ignoreMetadataWithoutName && keyPath.rfind("0x", 0) == 0)
            {   
                continue;
            }

            if (&keyPath == &keyPathList.back())
            {
                targetObject.Set(keyPath, value);
            }
            else if (!targetObject.Has(keyPath))
            {
                Napi::Object subObject = Napi::Object::New(env);
                targetObject.Set(keyPath, subObject);
                targetObject = subObject;
            }
            else
            {
                auto subObject = (targetObject.Get(keyPath));
                if (subObject.IsObject())
                {
                    targetObject = (subObject.As<Napi::Object>());
                }
                else
                {
                    throw Napi::Error::New(env, "KeyPath " + keyPath + " is not an object");
                }
            }
        }
    }
    return exifDataObject;
}

napi_value getPreviewForPath(Napi::Env env, std::string path, Napi::Object options)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path);
    image->readMetadata();

    Exiv2::PreviewManager loader(*image);
    Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();
    Exiv2::PreviewImage preview = loader.getPreviewImage(getBiggestPreview(list));

    std::string name = "preview" + preview.extension();
    std::stringstream strStream;

    Napi::Object previewImageObject = Napi::Object::New(env);
    previewImageObject.Set("width", Napi::String::New(env, std::to_string(preview.width())));
    previewImageObject.Set("height", Napi::String::New(env, std::to_string(preview.height())));
    previewImageObject.Set("mime", Napi::String::New(env, preview.mimeType()));
    if (options.Get("includeBase64").As<Napi::Boolean>().Value())
    {
        previewImageObject.Set("base64", Napi::String::New(env, base64_encode(preview.pData(), preview.size())));
    }

    Napi::Object previewObject = Napi::Object::New(env);
    previewObject.Set("image", previewImageObject);
    previewObject.Set("exifData", getExifDataFromImage(env, image, options.Get("ignoreMetadataWithoutName").As<Napi::Boolean>().Value()));

    return previewObject;
}

Napi::Object getOptions(Napi::Env env, Napi::Value optionsArgument)
{
    Napi::Object options = optionsArgument.IsUndefined() ? Napi::Object::New(env) : optionsArgument.As<Napi::Object>();

    if (options.Get("includeBase64").IsUndefined())
        options.Set("includeBase64", Napi::Boolean::New(env, true));

    if (options.Get("ignoreMetadata").IsUndefined())
        options.Set("ignoreMetadata", Napi::Array::New(env));

    if (options.Get("ignoreMetadataWithoutName").IsUndefined())
        options.Set("ignoreMetadataWithoutName", Napi::Boolean::New(env, true));

    return options;
}

Napi::Promise NodeRawPreview::getPreviewAndMetadata(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::String filePath = info[0].As<Napi::String>();
    Napi::Object options = getOptions(env, info[1]);

    napi_value returnValue = getPreviewForPath(env, filePath.Utf8Value(), options);

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
