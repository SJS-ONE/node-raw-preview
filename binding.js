let RawPreview;

if (process.env.DEBUG) {
    RawPreview= require('./build/Debug/native.node')
} else {
    RawPreview= require('./build/Release/native.node')
}

module.exports = RawPreview